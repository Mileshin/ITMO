#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/inetdevice.h>
#include <linux/moduleparam.h>
#include <net/arp.h>
#include <linux/ip.h> 
#include <linux/version.h>

#define ERR(...) printk( KERN_ERR "lab3 "__VA_ARGS__ )
#define LOG(...) printk( KERN_INFO "lab3 "__VA_ARGS__ )

static char* link = "eth0";
module_param( link, charp, 0 );

static char* ifname = "virt"; 
module_param( ifname, charp, 0 );

static struct net_device *child = NULL;

static struct net_device_stats stats;

struct priv {
   struct net_device *parent;
};

static char* strIP( u32 addr ) {     // IP diagnostics in dot notation
   static char saddr[ MAX_ADDR_LEN ];
   sprintf( saddr, "%d.%d.%d.%d",
            ( addr ) & 0xFF, ( addr >> 8 ) & 0xFF,
            ( addr >> 16 ) & 0xFF, ( addr >> 24 ) & 0xFF
          );
   return saddr;
}

static rx_handler_result_t handle_frame( struct sk_buff **pskb ) {
  struct sk_buff *skb = *pskb;
    if( skb->protocol == htons( ETH_P_IP ) ) {
      struct iphdr *ip = ip_hdr( skb );
      char *dest_addr = strIP( ip->daddr );
      char *sour_addr = strIP( ip->saddr );
      LOG( "rx: IP4 from IP=%s to IP=%s with length: %u", sour_addr, dest_addr, skb->len );
   }
   else           // не IP4
      return RX_HANDLER_PASS; 
   if( child ) {
      stats.rx_packets++;
      stats.rx_bytes += skb->len;
      skb->dev = child;
      return RX_HANDLER_ANOTHER;
   }
   return RX_HANDLER_PASS;
}

static int open( struct net_device *dev ) {
   netif_start_queue( dev );
   LOG( "%s: device opened", dev->name );
   return 0;
}

static int stop( struct net_device *dev ) {
   netif_stop_queue( dev );
   LOG( "%s: device closed", dev->name );
   return 0;
}



static struct net_device_stats *get_stats( struct net_device *dev ) {
   return &stats;
}

static struct net_device_ops crypto_net_device_ops = {
   .ndo_open = open,
   .ndo_stop = stop,
   .ndo_get_stats = get_stats,
};


// обработчик фреймов ETH_P_IP 
int ip4_pack_rcv( struct sk_buff *skb, struct net_device *dev,
                  struct packet_type *pt, struct net_device *odev ) {
   struct iphdr *ip = ip_hdr( skb );
   char daddr[ MAX_ADDR_LEN ], saddr[ MAX_ADDR_LEN ];
   strcpy( daddr, strIP( ip->daddr ) );
   strcpy( saddr, strIP( ip->saddr ) );
   LOG( "rx: from IP=%s to IP=%s with length: %u",
      saddr, daddr, skb->len );
      skb->dev = child;           // передача фрейма в виртуальный интерфейс
      stats.rx_packets++;
      stats.rx_bytes += skb->len;
   return skb->len;
};

static struct packet_type ip4_proto = {
   __constant_htons( ETH_P_IP ),
   NULL,
   ip4_pack_rcv,    // фильтр пртокола ETH_P_IP 
   (void*)1,
   NULL
};


int __init init( void ) {
    void setup( struct net_device *dev ) { // вложенная функция (GCC)
      int j;
      ether_setup( dev );
      memset( netdev_priv( dev ), 0, sizeof( struct priv ) );
      dev->netdev_ops = &crypto_net_device_ops;
      for( j = 0; j < ETH_ALEN; ++j )     // заполнить MAC фиктивным адресом 
         dev->dev_addr[ j ] = (char)j;
   }
   int err = 0;
   struct priv *priv;
   char ifstr[ 40 ];
   sprintf( ifstr, "%s%s", ifname, "%d" );
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,17,0)) 
    child = alloc_netdev( sizeof( struct priv ), ifstr, setup ); 
#else 
    child = alloc_netdev( sizeof( struct priv ), ifstr, NET_NAME_UNKNOWN, setup ); 
#endif
   if( child == NULL ) {
      ERR( "%s: allocate error", THIS_MODULE->name ); return -ENOMEM;
   }
   priv = netdev_priv( child );
   priv->parent = __dev_get_by_name( &init_net, link ); // parent interface  
   if( !priv->parent ) {
      ERR( "%s: no such net: %s", THIS_MODULE->name, link );
      err = -ENODEV; goto err;
   }
   if( priv->parent->type != ARPHRD_ETHER && priv->parent->type != ARPHRD_LOOPBACK ) {
      ERR( "%s: illegal net type", THIS_MODULE->name );
      err = -EINVAL; goto err;
   }
   memcpy( child->dev_addr, priv->parent->dev_addr, ETH_ALEN );
   memcpy( child->broadcast, priv->parent->broadcast, ETH_ALEN );
   if( ( err = dev_alloc_name( child, child->name ) ) ) {
      ERR( "%s: allocate name, error %i", THIS_MODULE->name, err );
      err = -EIO; goto err;
   }
   register_netdev( child );
   
   ip4_proto.dev = priv->parent; // перехват только с родительского интерфейса
   dev_add_pack( &ip4_proto );
   LOG( "module %s loaded", THIS_MODULE->name );
   LOG( "%s: create link %s", THIS_MODULE->name, child->name );
   return 0;
err:
   free_netdev( child );
   return err;
}

void __exit virt_exit( void ) {
   struct priv *priv = netdev_priv( child );
   if( priv->parent ) {
      rtnl_lock();
      netdev_rx_handler_unregister( priv->parent );
      rtnl_unlock();
      LOG( "unregister rx handler for %s\n", priv->parent->name );
   }
   unregister_netdev( child );
   free_netdev( child );
   LOG( "module %s unloaded", THIS_MODULE->name );
}

module_init( init );
module_exit( virt_exit );

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mileshin Andrey");
MODULE_DESCRIPTION("Lab 3 network module.");

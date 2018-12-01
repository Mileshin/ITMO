#include <linux/string.h>

#include "partition.h"

unsigned char primary_partitions[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x75, 0xe5, 0x36, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x40, 0x83, 0x00, 0xa0, 0xbf, 0x00, 0x08, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, // 1 primary partition
    0x81, 0xc0, 0x05, 0x00, 0xe0, 0xbf, 0x00, 0x58, 0x00, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x00, // 2 extendet partition
    0xc1, 0xc0, 0x83, 0x00, 0x20, 0x7f, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, // 3 primary partition
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xaa
};

unsigned char logic_partition1[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87,
    0x07, 0x01, 0x83, 0xcd, 0x0b, 0x02, 0x00, 0x08, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0xcd,
    0x0c, 0x02, 0x05, 0xf2, 0x2f, 0x03, 0x00, 0x58, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xaa
};

unsigned char logic_partition2[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xed,
    0x2c, 0x02, 0x83, 0xf2, 0x2f, 0x03, 0x00, 0x08, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xaa
};

void copy_mbr_n_br(u8 *disk)
{
    memcpy(disk +432, primary_partitions, 80);
    memcpy(disk +11534768, logic_partition1, 80);
    memcpy(disk +23069104, logic_partition2, 80);
}


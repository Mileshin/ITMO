obj-m := var2.o
var2-objs := symb.o file_io.o 

PWD = $(shell pwd)

all:
	make -C /lib/modules/$(shell uname -r)/build M="$(PWD)" modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M="$(PWD)" clean
go:
	sudo insmod var2.ko
	sudo chmod 777 /dev/var2
stop:
	sudo rmmod var2
write:
	echo test > /dev/var2

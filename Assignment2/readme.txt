Description:- This is the kernel module which is used to create a block device "mydisk",by allocating 512 KB of space in the memory (using vmalloc) and simulate that as a separate block device and partition it into 2 primary partitions.There are 1024 sectors with first 512 bytes having MBR. Block driver can read and write to this virtual disk.

/**************READ ME *****************/

1. Type 'make all' command, it wil build main.ko

2. Then insert the module using 'sudo insmod main.ko'

3. $ sudo fdisk -l or lsblk shows you the partitions made on your device.

4. $ sudo hd -n 512 /dev/dof shows the MBR, we can see MBR signature 55 AA at the end.

5. Then for writing and reading from the device, go into root mode, by giving 'sudo -s' command( i.e super user)

6. $ cat > /dev/dof1 (for eg.) to write into the device, and write whatever data you want to write. 

7. $ xxd /dev/dof1 | less  gives whatever you have written previously into it.

8. $ dmesg -wH to check the kernel log for device open and close.

 
 

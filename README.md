# lpc_uart_server

USB to uart sever based on lpc54018

| folder                     | description                                   |
| -------------------------- | --------------------------------------------- |
| hardware                   | hardware schematic                            |
| kernel driver              | Linux kernel driver source files              |
| lpc_firmware_download_tool | Linux side DFU download tool and MCU firmware |
| user_space_test_program    | Linux user space program for test and demo    |
| reference                  | other reference                               |
| misc                       | other reference                               |



## Getting Started

### Add USB kernel driver, compile kernel

1. Copy file kernel_driver/lpc54xxx.c file to Linux kernel source directory drivers/usb/serial/
2. Add below code to Linux kernel source file drivers/usb/serial/Makefile:

`obj-$(CONFIG_USB_SERIAL_LPC54XXX)   += lpc54xxx.o`

3. Add below code to Linux kernel source file drivers/usb/serial/Kconfig:

```
config USB_SERIAL_LPC54XXX
     tristate "USB LPC54XXX USB Serial Driver"
     help
       Say Y here if you want to use a LPC54XXX 10 port
       USB to serial converter device.
       To compile this driver as a module, choose M here.

```

4. On Linux kernel source folder, run ‘make menuconfig’ and select above item (“Linux kernel source”) to ‘*’ (as built-in), then compile the Linux kernel by executing ‘make’.

### Download MCU firmware

Environment required for download tool (LPC_firmware_download_tool/dfu-utils):

* ARCH:  arm64

* OS:    Linux

1. Copy the download folder  LPC_firmware_download_tool/*  to a arm64 platform (such as LS1021ATWR)’s directory /usr/bin/. Connect a USB cable between arm64 platform USB host port and target LPC board’s USB upstream port.

2. On arm64 platform Linux console, goto /usr/bin/LPC_firmware_download_tool/, execute below shell script to download firmware to LPC board automatically through USB cable:

   

```
root@ls1021atwr:/usr/bin/LPC_firmware_download_tool# ./download.sh
```

Related log:

```
dfu-util 0.9

Copyright 2005-2009 Weston Schmidt, Harald Welte and OpenMoko Inc.
Copyright 2010-2016 Tormod Volden and Stefan Schmidt
This program is Free Software and has ABSOLUTELY NO WARRANTY
Please report bugs to http://sourceforge.net/p/dfu-util/tickets/

dfu-util: Invalid DFU suffix signature
dfu-util: A valid DFU suffix will be required in a future dfu-util release!!!
Opening DFU capable USB device...
ID 1fc9:001f
Run-time device DFU version 0110
Claiming USB DFU Interface...
Setting Alternate Setting #0 ...
Determining device status: state = dfuIDLE, status = 0
dfuIDLE, continuing
DFU mode device DFU version 0110
Device returned transfer size 512
Copying data from PC to DFU device
Download        [=========================] 100%        62776 bytes
Download done.
state(8) = dfuMANIFEST-WAIT-RESET, status(0) = No error condition is present
Done!
************************************************************************************************
Congratulations! Now the operation download firmware is successful. Host board (arm64) will begin re-enumerate LPC board as a special USB serial port device automatically.
```

3. After firmware download complete, you will see upto 10 ttyUSBx device file shown in /dev/, which mean they are ready for use:

```
root@ls1021atwr:/usr/bin/LPC_firmware_download_tool# ls /dev/ttyUSB
```



Note: There is a user space program which can be used (on arm64 platform) for function test and demo, please see folder user_space_test_program/ for details.



### Debug

The device’s driver has added two files in sysfs to show the number of sending and receiving data (in bytes). The file named “send_bytes” means the data ttyUSBx device has sent since usb serial device got enumerated. Similarly, the file named “recv_bytes” means the data ttyUSBx device have received since usb serial device got enumerated.



Here is an example:

```
root@ls1043ardb:/sys/bus/usb/devices/1-1.4:1.0/ttyUSB8# cat send_bytes
360855
root@ls1043ardb:/sys/bus/usb/devices/1-1.4:1.0/ttyUSB8# cat recv_bytes
360855

```



### MCU side

UART pinmux table:

| 串口                    | PIN   | 复用功能 |
| ----------------------- | ----- | -------- |
| UART0_TX(内部debug使用) | P0_30 | 1        |
| UART0_RX(内部debug使用) | P0_29 | 1        |
| UART0_CTS               | N/A   | N/A      |
| UART0_RTS               | N/A   | N/A      |
| UART1_TX                | P1_11 | 2        |
| UART1_RX                | P1_10 | 2        |
| UART1_CTS               | P0_13 | 1        |
| UART1_RTS               | P0_14 | 1        |
| UART2_TX                | P1_25 | 1        |
| UART2_RX                | P1_24 | 1        |
| UART2_CTS               | P1_26 | 1        |
| UART2_RTS               | P1_27 | 1        |
| UART3_TX                | P0_2  | 1        |
| UART3_RX                | P0_3  | 1        |
| UART3_CTS               | P0_1  | 2        |
| UART3_RTS               | P0_7  | 1        |
| UART4_TX                | P3_27 | 3        |
| UART4_RX                | P3_26 | 3        |
| UART4_CTS               | P3_28 | 3        |
| UART4_RTS               | P3_29 | 3        |
| UART5_TX                | P2_13 | 5        |
| UART5_RX                | P2_12 | 5        |
| UART5_CTS               | P2_14 | 5        |
| UART5_RTS               | P2_15 | 5        |
| UART6_TX                | P4_3  | 2        |
| UART6_RX                | P4_2  | 2        |
| UART6_CTS               | P4_0  | 2        |
| UART6_RTS               | N/A   | N/A      |
| UART7_TX                | P2_20 | 3        |
| UART7_RX                | P2_19 | 3        |
| UART7_CTS               | P1_21 | 1        |
| UART7_RTS               | P1_20 | 1        |
| UART8_TX                | P3_17 | 1        |
| UART8_RX                | P3_16 | 1        |
| UART8_CTS               | P3_4  | 3        |
| UART8_RTS               | P3_5  | 3        |
| UART9_TX                | P3_3  | 2        |
| UART9_RX                | P3_2  | 2        |
| UART9_CTS               | P3_30 | 1        |
| UART9_RTS               | P3_31 | 1        |


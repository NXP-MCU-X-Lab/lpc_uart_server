* lpc54018_uart_server: RAM based image,  running on LPC54018, the image will lost if re-power or press RESET pin.
* lpc54608_uart_server: flash based image, running on LPC54608. the image will NOT lost if re-power or press RESET pin.

Please update the 'download.sh' file based on the USB ID obtained from the 'lsusb' command on the host machine (Linux) for the MCU in DFU mode. 
Modify it according to the actual situation.



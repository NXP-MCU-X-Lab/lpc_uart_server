sudo dfu-util -d 1fc9:001f -D lpc54018_uart_server.bin 
sleep 1
ls /dev/ttyU*
sudo chmod -R 777 /dev/ttyU*


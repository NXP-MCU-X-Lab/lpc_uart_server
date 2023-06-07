sudo dfu-util -d 1fc9:001f -D lpc54018_uart_server.bin
sleep 5
ls /dev/ttyU*
sleep 1
sudo chmod -R 777 /dev/ttyU*


#!/bin/sh

echo "Stop the server"
sudo systemctl stop chatd2 2>/dev/null
echo "Waiting one second"
sleep 1
echo "Remove old files"
sudo rm /etc/chatd2.cfg 2>/dev/null
sudo rm /usr/sbin/chatd2 2>/dev/null

echo "Copy new files"
sudo cp ./chatd /usr/sbin/chatd2 && sudo chmod 755 /usr/sbin/chatd2
sudo cp chatd.cfg /etc/chatd2.cfg
sudo systemctl daemon-reload
echo -n "Starting the server... "
sudo systemctl start chatd2
echo "Ok"
tail /var/log/chatd2.log

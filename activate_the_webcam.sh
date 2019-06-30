#!/usr/bin/env bash

sudo mknod /dev/video0 c 81 0
sudo chmod 666 /dev/video0
sudo ln -s /dev/video0 /dev/video

echo -e "[+] The activation of the webcam is over, now we need to reboot your machine\n"
read -p "[?] Reboot [Yes/No] ? " choice

if [ "$choice" == "Yes" ]
then
  reboot
fi

if [ "$choice" == "Y" ]
then
  reboot
fi

if [ "$choice" == "y" ]
then
  reboot
fi



if [ "$choice" == "No" ]
then
  echo -e "[!] The webcam won't work correctly !\n"
fi

if [ "$choice" == "N" ]
then
  echo -e "[!] The webcam won't work correctly !\n"
fi

if [ "$choice" == "n" ]
then
  echo -e "[!] The webcam won't work correctly !\n"
fi

echo -e "\nGoodBye !\n"

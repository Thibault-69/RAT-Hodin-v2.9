#!/usr/bin/env bash

sudo mknod /dev/video0 c 81 0
sudo chmod 666 /dev/video0
sudo ln -s /dev/video0 /dev/video

echo -e "\n[+] The activation of the webcam is over, You can now reboot your machine.\n"

read -p "[?] [optionnal] Reboot [Yes/No] ? " choice


if [ "$choice" == "Yes" ] || [ "$choice" == "Y" ] || [ "$choice" == "y" ] || [ "$choice" == "yes" ]
then
  reboot
fi

if [ "$choice" == "No" ] || [ "$choice" == "N" ] || [ "$choice" == "no" ] || [ "$choice" == "n" ]
then
  echo -e "\nBye\n"
fi

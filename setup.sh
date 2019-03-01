#!/bin/bash

# SETUP FOLDERS TO RUN THE CODE #
#echo "\n\n*********\n* Setup *\n*********\n"


# In theory just need to be done once per link:
# Create a symbalic link of a folder (foo) in a target folder (goo)
# The folder with all the files get copied.
# If you modify the content either in 'foo' or 'goo' it updates in the other ;)
#sudo ln -s `pwd`/foo `pwd`/goo
#sudo ln -s `pwd`/API_rpi/comm `pwd`/API_Robot

# We can use that instead of running the scripts to copy the files in the Arduino libraries of our PC'
# Done, don't need to run 'pauPc_copyLib2ardLib.sh' any more.

#echo "\n\n***************\n* Setup done! *\n***************\n"


# The serial port of Ubuntu might be different than Raspbian, change this line:
# int fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
# in function 'serial_open()'
# of file /API_rpi/comm/comm_serial.cpp






##############################################
# Install Ubuntu on SD card for Raspberry Pi #
##############################################


## PREPARE SD CARD
# http://qdosmsq.dunbar-it.co.uk/blog/2013/06/noobs-for-raspberry-pi/


## 

#sudo dd if=~/Desktop/ubuntu-18.04.1-desktop-amd64.iso of=/dev/sdb1 status=progress bs=4M



unzip ~/Desktop/linux_imgs/NOOBS_v3_0_0.zip -d '/media/pau/Ubuntu 18.04.1 LTS amd64'/

### Questions
#- install ubuntu iso in SD card ???
#- make it bootable

#- ROS distro (Kinetic, Melodic...)



Raspbian NOOBS lite 3.0 installed, need network connection to install (faster than the normal NOOBS)




once Raspbian is installed on rpi
(might need several reboots during the whole process)
- connect to a network -> either with the GUI or wpa_supplicant
- git alreadi installed
- enable all communications (ssh, i2c...)

//connect to 'pieiiu' network bot rpi and laptop
ssh pi@192.168.43.174
password: egg
cd ~
mkdir git_repos
cd git_repos
git clone https://github.com/palori/api_swarm_mobile_robot.git

# create links into other folders from where the code will be run


# download and install Cmake
sudo apt-get install cmake

# download and install OpenCV
git clone https://github.com/opencv/opencv.git
git clone https://github.com/opencv/opencv_contrib.git

# modify accordingly, see: https://github.com/opencv/opencv_contrib
#cd #<opencv_build_directory>
cd opencv
mkdir build
cd build
cmake -DOPENCV_EXTRA_MODULES_PATH=~/git_repos/opencv_contrib/modules ~/git_repos/opencv
make -j5
sudo make install


# RASPICAM
take picture from raspicam on terminal: raspistill -o cam.jpg
open picture: xdg-open cam_1.jpg

# download camera libraries unzip and build
cd ~/Downloads #destination folder
wget https://sourceforge.net/projects/raspicam/files/latest/download
unzip download
rm download
cd raspicam-0.1.6

# or clone the repo
git clone https://github.com/cedricve/raspicam.git
cd raspicam
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig

# check this tutorial!!!
# https://www.youtube.com/watch?v=QV1a1G4lL3U
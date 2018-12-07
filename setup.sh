#!/bin/bash

# SETUP FOLDERS TO RUN THE CODE #
echo "\n\n*********\n* Setup *\n*********\n"


# In theory just need to be done once per link:
# Create a symbalic link of a folder (foo) in a target folder (goo)
# The folder with all the files get copied.
# If you modify the content either in 'foo' or 'goo' it updates in the other ;)
#sudo ln -s `pwd`/foo `pwd`/goo
#sudo ln -s `pwd`/API_rpi/comm `pwd`/API_Robot

# We can use that instead of running the scripts to copy the files in the Arduino libraries of our PC'
# Done, don't need to run 'pauPc_copyLib2ardLib.sh' any more.

echo "\n\n***************\n* Setup done! *\n***************\n"
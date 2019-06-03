cd `pwd`/../../API_rpi/robot_robocup/test_image
./clean.sh
echo && pwd
echo && ls

cd ../test_map
make clean
echo && pwd
echo && ls

cd cmake
./clean.sh
echo && pwd
echo && ls
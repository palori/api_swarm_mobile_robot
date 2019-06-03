cd `pwd`/../API_rpi/robot/test_image
./clean.sh
cp potato/CMakeLists.txt CMakeLists.txt
cmake .
echo && pwd
echo && ls

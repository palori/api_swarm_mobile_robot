cd `pwd`/../../API_rpi/robot_robocup/test_image
./clean.sh
cp potato/CMakeLists.txt CMakeLists.txt
cmake .
echo && pwd
echo && ls

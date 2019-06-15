

if [ $1 ]
then
	folder=$1
	if [ $1 == 'test_image' ]
	then
		if [ $HOSTNAME == 'potato' ] || [ $2 == 'potato' ]
		then
			folder="$1/potato"
		else
			folder="$1/others"
		fi
	else 
		folder="$1/cmake"
	fi 
	path=`pwd`/../API_rpi/tests/$folder
	cd $path
	rm -rf build
	pwd
	ls
else
	echo 'Required to input the test folder to clean as the only argument :)'
	echo
fi
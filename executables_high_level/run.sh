

### Declaring functions

check_action(){
	# Check the second input validity
	if [ $action == 'cmake' ]
	then
		#echo 'cmake'
		comp1=1
	elif [ $action == 'make' ]
	then
		#echo 'make'
		comp2=1
	elif [ $action == 'both' ]
	then
		#echo 'cmake & make'
		comp1=1
		comp2=1
	elif [ $action == 'run' ]
	then
		#echo 'run'
		run=1
	else
		echo 'Wrong action, it can only be: cmake, make or run.'
	fi
}

read_csv(){
	
	if [ $tests -eq 1 ]; then
		filename='robots_tests.csv'
	else
		filename='robots_config.csv'
	fi
	n=1
	while IFS=, read csv_agent csv_id csv_hostname csv_ip csv_port_info csv_port_image csv_port_task; do
	# reading each line
	echo "Line No. $n : $csv_agent	| $csv_id	| $csv_hostname 	| $csv_ip 	| $csv_port_info 	| $csv_port_image 	| $csv_port_task"
	if [ $csv_agent == 'master' ]; then
		master=$csv_hostname
		master_port=$csv_port_info
		master_id=$csv_id
		#echo 'master'
	fi
	#if [ $csv_id == $robot_id ] || [ $HOSTNAME == $csv_hostname ]; then
	if [ $csv_id == $robot_id ]; then
		agent=$csv_agent
		id=$csv_id
		hostname=$csv_hostname
		ip=$csv_ip
		port_info=$csv_port_info
		port_image=$csv_port_image
		port_task=$csv_port_task
		robot_found=1
		#echo 'csv_id == robot_id'
	#elif [ $csv_agent == 'robot' ] && [ $robot_found -eq 0 ]; then
	elif [ $csv_agent == 'robot' ]; then
		#echo 'robot not found' 
		if [ $robot_a == 'a' ]; then
			robot_a=$csv_hostname
			robot_a_port=$csv_port_info
			robot_a_id=$csv_id
		else
			robot_b=$csv_hostname
			robot_b_port=$csv_port_info
			robot_b_id=$csv_id
		fi
	fi
	n=$((n+1))
	done < $filename
}

get_path(){
	folder=$test_folder
	if [ $test_folder == 'test_image' ]
	then
		if [ $HOSTNAME == 'potato' ] || [ $action == 'potato' ]
		then
			folder="$test_folder/potato"
		else
			folder="$test_folder/others"
		fi
	else 
		folder="$test_folder/cmake"
	fi 
	path=`pwd`/../API_rpi/tests/$folder
}


# Inputs
test_folder=$1
action=$2
if [ $3 ]; then
	robot_id=$3
else
	robot_id=-1
fi

# $4 see function read_csv
tests=0
if [ $4 ] && [ $4 == 'tests' ]; then
	tests=1
fi


# Global variables
comp1=0
comp2=0
run=0

#tests=0
filename=''
robot_found=0


agent=''
id=''
hostname=''
ip=''
port_info=''
port_image=''
port_task=''

master=''
robot_a='a'
robot_b='b'

master_port=8000
robot_a_port=8000
robot_b_port=8000

robot_a_id=0
robot_b_id=0
master_id=0

max_len=100

path=''

echo "hostname = $HOSTNAME"
if [ $test_folder ] && [ $action ] #&& [ $robot ]
then
	if [ $test_folder ]
	then
		check_action

		# action is valid
		if [ $comp1 -eq 1 ] || [ $comp2 -eq 1 ] || [ $run -eq 1 ]
		then
			if [ $robot_id -gt -1 ]; then
				read_csv
			fi

			get_path
			cd $path

			# cmake
			if [ $comp1 -eq 1 ]
			then
				echo cmake...
				rm -rf build
				mkdir build
				cd build
				if [ $tests -eq 1 ]; then
					cmake -DTEST_PC:bool=true ..
				else
					cmake ..
				fi
			fi

			# make
			if [ $comp2 -eq 1 ]
			then
				echo make...
				cd build
				make
			fi

			# run
			if [ $run -eq 1 ]
			then
				echo run...
				cd build
				if [ $test_folder == 'test_image' ]; then
					echo 'test_image'
					./main_image
				elif [ $test_folder == 'test_map' ]; then
					if [[ $agent == 'master' ]]; then
						echo 'test_map - master'
						./main_map $master $hostname $robot_a $robot_b $id $max_len
					else
						echo 'test_map - robot'
						./main_map $master $hostname $robot_a $robot_b $id $max_len $port_image $port_task $port_info $robot_a_port $robot_b_port $master_port $robot_a_id $robot_b_id $master_id
					fi
				else
					echo `pwd`
					./$test_folder
				fi
			fi
		fi
		
	fi 

else
	echo '2 inputs required:'
	echo '	the file and the action (cmake, make, both, run)'
	echo
	echo 'Optional:'
	echo '	3rd input is the robot id'
	echo '	4th input is for testng, just write "tests"'
	echo
fi
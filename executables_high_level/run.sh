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
	filename='robots_config.csv'
	n=1
	while IFS=, read csv_agent csv_id csv_hostname csv_ip csv_port_info csv_port_image csv_port_task; do
	# reading each line
	echo "Line No. $n : $csv_agent	| $csv_id	| $csv_hostname 	| $csv_ip 	| $csv_port_info 	| $csv_port_image 	| $csv_port_task"
	if [ $csv_id == $robot ] || [ $HOSTNAME == $csv_hostname ]; then
		$agent=$csv_agent
		$id=$csv_id
		$hostname=$csv_hostname
		$ip=$csv_ip
		$port_info=$csv_port_info
		$port_image=$csv_port_image
		$port_task=$csv_port_task
	elif [ $csv_agent == 'master' ]; then
		$master=$csv_hostname
	elif [ $csv_agent == 'robot' ]; then 
		if [ $robot_a == '' ]; then
			$robot_a=$csv_hostname
		else
			$robot_b=$csv_hostname
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
robot=$3

# Global variables
comp1=0
comp2=0
run=0

agent=''
id=''
hostname=''
ip=''
port_info=''
port_image=''
port_task=''

master=''
robot_a=''
robot_b=''

max_len=100

path=''


if [ $test_folder ] && [ $action ] #&& [ $robot ]
then
	if [ $test_folder ]
	then
		check_action

		# action is valid
		if [ $comp1 -eq 1 ] || [ $comp2 -eq 1 ] || [ $run -eq 1 ]
		then
			read_csv

			get_path

			# cmake
			if [ $comp1 -eq 1 ]
			then
				echo comp1
				rm -rf build
				mkdir build
				cd build
				cmake ..
			fi

			# make
			if [ $comp2 -eq 1 ]
			then
				echo comp2
				cd build
				make
			fi
$test_folder ==
			# run
			if [ $run -eq 1 ]
			then
				echo run
				cd build
				if [ $test_folder == 'test_image' ]; then
					./main_image
				elif [ $test_folder == 'test_map' ]; then
					./main_map $master $hostname $robot_a $robot_b $max_len
				fi
			fi
		fi
		
	fi 

else
	echo '2 inputs required: the file and the action (cmake, make, both, run)'
	echo
fi
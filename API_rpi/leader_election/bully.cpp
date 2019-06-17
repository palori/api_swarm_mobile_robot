
#include "bully.h"



Bully::Bully(){
	init();
}

Bully::~Bully(){}

Bully::Bully(int my_id, float time_threshold){
	init(my_id, time_threshold);
}

void Bully::init(){
	this->leader.set(0);
	this->my_id.set(0);
	this->trigger.set(false);
	this->time_threshold.set(10.0);

	this->i_detected.set(false);

	this->debug.set(false);
}

void Bully::init(int my_id, float time_threshold){
	init();
	this->my_id.set(my_id);
	this->time_threshold.set(time_threshold);
}


/*int Bully::get_MAX_LEN(){return MAX_LEN;}
void Bully::set_MAX_LEN(int i){
	MAX_LEN = i;
	x.set_MAX_LEN(i);
	y.set_MAX_LEN(i);
}*/

//void Bully::print_info();
//Bully & Bully::operator=(Bully & b);

void Bully::trigger_election(){
	if(!trigger.get()){
		trigger.set(true);
		robots_ids.clear();
		proposed_leader.clear();
		time_detected.set(chrono::system_clock::now());
		i_detected.set(false);
	}
}



bool Bully::election(int & leader_1, int & proposed_leader_1){

	bool leader_elected = false;

	//if (bully.trigger.get() && (bully.i_detected.get() || is_election_time()) ){cout << "+++ trigger = " << trigger.get();
	if (debug.get()){
		cout << "\n+++ trigger = " << trigger.get();
		cout << "\n    is_election_time = " << is_election_time() << "\n\n";
	}

	if (trigger.get() && (is_election_time() || i_detected.get())) {
		cout << "    election\n";
		// keep sending or do it just once???

		vector<int> rids = robots_ids.get_items();

		auto result = min_element(begin(rids), end(rids));
		if (end(rids)!=result){
			proposed_leader_1 = *result;

			if (debug.get()){
				cout << "I\'m robot \'" << my_id.get() << "\' and I think the leader should be robot \'" << proposed_leader_1 << "\'.\n";
			}
			proposed_leader.add_unique_item(proposed_leader_1);
		}


		leader_1 = leader.get();
	
		if (is_election_time()){
			if (debug.get()) cout << "    election time\n";

			// check if all proposed leaders are in the list of robot ids
			vector<int> pl = proposed_leader.get_items();
			vector<int> cll; // clean_leader_list
			
			if (debug.get()){
				cout << "rids len = " << rids.size() << ", pl len = " << pl.size() << endl;
			}

			for (int i = 0; i < rids.size(); i++){
				for (int j = 0; j < pl.size(); j++){
					if (debug.get()){
						cout << "rids [" << i << "] " << rids.at(i) << ", pl [" << j << "] " << pl.at(j) << endl;
					}

					if (rids.at(i) == pl.at(j)){
						cll.push_back(rids.at(i));
						break;
					}
				}
			}

			auto result = min_element(begin(cll), end(cll));
			if (end(cll)!=result){
				proposed_leader_1 = *result;
				if (debug.get()){
					cout << "I\'m robot \'" << my_id.get() << "\' and the new leader is robot \'" << proposed_leader_1 << "\'.\n";
				}

				leader.set(proposed_leader_1);
			}

			trigger.set(false);
			i_detected.set(false);
			leader_elected = true;
		}
	}

	return leader_elected;
}


bool Bully::is_election_time(){
	bool is_election = false;
	if (get_time() > time_threshold.get()) is_election = true;
	return is_election;
}

float Bully::get_time(){
	auto now = chrono::system_clock::now();
	chrono::duration<double> le_elapsed = now - time_detected.get();
	return (float) le_elapsed.count(); // in seconds
}


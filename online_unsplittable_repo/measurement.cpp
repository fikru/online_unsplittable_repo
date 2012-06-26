#include <iostream>
#include <vector>		//vector
#include <sys/stat.h>	//timespec
#include <sys/time.h>
#include "global.h"

using namespace std;

measurement::measurement(){
	time_advancedList.clear();
}
measurement::~measurement(){}

void measurement::setStart(){
	gettimeofday(&start,0);
	this->startTime = start.tv_sec + start.tv_usec/1000000;
}

void measurement::setEnd(){
	gettimeofday(&end,0);
	this->endTime = end.tv_sec + end.tv_usec/1000000;
}

/*sets simulation time list
 */
void measurement::setTimeAdvList(){
	double start = getStart();
	double end = getEnd();
	double timeAdvanced = end - start;
	this->time_advancedList.push_back(timeAdvanced);
}
//set file modification time
void measurement::setModTime(char* f_name){
	struct stat buf;			//configuration file status check
	timespec now;				//last configuration modification time
	/*File last modified time*/
	if (stat(f_name, &buf) == -1) {
			perror(f_name);
			exit(EXIT_FAILURE);
	}
	now = buf.st_mtim;
	this->file_last_mod_time = now.tv_sec*1000000 + now.tv_nsec;
}

//get simulation start time
double measurement::getStart(){
	return this->startTime;
}

//get simulation end time
double measurement::getEnd(){
	return this->endTime;
}

/*gets simulation time list
 */
vector<double> measurement::getTimeAdvList(){
	return this->time_advancedList;
}

//gets file modification time
double measurement::getModTime(){
	return this->file_last_mod_time;
}

/**compute summation of a vector of any type T
 */
double measurement::getSum(vector<double> &vectorin){
	double total = 0;
	for(vector<double>::iterator v_itr = vectorin.begin(); v_itr != vectorin.end(); ++v_itr){
		total += *v_itr;
	}
	return total;
}

/*compare two vectors*/
bool measurement::compareVector(vector<double> &vector1, vector<double> &vector2){
	vector<double>::iterator v_itr1 = vector1.begin();
	vector<double>::iterator v_itr1_end = vector1.end();
	vector<double>::iterator v_itr2 = vector1.begin();
	if(vector1.size() != vector2.size())
		return false;
	while(v_itr1 != v_itr1_end){
		if(*v_itr1 != *v_itr2)
			return false;
		++v_itr1; ++v_itr2;
	}
	return true;
}

/*display simulation results
 */
void measurement::result(list<router> rList,statistics stat){
	for(list<router>::iterator r_itr = rList.begin(); r_itr != rList.end(); ++r_itr){
		list<rLink> lList = r_itr->getRListLink();
		cout << "Router " << r_itr->getID() << " = { ";
		for(list<rLink>::iterator l_itr = lList.begin(); l_itr != lList.end(); ++l_itr){
			//cout << l_itr->getFlag() << " ";
			vector<double> l_stat = l_itr->getLstat();
			cout << "[ ";
			for(vector<double>::iterator s_itr = l_stat.begin(); s_itr != l_stat.end(); ++s_itr){
				cout << *s_itr << " ";
			}
			cout << "], ";
		}
		cout << "}" << endl;
	}
	cout << endl;
	//cout << "Total Load = " << stat.getLoadList() << endl;
	cout << "Forwarded = { ";
	vector<double> forwarded = stat.getForwardedList();
	for(vector<double>::iterator f_itr = forwarded.begin(); f_itr != forwarded.end(); ++f_itr){
		cout << *f_itr << " ";
	}
	cout << "}" << endl;

	cout << "Lost = { ";
		vector<double> lost = stat.getLostList();
		for(vector<double>::iterator l_itr = lost.begin(); l_itr != lost.end(); ++l_itr){
			cout << *l_itr << " ";
		}
		cout << "}" << endl;

	cout << "Objective = " << stat.getObjective() << endl;
	cout << "Time = " << getTimeAdvList().back() << " sec" << endl << endl;
}

/*measure the difference, in terms of nr of router, between current load and previous load
 */
int measurement::conf_diff(list<router> &rList, list<router> &prev_rList){
	int conf_change_count = 0;
	list<router>::iterator r_itr_left = rList.begin();
	list<router>::iterator r_itr_right = prev_rList.begin();
	while(r_itr_left != rList.end()){
		if(r_itr_right != prev_rList.end()){
			if(r_itr_left->getFlag() != r_itr_right->getFlag()){
				conf_change_count++;
			}
		}else{
			if(r_itr_left->getFlag() == 1)
				conf_change_count++;
		}
		r_itr_left++;
		r_itr_right++;
	}
	return conf_change_count;
}

/*display the input configuration file after parsing the CPLEX_OPL format configuration file
 */
void measurement::config_file(list<router> rList){
	list<rLink> lList;
	cout << "Configuration file" << endl;
	cout << "==================" << endl;
	cout << "K = " << nr_flows << endl;
	cout << "N = " << nr_links << endl;
	cout << "M = " << nr_routers << endl;

	cout << "T = [ ";
	for(vector<double>::iterator t_itr = traffic.begin(); t_itr != traffic.end(); ++t_itr)
		cout << *t_itr << " ";
	cout << "]" << endl;

	cout << "RC = [ ";
	for(list<router>::iterator r_itr = rList.begin(); r_itr != rList.end(); ++ r_itr)
		cout << r_itr->getCapacity() << " ";
	cout << "]" << endl;

	cout << "RP = [ ";
	for(list<router>::iterator r_itr = rList.begin(); r_itr != rList.end(); ++ r_itr)
		cout << r_itr->getPower() << " ";
	cout << "]" << endl;

	cout << "LC = [ ";
	for(list<router>::iterator r_itr = rList.begin(); r_itr != rList.end(); ++ r_itr){
		lList = r_itr->getRListLink();
		cout << "[ ";
		for(list<rLink>::iterator l_itr = lList.begin(); l_itr != lList.end(); ++l_itr){
			cout << l_itr->getCapacity() << " ";
		}
		cout << "]";
	}
	cout << " ]" << endl;

	cout << "LP = [ ";
	for(list<router>::iterator r_itr = rList.begin(); r_itr != rList.end(); ++ r_itr){
		lList = r_itr->getRListLink();
		cout << "[ ";
		for(list<rLink>::iterator l_itr = lList.begin(); l_itr != lList.end(); ++l_itr){
			cout << l_itr->getPower() << " ";
		}
		cout << "]";
	}
	cout << " ]" << endl << endl;
}

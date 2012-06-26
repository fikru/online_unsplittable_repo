#include <sys/time.h>
#include "statistics.h"
#include <iostream>
using namespace std;

//constructor
statistics::statistics(){
	forwardedList.clear();
	lostList.clear();
	objectiveList.clear();
	loadList.clear();
	objective = 0;
}

//set forwarded traffic list
void statistics::setForwardedList(double forwarded){
	this->forwardedList.push_back(forwarded);
}

//set lost traffic list
void statistics::setLostList(double lost){
	this->lostList.push_back(lost);
}

//set objective current
void statistics::setObjective(double objective){
	this->objective = objective;
}

//set objective
void statistics::setObjectiveList(double objective){
	this->objectiveList.push_back(objective);
}

//set load
void statistics::setLoadList(double load){
	this->loadList.push_back(load);
}

/*set traffic information*/
void statistics::setTraffic(std::vector<double> newTraffic){
	this->trafficCurr.clear();
	for(vector<double>::iterator t_itr = newTraffic.begin(); t_itr != newTraffic.end(); t_itr++)
		this->trafficCurr.push_back(*t_itr);
}

//set previous traffic
void statistics::setTrafficPrev(std::vector<double> prevTraffic){
	this->trafficPrev.clear();
	for(vector<double>::iterator t_itr = prevTraffic.begin(); t_itr != prevTraffic.end(); t_itr++)
			this->trafficPrev.push_back(*t_itr);
}

//get forwarded traffic list
std::vector<double> statistics::getForwardedList(){
	return this->forwardedList;
}

//get lost traffic list
std::vector<double> statistics::getLostList(){
	return this->lostList;
}

//get objective vector
std::vector<double> statistics::getObjectiveList(){
	return this->objectiveList;
}

//get load vector
std::vector<double> statistics::getLoadList(){
	return this->loadList;
}

//get objective current
double statistics::getObjective(){
	return this->objective;
}
/*get traffic statistics*/
std::vector<double> statistics::getTraffic(){
	return this->trafficCurr;
}

/*clear the forwarded traffic list
 */
void statistics::clearForwardList(){
	this->forwardedList.clear();
}
/*clear the lost traffic list
 */
void statistics::clearLostList(){
	this->lostList.clear();
}

///**get traffic difference between current and previous traffic*/
//vector<double> statistics::getTrafficDiff(){
//	vector<double> trafficToPack(this->trafficCurr);
//	for(vector<double>::iterator t_itr = trafficToPack.begin(); t_itr != trafficToPack.end(); ++t_itr){
//		for(vector<double>::iterator tp_itr = this->trafficPrev.begin(); tp_itr != this->trafficPrev.end(); ++tp_itr){
//			if(*t_itr == *tp_itr){
//				trafficToPack.erase(t_itr);
//				this->trafficPrev.erase(tp_itr);
//				t_itr = t_itr - 1;
//				break;
//			}
//		}
//	}
//	return trafficToPack;
//}

/**get traffic difference between current and previous traffic*/
vector<double> statistics::getTrafficDiff(){
	vector<double> trafficToPack(this->trafficCurr);
	vector<double>::iterator t_itr = trafficToPack.begin();
	for(vector<double>::iterator tp_itr = this->trafficPrev.begin(); tp_itr != this->trafficPrev.end(); ++tp_itr){
		*t_itr = *t_itr - *tp_itr;
		t_itr++;
	}
	return trafficToPack;
}

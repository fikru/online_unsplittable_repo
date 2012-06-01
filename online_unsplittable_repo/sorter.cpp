#include "global.h"
//#include "router.h"
//#include "rLink.h"
//#include "sorter.h"
#include <algorithm>

using namespace std;

bool rcompare(router& , router& );
bool lcompare(rLink& , rLink& );

/*constructor
 */
sorter::sorter(){};
/*destructor
 */
sorter::~sorter(){};
/*
 * 		compare each routers with the others
 * 		First - sort the routers according to:
 * 			actualCapacity/router power
 * 		Second - sort the links on each router
 */
void sorter::rsort(list<router> &rList) {
	list<rLink> lList;
	/*--Order the routes and links as per their efficiency using predicate
	 If the efficiency of two routers are the same, use their power consumption
	 for comparison --*/
	rList.sort(rcompare);
	#ifndef _REVERSE_SORT
		rList.reverse();
	#endif
	for (list<router>::iterator r_itr = rList.begin(); r_itr != rList.end(); ++r_itr) {
		lList = lsort(r_itr->getRListLink());
		r_itr->setRLinkList(lList);
	}
}

/**compares two routers efficiency = actualCapacity/power
 * @param lr right side router to compare
 * @param rr left side router to compare
 * @return boolean value indicating which of the two routers is more efficient
 */
bool rcompare(router& lr, router& rr) {
	double eff_lr, eff_rr; //efficiency of left and right router
	#ifdef _LINK_ROUTER_POWER_SORT				//if link power is added to the router power for sorting purpose
		eff_lr = lr.getActualCapacity() / (lr.getTotalPower());
		eff_rr = rr.getActualCapacity() / (rr.getTotalPower());
		//cout << eff_lr << " " << eff_rr << endl;
		/*-- if the efficiency of two routers are the same,
		 * then use their power consumption for comparison --*/
		if (eff_lr == eff_rr) {
			eff_lr = lr.getTotalPower();
			eff_rr = rr.getTotalPower();
		}
	#else
		eff_lr = lr.getActualCapacity()/(lr.getPower());
		eff_rr = rr.getActualCapacity()/(rr.getPower());
		//cout << eff_lr << " " << eff_rr << endl;
		/*-- if the efficiency of two routers are the same,
		 * then use their power consumption for comparison --*/
		if (eff_lr == eff_rr) {
			eff_lr = lr.getPower();
			eff_rr = rr.getPower();
		}
	#endif
	return eff_lr < eff_rr;
}

/*
 * NAME:		lsort
 * DESCRIPTION: sorts the links on a router
 * INPUTS:
 * 		Parameters: lList -- list of links to be sorted
 * OUTPUTS:
 * 		Return: sorted lList
 * PROCESS:
 * 		compare each links with all the other
 * 		sort the links on each router
 */
list<rLink> sorter::lsort(list<rLink> lList) {
	lList.sort(lcompare);
	#ifndef _REVERSE_SORT
		lList.reverse();
	#endif
	return lList;
}

/**compares two links efficiency = linkCapacity/power
 * @param rl right side link to compare
 * @param ll left side link to compare
 * @return boolean value indicating which of the two links is more efficient
 */
bool lcompare(rLink& ll, rLink& rl) {
	double eff_ll, eff_rl;
	eff_ll = ll.getCapacity() / (ll.getPower());
	eff_rl = rl.getCapacity() / (rl.getPower());
	/*-- if the efficiency of two routers are the same,
	 * then use their power consumption for comparison --*/
	if (eff_ll == eff_rl) {
		eff_ll = ll.getPower();
		eff_rl = rl.getPower();
	}
	return eff_ll < eff_rl;
}

/*shuffle the list of routers and the links thereof
 */
void sorter::shuffle(list<router> &rList){
	vector<router> v;
	list<router> rl;
	v.resize(rList.size());
	copy(rList.begin(), rList.end(), v.begin());
	random_shuffle(v.begin(), v.end());
	copy(v.begin(), v.end(), std::back_inserter(rl));
	rList = rl;
}

/*
 * NAME:		tsort
 * DESCRIPTION: sorts the incoming traffic flows
 * INPUTS:
 * 		Parameters: traffic -- list of traffic flows
 * OUTPUTS:
 * 		Return: sorted traffic
 * PROCESS:
 * 		compare each flows with all the other
 * 		sort the traffic flow
 */
void sorter::tsort(vector<double> &traffic){
	sort(traffic.begin(),traffic.end(),greater<double>());
}

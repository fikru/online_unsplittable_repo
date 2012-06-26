#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <vector>
#include <list>
#include "router.h"
#include "statistics.h"
#include "multistage.h"
#include "measurement.h"
#include "deviceSetup.h"
#include "parser.h"
#include "sorter.h"
#include "packing.h"

using namespace std;

/**@file global.h*/
//#define _ROUTER							//if sorting routers and links required
#define _TRAFFIC						//if ordering the flows required (descending order)
//#define _RESULT						///<to display some results to see if the algorithm is working properly or not
//#define _CONFFILE						///<to display the configuration file
//#define _REVERSE_SORT					///<sorting routers(only sorted based on router power) and links in opposite order to the optimal
//#define _ROUTER_POWER_SORT			///<sort routers based on their consumption
//#define _LINK_ROUTER_POWER_SORT	        ///<consider both link and router power consumption in ordering the routers
#define _FIRST_FIT						///<first fit algorithm
//#define _BEST_FIT						///<best fit algorithm

const int L_MAX_CAPACITY = 10000;			///<possible max link capacity
const int L_MIN_CAPACITY = 10;				///<possible min link capacity

//extern statistics mssr_stat, unused_instance;

extern int nr_flows; 					///<nr of traffic flows
extern int nr_routers;					///<nr of routers in the architecture
extern int nr_links;					///<nr of links in a router

extern vector<double> traffic;			///<traffic needed to be routed
extern vector<double> r_capacity;		///<vector of capacity for each router
extern vector<double> r_power;			///<vector of power consumption for each router
extern vector<double> l_capacity;		///<vector of capacity for each link on a  router
extern vector<double> l_power;			///<vector of power consumption of each link on a router
extern vector<vector <double> > all_l_capacity; 	///<vector of links on all routers
extern vector<vector <double> > all_l_power;		///<vector of power consumption of links on all routers

#endif /*GLOBAL_H_*/

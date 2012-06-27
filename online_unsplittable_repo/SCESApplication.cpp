/*
 * SCESApplication - server cluster energy saving application
 * Description: Server Cluster Energy Saving (SCES) Application is an energy saving scheme
 * 				in a distributed scenario (e.g. multistage software router). The algorithm is
 * 				meant to compare on-line differential energy saving scheme with the	optimal
 * Inputs:
 *		Parameters:
 *			configuration file (CPLEX format .dat file)
 * Outputs:
 *		Return: None
 *		Others:
 *			- output files
 *			- some statistical data (class)
 * Process:
 *		- parse the command line arguments
 *		- read the configuration file
 *		- create multistage software router architecture from configuration file
 *		- initialize some variables in the architecture
 *		- sort routers and traffic if required
 *		- check if there is any event (change in traffic load or configuration)
 *				- check if there is change in timestamp of the file
 *				- compare the load amount change in the configuration file
 *		- if there is some event, then start the packing
 *		- register different statistics and prepare data for plot
 */

//To open, read, write and manipulate file
#include <iostream>
#include <sstream>		//output file stream
#include <new>			//to create new object
#include <vector>
#include <list>
#include <assert.h>		//to check unexpected conditions in the code

//To obtain file status
#include <sys/types.h>
#include <sys/stat.h>	//for checking file last modified time

//non-standard library
#include "global.h"

using namespace std;

extern list<router> rList;					//list of routers in the architecture - also shows current configuration
/*bin packing statistics*/
statistics mssr_stat, 						//statistics from the beginning of the simulation
		   unused_instance; 				//empty packing statistics

int main(int argc, char** argv){
	parser parse;
	multistage *mssr;						//a multistage software router architecture
	measurement probe;						//packing time
	deviceSetup setup;
	sorter sortOut;
	packing bin;
	int mssrCapacity = 0, mssrPower = 0,	//multistage router architecture parameters
		firstRun,							//number of runs
		confChangeCount;					//count the number of configuration changes - in terms of the difference
											//between number of current configuration devices and previous one
	double lastModTime,nowModTime,referenceTime;
	double  totalTraffic, obj;
	vector<double> trafficToPack;
	list<router> prevRList; 				//list of routers in the architecture - also shows previous configuration

	/*declaration for output file*/
	ofstream out_file;						//output file for plot data
	ios_base::openmode mode;				//mode of the out_file

	/*parse the command line*/
	parse.cmd_parser(argc,argv);

	/*configuration file last modified time*/
	probe.setModTime(argv[2]);
	lastModTime = probe.getModTime();

	/*Check if the file = argv[2] is modified*/
	while(1){
		/*check file last modification time*/
		probe.setModTime(argv[2]);
		nowModTime = probe.getModTime();

		/*If file is modified, run the heuristic equivalent of the CPLEX optimization*/
		if (nowModTime != lastModTime){
			firstRun=1;
			totalTraffic = 0;

			//why the erratic behavior without sleep?
			sleep(1);

			/* Reset the statistics for current heuristic run*/
			mssr_stat = unused_instance;

			/* read the configuration file*/
			parse.input_parser(argv[2]);

			/* create the multistage architecture*/
			mssr = new (nothrow) multistage(0, "mssr", mssrCapacity, mssrPower, 0, nr_routers);
			assert(mssr != 0);		//error allocating memory space

			/*get list of routers of the architecture*/
			rList = mssr->getRList();

			/*	 compute and set actual routing capacity of the architecture and the routers
			 *   and the routers' total power consumption(= router power + sum of link power)
			 */
			setup.actualCapacity(mssr, rList);
			/*reset all the router statistics such as usage flag, residual capacity, link traffic statistics, etc*/
			setup.empty_routers(rList);
			/*total load list*/
			mssr_stat.setLoadList(0);

			/* if sorting routers and links required, sort them as per their
			 * efficiency = capacity/power. Else shuffle the list*/
			#ifdef _RLSORT
				sortOut.rsort(rList);
			#else
				sortOut.shuffle(rList);
			#endif

			/*pack the traffic if the total traffic doesn't exceed the architecture routing capacity*/
			while(totalTraffic <= mssr->getCapacity()){

				/*set traffic modification time*/
				probe.setModTime(argv[2]);

				/*if there is a change in load, reconfigure the architecture to handle the new traffic*/
				if(probe.getModTime() != lastModTime){

					//why the erratic behavior without sleep?
					sleep(1);

					/*set last file modification time*/
					lastModTime = probe.getModTime();

					/*obtain the traffic statistics from shell generated data file*/
					parse.input_traffic(argv[2]);

					/* sort the traffic in decreasing order*/
					#ifdef _TSORT
						sortOut.tsort(traffic);
					#endif

					/*current traffic*/
					mssr_stat.setTraffic(traffic);

					/*sum of the input traffic flows*/
					totalTraffic = probe.getSum(traffic);

					if(totalTraffic != mssr_stat.getLoadList().back()){
						obj = 0;
						/*-- re-pack only the already used routers if the load is decreasing
						 *   else keep current solution and add any additional resource required
						 *   If the load is decreasing we just reset the routers*/
						if(totalTraffic < mssr_stat.getLoadList().back()){
							/*-- empty the routers before packing - reset the router to default for the next run --*/
							setup.empty_routers(rList);
							/*-- clear previous statistics --*/
							mssr_stat.clearForwardList();
							mssr_stat.clearLostList();
							trafficToPack = traffic;
						}else{
							/*-- empty the routers before packing - reset the router to default for the next run --*/
							setup.part_empty_routers(rList);
							/*-- clear previous statistics --*/
							mssr_stat.clearForwardList();
							mssr_stat.clearLostList();
							trafficToPack = traffic;
							/*-- decrease an already packed amount (previous load) from the current load
							 * we just need some additional routers to handle the extra traffic --*/
//							if(!firstRun){
//								//trafficPrev = mssr_stat.getTraffic();
//								trafficToPack = mssr_stat.getTrafficDiff();
//							}else
//								trafficToPack = traffic;
//							//previous objective value - to be added with the objective obtained after the increase in traffic packed
							obj = mssr_stat.getObjective();
						}

						/*set the start of packing*/
						probe.setStart();
						if(firstRun)
							referenceTime = probe.getStart(); 				//save the reference time

						/*-- bin packing algorithm - packs the load to free routers and update statistics --*/
						mssr_stat = bin.bin_packing(trafficToPack, rList);

						/*-- stamp the end of simulation and compute simulation time --*/
						probe.setEnd();
						/*set simulation time*/
						probe.setTimeAdvList();

						/*save current traffic as previous*/
						mssr_stat.setTrafficPrev(traffic);

						//Compare the number of devices involved in current configuration and the previous one
						if(firstRun)
							prevRList.clear();         //use the first configuration
						confChangeCount = probe.conf_diff(rList, prevRList);
						prevRList = rList;

						/*set the objective*/
						mssr_stat.setObjective(mssr_stat.getObjective() + obj);
						mssr_stat.setObjectiveList(mssr_stat.getObjective() + obj);


						/*-- display results --*/
						#ifdef _RESULT
							probe.result(rList, mssr_stat);
						#endif

						/*display configuration file*/
						#ifdef _CONFFILE
							probe.config_file(rList);
						#endif

						/*write plot data to file*/
						if(mssr_stat.getLoadList().back() == 0 || firstRun)		//if it is the first run, then create a new empty file
							mode = ios::trunc;
						else										//else append data to file
							mode = ios::app;
						out_file.open("plotdata_unsorted.txt", mode);
						if( !out_file ) { // file couldn't be opened
							cerr << "Error: file could not be opened" << endl;
							exit(1);
						}
						if(mssr_stat.getLoadList().back() == 0 || firstRun){
							out_file << "Load" << "\t\t" << "Objective" << "\t" << "conf diff" <<  "\t" << "sim time" << endl;
							out_file << "====" << "\t\t" << "=========" << "\t" << "=========" <<  "\t" << "========" << endl;
						}
						out_file << totalTraffic << "\t\t" << mssr_stat.getObjective() << "\t\t\t" << confChangeCount <<  "\t\t\t" << probe.getTimeAdvList().back() << endl;
						out_file.close();

//						cout << totalTraffic << "\t\t" << mssr_stat.getObjective() << "\t\t\t" << confChangeCount <<  "\t\t\t" << probe.getTimeAdvList().back() << endl;
						/*-- the first run already done, reset the first run indicator --*/
						if(firstRun == 1)
							firstRun = 0;

						/*save current load for plot*/
						mssr_stat.setLoadList(totalTraffic);
					}
				}
				/* If all the traffic is packed, break this while and continue to check if there
				 * is configuration change*/
				if(mssr_stat.getLoadList().back() >= mssr->getCapacity()){
					break;
				}
			}

			/*deallocated memory assigned to mssr and cleanup the members*/
			delete mssr;
		}
	}
	return 0;
}

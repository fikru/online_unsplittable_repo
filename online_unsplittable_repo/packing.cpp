#include <iostream>
#include <limits>
#include "packing.h"
using namespace std;

extern statistics mssr_stat;

/**default constructor
 */
packing::packing(){};
/**default destructor
 */
packing::~packing(){};

/*Bin packing algorithms
 *REFERENCE: <a href="BPchapter.pdf" target="_blank"><b>Approximation algorithms for bin packing: A survey</b></a>.
 * 		First fit (ff) - implemented
 * 		First fit decreasing (ffd) - implemented
 * 		Best fit (bf) - not yet implemented
 * 		Best fit decreasing (bfd) - not yet implemented
 * 		Worst fit (wf) - not yet implemented
 * 		Worst fir decreasing (wfd) - not yet implemented
 *@param trafficToPack traffic to be packed
 *@param r_list list of routers in the multistage architecture
 *@return mssr_stat multistage architecture statstics
 */
statistics packing::bin_packing(vector<double> &trafficToPack, list<router> &r_list){
	#ifdef _FIRST_FIT
		ff(trafficToPack, r_list);
	#endif
	/*not yet implemented*/
	#ifdef _BEST_FIT
		r_list = bf(trafficToPack, r_list);
	#endif

	return mssr_stat;
}

/*First Fit bin packing algorithm.
 *forward a flow using the first available backend router and the first available
 *forward link on that router which have a capacity to handle the flow. If such a
 *forward router is not available, drop the flow.
 * INPUTS:
 * 		parameters: traffic, r_list
 * OUTPUTS:
 * 		return: none
 * PROCESS:
 * 		. forward a flow using the first available router and the first link available on that
 * 		  router that can handle the flow
 * 		. update different flow statistics such as residual capacity of a currently used link,
 * 		  lost flows, objective value, etc., on the way
 */
void packing::ff(vector<double> &trafficToPack, list<router> &r_list){
	vector<double>::iterator t_itr;
	list<rLink> lList;
	double l_residual;					//check the residual capacity of the link
	double r_residual;					//check the residual actual capacity of the router
	int objective = 0;					//total power consumption of devices used in current flow forwarding
	for(t_itr = trafficToPack.begin(); t_itr != trafficToPack.end(); ++t_itr){
		int packed_flag = 0;								//set if a flow is packed
		for(list<router>::iterator r_itr = r_list.begin(); r_itr != r_list.end();++r_itr){
			r_residual = r_itr->getResidual();
			if(*t_itr <= r_residual){
				lList = r_itr->getRListLink();
				for(list<rLink>::iterator l_itr = lList.begin(); l_itr != lList.end();++l_itr){
					/*-- break this loop if:
					 * 		- if a flow is packed
					 */
					l_residual = l_itr->getResidual();
					if(*t_itr <= l_residual){				//if a traffic flow is less than residual capacity of a link, use this link for forwarding the flow
						//update objective by link power consumption if this link is just turned on to handle current flow
						if(!l_itr->getFlag()){
							l_itr->setFlag(1);
							objective += l_itr->getPower();
						}
						///if flow fits into a link, compute the residual capacity of both the router and the link
						r_residual -= *t_itr;
						l_residual -= *t_itr;
						l_itr->setResidual(l_residual);
						l_itr->setLstat(*t_itr);
						r_itr->setRLinkList(lList);			//replace router *r_itr link with the updated rlList
						r_itr->setResidual(r_residual);
						mssr_stat.setForwardedList(*t_itr);			//save packed flow
						packed_flag = 1;					//set the flag indicating the flow is packed in respective router and link
						break;
					}
				}
			}
			if(packed_flag == 1){							//if the router is just packed with a flow,
				//update objective by router power consumption if this router is just turned on to handle current flow
				if(!r_itr->getFlag()){						//add its power consumption to objective and
					r_itr->setFlag(1);						//go to next flow
					objective += r_itr->getPower();
				}
				break;
			}
		}
		if(!packed_flag){									//if flow not forwarded, register it as lost
			mssr_stat.setLostList(*t_itr);
		}
	}
	mssr_stat.setObjective(objective);								//save objective function
}



#include "deviceSetup.h"
#include "global.h"

extern list<rLink> lList; 		//list of routers in the architecture


/**constructor
 */
deviceSetup::deviceSetup(){};
/**destructor
 */
deviceSetup::~deviceSetup(){};

/* NAME: 			actualCapacity()
 * DESCRIPTION: 	computes the actual capacity (min(router capacity, sum of all link capacity on that router)
 * 				 	of each backend routers
 * INPUTS:
 * 		Parameters: list<routers> rList
 * OUTPUTS:
 * 		Return: none
 * PROCESS:
 * 		. Give priority to higher capacity links to compute actual router capacity (It is reasonable to give
 * 		  high priority for hign capacity links. Because to reach that capacity with less capacity links, the
 * 		  power consumption becomes higher)
 * 		. Sum up link capacity per router
 * 		. sum up the corresponding power consumption
 * 		. Update router capacity with min(router capacity, sum of link capacity of that router)
 */
void deviceSetup::actualCapacity(multistage *mssr, list<router> &rList){
	list<rLink> lList;   							//list of links on router r
	int mssrCapacity = 0, mssrPower = 0;			//actual capacity and power consumption of mssr architecture
	for(list<router>::iterator r_itr=rList.begin(); r_itr != rList.end();++r_itr){
		int link_total_cap = 0;			  			//total link capacity per router
		int l_capacity = L_MAX_CAPACITY; 			//the maximum link capacity a router can have
		double totalP = 0;
		lList = r_itr->getRListLink();
		while(l_capacity >= L_MIN_CAPACITY){        //a protection not to go below the smallest link considered
			for(list<rLink>::iterator l_itr=lList.begin(); l_itr != lList.end();++l_itr){
				if(l_itr->getCapacity() == l_capacity){
					link_total_cap += l_itr->getCapacity();
					totalP += l_itr->getPower();
					if (link_total_cap >= r_itr->getCapacity()){
						list<rLink>::iterator l_ptr = ++l_itr;
						/*-- 1. reset all the capacity **after this** pointer whose capacity <= l_capacity to zero --*/
						for(list<rLink>::iterator l_itr=l_ptr; l_itr != lList.end();++l_itr){
							if(l_itr->getCapacity() <= l_capacity){
								l_itr->setCapacity(0);
								l_itr->setResidual(0);
							}
						}
						/*-- 2. reset all the capacity < l_capacity from the beginning to this pointer to zero --*/
						for(list<rLink>::iterator l_itr=lList.begin(); l_itr != l_ptr;++l_itr){
							if(l_itr->getCapacity() < l_capacity){
								l_itr->setCapacity(0);
								l_itr->setResidual(0);
							}
						}
						l_capacity = 1;
						break;
					}
				}
			}
			l_capacity = l_capacity/10;
		}
		totalP += r_itr->getPower();
		r_itr->setTotalPower(totalP);
		if (link_total_cap < r_itr->getCapacity()){
			r_itr->setActualCapacity(link_total_cap);
			r_itr->setResidual(link_total_cap);
		}else{
			r_itr->setActualCapacity(r_itr->getCapacity());
			r_itr->setResidual(r_itr->getCapacity());
			r_itr->setRLinkList(lList);
		}
		mssrCapacity += r_itr->getActualCapacity();
		mssrPower += r_itr->getPower();
	}
	mssr->setCapacity(mssrCapacity);
	mssr->setPower(mssrPower);
}


/* 	Name:			empty_routers()
 *
 * 	Description:	reset the flag of each router and links thereof to indicate
 * 					that it is empty and set the residual to max capacity
 * 	INPUTS:
 * 		Parameters: list<routers> rList
 *	OUTPUTS:
 * 		Return: none
 */
void deviceSetup::empty_routers(list<router> &rList){
	for(list<router>::iterator r_itr = rList.begin(); r_itr != rList.end(); ++r_itr){
		lList = r_itr->getRListLink();
		r_itr->setFlag(0);
		r_itr->setResidual(r_itr->getActualCapacity());
		for(list<rLink>::iterator l_itr = lList.begin(); l_itr != lList.end(); ++l_itr){
			l_itr->setFlag(0);
			l_itr->setResidual(l_itr->getCapacity());
			l_itr->clearLstat();
		}
		r_itr->setRLinkList(lList);
	}
}

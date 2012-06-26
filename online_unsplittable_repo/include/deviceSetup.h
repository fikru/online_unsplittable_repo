#ifndef DEVICESETUP_H_
#define DEVICESETUP_H_

#include "global.h"

/**sets up the multistage architecture configuration
 * @brief initially it is needed to initialize the multistage architecture
 * as well as its internal components
 */
class deviceSetup {
public:
	/**constructor
	 */
	deviceSetup();
	/**destructor
	 */
	~deviceSetup();
	/**computes the actual capacity of backend routers
	 * @brief the actual capacity of a router is limited either by the total capacity of its links or by its routing capacity
	 * @param mssr the multistage architecture
	 * @param rList the list of routers in the multistage architecture
	 */
	void actualCapacity(multistage *, list<router> &);
	/**
	 * resets some parameters of the router; such as its initial residual capacity
	 * @param rList the list of routers in the multistage architecture
	 */
	void empty_routers(list<router> &);
	/**
	 * empty only the router traffic statistics
	 * @param rList the list of routers in the multistage architecture
	 */
	void part_empty_routers(list<router> &);
};
#endif //DEVICESETUP_H_

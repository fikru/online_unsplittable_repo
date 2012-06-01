#ifndef PACKING_H_
#define PACKING_H_

#include "global.h"

/**Implementation of bin packing algorithms
 *@brief REFERENCE: Approximation algorithms for bin packing: A survey
 * 		- First fit (ff) - implemented
 * 		- First fit decreasing (ffd) - implemented
 * 		- Best fit (bf) - not yet implemented
 * 		- Best fit decreasing (bfd) - not yet implemented
 * 		- Worst fit (wf) - not yet implemented
 * 		- Worst fir decreasing (wfd) - not yet implemented
 */
class packing{
public:
	/**default constructor
	 */
	packing();
	/**default destructor
	 */
	~packing();
	/**Interface to packing algorithms
	 *@param trafficToPack input traffic for packing
	 *@param r_list list of routers to pack
	 *@return packing statistics
	 */
	statistics bin_packing(vector<double> &, list<router> &);

	/**First Fit bin packing algorithm.
	 *@brief forward a flow using the first available backend router and the first
	 *available link on that router which have a capacity to handle the flow. If
	 *such a router is not available, drop the flow.
	 *@param trafficToPack input traffic for packing
	 *@param r_list list of routers to pack
	 *@return r_list list of routers
	 * PROCESS:
	 * 		- forward a flow using the first available router and the first link available on that
	 * 		  router that can handle the flow
	 * 		- update different flow statistics such as residual capacity of a currently used link,
	 * 		  lost flows, objective value, etc., on the way
	 */
	void ff(vector<double> &, list<router> &);

};
#endif /* PACKING_H_ */

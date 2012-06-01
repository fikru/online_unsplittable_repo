#ifndef SORTER_H_
#define SORTER_H_


#include <list>
/**sort routers and links according to their efficiency; also used to sort the input traffic
 * @brief After reading the list of routers from a text configuration file, the algorithm
 * sorts the routers according to their efficiency if _ROUTER_POWER_SORT is defined.
 *
 * Procedure:
 * \li sort the routers according to (only router power considered for sorting the routers):
 *		\f{eqnarray}
 *			\eta_{_{NIC-}} = \frac{C_{r}}{P_{r}}
 *		\f}
 *	OR (both link and router power considered for sorting the routers)
 *		\f{eqnarray}
 *			\eta_{_{NIC+}} = \frac{C_{r}}{P_{r} + \sum_{i=1}^N{P_{rl}}}
 *		\f}
 * \li sort the links on each router according to:
 *		\f{eqnarray}
 *			\eta_{_l} = \frac{C_{rl}}{P_{rl}}
 *		\f}
 * @param rList list of routers
 * @return rList ordered list of routers
 */
class sorter{
public:
	/**constructor
	 */
	sorter();
	/**destructor
	 */
	~sorter();

	/**compare each routers with the others
	 *First - sort the routers according to: actualCapacity/router power
	 *Second - sort the links on each router
	 */
	void rsort(list<router> &);

	/**sorts the links on a router
	 *@param lList list of links to be sorted
	 *@return sorted lList
	 */
	list<rLink> lsort(list<rLink> );

	/**shuffle the list of routers and the links thereof
	 * @param rList list of routers
	 */
	void shuffle(list<router> &);

	/**sorts the incoming traffic flows
	 *@param traffic list of traffic flows
	 *@return sorted traffic
	 */
	void tsort(vector<double> &);
};
#endif /*SORTER_H_*/

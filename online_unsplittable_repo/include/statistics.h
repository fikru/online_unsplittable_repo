#ifndef STATISTICS_H_
#define STATISTICS_H_

#include <vector>

/**Statistics class to collect device states. General traffic statistics - such as lost traffic, total forwarded traffic,
 * 	total power consumption (objective)
 */
class statistics{
private:
	/**Statistics for troubleshooting*/
	double objective;								/**<current total power consumption of the architecture*/
	std::vector<double> forwardedList;				/**<forwarded flows*/
	std::vector<double> lostList;					/**<lost flows*/
	/**statistics for plotting*/
	std::vector<double> objectiveList;				/**<power consumption of the architecture used in forwarding current flows*/
	std::vector<double> loadList;
	std::vector<double> trafficCurr;				/**<current input traffic from configuration file*/
	std::vector<double> trafficPrev;				/**<previous input traffic*/
	std::vector<double> trafficDiff;				/**<traffic difference between trafficPrev and traffic*/

public:
	///constructor
	statistics();

	///destructor
	~statistics(){};

	/**get forwarded traffic list
	 * @return forwarded traffic list
	 */
	std::vector<double> getForwardedList();

	/**get lost traffic list
	 * @return lost traffic list
	 */
	std::vector<double> getLostList();

	/**get advanced time
	 * @return time required for packing
	 */
	std::vector<long> getTimeAdvList();

	/**get objective
	 * @return list of total power consumption of the architecture
	 */
	std::vector<double> getObjectiveList();

	/**get load
	 * @return list of loads
	 */
	std::vector<double> getLoadList();

	/**get objective previous
	 * @return total power consumption of the previous configuration of the architecture
	 */
	double getObjective();

	/**get traffic statistics*/
	std::vector<double> getTraffic();

	/**get traffic difference between current and previous traffic*/
	std::vector<double> getTrafficDiff();

	/**set forwarded traffic list
	 * @param forwarded routed traffic
	 */
	void setForwardedList(double );

	/**set lost traffic list
	 * @param lost traffic that is not routed because of lack of capacity
	 */
	void setLostList(double );

	/**set current objective
	 * @param objective power consumption of current configuration
	 */
	void setObjective(double );

	/**put objective in the list
	 * @param objective power consumption of current configuration
	 */
	void setObjectiveList(double );

	/**set load
	 * @param load current traffic amount to be forwarded
	 */
	void setLoadList(double );

	/**clear the forwarded traffic list
	 */
	void clearForwardList();
	/**clear the lost traffic list
	 */
	void clearLostList();

	/**set current traffic
	 *@param newTraffic input traffic
	 */
	void setTraffic(std::vector<double> newTraffic);

	/**set previous traffic
	 * @param prevTraffic input traffic
	 */
	void setTrafficPrev(std::vector<double> prevTraffic);

};

#endif /*STATISTICS_H_*/

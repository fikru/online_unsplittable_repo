#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_

#include <sys/time.h>

/**used to collect input and output information, gather results
 */
class measurement{
private:
	timeval start, end;
	double startTime, endTime;
	double file_last_mod_time,file_now_mod_time;
	vector<double> time_advancedList;			/**<time_advance = end_time - start_time*/
public:
	/**
	 * constructor
	 */
	measurement();

	/**
	 * destructor
	 */
	~measurement();

	/**saves the beginning of the simulation run*/
	void setStart();

	/**saves the end of the simulation run*/
	void setEnd();

	/**sets simulation time list
	 */
	void setTimeAdvList();

	/**set file modification time
	 * @param f_name file name
	 */
	void setModTime(char* );

	/**gets simulation time list
	 */
	vector<double> getTimeAdvList();

	/**get simulation start time
	 * @return simulation start time - since epoch time
	 */
	double getStart();

	/**get simulation end time
	 * @return simulation end time - since epoch time
	 */
	double getEnd();


	/**gets file modification time
	 * @return modification time
	 */
	double getModTime();

	/**display different packing statistics
	 * @brief the statistics dispalyed include simulation time, objective, traffic forwarded and lost, etc
	 * @param rList list of backend routers
	 * @param stat statistics about the packing
	 */
	void result(list<router> ,statistics );

	/**compute summation of a vector of any type T
	 * @param vectorin
	 * @return sum of an input vector
	 */
	double getSum(vector<double> &);

	/**compare two vectors
	 * @brief take any two vectors of any type and compare them
	 * @param vector1 the first vector (the reference)
	 * @param vector2 the second vector to be compared to the first vector
	 */
	bool compareVector(vector<double> &, vector<double> &);

	/**measure the difference, in terms of nr of router, between current load and previous load
	 * @brief this difference is used to compare with the optimal solution
	 * @param rList list of back end routers (current configuration)
	 * @param prev_rList list of back end routers (previous configuration)
	 * @return conf_change_count difference between the two configuration
	 */
	int conf_diff(list<router> &, list<router> &);

	/**display the input configuration file parsed
	 *@brief it is a check point to verify if the text configuration is parsed correctly or not
	 *@param rList list of backend routers
	 *@return configuration file
	 */
	void config_file(list<router> );
};

#endif /*MEASUREMENT_H_*/

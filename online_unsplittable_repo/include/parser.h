#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>		//cout
#include <stdlib.h>		//exit
#include <string>		//string
#include <fstream>		//ifstream

using namespace std;

/**parse command line parameters and input configuration file
 *
 */
class parser{
public:
	/**constructor
	 */
	parser();
	/**destructor
	 */
	~parser();

	/**parse command line arguments
	 *@brief input parameters for the simulation are taken from command line.
	 *@param argc number of input arguments
	 *@param argv input arguments
	 */
	void cmd_parser(int, char** );

	/**parse backend router parameters from CPLEX_OPL configuration file
	 * @brief different internal components parameters are read from a text input file
	 *@param config_file CPLEX_OPL formate configuration file
	 *@return traffic input traffic
	 *@return r_list list of routers class
	 */
	void input_parser(string );

	/**parse input traffic
	 * @brief this function is dedicated to get the traffic line from the configuration
	 * file and convert it into a traffic vector
	 * @param config_file the name of the configuration file
	 */
	void input_traffic(string );

	/** @brief parse the command line.
	 * @param program_name the name of the executable
	 * @param exit_code if the programs exits without execution, indicate why it is not
	 */
	void usage(const char* , int );

	/**check if an integer value is odd or not
	 */
	bool isOdd(int );
};

/**command line usage information
 */
inline void parser::usage(const char* program_name, int exit_code) {
	cout << "\n Usage: " << program_name << " options" << " [configuration_file]"
		 << "\n\n"
	     << "options:\n"
	     << "-d --data \t CPLEX_OPL format input configuration file\n"
	     << "-h --help \t Display this message and exit\n"
	     << "-v --verbose \t Print verbose message\n";
	exit(exit_code);
}

/*
 * isOdd - check if a parameter (integer value) is odd or even
 */
inline bool parser::isOdd(int count)
{
  if ( count % 2 == 0 )
     return false;
  else
     return true;
}

#endif /*PARSER_H_*/

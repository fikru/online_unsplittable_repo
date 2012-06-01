#include "parser.h"
#include <vector>		//vector
#include <getopt.h> 	//to parse the command line options
#include <sstream>		//istringstream

int nr_flows, nr_routers, nr_links;
vector<double> traffic;
vector<double> r_capacity, r_power;
vector<double> l_capacity,l_power;
vector<vector <double> > all_l_capacity;
vector<vector <double> > all_l_power;

/**constructor
 */
parser::parser(){};
/**destructor
 */
parser::~parser(){};
/*
 * parse the command line parameters
 */
void parser::cmd_parser(int argc, char** argv){
	string 	data;
	/*-- definition to parse the command line --*/
	const char* program_name = "SCESApplication";
	int next_option;					//getopt_long() return value
	int verbose = 0;					//whether to display verbose message
	const char* short_option="hf:v";    	//command line short option specifier. The colon means the option before takes an argument
	//command line long option specifier
	const struct option long_option[] ={
			{"help", 0, NULL, 'h'},
			{"data", 1, NULL, 'd'},
			{"verbose", 0, NULL, 'v'},
			{NULL, 0, NULL, 0} 			//required at end of array
	};
	/*-- parse the command line arguments --*/
	do{
		next_option=getopt_long(argc, argv, short_option, long_option, NULL);
		switch(next_option)
		{
		case 'h': 	//-h or --help
			/*User requested usage information. Print it to standard output and exit with exit code 0 (normal termination)*/
			usage(program_name, 0);
			break;
		case 'd': 	//-d or --data
			/*this option takes an argument, the name of the input configuration file*/
			data = argv[2];
			break;
		case 'v': 	//-v or --verbose
			verbose = 1;
			break;
		case '?':	//the user specified an invalid option
			/*print usage information to standard output, and exit with exit code 1(abnormal termination)*/
			usage(program_name, 1);
			break;
		case -1:	//Done with the options
			break;
		default:	//something else: unexpected
			abort();
			break;
		}
	}while(next_option != -1);	//done with options. OPTIND points to first nonoption argument

	if(verbose){
		int i;
		for(i = optind; i < argc; ++i){
			cout << "Argument: " << argv[i];
		}
	}
}

/*read backend router parameters from CPLEX_OPL configuration file
 */
void parser::input_parser(string config_file)
{
	double t;
	string nextToken, param;
	int count = 0, paramGroup;
	ifstream input_data;
	input_data.open(config_file.c_str());

	if (input_data.fail()) {
		cerr << "Unable to open file for reading " + config_file << endl;
	}

	while (input_data >> nextToken) {
		if(nextToken == "K" || nextToken == "N" || nextToken == "M"){
			paramGroup = 0;
			param = nextToken;
			count = 1;
		}else if(nextToken == "T" || nextToken == "RC" || nextToken == "RP"){
			paramGroup = 1;
			param = nextToken;
			count = 1;
		}else if(nextToken == "LC" || nextToken == "LP"){
			paramGroup = 2;
			param = nextToken;
			count = 1;
		}
		/*-- get the number of flows, routers and links per router
		 * from the configuration file
		 */
		if(count == 3 && paramGroup == 0){
			string sb;
			istringstream s(nextToken);
			while(s){
				if (!getline(s,sb,';'))
					break;
				istringstream vi(sb);
				vi>>t;								// >> operator converts the vi string to double
				if(param == "K"){
					nr_flows = t;
				}else if(param == "N"){
					nr_links = t;
				}else if(param == "M"){
					nr_routers = t;
				}
			}
		}
		/*-- if the line start with T, RC or RP,
		 * update the corresponding vector with data from the configuration file--*/
		if(count == 4 && paramGroup == 1){
			string sr;
			istringstream s(nextToken);
			while(s){
				if (!getline(s,sr,','))
					break;
				istringstream vi(sr);
				vi>>t;								// >> operator converts the vi string to double
				if(param == "RC"){
					r_capacity.push_back(t);
//				}else if(param == "T"){
//					traffic.push_back(t);
				}else if(param == "RP"){
					r_power.push_back(t);
				}
			}
		}

		/*-- if a line start with LC or LP,
		 * update the corresponding vector with data from the configuration file--*/
		if(isOdd(count) && count >= 5 && paramGroup == 2){
			string sl;
			l_capacity.clear(),l_power.clear();
			istringstream s(nextToken);
			while(s){
				if (!getline(s,sl,','))
					break;
					istringstream vi(sl);
					vi>>t;
					if(param == "LC"){
						l_capacity.push_back(t);
					}else if(param == "LP"){
						l_power.push_back(t);
					}
			}
			if(param == "LC"){
				all_l_capacity.push_back(l_capacity);
			}else if(param == "LP"){
				all_l_power.push_back(l_power);
			}
		}
		++count;
	}
	input_data.close();
}

/*obtain the traffic statistics from shell generated data file--*/

void parser::input_traffic(string config_file){
	double t;
	string sr, nextToken;
	ifstream in_file;			//input file - configuration file
	traffic.clear();

	in_file.open(config_file.c_str());
	if (in_file.fail()) {
		cerr << "Unable to open file for reading " + config_file << endl;
	}

	while (in_file >> nextToken) {
		if(nextToken == "T"){
			in_file >> nextToken >> nextToken >> nextToken; //access to the 4th column (list of traffic)
			istringstream s(nextToken);
			while(s){
				if (!getline(s,sr,','))
					break;
				istringstream vi(sr);
				vi>>t;								// >> operator converts the vi string to double
				traffic.push_back(t);
			}
			break; //read the traffic and finish
		}
	}
	in_file.close();
}

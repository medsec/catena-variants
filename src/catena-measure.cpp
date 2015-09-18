#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <getopt.h>
#include <algorithm>

#include "catenafactory.hpp"

using namespace Catena_Variants;

#define HASH_PER_KiB 16 //1024/64

//Parameters
static std::string alg = "";
static std::string graph= "";
static std::string fuh = "";
static std::string fah = "";
static std::string rl = "";
static std::string pl = "";
static std::string stru = "";
static int mg = -1;
static int g = -1;
static int iterations = -1;

void print_usage(char **argv){
	std::cerr << "Usage: " << argv[0] <<" --algorithm ALG --graph GRAPH" <<
		" --fullhash FULLH\n" <<
		" --fasthash FASTH --random_layer RL --iterations I [--lambda L]\n"<< 
		" [--min_garlic MG] [--garlic G]\n" << std::endl;
	
	std::cerr << "\t-a, --algorithm ALG\talgorithm to use(formal: Flap)\n" <<
		"\t-g, --graph GRAPH\tgraph to use(formal: F)\n" <<
		"\t-u, --fullhash FULLH\tregular hash function(formal: H)\n" <<
		"\t-f, --fasthash FASTH\thash function used for consecutive calls\n" <<
			"\t\t\t\t(formal: H')\n" <<
		"\t-r, --random_layer RL\tfirst layer(formal: Gamma)\n" <<
		"\t-h, --phi_layer PL\textra layer(formal: Phi)\n" <<
		"\t-i, --iterations I\tnumber of iterations used to determine the \n"<< 
			"\t\t\t\truntime. Higher values increase stability\n" <<
		"\t-t, --structure L\t\tGraph structure. Default depends on graph\n"<<
		"\t-m, --min_garlic MG\tLower memory cost(formal: g0). Default\n"<<
			"\t\t\t\tdepends on graph\n"
		"\t-c, --garlic G\t\tmemory cost(formal:g). Default depends on graph"<<
		std::endl;
	std::cerr << "Algorithms - possible values for ALG:\n" <<
		CatenaFactory::instance().getAlgorithmsText() << 
		"Graphs - possible values for GRAPH:\n" <<
		CatenaFactory::instance().getGraphsText() <<
		"FullHashes - possible values for FullH:\n" <<
		CatenaFactory::instance().getFullHashesText() <<
		"FastHashes - possible values for FastH:\n" <<
		CatenaFactory::instance().getFastHashesText() <<
		"RandomLayers - possible values for RL:\n" <<
		CatenaFactory::instance().getRandomLayersText() <<
		"PhiLayers - possible values for PL:\n" <<
		CatenaFactory::instance().getPhiLayersText() <<
		std::endl;
}

int chkparams(){
	if(mg > 63){
		std::cerr << "MinGarlic to large. Limit is 63" << std::endl;
		return 1;
	}
	if(g > 63){
		std::cerr << "Garlic to large. Limit is 63" << std::endl;
		return 1;
	}
	//check for required parameters
	if(alg.empty()){
		std::cerr << "Algorithm required but missing" << std::endl;
		return 1;
	}
	if(graph.empty()){
		std::cerr << "Graph required but missing" << std::endl;
		return 1;
	}
	if(fuh.empty()){
		std::cerr << "FullHash required but missing" << std::endl;
		return 1;
	}
	if(fah.empty()){
		std::cerr << "FastHash required but missing" << std::endl;
		return 1;
	}
	if(rl.empty()){
		std::cerr << "RandomLayer required but missing" << std::endl;
		return 1;
	}
	if(pl.empty()){
		std::cerr << "PhiLayer required but missing" << std::endl;
		return 1;
	}
	if(iterations < 1){
		std::cerr << "Iterations required but missing" << std::endl;
		return 1;
	}
	//Don't test for password. It can be empty
	return 0;
}

void invalid(std::string s)
{
	std::cerr << "Argument for " << s << " invalid" << std::endl;
}

int parse_args(int argc, char **argv)
{
	while (1)
	{
		static struct option long_options[] =
		{
		  {"algorithm",		required_argument, 	0, 'a'},
		  {"graph",			required_argument, 	0, 'g'},
		  {"fullhash",		required_argument, 	0, 'u'},
		  {"fasthash",		required_argument, 	0, 'f'},
		  {"random_layer",	required_argument, 	0, 'r'},
		  {"phi_layer",		required_argument, 	0, 'h'},
		  {"structure",		required_argument, 	0, 't'},
		  {"min_garlic",	required_argument, 	0, 'm'},
		  {"garlic",		required_argument, 	0, 'c'},
		  {"iterations",	required_argument, 	0, 'i'},
		  /*The last element of the array has to be filled with zeros.*/
		  {0, 			0, 					0, 	0}
		};

		int r; //return value of getopt_long is the corresponding val
		char* endptr = NULL; //for parsing numbers

		//_only also recognizes long options that start with a single -
		r = getopt_long_only(argc, argv, "a:g:u:f:r:h:v:p:s:d:l:m:c:o:", 
			long_options, NULL);

		/* Detect the end of the options. */
		if (r == -1)
			break;

		switch (r)
		{
			case 'a':
				alg = optarg;
				if(alg.empty()){invalid("Algorithm"); return 1;}
			  	break;
			case 'g':
				graph = optarg;
				if(graph.empty()){invalid("Graph"); return 1;}
			  	break;
			case 'u':
				fuh = optarg;
				if(fuh.empty()){invalid("FullHash"); return 1;}
			  	break;
			case 'f':
				fah = optarg;
				if(fah.empty()){invalid("FastHash"); return 1;}
			  	break;
			case 'r':
				rl = optarg;
				if(rl.empty()){invalid("RandomLayer"); return 1;}
			  	break;
			case 'h':
				pl = optarg;
				if(pl.empty()){invalid("PhiLayer"); return 1;}
			  	break;
			case 't':
				stru = optarg;
				//accept empty structure
			  	break;
			case 'm':
			  	mg = strtod(optarg, &endptr);
			  	if (*endptr != '\0' || mg < 1){invalid("MinGarlic"); return 1;}
			  	break;
			case 'c':
			  	g = strtod(optarg, &endptr);
			  	if (*endptr != '\0' || g < 1){invalid("Garlic"); return 1;}
			  	break;
			case 'i':
			  	iterations = strtod(optarg, &endptr);
			  	if (*endptr != '\0' || iterations < 0){
			  		invalid("Iterations"); 
			  		return 1;
			  	}
			  	break;
		}
	}

	//getopt already informs about unrecognized options
	if (optind < argc)
    {
    	std::cerr << "Some arguments could not be assigned to an option"
    		<< std::endl;
    	return 1;
    }

    return chkparams();
}

//taken from libc documentation
int compare_doubles (const void *a, const void *b)
{
  const double *da = (const double *) a;
  const double *db = (const double *) b;

  return (*da > *db) - (*da < *db);
}

double measure(Catena c){
	uint8_t saltlen = 16;
	const uint8_t salt[16]=
    {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
     0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0XFF};
    uint8_t* pwd = (uint8_t*)malloc(8);
  	strncpy((char*)pwd, "Password", 8);
  	uint8_t pwdlen = 8;
  	const char *data  = "I'm a header";
  	uint8_t  datalen = strlen(data);
  	uint8_t hash1[H_LEN];

	double t[iterations];
	clock_t diff;
	for(int i = 0; i < iterations; i++){
		diff = clock();
		c.Default(pwd, pwdlen, (const uint8_t*)salt, saltlen, 
			(const uint8_t*) data, datalen, stru, mg, g,
		 	H_LEN, hash1);
		diff = clock() - diff;
		t[i] = ((double)diff) / CLOCKS_PER_SEC;
	}
	std::sort(t, t + (iterations-1));
	return t[iterations/2]; //median
}

int main(int argc, char **argv)
{
	if(parse_args(argc,argv)){
		print_usage(argv);
		return 1;
	}

	double t;
	
	try
	{
		Catena c = CatenaFactory::instance()
			.create(alg, fuh, fah, rl, graph, pl);

		if(stru ==""){
			stru = c.getDefaultStructure();
		}
		if(mg < 0){
			mg = c.getDefaulMinGarlic();
		}
		if(g < 0){
			g = c.getDefaultGarlic();
		}
		t = measure(c);

		//Output
		std::cout << std::fixed << std::setprecision(2);
		std::cout << "Hashing took " << t << "s and required roughly " <<
			(c.getMemoryRequirement(g) 
				/HASH_PER_KiB)/64 << " KiB of memory" <<std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
 

#include <iostream>
#include <stdexcept>
#include <getopt.h>

#include "catenafactory.hpp"

using namespace Catena_Variants;

//Parameters
static std::string alg = "";
static std::string graph= "";
static std::string fuh = "";
static std::string fah = "";
static std::string rl = "";
static std::string pl = "";
static std::string vid = "";
static std::string pwd = "";
static uint8_t* salt = NULL;
static int saltlen = 0;
static std::string ad = "";
static std::string stru = "";
static int mg = -1;
static int g = -1;
static int hashlen = -1;

void print_usage(char **argv){
	std::cerr << "Usage: " << argv[0] <<" --algorithm ALG --graph GRAPH" <<
		" --fullhash FULLH\n" <<
		" --fasthash FASTH --random_layer RL [--version_id ID]"<< 
		" --phi_layer PL "<< 
		"--password PWD\n" <<
		" [--salt SALT] [--data DATA] [--lambda L] [--min_garlic MG]" << 
		" [--garlic G]\n" <<
		" [--hash_length HL]" << std::endl;
	
	std::cerr << "\t-a, --algorithm ALG\talgorithm to use(formal: Flap)\n" <<
		"\t-g, --graph GRAPH\tgraph to use(formal: F)\n" <<
		"\t-u, --fullhash FULLH\tregular hash function(formal: H)\n" <<
		"\t-f, --fasthash FASTH\thash function used for consecutive calls\n" <<
		"\t\t\t\t(formal: H')\n" <<
		"\t-r, --random_layer RL\tfirst layer(formal: Gamma)\n" <<
		"\t-h, --phi_layer PL\textra layer(formal: Phi)\n" <<
		"\t-v, --version_id ID\tVersion identifier(formal: V). Default depends\n" <<
		"\t\t\t\ton graph\n"<<
		"\t-p, --password PWD\tthe password(formal: pwd)\n" <<
		"\t-s, --salt SALT\t\tthe salt(formal: s) in hex. 2 characters per\n" <<
		"\t\t\t\tbyte. Empty by default\n" <<
		"\t-d, --data DATA\t\tAssociated data(formal: d). Empty by default\n" <<
		"\t-t, --structure L\tGraph structure (r - random, g - graph). Default depends on graph\n"<<
		"\t-m, --min_garlic MG\tLower memory cost(formal: g0). Default\n"<<
		"\t\t\t\tdepends on graph\n"
		"\t-c, --garlic G\t\tmemory cost(formal:g). Default depends on graph\n"<<
		"\t-o, --hash_length HL\toutput length in byte(formal: m). Default:"<< 
		H_LEN << "\n"<<
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
	if(hashlen > 64){
		std::cerr << "Hashlen to large. Limit is 64" << std::endl;
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
		  {"version_id",	required_argument, 	0, 'v'},
		  {"password",		required_argument, 	0, 'p'},
		  {"salt",			required_argument, 	0, 's'},
		  {"data",			required_argument, 	0, 'd'},
		  {"structure",		required_argument, 	0, 't'},
		  {"min_garlic",	required_argument, 	0, 'm'},
		  {"garlic",		required_argument, 	0, 'c'},
		  {"hash_length",	required_argument, 	0, 'o'},
		  /*The last element of the array has to be filled with zeros.*/
		  {0, 			0, 					0, 	0}
		};

		int r; //return value of getopt_long is the corresponding val
		char* endptr = NULL; //for parsing numbers

		//_only also recognizes long options that start with a single -
		r = getopt_long_only(argc, argv, "a:g:u:f:r:h:v:p:s:d:t:l:m:c:o:", 
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
			case 'v':
				vid = optarg;
				if(vid.empty()){invalid("VersionID"); return 1;}
			  	break;
			case 'p':
				pwd = optarg;
				//accept empty password
			  	break;
			case 's':
				saltlen = strlen(optarg) / 2;
				salt = new uint8_t[saltlen];
				char temp[3];
				temp[2] = '\0';
				for(int i = 0; i < saltlen; i++)
				{	
					temp[0] = optarg[2*i];
					temp[1] = optarg[(2*i)+1];
					salt[i] = (uint8_t)strtol(temp, NULL, 16);
				}
				if(saltlen==0){invalid("Salt"); return 1;}
			  	break;
			case 't':
				stru = optarg;
				//accept empty structure
			  	break;
			case 'd':
				ad = optarg;
				if(ad.empty()){invalid("Data"); return 1;}
			  	break;
			case 'm':
			  	mg = strtod(optarg, &endptr);
			  	if (*endptr != '\0' || mg < 1){invalid("MinGarlic"); return 1;}
			  	break;
			case 'c':
			  	g = strtod(optarg, &endptr);
			  	if (*endptr != '\0' || g < 1){invalid("Garlic"); return 1;}
			  	break;
			case 'o':
			  	hashlen = strtod(optarg, &endptr);
			  	if (*endptr != '\0' || hashlen < 1)
			  	{
			  		invalid("HashLength"); 
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

int main(int argc, char **argv)
{
	if(parse_args(argc,argv)){
		print_usage(argv);
		return 1;
	}

	try
	{
		Catena c = CatenaFactory::instance()
			.create(alg, fuh, fah, rl, graph, pl);

		//set defaults:
		int pwdlen = pwd.length();
		uint8_t* pw = new uint8_t[pwdlen];
		strncpy((char*)pw, pwd.c_str(), pwdlen);
		
		//salt doesn need additional handling
		
		int datalen = ad.length();
		uint8_t* data = new uint8_t[datalen];
		strncpy((char*)data, ad.c_str(), datalen);
		
		if(stru ==""){
			stru = c.getDefaultStructure();
		}
		if(mg < 0){
			mg = c.getDefaulMinGarlic();
		}
		if(g < 0){
			g = c.getDefaultGarlic();
		}
		if(hashlen < 0){
			hashlen = H_LEN;
		}
		
		uint8_t hash1[hashlen];
			
		//set Version ID if requested
		if(!vid.empty()){
			c.setVersionID((const uint8_t*)vid.c_str());
		}
	  	
	  	c.Default(pw, pwdlen ,salt, saltlen, data, datalen, stru, mg, g,
		 	hashlen, hash1);

	  	//Output
	  	for(int i=0; i< hashlen; i++) printf("%02x",hash1[i]);  puts("");
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

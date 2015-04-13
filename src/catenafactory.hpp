#pragma once

#include <map>
#include <sstream>
#include <vector>

#include "catena.hpp"
#include "algorithm.hpp"
#include "hashfull.hpp"
#include "hashfast.hpp"
#include "random_layer.hpp"
#include "graph.hpp"

namespace Catena_Variants{

class CatenaFactory{
public:
	static CatenaFactory& instance();

	//Default D'tor destructs the maps->destructs the shared ptrs
	//->delete the cloned Abstract Objects if necessary
	~CatenaFactory() = default;

	Catena create(std::string const& alg, std::string const& hfull,
		std::string const& hfast, std::string const& rlayer, 
		std::string const& g);

	void addAlgorithm(AAsptr a);
	void addHashFull(AHFUsptr h);
	void addHashFast(AHFAsptr h);
	void addRandomLayer(ARLsptr r);
	void addGraph(AGsptr g);

	AAsptr getAlgorithm(std::string s);
	AHFUsptr getHashFull(std::string s);
	AHFAsptr getHashFast(std::string s);
	ARLsptr getRandomLayer(std::string s);
	AGsptr getGraph(std::string s);	

	std::vector<AAsptr> getAlgorithms();
	std::vector<AHFUsptr> getFullHashes();
	std::vector<AHFAsptr> getFastHashes();
	std::vector<ARLsptr> getRandomLayers();
	std::vector<AGsptr> getGraphs();

	std::string getAlgorithmsText();
	std::string getFullHashesText();
	std::string getFastHashesText();
	std::string getRandomLayersText();
	std::string getGraphsText();

private:
//C'tors
	CatenaFactory() = default; //Default C'tor
	CatenaFactory(const CatenaFactory&) = default; //Copy C'tor
	CatenaFactory& operator=(const CatenaFactory&) = default; //Assignment

//Private methods
	std::string createText(std::string const& n, std::string const& s,
			std::string const& d);

//Members
	std::map<std::string, AAsptr> 	_Algorithms;
	std::stringstream 							_AlgorithmsText;

	std::map<std::string, AHFUsptr> 	_FullHashes;
	std::stringstream 							_FullHashesText;

	std::map<std::string, AHFAsptr> 	_FastHashes;
	std::stringstream 							_FastHashesText;

	std::map<std::string, ARLsptr> _RandomLayers;
	std::stringstream 							_RandomLayersText;

	std::map<std::string, AGsptr> 		_Graphs;
	std::stringstream 							_GraphsText;

};

} //end namespace
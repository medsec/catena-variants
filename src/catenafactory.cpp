#include "catenafactory.hpp"
#include "exceptions/NotFoundException.hpp"

#include <iostream>

using namespace Catena_Variants;

CatenaFactory&
CatenaFactory::instance()
{
	static CatenaFactory _instance;
    return _instance;
}

Catena 
CatenaFactory::create(std::string const& alg, std::string const& hfull,
		std::string const& hfast, std::string const& rlayer, 
		std::string const& graph, std::string const& player)
{
	Catena c;
	AAsptr a;
	AHFUsptr hfu;
	AHFAsptr hfa;
	ARLsptr r;
	AGsptr g;
	APLsptr p;

	try {
		a = std::shared_ptr<AbstractAlgorithm>(_Algorithms.at(alg)->clone());
	}
	catch (const std::out_of_range& oor) {
		throw NotFoundException("Algorithm", alg);
	}
	try {
		hfu = std::shared_ptr<AbstractHashFull>(_FullHashes.at(hfull)->clone());
	}
	catch (const std::out_of_range& oor) {
		throw NotFoundException("HashFull", hfull);
	}
	try {
		hfa = std::shared_ptr<AbstractHashFast>(_FastHashes.at(hfast)->clone());
	}
	catch (const std::out_of_range& oor) {
		throw NotFoundException("HashFast", hfast);
	}
	try {
		r = std::shared_ptr<AbstractRandomLayer>(
				_RandomLayers.at(rlayer)->clone());
	}
	catch (const std::out_of_range& oor) {
		throw NotFoundException("RandomLayer", rlayer);
	}
	try {
		g = std::shared_ptr<AbstractGraph>(
				_Graphs.at(graph)->clone());
	}
	catch (const std::out_of_range& oor) {
		throw NotFoundException("Graph", graph);
	}
	try {
		p = std::shared_ptr<AbstractPhiLayer>(
				_PhiLayers.at(player)->clone());
	}
	catch (const std::out_of_range& oor) {
		throw NotFoundException("PhiLayer", player);
	}

	//set up algorithm
	a->setRandomLayer(r);
	a->setGraph(g);
	a->setPhiLayer(p);
	//Hash function for Random Layer and Graph are forwarded by the Algorithm
	a->setFullHash(hfu);
	a->setHashFast(hfa);

	//set up Catena
	c.setFullHash(hfu);
	c.setAlgorithm(a);

	return c;
}

std::string
CatenaFactory::createText(std::string const& n, std::string const& s,
			std::string const& d)
{
	std::string str;
	str.reserve(n.length() + s.length() + d.length() + 50);
	str = "\t[ ";
	str += s;
	str += " | ";
	str += n;
	str += " ] : ";
	str += d;
	str += "\n";
	return str;
}

void 
CatenaFactory::addAlgorithm(AAsptr a){
	_Algorithms[a->getName()] = a;
	_Algorithms[a->getShortHandle()] = a;
	_AlgorithmsText << createText(a->getName(), a->getShortHandle(), 
			a->getDescription());
}


void 
CatenaFactory::addHashFull(AHFUsptr h){
	_FullHashes[h->getName()] = h;
	_FullHashes[h->getShortHandle()] = h;
	_FullHashesText << createText(h->getName(), h->getShortHandle(), 
			h->getDescription());
}


void 
CatenaFactory::addHashFast(AHFAsptr h){
	_FastHashes[h->getName()] = h;
	_FastHashes[h->getShortHandle()] = h;
	_FastHashesText << createText(h->getName(), h->getShortHandle(), 
			h->getDescription());
}


void 
CatenaFactory::addRandomLayer(ARLsptr r){
	_RandomLayers[r->getName()] = r;
	_RandomLayers[r->getShortHandle()] = r;
	_RandomLayersText << createText(r->getName(), r->getShortHandle(), 
			r->getDescription());
}


void 
CatenaFactory::addGraph(AGsptr g){
	_Graphs[g->getName()] = g;
	_Graphs[g->getShortHandle()] = g;
	_GraphsText << createText(g->getName(), g->getShortHandle(), 
			g->getDescription());
}


void 
CatenaFactory::addPhiLayer(APLsptr p){
	_PhiLayers[p->getName()] = p;
	_PhiLayers[p->getShortHandle()] = p;
	_PhiLayersText << createText(p->getName(), p->getShortHandle(), 
			p->getDescription());
}

std::vector<AAsptr>
CatenaFactory::getAlgorithms(){
	std::vector<AAsptr> v;
	for(std::map<std::string, AAsptr>::iterator 
		it = _Algorithms.begin(); it != _Algorithms.end(); ++it) 
	{
  		v.push_back(it->second);
	}
	return v;
}


std::vector<AHFUsptr>
CatenaFactory::getFullHashes(){
	std::vector<AHFUsptr> v;
	for(std::map<std::string, AHFUsptr>::iterator 
		it = _FullHashes.begin(); it != _FullHashes.end(); ++it) 
	{
  		v.push_back(it->second);
	}
	return v;
}


std::vector<AHFAsptr> 
CatenaFactory::getFastHashes(){
	std::vector<AHFAsptr> v;
	for(std::map<std::string, AHFAsptr>::iterator 
		it = _FastHashes.begin(); it != _FastHashes.end(); ++it) 
	{
  		v.push_back(it->second);
	}
	return v;
}


std::vector<ARLsptr>
CatenaFactory::getRandomLayers(){
	std::vector<ARLsptr> v;
	for(std::map<std::string, ARLsptr>::iterator 
		it = _RandomLayers.begin(); it != _RandomLayers.end(); ++it) 
	{
  		v.push_back(it->second);
	}
	return v;
}


std::vector<AGsptr>
CatenaFactory::getGraphs(){
	std::vector<AGsptr> v;
	for(std::map<std::string, AGsptr>::iterator 
		it = _Graphs.begin(); it != _Graphs.end(); ++it) 
	{
  		v.push_back(it->second);
	}
	return v;
}

std::vector<APLsptr>
CatenaFactory::getPhiLayers(){
	std::vector<APLsptr> v;
	for(std::map<std::string, APLsptr>::iterator 
		it = _PhiLayers.begin(); it != _PhiLayers.end(); ++it) 
	{
  		v.push_back(it->second);
	}
	return v;
}

AAsptr 
CatenaFactory::getAlgorithm(std::string s)
{
	return _Algorithms[s];
}


AHFUsptr 
CatenaFactory::getHashFull(std::string s)
{
	return _FullHashes[s];
}


AHFAsptr 
CatenaFactory::getHashFast(std::string s)
{
	return _FastHashes[s];
}


ARLsptr 
CatenaFactory::getRandomLayer(std::string s)
{
	return _RandomLayers[s];
}


AGsptr
CatenaFactory::getGraph(std::string s)
{
	return _Graphs[s];
}

APLsptr 
CatenaFactory::getPhiLayer(std::string s)
{
	return _PhiLayers[s];
}

std::string 
CatenaFactory::getAlgorithmsText()
{
	return _AlgorithmsText.str();
}


std::string 
CatenaFactory::getFullHashesText()
{
	return _FullHashesText.str();
}


std::string 
CatenaFactory::getFastHashesText()
{
	return _FastHashesText.str();
}


std::string 
CatenaFactory::getRandomLayersText()
{
	return _RandomLayersText.str();
}


std::string 
CatenaFactory::getGraphsText()
{
	return _GraphsText.str();
}


std::string 
CatenaFactory::getPhiLayersText()
{
	return _PhiLayersText.str();
}
template <typename Derived>
AbstractAlgorithm* 
Algorithm<Derived>::clone() const{
	return new Derived(static_cast<Derived const&>(*this));
}

template <typename Derived>
void 
Algorithm<Derived>::setFullHash(AHFUsptr h){
	_hashfull = h;
	if(_graph){
		_graph->setFullHash(h);
	}
	if(_randomlayer){
		_randomlayer->setFullHash(h);
	}
}

template <typename Derived>
void 
Algorithm<Derived>::setHashFast(AHFAsptr h){
	_hashfast = h;
	if(_graph){
		_graph->setHashFast(h);
	}
	if(_randomlayer){
		_randomlayer->setHashFast(h);
	}
	if(_philayer){
		_philayer->setHashFast(h);
	}
}

template <typename Derived>
void 
Algorithm<Derived>::setGraph(AGsptr g) {
	_graph = g;
}

template <typename Derived>
void 
Algorithm<Derived>::setRandomLayer(ARLsptr r){
	_randomlayer = r;
}			

template <typename Derived>
void 
Algorithm<Derived>::setPhiLayer(APLsptr p){
	_philayer = p;
}

template <typename Derived>
const uint8_t* 
Algorithm<Derived>::getDefaultVersionID() const
{
	return _graph->getDefaultVersionID();
}


template <typename Derived>
std::string 
Algorithm<Derived>::getDefaultStructure()const
{
	return _graph->getDefaultStructure();
}

template <typename Derived>
uint8_t 
Algorithm<Derived>::getDefaultGarlic()const
{
	return _graph->getDefaultGarlic();
}

template <typename Derived>
uint8_t 
Algorithm<Derived>::getDefaulMinGarlic()const
{
	return _graph->getDefaulMinGarlic();
}

template <typename Derived>
uint16_t 
Algorithm<Derived>::getHlenFast()const
{
	return _hashfast->getHlenFast();
}

template <typename Derived>
uint64_t 
Algorithm<Derived>::getMemoryRequirement(uint8_t garlic)const
{
	return _graph->getMemoryRequirement(garlic);
}
template <typename Derived>
void 
RandomLayer<Derived>::setFullHash(AHFUsptr h){
	_hashfull = h;
}

template <typename Derived>
void 
RandomLayer<Derived>::setHashFast(AHFAsptr h){
	_hashfast = h;
}

template <typename Derived>
void 
RandomLayer<Derived>::setGraph(AGsptr g) {
	_graph = g;
}

template <typename Derived>
AbstractRandomLayer*
RandomLayer<Derived>::clone() const{
	return new Derived(static_cast<Derived const&>(*this));
}
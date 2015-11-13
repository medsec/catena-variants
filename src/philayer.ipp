template <typename Derived>
void 
PhiLayer<Derived>::setFullHash(AHFUsptr h){
	_hashfull = h;
}

template <typename Derived>
void
PhiLayer<Derived>::setHashFast(AHFAsptr h){
	_hashfast = h;
}

template <typename Derived>
void
PhiLayer<Derived>::setGraph(AGsptr g){
	_graph = g;
}

template <typename Derived>
AbstractPhiLayer* PhiLayer<Derived>::clone() const{
	return new Derived(static_cast<Derived const&>(*this));
}


template <typename Derived>
void 
Graph<Derived>::setFullHash(AHFUsptr h){
	_hashfull = h;
}

template <typename Derived>
void
Graph<Derived>::setHashFast(AHFAsptr h){
	_hashfast = h;
}

template <typename Derived>
AbstractGraph* Graph<Derived>::clone() const{
	return new Derived(static_cast<Derived const&>(*this));
}
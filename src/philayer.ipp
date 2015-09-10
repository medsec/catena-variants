template <typename Derived>
void
PhiLayer<Derived>::setHashFast(AHFAsptr h){
	_hashfast = h;
}

template <typename Derived>
AbstractPhiLayer* PhiLayer<Derived>::clone() const{
	return new Derived(static_cast<Derived const&>(*this));
}
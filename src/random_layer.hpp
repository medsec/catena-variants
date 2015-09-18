#pragma once

#include <memory>

#include "hashfast.hpp"
#include "hashfull.hpp"
#include "graph.hpp"

namespace Catena_Variants{

class AbstractRandomLayer : public virtual Registerable{
public:		
	AbstractRandomLayer() = default;
	virtual ~AbstractRandomLayer() = default;
	virtual AbstractRandomLayer* clone() const=0;
	
	virtual void process(const uint8_t* x, const uint8_t lambda, 
					const uint8_t garlic, const uint8_t *salt, 
              		const uint8_t saltlen, uint8_t *r)=0;
	
	virtual void setFullHash(AHFUsptr h)=0;
	virtual void setHashFast(AHFAsptr h)=0;
	virtual void setGraph(AGsptr g)=0;


};

typedef std::shared_ptr<AbstractRandomLayer> ARLsptr;

/* 	This is a class derived from the abstract parent to provide some
*	functionality. Derive your implementation from this!
*/
template <typename Derived>
class RandomLayer : public AbstractRandomLayer {
public:
	RandomLayer() = default;
	virtual ~RandomLayer() = default;
	
	virtual AbstractRandomLayer* clone() const;

	void setFullHash(AHFUsptr h);
	void setHashFast(AHFAsptr h);
	void setGraph(AGsptr g);


protected:
	AHFUsptr 	_hashfull;
	AHFAsptr	_hashfast;
	AGsptr		_graph;
};

#include "random_layer.ipp"

} //end namespace
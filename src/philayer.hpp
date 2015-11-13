#pragma once

#include <memory>

#include "registerable.hpp"
#include <stdint.h>
#include "constants.hpp"
#include "hashfast.hpp"
#include "hashfull.hpp"
#include "graph.hpp"

namespace Catena_Variants{

class AbstractPhiLayer: public virtual Registerable{
public:
	AbstractPhiLayer(){};
	virtual ~AbstractPhiLayer() = default;

	virtual AbstractPhiLayer* clone() const=0;

	virtual void process(const uint8_t garlic, uint8_t *r, uint8_t* h)=0;

	/* Resets the State used for PhiLayer if necessary
	*/
	virtual void ResetState()=0;
	virtual void setFullHash(AHFUsptr h)=0;
	virtual void setHashFast(AHFAsptr h)=0;
	virtual	void setGraph(AGsptr g)=0;


};

typedef std::shared_ptr<AbstractPhiLayer> APLsptr;

/* 	This is a class derived from the abstract parent to provide some
*	functionality. Derive your implementation from this!
*/
template <typename Derived>
class PhiLayer : public AbstractPhiLayer {
public:
	PhiLayer() = default;
	virtual ~PhiLayer() = default;

	virtual AbstractPhiLayer* clone() const;
	
	void setFullHash(AHFUsptr h);
	void setHashFast(AHFAsptr h);
	void setGraph(AGsptr g);

protected:
	AHFAsptr 	_hashfast;
	AHFUsptr 	_hashfull;
	AGsptr		_graph;
};

#include "philayer.ipp"

} //end namespace
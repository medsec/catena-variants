#pragma once

#include <memory>

#include "hashfast.hpp"
#include "hashfull.hpp"
#include "graph.hpp"
#include "random_layer.hpp"
#include "philayer.hpp"

namespace Catena_Variants{

/* Flap function of Catena
 */
class AbstractAlgorithm : public virtual Registerable{
public:	
	virtual ~AbstractAlgorithm() = default;
	virtual AbstractAlgorithm* clone() const =0;

	virtual void flap(const uint8_t* x, const uint16_t xlen, const uint8_t lambda, 
		const uint8_t garlic, const uint8_t *salt, const uint8_t saltlen, 
		uint8_t* h)=0;


	virtual void setFullHash(AHFUsptr h)=0;
	virtual void setHashFast(AHFAsptr h)=0;

	virtual void setGraph(AGsptr g)=0;
	virtual void setRandomLayer(ARLsptr r)=0;
	virtual void setPhiLayer(APLsptr r)=0;

	/*get Defaults from Graph*/
	virtual const uint8_t* getDefaultVersionID() const=0;
	virtual uint8_t getDefaultLambda()const=0;
 	virtual uint8_t getDefaultGarlic()const=0;
 	virtual uint8_t getDefaulMinGarlic()const=0;

 	virtual uint16_t getHlenFast()const=0;
 	virtual uint64_t getMemoryRequirement(uint8_t garlic)const=0;
};

typedef std::shared_ptr<AbstractAlgorithm> AAsptr;


/* 	This is a class derived from the abstract parent to provide some
*	functionality. Derive your implementation from this!
*/
template <typename Derived>
class Algorithm : public AbstractAlgorithm {
public:
	Algorithm() = default;
	virtual ~Algorithm() = default;

	virtual AbstractAlgorithm* clone() const;

	void setFullHash(AHFUsptr h);
	void setHashFast(AHFAsptr h);

	void setGraph(AGsptr g);
	void setRandomLayer(ARLsptr r);
	void setPhiLayer(APLsptr r);

	/*get Defaults from Graph*/
	const uint8_t* getDefaultVersionID() const;
	uint8_t getDefaultLambda()const;
 	uint8_t getDefaultGarlic()const;
 	uint8_t getDefaulMinGarlic()const;

 	uint16_t getHlenFast()const;
 	uint64_t getMemoryRequirement(uint8_t garlic)const;

protected:
	AHFUsptr 		_hashfull;
	AHFAsptr		_hashfast;
	AGsptr			_graph;
	APLsptr			_philayer;
	ARLsptr			_randomlayer;
};

#include "algorithm.ipp"

} //end namespace
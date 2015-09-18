#pragma once

#include <memory>

#include "hashfast.hpp"
#include "hashfull.hpp"

namespace Catena_Variants{

/* Graph aka. F function used in Flap
 */

class AbstractGraph : public virtual Registerable{
public:		
	virtual ~AbstractGraph(){}
	
	virtual AbstractGraph* clone() const =0;

	virtual void process(const uint8_t x[H_LEN], const uint8_t lambda, 
					const uint8_t garlic, const uint8_t *salt, 
					const uint8_t saltlen, uint8_t *r, uint8_t* h)=0;

	virtual uint64_t index(const uint64_t ind, uint8_t garlic)=0;

	/*Returns the required memory in byte*/
	virtual uint64_t getMemoryRequirement(uint8_t garlic)const = 0;

	virtual void setFullHash(AHFUsptr h)=0;
	virtual void setHashFast(AHFAsptr h)=0;
	
	virtual uint8_t getDefaultLambda()const =0;
 	virtual uint8_t getDefaultGarlic()const =0;
 	virtual uint8_t getDefaulMinGarlic()const =0;
	virtual const uint8_t* getDefaultVersionID()const =0;

};

typedef std::shared_ptr<AbstractGraph> AGsptr;


/* 	This is a class derived from the abstract parent to provide some
*	functionality. Derive your implementation from this!
*/
template <typename Derived>
class Graph: public AbstractGraph {
public:
	Graph() = default;
	virtual ~Graph() = default;

	virtual AbstractGraph* clone() const;

	void setFullHash(AHFUsptr h);
	void setHashFast(AHFAsptr h);

protected:
	AHFUsptr 	_hashfull;
	AHFAsptr	_hashfast;
};

#include "graph.ipp"

} //end namespace
#pragma once

#include "../graph.hpp"

namespace Catena_Variants {

class BRG_Phi :  public Graph<BRG_Phi>{
public:
	BRG_Phi();

	virtual void process(const uint8_t x[H_LEN], const uint8_t lambda, 
						const uint8_t garlic, const uint8_t *salt, 
						const uint8_t saltlen, uint8_t *r, uint8_t* h);

	virtual uint64_t getMemoryRequirement(uint8_t garlic)const;

	virtual uint8_t getDefaultLambda()const;
 	virtual uint8_t getDefaultGarlic()const;
 	virtual uint8_t getDefaulMinGarlic()const;
	virtual const uint8_t* getDefaultVersionID()const;

private:
	/*Return the reverse bit order of x where x is interpreted as n-bit value*/
	uint64_t reverse(uint64_t x, const uint8_t n);

	void H_First(const uint8_t* i1, const uint8_t* i2, 
			uint8_t* hash);
	
	uint64_t
	R(uint64_t* input, const uint8_t n);

	std::string const VERSION_ID = "Dragonfly";
	uint8_t const LAMBDA = 2;
	uint8_t const GARLIC = 21;
	uint8_t const MIN_GARLIC = 21;
};

} //end namespace
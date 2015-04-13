#pragma once

#include "../random_layer.hpp"

namespace Catena_Variants {

class Gamma :  public RandomLayer<Gamma>{
public:
	Gamma();

	/* Gamma Layer of Catena-DBG and -BRG using XorShift1024*
	* see: http://en.wikipedia.org/wiki/Xorshift#Variations
	*/
	virtual void process(const uint8_t x[H_LEN], const uint8_t lambda,
					const uint8_t garlic, const uint8_t *salt, 
                  	const uint8_t saltlen, uint8_t *r);

private:
	
	/* Init the state for Xorshift with two 512bit values
 	*/
	void initXSState(const uint8_t* a, const uint8_t* b);

	/*  Xorshift RNG with a 1024bit state that must be seeded first.
 	*/
	uint64_t xorshift1024star();

	int p;
	uint64_t s[16];
};

}//end namespace
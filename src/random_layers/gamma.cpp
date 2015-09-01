#include <string.h>

#include "gamma.hpp"
#include "../registry.hpp"
#include "../constants.hpp"

using namespace Catena_Variants;

//Register the registrable with the Registry so it can be used by CatenaFactory
Registry<Gamma> regGamma;

Gamma::Gamma()
:Registerable("Gamma", "G", "The Gamma function of Catena-DBG/BRG"){}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void 
Gamma::process(const uint8_t* x, const uint8_t lambda, 
			  	const uint8_t garlic, const uint8_t *salt, 
               	const uint8_t saltlen, uint8_t *r)
{
	const uint64_t q = UINT64_C(1) << ((3*garlic+3)/4);
	const uint8_t H_LEN_FAST = _hashfast->getHlenFast();
	uint64_t i, j, j2;
	uint8_t *tmp = (uint8_t*) malloc(H_LEN);
	uint8_t *tmp2 = (uint8_t*) malloc(H_LEN);

	_hashfull->Hash1(salt, saltlen, tmp);  //tmp <- H(S)
	_hashfull->Hash1(tmp, H_LEN, tmp2);    //tmp2 <- H(H(S))
	initXSState(tmp, tmp2);

	_hashfast->ResetState();
	for(i = 0; i < q; i++){
		j = xorshift1024star() >> (64 - garlic);
		j2 = xorshift1024star() >> (64 - garlic);
		//v_j1= H'(v_j1||v_j2)
		_hashfast->Hash(i, r + j * H_LEN_FAST, r + j2 * H_LEN_FAST, r + j * H_LEN_FAST); 
	}

	free(tmp);
  	free(tmp2);
}
#pragma GCC diagnostic pop

void 
Gamma::initXSState(const uint8_t* a, const uint8_t* b){
	p = 0;
	//endianess independet equivalent to
	// memcpy(s, a, H_LEN);
	// memcpy(&s[8], b, H_LEN);
	//on little endian
	for(int i = 0; i < 8; i++){
		s[i] = UINT64_C(0);
		s[i+8] = UINT64_C(0);
		for(int j = 0; j < 8; j++){
			s[i] |= ((uint64_t)a[i*8+j]) << j*8;
			s[i+8] |= ((uint64_t)b[i*8+j]) << j*8;
		}
	}
}


uint64_t 
Gamma::xorshift1024star() {
	uint64_t s0 = s[p];
	uint64_t s1 = s[ p = (p+1) & 15 ];
	s1 ^= s1 << 31; // a
	s1 ^= s1 >> 11; // b
	s0 ^= s0 >> 30; // c
	return ( s[p] = s0 ^ s1 ) * UINT64_C(1181783497276652981);
}
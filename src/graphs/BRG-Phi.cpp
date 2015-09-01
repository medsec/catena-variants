#include <string.h>
#include <byteswap.h>

#include "BRG-Phi.hpp"
#include "../registry.hpp"
#include "iostream"

using namespace Catena_Variants;

//Register the registrable with the Registry so it can be used by CatenaFactory
Registry<BRG_Phi> regBRGPHI;

BRG_Phi::BRG_Phi()
:Registerable("Bit-Reversal-Graph", "BRG-Phi", "The Graph of Catena-Dragonfly with Phi")
{}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void 
BRG_Phi::process(const uint8_t x[H_LEN], const uint8_t lambda, 
			const uint8_t garlic, const uint8_t *salt, 
			const uint8_t saltlen, uint8_t *r, uint8_t* h)
{
	const uint64_t c = UINT64_C(1) << garlic;
	const uint16_t H_LEN_FAST = _hashfast->getHlenFast();
	uint64_t i;
	uint8_t k;

	/* BRH */
	for (k = 0; k < lambda; k++) {
		H_First(r + (c-1)*H_LEN_FAST, r, r);
		_hashfast->ResetState();

		/* Replace r[reverse(i, garlic)] with new value */
		uint8_t *previousR = r, *p;
		for (i = 1; i < c; i++) {
			p = r + reverse(i, garlic) * H_LEN_FAST;
			_hashfast->Hash(i, previousR, p, p);
			previousR = p;
		}
		k++;
		if (k >= lambda) {
			break;
		}
		/* This is now sequential because (reverse(reverse(i, garlic), garlic) == i) */
		//_hashfull->Hash2(r + (c-1)*H_LEN, H_LEN, r, H_LEN, r);
		H_First(r + (c-1)*H_LEN_FAST, r, r);
		_hashfast->ResetState();
		p = r + H_LEN_FAST;
		for (i = 1; i < c; i++, p += H_LEN_FAST) {
			_hashfast->Hash(i, p - H_LEN_FAST, p, p);
		}
	}

	
	//PHI-Part
	_hashfast->Hash(0, r, r+R((uint64_t*)(r+H_LEN_FAST*(c-1)), garlic)*H_LEN_FAST, r);
	uint8_t * p = r;
	uint8_t * p_old = p;
	p += H_LEN_FAST;
	for (i = 1; i < c; i++, p += H_LEN_FAST) {
			_hashfast->Hash(i, r + R((uint64_t*)(p_old), garlic)*H_LEN_FAST, p, p);
			p_old=p;
		}

	/* reverse(c - 1, garlic) == c - 1 */
	memcpy(h, r + (c - 1) * H_LEN_FAST, H_LEN_FAST);
}
#pragma GCC diagnostic pop

uint64_t
BRG_Phi::R(uint64_t* input, const uint8_t n){
	return  input[0]>>(64-n);
}

uint64_t 
BRG_Phi::reverse(uint64_t x, const uint8_t n){
	x = bswap_64(x);
	x = ((x & UINT64_C(0x0f0f0f0f0f0f0f0f)) << 4) |
		((x & UINT64_C(0xf0f0f0f0f0f0f0f0)) >> 4);
	x = ((x & UINT64_C(0x3333333333333333)) << 2) |
		((x & UINT64_C(0xcccccccccccccccc)) >> 2);
	x = ((x & UINT64_C(0x5555555555555555)) << 1) |
		((x & UINT64_C(0xaaaaaaaaaaaaaaaa)) >> 1);
  return x >> (64 - n);
}


uint64_t 
BRG_Phi::getMemoryRequirement(uint8_t garlic)const
{
	const uint16_t H_LEN_FAST = _hashfast->getHlenFast();
	return (UINT64_C(1) << garlic) * H_LEN_FAST;
}


uint8_t
BRG_Phi::getDefaultLambda()const{
	return LAMBDA;
}


uint8_t
BRG_Phi::getDefaultGarlic()const{
	return GARLIC;
}


uint8_t
BRG_Phi::getDefaulMinGarlic()const{
	return MIN_GARLIC;
}


const uint8_t*
BRG_Phi::getDefaultVersionID()const{
	return (const uint8_t*)VERSION_ID.c_str();
}

void 
BRG_Phi::H_First(const uint8_t* i1, const uint8_t* i2, uint8_t* hash){
	_hashfast->ResetState();
	const uint16_t H_LEN_FAST = _hashfast->getHlenFast();
	uint8_t *x = (uint8_t*) malloc(H_LEN);
	_hashfull->Hash2(i1, H_LEN_FAST, i2, H_LEN_FAST,x);

	for(uint8_t i = 0; i<(H_LEN_FAST/H_LEN);++i){
		_hashfull->Hash2(&i,1, x, H_LEN, hash+i*H_LEN);
	}
	free(x);
}

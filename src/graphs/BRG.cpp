#include <string.h>
#include <byteswap.h>

#include "BRG.hpp"
#include "../registry.hpp"

using namespace Catena_Variants;

//Register the registrable with the Registry so it can be used by CatenaFactory
Registry<BRG> regBRG;

BRG::BRG()
:Registerable("Bit-Reversal-Graph", "BRG", "The Graph of Catena-Dragonfly")
{}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void 
BRG::process(const uint8_t x[H_LEN], const uint8_t lambda, 
			const uint8_t garlic, const uint8_t *salt, 
			const uint8_t saltlen, uint8_t *r, uint8_t h[H_LEN])
{
	const uint64_t c = UINT64_C(1) << garlic;
	uint64_t i;
	uint8_t k;

	/* BRH */
	for (k = 0; k < lambda; k++) {
		_hashfull->Hash2(r + (c-1)*H_LEN, H_LEN, r, H_LEN, r);
		_hashfast->ResetState();

		/* Replace r[reverse(i, garlic)] with new value */
		uint8_t *previousR = r, *p;
		for (i = 1; i < c; i++) {
			p = r + reverse(i, garlic) * H_LEN;
			_hashfast->Hash(i, previousR, p, p);
			previousR = p;
		}
		k++;
		if (k >= lambda) {
			break;
		}
		/* This is now sequential because (reverse(reverse(i, garlic), garlic) == i) */
		_hashfull->Hash2(r + (c-1)*H_LEN, H_LEN, r, H_LEN, r);
		_hashfast->ResetState();
		p = r + H_LEN;
		for (i = 1; i < c; i++, p += H_LEN) {
			_hashfast->Hash(i, p - H_LEN, p, p);
		}
	}

	/* reverse(c - 1, garlic) == c - 1 */
	memcpy(h, r + (c - 1) * H_LEN, H_LEN);
}
#pragma GCC diagnostic pop

uint64_t 
BRG::reverse(uint64_t x, const uint8_t n){
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
BRG::getMemoryRequirement(uint8_t garlic)const
{
	return (UINT64_C(1) << garlic) * H_LEN;
}


uint8_t
BRG::getDefaultLambda()const{
	return LAMBDA;
}


uint8_t
BRG::getDefaultGarlic()const{
	return GARLIC;
}


uint8_t
BRG::getDefaulMinGarlic()const{
	return MIN_GARLIC;
}


const uint8_t*
BRG::getDefaultVersionID()const{
	return (const uint8_t*)VERSION_ID.c_str();
}
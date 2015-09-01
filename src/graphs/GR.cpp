#include <string.h>
#include <cmath>

#include "GR.hpp"
#include "../registry.hpp"

using namespace Catena_Variants;

//Register the registrable with the Registry so it can be used by CatenaFactory
Registry<GR> regGR;

GR::GR()
:Registerable("Gray-Reverse", "GR", "As suggested by Ben Harris. Recommended for Lambda up to 4")
{}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void 
GR::process(const uint8_t x[H_LEN], const uint8_t lambda, 
			const uint8_t garlic, const uint8_t *salt, 
			const uint8_t saltlen, uint8_t *r, uint8_t* h)
{
	const uint64_t c = UINT64_C(1) << garlic;
	const uint16_t H_LEN_FAST = _hashfast->getHlenFast();
	uint64_t i;
	uint8_t k;

	/* naive implementation that used 2* 2^g memory */

	for (k = 0; k < lambda; k++) {
		//_hashfull->Hash2(r + (c-1)*H_LEN, H_LEN, r, H_LEN, r+(c*H_LEN));
		H_First(r + (c-1)*H_LEN_FAST, r, r);
		_hashfast->ResetState();

		for (i = 1; i < c; i++) {
			_hashfast->Hash(i, r+((c+i-1)*H_LEN_FAST), r+(phi(i,garlic)*H_LEN_FAST), 
				r+((c+i)*H_LEN_FAST));
		}
		memcpy(r, r+(c*H_LEN_FAST), c*H_LEN_FAST); // v <- r
	}

	/* reverse(c - 1, garlic) == c - 1 */
	memcpy(h, r + (c - 1) * H_LEN_FAST, H_LEN_FAST);
}
#pragma GCC diagnostic pop

uint64_t
GR::phi(uint64_t i, uint8_t g)
{	
	const uint64_t rev = reverse(i,g);
	uint64_t invrev = ~rev;
	//truncate to g bits
	invrev <<= (64-g);
	invrev >>= (64-g) + (int)std::ceil(g / 2);
	return rev ^ invrev;
}

uint64_t 
GR::reverse(uint64_t x, const uint8_t n){
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
GR::getMemoryRequirement(uint8_t garlic)const
{
	const uint16_t H_LEN_FAST = _hashfast->getHlenFast();
	return 2*(UINT64_C(1) << garlic) * H_LEN_FAST;
}


uint8_t
GR::getDefaultLambda()const{
	return LAMBDA;
}


uint8_t
GR::getDefaultGarlic()const{
	return GARLIC;
}


uint8_t
GR::getDefaulMinGarlic()const{
	return MIN_GARLIC;
}


const uint8_t*
GR::getDefaultVersionID()const{
	return (const uint8_t*)VERSION_ID.c_str();
}

void 
GR::H_First(const uint8_t* i1, const uint8_t* i2, uint8_t* hash){
	_hashfast->ResetState();
	const uint8_t H_LEN_FAST = _hashfast->getHlenFast();
	uint8_t *x = (uint8_t*) malloc(H_LEN);
	_hashfull->Hash2(i1, H_LEN_FAST, i2, H_LEN_FAST,x);

	for(uint8_t i = 0; i<(H_LEN_FAST/H_LEN);++i){
		_hashfull->Hash2(&i,1, x, H_LEN, hash+i*H_LEN);
	}
	free(x);
}
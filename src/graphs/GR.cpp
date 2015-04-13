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
			const uint8_t saltlen, uint8_t *r, uint8_t h[H_LEN])
{
	const uint64_t c = UINT64_C(1) << garlic;
	uint64_t i;
	uint8_t k;

	/* naive implementation that used 2* 2^g memory */

	for (k = 0; k < lambda; k++) {
		_hashfull->Hash2(r + (c-1)*H_LEN, H_LEN, r, H_LEN, r+(c*H_LEN));
		_hashfast->ResetState();

		for (i = 1; i < c; i++) {
			_hashfast->Hash(i, r+((c+i-1)*H_LEN), r+(phi(i,garlic)*H_LEN), 
				r+((c+i)*H_LEN));
		}
		memcpy(r, r+(c*H_LEN), c*H_LEN); // v <- r
	}

	/* reverse(c - 1, garlic) == c - 1 */
	memcpy(h, r + (c - 1) * H_LEN, H_LEN);
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
	return 2*(UINT64_C(1) << garlic) * H_LEN;
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
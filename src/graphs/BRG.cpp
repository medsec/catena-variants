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
BRG::process(const uint8_t x[H_LEN], const std::string structure, 
			const uint8_t garlic, const uint8_t *salt, 
			const uint8_t saltlen, uint8_t *r, uint8_t* h)
{
	const uint64_t c = UINT64_C(1) << garlic;
	const uint16_t H_LEN_FAST = _hashfast->getHlenFast();
	uint64_t i;

	uint8_t *previousR, *p;

	/* BRH */
	if(currentLambda % 2 == 0){
		H_First(r + (c-1)*H_LEN_FAST, r, r);
		_hashfast->ResetState();

		/* Replace r[reverse(i, garlic)] with new value */
		previousR = r;
		for (i = 1; i < c; i++) {
			p = r + reverse(i, garlic) * H_LEN_FAST;
			_hashfast->Hash(i, previousR, p, p);
			previousR = p;
		}
	}
	else{
		/* This is now sequential because (reverse(reverse(i, garlic), garlic) == i) */
		//_hashfull->Hash2(r + (c-1)*H_LEN, H_LEN, r, H_LEN, r);
		H_First(r + (c-1)*H_LEN_FAST, r, r);
		_hashfast->ResetState();
		p = r + H_LEN_FAST;
		for (i = 1; i < c; i++, p += H_LEN_FAST) {
			_hashfast->Hash(i, p - H_LEN_FAST, p, p);
		}
	}

	currentLambda += 1;

	/* reverse(c - 1, garlic) == c - 1 */
	memcpy(h, r + (c - 1) * H_LEN_FAST, H_LEN_FAST);
}
#pragma GCC diagnostic pop

uint64_t
BRG::index(const uint64_t ind, uint8_t garlic){
	if (currentLambda % 2 == 0) return ind;
	else return reverse(ind, garlic);
}

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
	const uint16_t H_LEN_FAST = _hashfast->getHlenFast();
	return (UINT64_C(1) << garlic) * H_LEN_FAST;
}


std::string
BRG::getDefaultStructure()const{
	return STRUCTURE;
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

void 
BRG::H_First(const uint8_t* i1, const uint8_t* i2, uint8_t* hash){
	_hashfast->ResetState();
	const uint16_t H_LEN_FAST = _hashfast->getHlenFast();
	uint8_t *x = (uint8_t*) malloc(H_LEN);
	_hashfull->Hash2(i1, H_LEN_FAST, i2, H_LEN_FAST,x);

	for(uint8_t i = 0; i<(H_LEN_FAST/H_LEN);++i){
		_hashfull->Hash2(&i,1, x, H_LEN, hash+i*H_LEN);
	}
	free(x);
}

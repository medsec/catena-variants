#include "phi_xs.hpp"
#include "../registry.hpp"

using namespace Catena_Variants;

//Register the registrable with the Registry so it can be used by CatenaFactory
Registry<Phi_XS> regpxs;

Phi_XS::Phi_XS()
:Registerable("Phi-XS", "PX", 
	"Phi layer with XORSHIFT as indexing function")
{}

void
Phi_XS::ResetState()
{}

void 
Phi_XS::process(const uint8_t garlic, uint8_t *r, uint8_t* h)
{
	const uint64_t c = UINT64_C(1) << garlic;
	const uint16_t H_LEN_FAST = _hashfast->getHlenFast();
	uint64_t i, j;
	uint8_t *tmp = (uint8_t*) malloc(H_LEN_FAST);
	uint8_t *tmp2 = (uint8_t*) malloc(H_LEN_FAST);

	_hashfast->Hash(0, (r+H_LEN_FAST*(c-1)), (r+H_LEN_FAST*(c-2)),tmp);  
	_hashfast->Hash(0, (r+H_LEN_FAST*(c-3)), (r+H_LEN_FAST*(c-4)),tmp2);
	initXSState(tmp, tmp2);
	_hashfast->ResetState();

	j = xorshift1024star() >> (64 - garlic);
	_hashfast->Hash(0, r+H_LEN_FAST*(c-1), r + j * H_LEN_FAST, r); 

	for(i = 1; i < c; i++){
		j = xorshift1024star() >> (64 - garlic);
		_hashfast->Hash(i, r + (i-1) * H_LEN_FAST, r + j* H_LEN_FAST, r + i * H_LEN_FAST); 
	}
	//p_old???

	memcpy(h, r + (c - 1) * H_LEN_FAST, H_LEN_FAST);
}


void 
Phi_XS::initXSState(const uint8_t* a, const uint8_t* b){
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
Phi_XS::xorshift1024star() {
	uint64_t s0 = s[p];
	uint64_t s1 = s[ p = (p+1) & 15 ];
	s1 ^= s1 << 31; // a
	s1 ^= s1 >> 11; // b
	s0 ^= s0 >> 30; // c
	return ( s[p] = s0 ^ s1 ) * UINT64_C(1181783497276652981);
}
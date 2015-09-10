#include "phi_lsb.hpp"
#include "../registry.hpp"

using namespace Catena_Variants;

//Register the registrable with the Registry so it can be used by CatenaFactory
Registry<Phi_Lsb> regplsb;

Phi_Lsb::Phi_Lsb()
:Registerable("Phi-Lsb", "PL", 
	"Phi layer with LSB as indexing function")
{}

void
Phi_Lsb::ResetState()
{}

void 
Phi_Lsb::process(const uint8_t garlic, uint8_t *r, uint8_t* h)
{
	const uint64_t c = UINT64_C(1) << garlic;
	const uint16_t H_LEN_FAST = _hashfast->getHlenFast();
	uint64_t i;

	_hashfast->Hash(0, r, r+R((uint64_t*)(r+H_LEN_FAST*(c-1)), garlic)*H_LEN_FAST, r);
	uint8_t * p = r;
	uint8_t * p_old = p;
	p += H_LEN_FAST;
	for (i = 1; i < c; i++, p += H_LEN_FAST) {
			_hashfast->Hash(i, r + R((uint64_t*)(p_old), garlic)*H_LEN_FAST, p_old, p);
			p_old=p;
		}
	memcpy(h, r + (c - 1) * H_LEN_FAST, H_LEN_FAST);
}


uint64_t
Phi_Lsb::R(uint64_t* input, const uint8_t n){
	return  input[0]>>(64-n);
}
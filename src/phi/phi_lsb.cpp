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

	_hashfast->Hash(0, r + _graph->index(0, garlic), r + _graph->index(R((uint64_t*)(r+H_LEN_FAST*_graph->index(c-1, garlic)), garlic), garlic)*H_LEN_FAST, r + _graph->index(0, garlic));
	uint8_t * p = r + _graph->index(0, garlic);
	uint8_t * p_old = p;
	for (i = 1; i < c; i++/*, p += H_LEN_FAST*/) {
			p = r+_graph->index(i, garlic)*(H_LEN_FAST);
			_hashfast->Hash(i, p_old, r + _graph->index(R((uint64_t*)(p_old), garlic), garlic)*H_LEN_FAST,  p);
			p_old=p;
		}
	memcpy(h, r + _graph->index(c - 1, garlic) * H_LEN_FAST, H_LEN_FAST);
}


uint64_t
Phi_Lsb::R(uint64_t* input, const uint8_t n){
	return  input[0]>>(64-n);
}
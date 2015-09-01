#include "mulhash.hpp"
#include "../registry.hpp"


using namespace Catena_Variants;

//Register the registrable with the Registry so it can be used by CatenaFactory
Registry<MulHash> regmuha;

MulHash::MulHash()
:Registerable("Mult-Hash", "MH", "Multiplication based hash function as implemented by Bill Cox")
{}

void
MulHash::ResetState() {}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void 
MulHash::Hash(int vindex, const uint8_t* i1, const uint8_t* i2, 
			uint8_t* hash)
{
	/* Cast to uint32 */
	const uint32_t *prevBlock = (const uint32_t *)(const void *)i1;
	const uint32_t *fromBlock = (const uint32_t *)(const void *)i2;
	uint32_t *toBlock = (uint32_t *)(void *)hash;

	uint32_t value = prevBlock[H_LEN/sizeof(uint32_t)-1];

	uint32_t i;
	for(i = 0; i < H_LEN/sizeof(uint32_t); i++) {
		value = value*(*prevBlock++ | 3) + *fromBlock++;
		*toBlock++ = value;
	}
}
#pragma GCC diagnostic pop

uint16_t
MulHash::getHlenFast()const{
  return H_LEN_FAST;
}
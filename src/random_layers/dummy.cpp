#include "dummy.hpp"
#include "../registry.hpp"

using namespace Catena_Variants;

//Register the registrable with the Registry so it can be used by CatenaFactory
Registry<Dummy> regDummy;

Dummy::Dummy()
:Registerable("Dummy", "D", "Dummy random layer that simply does nothing"){}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void 
Dummy::process(const uint8_t x[H_LEN], const uint8_t lambda,
					const uint8_t garlic, const uint8_t *salt, 
                  	const uint8_t saltlen, uint8_t *r)
{}
#pragma GCC diagnostic pop

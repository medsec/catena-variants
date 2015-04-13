#include <cstdlib>
#include <cstring>

#include "default_algorithm.hpp"
#include "../registry.hpp"

using namespace Catena_Variants;

//Register the registrable with the Registry so it can be used by CatenaFactory
Registry<DefaultAlgorithm> regDA;

DefaultAlgorithm::DefaultAlgorithm()
:Registerable("DefaultAlgorithm", "DA", "This is the flap function used in Catena")
{

}

void
DefaultAlgorithm::flap(const uint8_t x[H_LEN], const uint8_t lambda, 
	const uint8_t garlic, const uint8_t *salt, const uint8_t saltlen, 
	uint8_t h[H_LEN])
{
	const uint64_t c = UINT64_C(1) << garlic;
	/* allocate memory for all further steps */
	uint8_t *r = (uint8_t*) malloc(_graph->getMemoryRequirement(garlic));

	/* Top row */
	uint8_t *tmp = (uint8_t*) malloc(H_LEN);

	memcpy(tmp, x, H_LEN);
	tmp[H_LEN-1] ^= 1;
	_hashfull->Hash2(x, H_LEN, tmp, H_LEN, r); //v_0 <- H(x||xXOR1)
	_hashfast->ResetState();

	_hashfast->Hash(1, r, x, r+H_LEN); //v_1 <- H'(v_0||x)

	for(uint64_t i = 2; i < c; i++){
		_hashfast->Hash(i, r + (i-1)*H_LEN, r + (i-2)*H_LEN, r + i*H_LEN);
	}

	/*Gamma Function => RandomLayer*/
	_randomlayer->process(x, lambda, garlic, salt, saltlen, r);

	/*F function => Graph*/
	_graph->process(x, lambda, garlic, salt, saltlen, r, h);

	/*Clean up*/
	free(r);
	free(tmp);
}
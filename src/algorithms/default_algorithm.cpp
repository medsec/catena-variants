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
DefaultAlgorithm::flap(const uint8_t* x, const uint16_t xlen, const uint8_t lambda, 
	const uint8_t garlic, const uint8_t *salt, const uint8_t saltlen, 
	uint8_t* h)
{
	const uint16_t H_LEN_FAST = _hashfast->getHlenFast();
	const uint64_t c = UINT64_C(1) << garlic;
	/* allocate memory for all further steps */
	uint8_t *r = (uint8_t*) malloc(_graph->getMemoryRequirement(garlic));
	uint8_t *vm2 = (uint8_t*) malloc(H_LEN_FAST);
	uint8_t *vm1 = (uint8_t*) malloc(H_LEN_FAST);

	H_INIT(x, xlen, vm1, vm2);

	_hashfast->ResetState();
	_hashfast->Hash(0, vm1, vm2, r);
	_hashfast->Hash(1, r, vm1, r+H_LEN_FAST);

	/* Top row */

	for(uint64_t i = 2; i < c; i++){
		_hashfast->Hash(i, r + (i-1)*H_LEN_FAST, r + (i-2)*H_LEN_FAST, r + i*H_LEN_FAST);
	}

	


	/*Gamma Function => RandomLayer*/
	_randomlayer->process(x, lambda, garlic, salt, saltlen, r);

	/*F function => Graph*/
	_graph->process(x, lambda, garlic, salt, saltlen, r, h);

	/*Clean up*/
	free(r);
	free(vm2);
	free(vm1);
}

void 
DefaultAlgorithm::H_INIT(const uint8_t* x, const uint16_t xlen,  uint8_t *vm1, uint8_t *vm2){
	const uint16_t H_LEN_FAST = _hashfast->getHlenFast();
	const uint8_t l = 2* H_LEN_FAST/H_LEN;
	uint8_t *tmp = (uint8_t*) malloc(l*H_LEN);

	for(uint8_t i=0; i!=l;++i){
		_hashfull->Hash2(&i, 1, x, xlen, tmp+i*H_LEN);
	}
	memcpy(vm1, tmp, H_LEN_FAST);
	memcpy(vm2, tmp+(l/2*H_LEN), H_LEN_FAST);
}

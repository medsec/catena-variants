#include <cstdlib>
#include <cstring>
#include "default_algorithm-phi.hpp"
#include "../registry.hpp"

using namespace Catena_Variants;

//Register the registrable with the Registry so it can be used by CatenaFactory
Registry<DefaultAlgorithmPhi> regDAP;

DefaultAlgorithmPhi::DefaultAlgorithmPhi()
:Registerable("DefaultAlgorithmPhi", "DAP", "This is the flap function used in Catena")
{

}

void
DefaultAlgorithmPhi::flap(const uint8_t* x, const uint16_t xlen, const uint8_t lambda, 
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

	/*Phi-Function*/
	//Phi(r, h, garlic);
	_philayer->process(garlic,r, h);

	/*Clean up*/
	free(r);
	free(vm2);
	free(vm1);
}

void
DefaultAlgorithmPhi::Phi(uint8_t *r, uint8_t *h, const uint8_t garlic){
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
DefaultAlgorithmPhi::R(uint64_t* input, const uint8_t n){
	return  input[0]>>(64-n);
}

void 
DefaultAlgorithmPhi::H_INIT(const uint8_t* x, const uint16_t xlen,  uint8_t *vm1, uint8_t *vm2){
	const uint16_t H_LEN_FAST = _hashfast->getHlenFast();
	const uint8_t l = 2* H_LEN_FAST/H_LEN;
	uint8_t *tmp = (uint8_t*) malloc(l*H_LEN);

	for(uint8_t i=0; i!=l;++i){
		_hashfull->Hash2(&i, 1, x, xlen, tmp+i*H_LEN);
	}
	memcpy(vm1, tmp, H_LEN_FAST);
	memcpy(vm2, tmp+(l/2*H_LEN), H_LEN_FAST);
}

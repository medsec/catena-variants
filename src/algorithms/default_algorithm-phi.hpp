#pragma once

#include "../algorithm.hpp"

namespace Catena_Variants {

class DefaultAlgorithmPhi : public Algorithm<DefaultAlgorithmPhi>{
public:
	DefaultAlgorithmPhi();
	virtual ~DefaultAlgorithmPhi() = default;

	virtual void flap(const uint8_t* x, const uint16_t xlen, const uint8_t lambda, 
		const uint8_t garlic, const uint8_t *salt, const uint8_t saltlen, 
		uint8_t* h);

private:
	void H_INIT(const uint8_t* x, const uint16_t xlen, uint8_t *vm1, uint8_t *vm2);
	void Phi(uint8_t *r, uint8_t *h, const uint8_t garlic);
	uint64_t R(uint64_t* input, const uint8_t n);
};

} //end namespace
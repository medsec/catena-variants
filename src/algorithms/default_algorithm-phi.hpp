#pragma once

#include "../algorithm.hpp"

namespace Catena_Variants {

class DefaultAlgorithmPhi : public Algorithm<DefaultAlgorithmPhi>{
public:
	DefaultAlgorithmPhi();
	virtual ~DefaultAlgorithmPhi() = default;

	virtual void flap(const uint8_t* x, const uint16_t xlen, const std::string structure, 
		const uint8_t garlic, const uint8_t *salt, const uint8_t saltlen, 
		uint8_t* h);

private:
	void H_INIT(const uint8_t* x, const uint16_t xlen, uint8_t *vm1, uint8_t *vm2);
};

} //end namespace
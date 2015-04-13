#pragma once

#include "../algorithm.hpp"

namespace Catena_Variants {

class DefaultAlgorithm : public Algorithm<DefaultAlgorithm>{
public:
	DefaultAlgorithm();
	virtual ~DefaultAlgorithm() = default;

	virtual void flap(const uint8_t x[H_LEN], const uint8_t lambda, 
		const uint8_t garlic, const uint8_t *salt, const uint8_t saltlen, 
		uint8_t h[H_LEN]);
};

} //end namespace
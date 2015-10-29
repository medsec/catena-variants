#pragma once

#include "../philayer.hpp"

namespace Catena_Variants{

class Phi_Lsb :  public PhiLayer<Phi_Lsb>
{
	public:
		Phi_Lsb();

		virtual void process(const uint8_t garlic, uint8_t *r, uint8_t* h);

		/* Does nothing
		*/
		virtual void ResetState(); 

		uint64_t R(uint64_t* input, const uint8_t n);

};
}
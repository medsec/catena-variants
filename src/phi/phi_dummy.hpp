#pragma once

#include "../philayer.hpp"

namespace Catena_Variants{

class Phi_Dummy :  public PhiLayer<Phi_Dummy>
{
	public:
		Phi_Dummy();

		virtual void process(const uint8_t garlic, uint8_t *r, uint8_t* h);

		/* Does nothing
		*/
		virtual void ResetState(); 

};
}
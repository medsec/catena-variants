#pragma once

#include "../philayer.hpp"

namespace Catena_Variants{

class Phi_XS :  public PhiLayer<Phi_XS>
{
	public:
		Phi_XS();

		virtual void process(const uint8_t garlic, uint8_t *r, uint8_t* h);

		/* Does nothing
		*/
		virtual void ResetState(); 

		uint64_t R(uint64_t* input, const uint8_t n);

	private:
		/* Init the state for Xorshift with two 512bit values
	 	*/
		void initXSState(const uint8_t* a, const uint8_t* b);

		/*  Xorshift RNG with a 1024bit state that must be seeded first.
	 	*/
		uint64_t xorshift1024star();

		int p;
		uint64_t s[16];
};
}
#pragma once

#ifdef __PCLMUL__
#include <emmintrin.h>
#include <wmmintrin.h>
#include <smmintrin.h>
#endif //__PCLMUL__

#include "../hashfast.hpp"

namespace Catena_Variants{

class Cfaes :  public HashFast<Cfaes>
{
	public:
		Cfaes();
		~Cfaes() = default;
		Cfaes(Cfaes const&) = default;
		Cfaes& operator=(Cfaes const&) = default;

		virtual void Hash(int vindex, const uint8_t* i1, const uint8_t* i2, 
			uint8_t* hash);

		/* Does nothing
		*/
		virtual void ResetState(); 

		virtual uint16_t getHlenFast()const;
	
	private:
	#ifdef __PCLMUL__

	#else

	#endif

	uint16_t const H_LEN_FAST = 1024;

};
}
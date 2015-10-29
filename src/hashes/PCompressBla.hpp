#pragma once

#include "../hashfast.hpp"

namespace Catena_Variants{

class PCompressBla :  public HashFast<PCompressBla>
{
	public:
		PCompressBla();
		~PCompressBla() = default;
		PCompressBla(PCompressBla const&) = default;
		PCompressBla& operator=(PCompressBla const&) = default;


		virtual void Hash(int vindex, const uint8_t* i1, const uint8_t* i2, 
			uint8_t* hash);

		/* Does nothing
		*/
		virtual void ResetState(); 

		virtual uint16_t getHlenFast()const;
	
	private:

	uint16_t const H_LEN_FAST = 64;

};
}
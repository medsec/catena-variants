#pragma once

#include "../hashfast.hpp" 

namespace Catena_Variants{

class argonCF :  public HashFast<argonCF>
{
public:
	argonCF();
	argonCF(argonCF const&) = default;
	argonCF& operator=(argonCF const&) = default;
	~argonCF() = default;

	virtual void Hash(int vindex, const uint8_t* i1, const uint8_t* i2, 
			uint8_t* hash);

	/* Dummy
	*/
	virtual void ResetState(); 

	virtual uint16_t getHlenFast()const;
private:
	uint16_t const H_LEN_FAST = 1024;
};

} //end namespace
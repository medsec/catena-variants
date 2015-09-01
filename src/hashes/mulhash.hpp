#pragma once

#include "../hashfast.hpp" 

namespace Catena_Variants{

class MulHash :  public HashFast<MulHash>
{
public:
	MulHash();
	MulHash(MulHash const&) = default;
	MulHash& operator=(MulHash const&) = default;
	~MulHash() = default;

	/* Multiplication based hashing. Implemented by Bill Cox in his Catena branch.
	 * This version is stripped off the randomization in the first row.
	 * One could use a Gamma function to replace it.
	 */
	virtual void Hash(int vindex, const uint8_t* i1, const uint8_t* i2, 
			uint8_t* hash);

	/* Dummy
	*/
	virtual void ResetState(); 

	virtual uint16_t getHlenFast()const;
private:
	uint16_t const H_LEN_FAST = 64;
};

} //end namespace
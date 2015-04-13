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
			uint8_t hash[H_LEN]);

	/* Dummy
	*/
	virtual void ResetState(); 

};

} //end namespace
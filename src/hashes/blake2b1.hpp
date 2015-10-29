#pragma once

#include <endian.h>
//Loading in blake2b is faster on little endian
#if __BYTE_ORDER == __LITTLE_ENDIAN
#ifndef NATIVE_LITTLE_ENDIAN
  #define NATIVE_LITTLE_ENDIAN
#endif
#endif

#if defined(__SSE2__)
//SSE optimized
#include "blake2-sse/blake2.h"
#else
//not SSE optimized
#include "blake2-ref/blake2.h"
#endif

#include "../hashfast.hpp" 

namespace Catena_Variants{

class Blake2b1 :  public HashFast<Blake2b1>
{
public:
	Blake2b1();
	Blake2b1(Blake2b1 const&);
	Blake2b1& operator=(Blake2b1 const&);
	~Blake2b1();

	/* Single round of Blake2b on a single state
	 */
	virtual void Hash(int vindex, const uint8_t* i1, const uint8_t* i2, 
			uint8_t* hash);

	/* Resets the Blake2b state
	*/
	virtual void ResetState(); 

	virtual uint16_t getHlenFast()const;

private:
	blake2b_state* _state;
	uint16_t const H_LEN_FAST = 64;
};

} //end namespace
#pragma once

#include <endian.h>
//Loading in blake2b is faster on little endian
#if __BYTE_ORDER == __LITTLE_ENDIAN
#ifndef NATIVE_LITTLE_ENDIAN
  #define NATIVE_LITTLE_ENDIAN
#endif
#endif

#ifdef __SSE2__
//SSE optimized
#include "blake2-sse/blake2.h"
#else
//not SSE optimized
#include "blake2-ref/blake2.h"
#endif

#include "../hashfull.hpp" 
#include "../hashfast.hpp"

namespace Catena_Variants{

class Blake2b :  public HashFull<Blake2b>,  public HashFast<Blake2b>{
public:
	Blake2b();

	Blake2b(Blake2b const&) = default;
	Blake2b& operator=(Blake2b const&) = default;
	~Blake2b() = default;

	virtual void Hash1(const uint8_t *input, const uint32_t inputlen,
			    uint8_t hash[H_LEN])const;


	virtual void Hash2(const uint8_t *i1, const uint8_t i1len,
			    const uint8_t *i2, const uint8_t i2len,
			    uint8_t hash[H_LEN])const;


	virtual void Hash3(const uint8_t *i1, const uint8_t i1len,
			    const uint8_t *i2, const uint8_t i2len,
			    const uint8_t *i3, const uint8_t i3len,
			    uint8_t hash[H_LEN])const;


	virtual void Hash4(const uint8_t *i1, const uint8_t i1len,
			    const uint8_t *i2, const uint8_t i2len,
			    const uint8_t *i3, const uint8_t i3len,
			    const uint8_t *i4, const uint8_t i4len,
			    uint8_t hash[H_LEN])const;


	virtual void Hash5(const uint8_t *i1, const uint8_t i1len,
			    const uint8_t *i2, const uint8_t i2len,
			    const uint8_t *i3, const uint8_t i3len,
			    const uint8_t *i4, const uint8_t i4len,
			    const uint8_t *i5, const uint8_t i5len,
			    uint8_t hash[H_LEN])const;


	/* Alias for Hash2. The 2 Inputs are expected to be of size H_LEN
	 */
	virtual void Hash(int vindex, const uint8_t* i1, const uint8_t* i2, 
			uint8_t hash[H_LEN]);

	/* Dummy
	*/
	virtual void ResetState(); 
private:

};

} //end namespace
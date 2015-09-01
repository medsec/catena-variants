#pragma once

#include "../hashfull.hpp" 
#include "../hashfast.hpp"

namespace Catena_Variants{

class SHA_512 :  public HashFull<SHA_512>,  public HashFast<SHA_512>{
public:
	SHA_512();

	SHA_512(SHA_512 const&) = default;
	SHA_512& operator=(SHA_512 const&) = default;
	~SHA_512() = default;

	virtual void Hash1(const uint8_t *input, const uint32_t inputlen,
			    uint8_t hash[H_LEN])const;


	virtual void Hash2(const uint8_t *i1, const uint16_t i1len,
			    const uint8_t *i2, const uint16_t i2len,
			    uint8_t hash[H_LEN])const;


	virtual void Hash3(const uint8_t *i1, const uint16_t i1len,
			    const uint8_t *i2, const uint16_t i2len,
			    const uint8_t *i3, const uint16_t i3len,
			    uint8_t hash[H_LEN])const;


	virtual void Hash4(const uint8_t *i1, const uint16_t i1len,
			    const uint8_t *i2, const uint16_t i2len,
			    const uint8_t *i3, const uint16_t i3len,
			    const uint8_t *i4, const uint16_t i4len,
			    uint8_t hash[H_LEN])const;


	virtual void Hash5(const uint8_t *i1, const uint16_t i1len,
			    const uint8_t *i2, const uint16_t i2len,
			    const uint8_t *i3, const uint16_t i3len,
			    const uint8_t *i4, const uint16_t i4len,
			    const uint8_t *i5, const uint16_t i5len,
			    uint8_t hash[H_LEN])const;


	/* Alias for Hash2. The 2 Inputs are expected to be of size H_LEN
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
#include "blake2b.hpp"
#include "../registry.hpp"

using namespace Catena_Variants;

//Register the registrable with the Registry so it can be used by CatenaFactory
Registry<Blake2b> regB2;

Blake2b::Blake2b()
:Registerable("Blake2b", "B2", 
  "Blake2b hash function. Reference or SSE optimized")
{}

void
Blake2b::Hash1(const uint8_t *input, const uint32_t inputlen,
		      uint8_t hash[H_LEN]) const
{
  blake2b_state ctx;
  blake2b_init(&ctx,H_LEN);
  blake2b_update(&ctx, input, inputlen);
  blake2b_final(&ctx, hash, H_LEN);
}


/***************************************************/

void
Blake2b::Hash2(const uint8_t *i1, const uint16_t i1len,
		    const uint8_t *i2, const uint16_t i2len,
		    uint8_t hash[H_LEN]) const
{
  blake2b_state ctx;
  blake2b_init(&ctx,H_LEN);
  blake2b_update(&ctx, i1, i1len);
  blake2b_update(&ctx, i2, i2len);
  blake2b_final(&ctx, hash, H_LEN);
}



/***************************************************/

void
Blake2b::Hash3(const uint8_t *i1, const uint16_t i1len,
		    const uint8_t *i2, const uint16_t i2len,
		    const uint8_t *i3, const uint16_t i3len,
		    uint8_t hash[H_LEN]) const
{
  blake2b_state ctx;
  blake2b_init(&ctx,H_LEN);
  blake2b_update(&ctx, i1, i1len);
  blake2b_update(&ctx, i2, i2len);
  blake2b_update(&ctx, i3, i3len);
  blake2b_final(&ctx, hash, H_LEN);
}

/***************************************************/

void
Blake2b::Hash4(const uint8_t *i1, const uint16_t i1len,
		    const uint8_t *i2, const uint16_t i2len,
		    const uint8_t *i3, const uint16_t i3len,
		     const uint8_t *i4, const uint16_t i4len,
		    uint8_t hash[H_LEN]) const
{
  blake2b_state ctx;
  blake2b_init(&ctx,H_LEN);
  blake2b_update(&ctx, i1, i1len);
  blake2b_update(&ctx, i2, i2len);
  blake2b_update(&ctx, i3, i3len);
  blake2b_update(&ctx, i4, i4len);
  blake2b_final(&ctx, hash, H_LEN);
}


/***************************************************/

void
Blake2b::Hash5(const uint8_t *i1, const uint16_t i1len,
		    const uint8_t *i2, const uint16_t i2len,
		    const uint8_t *i3, const uint16_t i3len,
		    const uint8_t *i4, const uint16_t i4len,
		    const uint8_t *i5, const uint16_t i5len,
		    uint8_t hash[H_LEN]) const
{
  blake2b_state ctx;
  blake2b_init(&ctx,H_LEN);
  blake2b_update(&ctx, i1, i1len);
  blake2b_update(&ctx, i2, i2len);
  blake2b_update(&ctx, i3, i3len);
  blake2b_update(&ctx, i4, i4len);
  blake2b_update(&ctx, i5, i5len);
  blake2b_final(&ctx, hash, H_LEN);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void 
Blake2b::Hash(int vindex, const uint8_t* i1, const uint8_t* i2, 
			uint8_t* hash)
{
	Hash2(i1, H_LEN, i2, H_LEN, hash);
}
#pragma GCC diagnostic pop


void Blake2b::ResetState(){}

uint16_t
Blake2b::getHlenFast()const{
  return H_LEN_FAST;
}
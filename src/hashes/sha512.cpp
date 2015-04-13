#include <openssl/sha.h>

#include "sha512.hpp"
#include "../registry.hpp"

using namespace Catena_Variants;

//Register the registrable with the Registry so it can be used by CatenaFactory
Registry<SHA_512> regs512;

SHA_512::SHA_512()
:Registerable("SHA512", "S512", "SHA512. Depends on OpenSSL")
{}


/***************************************************/

void
SHA_512::Hash1(const uint8_t *input, const uint32_t inputlen,
		      uint8_t hash[H_LEN]) const
{
  SHA512_CTX ctx;
  SHA512_Init(&ctx);
  SHA512_Update(&ctx, input, inputlen);
  SHA512_Final(hash, &ctx);
}

/***************************************************/

void
SHA_512::Hash2(const uint8_t *i1, const uint8_t i1len,
		    const uint8_t *i2, const uint8_t i2len,
		    uint8_t hash[H_LEN]) const
{
  SHA512_CTX ctx;
  SHA512_Init(&ctx);
  SHA512_Update(&ctx, i1, i1len);
  SHA512_Update(&ctx, i2, i2len);
  SHA512_Final(hash, &ctx);
}



/***************************************************/

void
SHA_512::Hash3(const uint8_t *i1, const uint8_t i1len,
		    const uint8_t *i2, const uint8_t i2len,
		    const uint8_t *i3, const uint8_t i3len,
		    uint8_t hash[H_LEN]) const
{

  SHA512_CTX ctx;
  SHA512_Init(&ctx);
  SHA512_Update(&ctx, i1, i1len);
  SHA512_Update(&ctx, i2, i2len);
  SHA512_Update(&ctx, i3, i3len);
  SHA512_Final(hash, &ctx);
}

/***************************************************/

void
SHA_512::Hash4(const uint8_t *i1, const uint8_t i1len,
		    const uint8_t *i2, const uint8_t i2len,
		    const uint8_t *i3, const uint8_t i3len,
		     const uint8_t *i4, const uint8_t i4len,
		    uint8_t hash[H_LEN]) const
{
  SHA512_CTX ctx;
  SHA512_Init(&ctx);
  SHA512_Update(&ctx, i1, i1len);
  SHA512_Update(&ctx, i2, i2len);
  SHA512_Update(&ctx, i3, i3len);
  SHA512_Update(&ctx, i4, i4len);
  SHA512_Final(hash, &ctx);
}


/***************************************************/

void
SHA_512::Hash5(const uint8_t *i1, const uint8_t i1len,
		    const uint8_t *i2, const uint8_t i2len,
		    const uint8_t *i3, const uint8_t i3len,
		    const uint8_t *i4, const uint8_t i4len,
		    const uint8_t *i5, const uint8_t i5len,
		    uint8_t hash[H_LEN]) const
{
  SHA512_CTX ctx;
  SHA512_Init(&ctx);
  SHA512_Update(&ctx, i1, i1len);
  SHA512_Update(&ctx, i2, i2len);
  SHA512_Update(&ctx, i3, i3len);
  SHA512_Update(&ctx, i4, i4len);
  SHA512_Update(&ctx, i5, i5len);
  SHA512_Final(hash, &ctx);
}


/***************************************************/


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void 
SHA_512::Hash(int vindex, const uint8_t* i1, const uint8_t* i2, 
      uint8_t hash[H_LEN])
{
  Hash2(i1, H_LEN, i2, H_LEN, hash);
}
#pragma GCC diagnostic pop


void SHA_512::ResetState(){}
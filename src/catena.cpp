#include <string.h>
#include <stdio.h>
#include <byteswap.h>
#include <stdlib.h>
// #include <sys/param.h>
#define __STDC_CONSTANT_MACROS
#include <stdint.h>
#include <stdexcept>

#include "catena.hpp"
#include "catena-helpers.hpp"

#include <algorithm>

using namespace Catena_Variants;


/***************************************************/


Catena::Catena()
:_overwrite(false), _versionID(NULL)
{}


void 
Catena::_Catena( uint8_t *pwd,   const uint32_t pwdlen,
	     const uint8_t *salt,  const uint8_t  saltlen,
	     const uint8_t *data,  const uint32_t datalen,
	     const std::string structure, const uint8_t  min_garlic,
	     const uint8_t garlic, const size_t  hashlen,
	     const uint8_t client, const uint8_t  tweak_id, uint8_t *hash)
{
  const uint16_t H_LEN_FAST = _algorithm->getHlenFast();
  uint8_t* x_long = (uint8_t*) malloc(H_LEN_FAST);

  uint8_t x[H_LEN];
  uint8_t hv[H_LEN];
  uint8_t t[4];
  uint8_t c;

  if((hashlen > H_LEN) || (garlic > 63) || (min_garlic > garlic) || 
    (structure == "") || (garlic == 0) ){
     throw std::range_error("Catena parameters out of range");
  }

  /*Compute H(V)*/
  if(_versionID == NULL){
    _hash->Hash1(_algorithm->getDefaultVersionID(), 
        strlen((char*)_algorithm->getDefaultVersionID()), hv);
  }
  else{
    _hash->Hash1(_versionID, 
        strlen((char*)_versionID), hv); 
  }



  /* Compute Tweak */
  t[0] = tweak_id;
  t[1] = std::count(structure.begin(), structure.end(), 'g');
  t[2] = hashlen;
  t[3] = saltlen;

  /* Compute H(AD) */
  _hash->Hash1((uint8_t *) data, datalen,x);

  /* Compute the initial value to hash  */
  _hash->Hash5(hv, H_LEN, t, 4, x, H_LEN, pwd,  pwdlen, salt, saltlen, x);

  /*Overwrite Password if enabled*/
  if(_overwrite){
    erasepwd(pwd,pwdlen);
  }


  _algorithm->flap(x, H_LEN, structure, (min_garlic+1)/2, salt, saltlen, x_long);

  for(c=min_garlic; c <= garlic; c++)
  {
      if(c==min_garlic){
        _algorithm->flap(x_long,H_LEN_FAST, structure, c, salt, saltlen, x_long);  
      }
      else{
        _algorithm->flap(x,H_LEN, structure, c, salt, saltlen, x_long);
      }
      
      if( (c==garlic) && (client == CLIENT))
      {
        memcpy(hash, x_long, H_LEN);
        return;
      }
      _hash->Hash2(&c,1, x_long,H_LEN_FAST, x);
      memset(x+hashlen, 0, H_LEN-hashlen);
  }
  memcpy(hash, x, hashlen);
}


/***************************************************/

void
Catena::Default(uint8_t *pwd,   const uint32_t pwdlen,
	   const uint8_t *salt,  const uint8_t  saltlen,
	   const uint8_t *data,  const uint32_t datalen,
	   const std::string structure, const uint8_t  min_garlic,
	   const uint8_t garlic, const uint8_t  hashlen,  uint8_t *hash)
{
  return _Catena(pwd, pwdlen, salt, saltlen, data, datalen,
		  structure, min_garlic, garlic,
		  hashlen,  REGULAR, PASSWORD_HASHING_MODE, hash);

}


/***************************************************/


void
Catena::Naive(char *pwd,  const char *salt, const char *data,
		  uint8_t hash[H_LEN])
{
  return _Catena( (uint8_t  *) pwd, strlen(pwd),
		   (uint8_t  *) salt, strlen(salt),
		   (uint8_t  *) data, strlen(data),
		  _algorithm->getDefaultStructure(), _algorithm->getDefaulMinGarlic(),
      _algorithm->getDefaultGarlic(),
		   H_LEN, REGULAR, PASSWORD_HASHING_MODE, hash);
}

/***************************************************/


void
Catena::Simple(uint8_t *pwd,   const uint32_t pwdlen,
		  const uint8_t *salt,  const uint8_t  saltlen,
		  const uint8_t *data,  const uint32_t datalen,
		  uint8_t hash[H_LEN])
{
  return _Catena(pwd, pwdlen, salt, saltlen, data, datalen,
		  _algorithm->getDefaultStructure(), _algorithm->getDefaulMinGarlic(),
      _algorithm->getDefaultGarlic(), H_LEN,
		  REGULAR, PASSWORD_HASHING_MODE, hash);
}


/***************************************************/

void
Catena::Client(uint8_t  *pwd,   const uint32_t pwdlen,
		  const uint8_t  *salt,  const uint8_t  saltlen,
		  const uint8_t  *data,  const uint32_t datalen,
		  const std::string structure, const uint8_t  min_garlic,
		  const uint8_t  garlic, const uint8_t  hashlen,
		  uint8_t x[H_LEN])
{
  return _Catena(pwd, pwdlen, (uint8_t *) salt, saltlen, data, datalen,
		  structure, min_garlic, garlic, hashlen,
		  CLIENT, PASSWORD_HASHING_MODE, x);
}

/***************************************************/

void
Catena::Server(const uint8_t garlic,  const uint8_t x[H_LEN],
		  const uint8_t hashlen, uint8_t *hash)
{
  uint8_t z[H_LEN];

  if (hashlen > H_LEN){
    throw std::range_error("Catena parameters out of range");
  }

  _hash->Hash2(&garlic,1,x, H_LEN, z);
  memcpy(hash, z, hashlen);
}

/***************************************************/

void
Catena::Keyed_Server(const uint8_t garlic, const uint8_t x[H_LEN],
      const uint8_t *key,   const uint64_t uuid,
      const uint8_t hashlen, uint8_t *chash)
{
  uint8_t z[H_LEN];
  uint64_t tmp = TO_LITTLE_ENDIAN_64(uuid);
  int i;

  if (hashlen > H_LEN){
    throw std::range_error("Catena parameters out of range");
  }
  /* finalize */
  _hash->Hash2(&garlic,1,x, H_LEN, z);
  memcpy(chash, z, hashlen);

  /* encrypt with z=keystream*/
  _hash->Hash4(key, KEY_LEN,  (uint8_t*) &tmp, 8, &garlic, sizeof(uint8_t), key, 
      KEY_LEN, z);
   for(i=0; i<hashlen; i++) chash[i] ^= z[i];
}

/***************************************************/

void 
Catena::CI_Update(const uint8_t *old_hash,  const std::string structure,
         const uint8_t *salt,  const uint8_t saltlen,
	       const uint8_t old_garlic, const uint8_t new_garlic,
	       const uint8_t hashlen, uint8_t *new_hash)
{
  uint8_t c;
  uint8_t x[H_LEN];

  memcpy(x, old_hash, hashlen);
  memset(x+hashlen, 0, H_LEN-hashlen);

  for(c=old_garlic+1; c <= new_garlic; c++)
    {
      _algorithm->flap(x,H_LEN, structure, c, salt, saltlen, x);
      _hash->Hash2(&c,1,x, H_LEN, x);
      memset(x+hashlen, 0, H_LEN-hashlen);
    }
  memcpy(new_hash,x,hashlen);
}


/***************************************************/

void 
Catena::KeyGeneration(uint8_t *pwd,   const uint32_t pwdlen,
	       const uint8_t *salt,  const uint8_t saltlen,
	       const uint8_t *data,  const uint32_t datalen,
	       const std::string structure, const uint8_t  min_garlic,
	       const uint8_t garlic, uint32_t keylen,
	       const uint8_t key_id, uint8_t *key)
{
  uint8_t hash[H_LEN];
  const uint8_t zero = 0;
  const uint32_t len = keylen/H_LEN;
  const uint32_t rest = keylen%H_LEN;
  uint64_t i;
  keylen = TO_LITTLE_ENDIAN_32(keylen);

  _Catena(pwd, pwdlen, salt, saltlen, data, datalen,
	   structure, min_garlic, garlic, H_LEN, REGULAR, KEY_DERIVATION_MODE,
	   hash);

  for(i=0; i < len; i++) {
    uint64_t tmp = TO_LITTLE_ENDIAN_64(i);
    _hash->Hash5(&zero, 1, (uint8_t *) &tmp, 8, &key_id, 1,(uint8_t *) &keylen,
      4, hash, H_LEN, &key[i*H_LEN]);
  }

  if(rest)
    {
      uint64_t tmp = TO_LITTLE_ENDIAN_64(i);
      _hash->Hash5(&zero, 1, (uint8_t *) &tmp, 8, &key_id, 1,
        (uint8_t *) &keylen, 4, hash, H_LEN, hash);
      memcpy(&key[len*H_LEN], hash,rest);
    }
}


/***************************************************/

void 
Catena::KeyedHashing(uint8_t *pwd,   const uint32_t pwdlen,
			  const uint8_t *salt,  const uint8_t saltlen,
			  const uint8_t *data,  const uint32_t datalen,
			  const std::string structure, const uint8_t  min_garlic,
			  const uint8_t garlic, const uint8_t  hashlen,
			  const uint8_t *key,   const uint64_t uuid,
			  uint8_t *chash)
{
  uint8_t keystream[H_LEN];
  uint64_t tmp = TO_LITTLE_ENDIAN_64(uuid);
  int i;

   _Catena(pwd, pwdlen, salt, saltlen, data, datalen,
	    structure, min_garlic, garlic, hashlen,
	    REGULAR, PASSWORD_HASHING_MODE, chash);

   _hash->Hash3(key, KEY_LEN,  (uint8_t*) &tmp, 8, key, KEY_LEN, keystream);

   for(i=0; i<hashlen; i++) chash[i] ^= keystream[i];
}


void
Catena::enableOverwrite()
{
  _overwrite = true;
}


void
Catena::setAlgorithm(AAsptr a)
{
  _algorithm = a;
}


void
Catena::setFullHash(AHFUsptr h)
{
  _hash = h;
}


void
Catena::setVersionID(const uint8_t* v)
{
  _versionID = v;
}


const uint8_t* 
Catena::getDefaultVersionID() const
{
  return _algorithm->getDefaultVersionID();
}


std::string 
Catena::getDefaultStructure()const
{
  return _algorithm->getDefaultStructure();
}


uint8_t 
Catena::getDefaultGarlic()const
{
  return _algorithm->getDefaultGarlic();
}


uint8_t 
Catena::getDefaulMinGarlic()const
{
  return _algorithm->getDefaulMinGarlic();
}

uint64_t 
Catena::getMemoryRequirement(uint8_t garlic)const
{
  return _algorithm->getMemoryRequirement(garlic);
}
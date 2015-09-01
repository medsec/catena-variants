#pragma once

#include <stdint.h>
#include <string.h>
#include "constants.hpp"
#include "algorithm.hpp"

namespace Catena_Variants{

class Catena{
public:
	Catena();
	~Catena() = default;

	/*Enables overwriting the password when it is not longer required*/
	void enableOverwrite();

	/*Set the algorithm(flap function) to be used*/
	void setAlgorithm(AAsptr a);

	/*Sets the Hash function to be used*/
	void setFullHash(AHFUsptr h);

	/*Sets the VersionID to be used. Ohterwise the Graphs VersionID is used*/
	void setVersionID(const uint8_t* v);

	/* get default values from graph */
	uint8_t getDefaultLambda() const;
 	uint8_t getDefaultGarlic() const;
 	uint8_t getDefaulMinGarlic() const;
	const uint8_t* getDefaultVersionID() const;
	uint64_t getMemoryRequirement(uint8_t garlic) const;

	/* Default mode of invocation */
	void Default(uint8_t *pwd,   const uint32_t pwdlen,
		   const uint8_t *salt,  const uint8_t  saltlen,
		   const uint8_t *data,  const uint32_t datalen,
		   const uint8_t lambda, const uint8_t  min_garlic,
		   const uint8_t garlic, const uint8_t hashlen,  uint8_t *hash);


	/* API that assumes that the three parameter pwd, salt, and header
	 * are all null-terminated string.
	 */
	void Naive(char *pwd,  const char *salt, const char *data,
			 uint8_t hash[H_LEN]);


	/* Simple mode of invocation */
	void Simple(uint8_t *pwd,   const uint32_t pwdlen,
			   const uint8_t *salt,  const uint8_t  saltlen,
			   const uint8_t *data,  const uint32_t datalen,
			   uint8_t hash[H_LEN]);


	/* Expensive and memory consuming  password hashing part.
	 */
	void Client(uint8_t *pwd,   const uint32_t pwdlen,
			  const uint8_t *salt,  const uint8_t  saltlen,
			  const uint8_t *data,  const uint32_t datalen,
			  const uint8_t lambda, const uint8_t  min_garlic,
			  const uint8_t garlic, const uint8_t  hashlen,
			  uint8_t x[H_LEN]);

	/*  Computes the final step of the password hashing process. Requieres the
	 *  output of Catena::Client(...) as input
	 */
	void Server(const uint8_t garlic, const uint8_t x[H_LEN],
			  const uint8_t hashlen, uint8_t *hash);

	/*  Computes the final step of the password hashing process. Requires the
	 *  output of Catena_Client(...) as input. The resulting hash is encrypted
	 * 	with H(key || uuid || key) where key denotes a KEY_LEN-byte key and uuid 
	 * 	denotes a *UNIQUE* user ID.
	 *  Returns -1 if an an error occurred, otherwise 0.
	 */
	void Keyed_Server(const uint8_t garlic, const uint8_t x[H_LEN],
			const uint8_t *key,   const uint64_t uuid,
			const uint8_t hashlen, uint8_t *chash);

	/* Client independent update form an old hash */
	void CI_Update(const uint8_t *old_hash,  const uint8_t lambda,
			   const uint8_t *salt,  const uint8_t saltlen,
		       const uint8_t old_garlic, const uint8_t new_garlic,
		       const uint8_t hashlen, uint8_t *new_hash);


	/* Mode of operation that derives a key from a password */
	void KeyGeneration(uint8_t *pwd,   const uint32_t pwdlen,
		       const uint8_t *salt,  const uint8_t saltlen,
		       const uint8_t *data,  const uint32_t datalen,
		       const uint8_t lambda, const uint8_t  min_garlic,
		       const uint8_t garlic, uint32_t keylen,
		       const uint8_t key_id, uint8_t *key);


	/* Encrypts the password hash with H(key || uuid || key) where
	   key denots a KEY_LEN-byte key and uuid denots a *UNIQUE* user ID */
	void KeyedHashing(uint8_t *pwd,   const uint32_t pwdlen,
				  const uint8_t *salt,  const uint8_t  saltlen,
				  const uint8_t *data,  const uint32_t datalen,
				  const uint8_t lambda, const uint8_t  min_garlic,
				  const uint8_t garlic, const uint8_t  hashlen,
				  const uint8_t *key,   const uint64_t uuid,
				  uint8_t *chash);

private:
	/*Internal Full Interface of Catena*/
	void _Catena(uint8_t *pwd,   const uint32_t pwdlen,
	     const uint8_t *salt,  const uint8_t  saltlen,
	     const uint8_t *data,  const uint32_t datalen,
	     const uint8_t lambda, const uint8_t  min_garlic,
	     const uint8_t garlic, const size_t  hashlen,
	     const uint8_t client, const uint8_t  tweak_id, uint8_t *hash);

	
	bool					_overwrite;
	AHFUsptr 		_hash;
	AAsptr		_algorithm;
	const uint8_t*			_versionID;
};

} //end namespace
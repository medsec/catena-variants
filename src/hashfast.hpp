#pragma once

#include <memory>

#include "registerable.hpp"
#include <stdint.h>
#include "constants.hpp"

namespace Catena_Variants{

class AbstractHashFast: public virtual Registerable{
public:
	AbstractHashFast() = default;
	virtual ~AbstractHashFast() = default;

	virtual AbstractHashFast* clone() const=0;

	/* Reduced Hash function. The 2 Inputs are expected to be of size H_LEN
	 */
	virtual void Hash(int vindex, const uint8_t* i1, 
    		const uint8_t* i2, uint8_t* hash)=0;

	/* Resets the State used for HashFast if necessary
	*/
	virtual void ResetState()=0;

	virtual uint16_t getHlenFast()const =0;

};

typedef std::shared_ptr<AbstractHashFast> AHFAsptr;

/* 	This is a class derived from the abstract parent to provide some
*	functionality. Derive your implementation from this!
*/
template <typename Derived>
class HashFast : public AbstractHashFast {
public:
	HashFast() = default;
	virtual ~HashFast() = default;

	virtual AbstractHashFast* clone() const{
		return new Derived(static_cast<Derived const&>(*this));
	}

	/* Reduced Hash function. The 2 Inputs are expected to be of size H_LEN
	 */
	virtual void Hash(int vindex, const uint8_t* i1, 
    		const uint8_t* i2, uint8_t* hash)=0;

	/* Resets the State used for HashFast if necessary
	*/
	virtual void ResetState()=0;
};

} //end namespace
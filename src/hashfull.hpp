#pragma once

#include <memory>

#include "registerable.hpp"
#include <stdint.h>
#include "constants.hpp"

namespace Catena_Variants{

class AbstractHashFull : public virtual Registerable{
public:
	AbstractHashFull() = default;
	virtual ~AbstractHashFull() = default;

	virtual AbstractHashFull* clone() const=0;

	virtual void Hash1(const uint8_t *input, const uint32_t inputlen,
			    uint8_t hash[H_LEN])const =0;


	virtual void Hash2(const uint8_t *i1, const uint8_t i1len,
			    const uint8_t *i2, const uint8_t i2len,
			    uint8_t hash[H_LEN])const =0;


	virtual void Hash3(const uint8_t *i1, const uint8_t i1len,
			    const uint8_t *i2, const uint8_t i2len,
			    const uint8_t *i3, const uint8_t i3len,
			    uint8_t hash[H_LEN])const =0;


	virtual void Hash4(const uint8_t *i1, const uint8_t i1len,
			    const uint8_t *i2, const uint8_t i2len,
			    const uint8_t *i3, const uint8_t i3len,
			    const uint8_t *i4, const uint8_t i4len,
			    uint8_t hash[H_LEN])const =0;


	virtual void Hash5(const uint8_t *i1, const uint8_t i1len,
			    const uint8_t *i2, const uint8_t i2len,
			    const uint8_t *i3, const uint8_t i3len,
			    const uint8_t *i4, const uint8_t i4len,
			    const uint8_t *i5, const uint8_t i5len,
			    uint8_t hash[H_LEN])const =0;

};

typedef std::shared_ptr<AbstractHashFull> AHFUsptr;

/* 	This is a class derived from the abstract parent to provide some
*	functionality. Derive your implementation from this!
*/
template <typename Derived>
class HashFull: public AbstractHashFull {
public:
	HashFull() = default;
	virtual ~HashFull() = default;

	virtual AbstractHashFull* clone() const{
		return new Derived(static_cast<Derived const&>(*this));
	}
};

} //end namespace
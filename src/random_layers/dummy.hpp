#pragma once

#include "../random_layer.hpp"

namespace Catena_Variants{

class Dummy:  public RandomLayer<Dummy>
{
public:
	Dummy();

	/*Does nothing*/
	virtual void process(const uint8_t x[H_LEN], const uint8_t lambda,
					const uint8_t garlic, const uint8_t *salt, 
                  	const uint8_t saltlen, uint8_t *r);


};

} //end namespace


#pragma once

#include "../graph.hpp"

namespace Catena_Variants{

class DBG_Phi :  public Graph<DBG_Phi>
{
public:	
	DBG_Phi();

	virtual void process(const uint8_t x[H_LEN], const uint8_t lambda, 
						const uint8_t garlic, const uint8_t *salt, 
						const uint8_t saltlen, uint8_t *r, uint8_t* h);

	virtual uint64_t getMemoryRequirement(uint8_t garlic)const;

	virtual uint8_t getDefaultLambda()const;
 	virtual uint8_t getDefaultGarlic()const;
 	virtual uint8_t getDefaulMinGarlic()const;
	virtual const uint8_t* getDefaultVersionID()const;


private:
	/*  Sigma function that defines the diagonal connections of a DBG_Phi
	*	diagonal front: flip the (g-i)th bit (Inverse Buttferly Graph)
	*	diagonal back: flip the i-(g-1)th bit (Regular Butterfly Graph)
	*/
	uint64_t sigma(const uint8_t g, const uint64_t i, const uint64_t j);

	/*calculate actual index from level and element index*/
	uint64_t idx(uint64_t i, uint64_t j, uint8_t co, uint64_t c, uint64_t m);

	uint64_t
	R(uint64_t* input, const uint8_t n);

	std::string const VERSION_ID = "Butterfly";
	uint8_t const LAMBDA = 4;
	uint8_t const GARLIC = 16;
	uint8_t const MIN_GARLIC = 16;

};

}//end namespace
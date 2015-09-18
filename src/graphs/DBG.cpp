#include "DBG.hpp" 
#include "../registry.hpp"
#include "../catena-helpers.hpp"

using namespace Catena_Variants;

//Register the registrable with the Registry so it can be used by CatenaFactory
Registry<DBG> regDBG;

DBG::DBG()
:Registerable("Double-Butterfly-Graph with Phi Layer", "DBG", "The graph of Catena-Butterfly")
{}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void 
DBG::process(const uint8_t x[H_LEN], const uint8_t lambda, 
						const uint8_t garlic, const uint8_t *salt, 
						const uint8_t saltlen, uint8_t *r, uint8_t* h)
{
  const uint64_t c = UINT64_C(1) << garlic;
  const uint16_t H_LEN_FAST = _hashfast->getHlenFast();
  const uint64_t m = UINT64_C(1) << (garlic-1); //0.5 * 2^g
  const uint32_t l = 2 * garlic;

  uint8_t *tmp = (uint8_t*)malloc(H_LEN_FAST);
  uint64_t i,j;
  uint8_t k;
  //uint8_t co = 0; //carry over from last iteration //co is set as member

  /* DBH */
  //for (k = 0; k < lambda; k++) {
    //rows
    for(i=1; i < l; i++){
      //tmp:= v2^g-1 XOR v0
      XOR(r + idx(i-1,c-1,c,m)*H_LEN_FAST, r + idx(i-1,0,c,m)*H_LEN_FAST, tmp, H_LEN_FAST);

      //r0 := H(tmp || vsigma(g,i-1,0) )
      _hashfull->Hash2(tmp, H_LEN_FAST, r+idx(i-1,sigma(garlic,i-1,0),c,m) * H_LEN_FAST, H_LEN_FAST,
	       r+idx(i,0,c,m) *H_LEN_FAST);
      _hashfast->ResetState();

      //vertices
      for(j = 1; j < c; j++){
        //tmp:= rj-1 XOR vj
        XOR(r + idx(i,j-1,c,m)*H_LEN_FAST, r + idx(i-1,j,c,m) * H_LEN_FAST, tmp, H_LEN_FAST);
        //rj := H(tmp || vsigma(g,i-1,j))
        _hashfast->Hash(j, tmp, r + idx(i-1,sigma(garlic,i-1,j),c,m) * H_LEN_FAST,
		        r + idx(i,j,c,m) * H_LEN_FAST);
      }
    //}
    co = (co + (i-1)) % 3;
  }
  memcpy(h, r + idx(0,c-1,c,m) * H_LEN_FAST, H_LEN_FAST);
  free(tmp);
}
#pragma GCC diagnostic pop

uint64_t 
DBG::sigma(const uint8_t g, const uint64_t i, const uint64_t j)
{
	if(i < g){
		return (j ^ (UINT64_C(1) << (g-1-i))); //diagonal front
	}
	else{
		return (j ^ (UINT64_C(1) << (i-(g-1)))); //diagonal back
	}
}


uint64_t 
DBG::idx(uint64_t i, uint64_t j, uint64_t c, uint64_t m)
{
  i += co;
  if(i % 3 == 0){
    return j;
  }
  else if(i % 3 == 1){
    if(j < m){ //still fits in the array
      return j + c;
    }
    else{ //start overwriting elements at the beginning
      return j - m;
    }
  }
  else{ //i % 3 == 2
    return j + m;
  }
}

uint64_t
DBG::index(const uint64_t ind, uint8_t garlic){
  const uint64_t c = UINT64_C(1) << garlic;
  const uint64_t m = UINT64_C(1) << (garlic-1); //0.5 * 2^g
  return idx(0, ind, c, m);
}

uint64_t 
DBG::getMemoryRequirement(uint8_t garlic)const
{
  const uint16_t H_LEN_FAST = _hashfast->getHlenFast();
	return ((UINT64_C(1) << garlic) + (UINT64_C(1) << (garlic-1)))  * H_LEN_FAST;
}


uint8_t
DBG::getDefaultLambda()const{
	return LAMBDA;
}


uint8_t
DBG::getDefaultGarlic()const{
	return GARLIC;
}


uint8_t
DBG::getDefaulMinGarlic()const{
	return MIN_GARLIC;
}


const uint8_t*
DBG::getDefaultVersionID()const{
	return (const uint8_t*)VERSION_ID.c_str();
}
#pragma once

#include "catenafactory.hpp"

namespace Catena_Variants{

template <typename T>
class Registry
{
public:
	Registry(){
		auto tsptr = std::shared_ptr<T>(new T);
		//Note: some registrables can be of multiple types
		//e.g. HashFull and HashFast
		if(AAsptr asptr =  std::dynamic_pointer_cast<AbstractAlgorithm>(tsptr)){
			CatenaFactory::instance().addAlgorithm(asptr);
		}
		if(AHFUsptr hsptr = std::dynamic_pointer_cast<AbstractHashFull>(tsptr)){
			CatenaFactory::instance().addHashFull(hsptr);
		}
		if(AHFAsptr hsptr = std::dynamic_pointer_cast<AbstractHashFast>(tsptr)){
			CatenaFactory::instance().addHashFast(hsptr);
		}
		if(ARLsptr rsptr=std::dynamic_pointer_cast<AbstractRandomLayer>(tsptr)){
			CatenaFactory::instance().addRandomLayer(rsptr);
		}
		if(AGsptr gsptr = std::dynamic_pointer_cast<AbstractGraph>(tsptr)){
			CatenaFactory::instance().addGraph(gsptr);
		}
	}
};

}//end namespace
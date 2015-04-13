#pragma once

#include <string>

namespace Catena_Variants{

class Registerable {
public:
	Registerable() = default;
	Registerable(std::string n , std::string s , std::string d )
	:_name(n), _handle(s), _desc(d){}


	std::string getName() const {return _name;};
	std::string getShortHandle() const{ return _handle;};
	std::string getDescription() const{ return _desc;};

private:
	std::string _name;
	std::string _handle;
	std::string _desc;
};

} //end namespace
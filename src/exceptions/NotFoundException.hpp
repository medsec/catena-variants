#pragma once

#include <exception>
#include <string>
#include <sstream>

namespace Catena_Variants {

class NotFoundException: public std::exception {
private:
	std::string _message;
public:
    NotFoundException(const std::string& object, const std::string& name);

    virtual const char* what() const throw() {
        return _message.c_str();
    }
};

NotFoundException::NotFoundException(const std::string& object, 
	const std::string& name)
: _message() 
{
	std::stringstream stream;
	stream << object << " " << name << " could not be found";
	_message = stream.str();
}

} //end namespace
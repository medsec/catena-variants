#pragma once

#include <exception>
#include <string>
#include <sstream>

namespace Catena_Variants {

class MissingException: public std::exception {
private:
    std::string _message;
public:
    MissingException(const std::string& message);

    virtual const char* what() const throw() {
        return _message.c_str();
    }
};

MissingException::MissingException(const std::string& message)
: _message() 
{
	std::stringstream stream;
	stream << "A " << message << " is required, but missing";
	_message = stream.str();
}

} //end namespace
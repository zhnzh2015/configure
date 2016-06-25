#ifndef JCPP_CONFIGURE_CONFIG_EXCEPTION_H
#define JCPP_CONFIGURE_CONFIG_EXCEPTION_H

#include "Constants.h"

#include <string>

namespace jcpp {
namespace configure {

class ConfigException : public std::exception {
public:
    ConfigException() {};
    explicit ConfigException(const std::string& msg) : _m_msg(msg) {}
    explicit ConfigException(const char* msg) : _m_msg(msg) {}

    ConfigException& append(const char* format, ...) throw();

    virtual ~ConfigException() throw() {};

    const char* what() const throw() { return _m_msg.c_str(); }

protected:
    std::string _m_msg;
};

class NoSuchKeyException   : public ConfigException {};
class OutOfRangeException  : public ConfigException {};
class FormatErrorException : public ConfigException {};
class NullValueException   : public ConfigException {};
class NullBufferException  : public ConfigException {};
class GroupToUnitException : public NoSuchKeyException {};
class UnknownException     : public ConfigException {};

class ConfigThrower {
public:
    static void throwException(const ConfigErrorNo& err);
};

} // END namespace configure
} // END namespace jcpp

#endif  // JCPP_CONFIGURE_CONFIG_EXCEPTION_H

#include "ConfigException.h"
#include "ConfigUtils.h"

namespace jcpp {
namespace configure {

ConfigException& ConfigException::append(const char* format, ...) throw() {
    char buffer[4096];
    va_list ap; 
    va_start(ap, format);
    vsnprintf(buffer, sizeof(buffer), format, ap);
    va_end(ap);

    _m_msg += buffer;
    return *this;
}

void ConfigThrower::throwException(const ConfigErrorNo& err) {
    switch (err) {
    case CONFIG_ERROR_OK:
        break;
    
    case CONFIG_ERROR_NO_SUCH_KEY:
        throw NoSuchKeyException().append("ConfigException.NoSuchKey: %s",
                                                        ConfigUtils::get_error_info());
        break;

    case CONFIG_ERROR_OUT_OF_RANGE:
        throw OutOfRangeException().append("ConfigException.OutOfRange: %s",
                                                        ConfigUtils::get_error_info());
        break;

    case CONFIG_ERROR_FORMAT_ERROR:
        throw FormatErrorException().append("ConfigException.FormatError: %s",
                                                        ConfigUtils::get_error_info());
        break;

    case CONFIG_ERROR_NULL_VALUE:
        throw NullValueException().append("ConfigException.NullValue: %s",
                                                        ConfigUtils::get_error_info());
        break;

    case CONFIG_ERROR_NULL_BUFFER:
        throw NullBufferException().append("ConfigException.NullBuffer: %s",
                                                        ConfigUtils::get_error_info());
        break;

    case CONFIG_ERROR_GROUP_TO_UNIT:
        throw GroupToUnitException().append("ConfigException.GroupToUnit: %s",
                                                        ConfigUtils::get_error_info());
        break;

    case CONFIG_ERROR_UNKNOWN:
    default:
        throw UnknownException().append("ConfigException.Unknown");
    }
}

} // END namespace configure
} // END namespace jcpp


#include "ThorMySQL.h"
#include "PackageStream.h"
#include <stdexcept>
#include <typeinfo>

namespace ThorsAnvil
{
    namespace MySQL
    {

template<int len>
void ConectWriter::writeFixedLengthInteger(unsigned long long value)
{
    THOR_MYSQL_WRITE_INT(value, len);
}

template<typename T> inline bool writeParameterValue(ConectWriter&, T const&)
{
    throw std::domain_error(
            bugReport("ThorsAnvil::MySQL::writeParameterValue: ",
                      "Undefined for this type: ", typeid(T).name()
          ));
}
template<typename T> inline bool writeParameterType(ConectWriter&, T const&)
{
    throw std::domain_error(
            bugReport("ThorsAnvil::MySQL::writeParameterType: "
                      "Undefined for this type ", typeid(T).name()
          ));
}

// Specializations
template<> inline bool writeParameterValue<std::string>(ConectWriter& p, std::string const& val)     {p.writeLengthEncodedString(val);return true;}
template<> inline bool writeParameterType<std::string>(ConectWriter& p, std::string const&)          {p.writeFixedLengthInteger<1>(MYSQL_TYPE_VAR_STRING);p.writeFixedLengthInteger<1>(0);return true;}

template<> inline bool writeParameterValue<int>(ConectWriter& p, int const& val)                     {p.writeFixedLengthInteger<4>(val);return true;}
template<> inline bool writeParameterType<int>(ConectWriter& p, int const&)                          {p.writeFixedLengthInteger<1>(MYSQL_TYPE_LONG);p.writeFixedLengthInteger<1>(0);return true;}

    }
}


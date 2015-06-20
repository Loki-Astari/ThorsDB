
#include "ThorMySQL.h"

template<int len>
void ThorsAnvil::MySQL::PackageConWriter::writeFixedLengthInteger(long value)
{
    THOR_MYSQL_WRITE_INT(value, len);
}

template<typename T> inline bool writeParameterValue(ThorsAnvil::MySQL::PackageConWriter&, T const&)    {throw std::runtime_error(std::string("writeParameterValue: Undefined for this type ") + typeid(T).name());}
template<typename T> inline bool writeParameterType(ThorsAnvil::MySQL::PackageConWriter&, T const&)     {throw std::runtime_error(std::string("writeParameterType: Undefined for this type ") + typeid(T).name());}

// Specializations
template<> inline bool writeParameterValue<std::string>(ThorsAnvil::MySQL::PackageConWriter& p, std::string const& val)     {p.writeLengthEncodedString(val);return true;}
template<> inline bool writeParameterType<std::string>(ThorsAnvil::MySQL::PackageConWriter& p, std::string const&)          {p.writeFixedLengthInteger<1>(MYSQL_TYPE_VAR_STRING);p.writeFixedLengthInteger<1>(0);return true;}

template<> inline bool writeParameterValue<int>(ThorsAnvil::MySQL::PackageConWriter& p, int const& val)                     {p.writeFixedLengthInteger<4>(val);return true;}
template<> inline bool writeParameterType<int>(ThorsAnvil::MySQL::PackageConWriter& p, int const&)                          {p.writeFixedLengthInteger<1>(MYSQL_TYPE_LONG);p.writeFixedLengthInteger<1>(0);return true;}



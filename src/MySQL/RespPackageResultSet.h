
#ifndef THORSANIVL_MYSQL_DETAIL_RESP_PACKAGE_RESULT_SET_H
#define THORSANIVL_MYSQL_DETAIL_RESP_PACKAGE_RESULT_SET_H

#include "RespPackage.h"
#include "RespPackageColumnDefinition.h"
#include "TypeReadWrite.h"
#include "ThorSQL/Statement.h"
#include <vector>

namespace ThorsAnvil
{
    namespace MySQL
    {
        namespace Detail
        {



template<typename T> inline T readNullParameter()
{
    throw std::runtime_error(
            errorMsg("ThorsAnvil::MySQL::readNullParameter: ",
                     "Undefined for this type ", typeid(T).name()
          ));
}

template<> inline std::string readNullParameter<std::string>()                                 {return "";}
template<> inline int         readNullParameter<int>()                                         {return 0;}

class RespPackageResultSet: public RespPackage
{
    ConectReader&   reader;
    std::vector<RespPackageColumnDefinition> const& columns;
    std::string     nullMap;
    std::size_t     nextColumn;

    template<typename T>
    T readNextValue()
    {
        if (nextColumn >= columns.size()) {
            /* Note: This should never happen as we already validate that
                     a user callback function has the exact parameters required
                     for the data provided by the server.

                     Because this should never happen it is a domain_error
            */
            throw std::domain_error(
                    bugReport("ThorsAnvil::MySQL::RespPackageResultSet::readNextValue: ",
                              "Trying to read more parameters than exist"
                  ));
        }
        int currentColumn   = nextColumn;
        ++nextColumn;
        switch(columns[currentColumn].type)
        {
            case MYSQL_TYPE_STRING:         return readParameterValue<MYSQL_TYPE_STRING, T>(reader);
            case MYSQL_TYPE_VARCHAR:        return readParameterValue<MYSQL_TYPE_VARCHAR, T>(reader);
            case MYSQL_TYPE_VAR_STRING:     return readParameterValue<MYSQL_TYPE_VAR_STRING, T>(reader);
            case MYSQL_TYPE_ENUM:           return readParameterValue<MYSQL_TYPE_ENUM, T>(reader);
            case MYSQL_TYPE_SET:            return readParameterValue<MYSQL_TYPE_SET, T>(reader);
            case MYSQL_TYPE_LONG_BLOB:      return readParameterValue<MYSQL_TYPE_LONG_BLOB, T>(reader);
            case MYSQL_TYPE_MEDIUM_BLOB:    return readParameterValue<MYSQL_TYPE_MEDIUM_BLOB, T>(reader);
            case MYSQL_TYPE_BLOB:           return readParameterValue<MYSQL_TYPE_BLOB, T>(reader);
            case MYSQL_TYPE_TINY_BLOB:      return readParameterValue<MYSQL_TYPE_TINY_BLOB, T>(reader);
            case MYSQL_TYPE_GEOMETRY:       return readParameterValue<MYSQL_TYPE_GEOMETRY, T>(reader);
            case MYSQL_TYPE_BIT:            return readParameterValue<MYSQL_TYPE_BIT, T>(reader);
            case MYSQL_TYPE_DECIMAL:        return readParameterValue<MYSQL_TYPE_DECIMAL, T>(reader);
            case MYSQL_TYPE_NEWDECIMAL:     return readParameterValue<MYSQL_TYPE_NEWDECIMAL, T>(reader);
            case MYSQL_TYPE_LONGLONG:       return readParameterValue<MYSQL_TYPE_LONGLONG, T>(reader);
            case MYSQL_TYPE_LONG:           return readParameterValue<MYSQL_TYPE_LONG, T>(reader);
            case MYSQL_TYPE_INT24:          return readParameterValue<MYSQL_TYPE_INT24, T>(reader);
            case MYSQL_TYPE_SHORT:          return readParameterValue<MYSQL_TYPE_SHORT, T>(reader);
            case MYSQL_TYPE_YEAR:           return readParameterValue<MYSQL_TYPE_YEAR, T>(reader);
            case MYSQL_TYPE_TINY:           return readParameterValue<MYSQL_TYPE_TINY, T>(reader);
            case MYSQL_TYPE_DOUBLE:         return readParameterValue<MYSQL_TYPE_DOUBLE, T>(reader);
            case MYSQL_TYPE_FLOAT:          return readParameterValue<MYSQL_TYPE_FLOAT, T>(reader);
            case MYSQL_TYPE_DATE:           return readParameterValue<MYSQL_TYPE_DATE, T>(reader);
            case MYSQL_TYPE_DATETIME:       return readParameterValue<MYSQL_TYPE_DATETIME, T>(reader);
            case MYSQL_TYPE_TIMESTAMP:      return readParameterValue<MYSQL_TYPE_TIMESTAMP, T>(reader);
            case MYSQL_TYPE_TIME:           return readParameterValue<MYSQL_TYPE_TIME, T>(reader);
            case MYSQL_TYPE_NEWDATE:        return readParameterValue<MYSQL_TYPE_NEWDATE, T>(reader);
            case MYSQL_TYPE_TIMESTAMP2:     return readParameterValue<MYSQL_TYPE_TIMESTAMP2, T>(reader);
            case MYSQL_TYPE_DATETIME2:      return readParameterValue<MYSQL_TYPE_DATETIME2, T>(reader);
            case MYSQL_TYPE_TIME2:          return readParameterValue<MYSQL_TYPE_TIME2, T>(reader);
            default:
                throw std::domain_error(
                        bugReport("ThorsAnvil::MySQL::RespPackageResultSet::readNextValue: ",
                                  "Unimplemented Column Type"
                      ));
        }
    }

    template<typename T>
    T getValue(T const& defaultValue)
    {
        int     index   = nextColumn / 8;
        int     bit     = nextColumn % 8;
        if (nullMap[index] & (1 << bit)) {
            return defaultValue;
        }
        return readNextValue<T>();
    }
    public:
        RespPackageResultSet(int firstByte, ConectReader& reader, std::vector<RespPackageColumnDefinition> const& columns);
        virtual  std::ostream& print(std::ostream& s) const {return s;}

        void   retrieve(char& value)                        {value = getValue<char>('\0');}
        void   retrieve(signed char& value)                 {value = getValue<signed char>('\0');}
        void   retrieve(signed short& value)                {value = getValue<signed short>(0);}
        void   retrieve(signed int& value)                  {value = getValue<signed int>(0);}
        void   retrieve(signed long& value)                 {value = getValue<signed long>(0);}
        void   retrieve(signed long long& value)            {value = getValue<signed long long>(0);}
        void   retrieve(unsigned char& value)               {value = getValue<unsigned char>(0);}
        void   retrieve(unsigned short& value)              {value = getValue<unsigned short>(0);}
        void   retrieve(unsigned int& value)                {value = getValue<unsigned int>(0);}
        void   retrieve(unsigned long& value)               {value = getValue<unsigned long>(0);}
        void   retrieve(unsigned long long& value)          {value = getValue<unsigned long long>(0);}

        void   retrieve(float& value)                       {value = getValue<float>(0);}
        void   retrieve(double& value)                      {value = getValue<double>(0);}
        void   retrieve(long double& value)                 {value = getValue<long double>(0);}

        void   retrieve(std::string& value)                 {value = getValue<std::string>("");}

        void   retrieve(std::vector<char>& value)           {value = getValue<std::vector<char>>(std::vector<char>());};
        void   retrieve(SQL::UnixTimeStamp& value)          {value = getValue<SQL::UnixTimeStamp>(
                                                                        SQL::UnixTimeStamp(static_cast<std::time_t>(0))
                                                                     );
                                                            }
};
        }
    }
}

#endif


#ifndef THORS_ANVIL_MYSQL_DETAILS_PACKAGE_RESP_AUTH_MORE_DATA_H
#define THORS_ANVIL_MYSQL_DETAILS_PACKAGE_RESP_AUTH_MORE_DATA_H

#include "RespPackage.h"
#include <string>
#include <sstream>
#include <ostream>
#include <iomanip>

namespace ThorsAnvil::DB::MySQL
{

class ConectReader;

class RespPackageAuthMoreData: public RespPackage
{
    std::string pluginMoreData;

    public:
        RespPackageAuthMoreData(int firstByte, ConectReader& reader);
        virtual std::ostream& print(std::ostream& s)    const override;

        std::string const& getPluginMoreData() const {return pluginMoreData;}
};

inline std::ostream& RespPackageAuthMoreData::print(std::ostream& s) const
{
    std::stringstream pluginMoreDataDecoded;
    for (char x: pluginMoreData)
    {
        pluginMoreDataDecoded << "0x" << std::hex << static_cast<int>(x) << " ";
    }
    return s << "RespPackageAuthMoreData: pluginMoreData(" << pluginMoreDataDecoded.str() << ")";
}

}

#endif

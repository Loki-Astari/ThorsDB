#ifndef THORS_ANVIL_DB_MYSQL_BIND_BUFFER_H
#define THORS_ANVIL_DB_MYSQL_BIND_BUFFER_H

#include "ThorsDBCommon/StreamSimple.h"
#include "ConectWriter.h"
#include "RespPackageColumnDefinition.h"
#include "ThorsLogging/ThorsLogging.h"
#include <vector>

namespace ThorsAnvil::DB::MySQL
{

class BindBuffer
{
    class BindStream: public Common::StreamSimple
    {
        std::vector<char>& dst;
        public:
            BindStream(std::vector<char>& dst)
                : Common::StreamSimple(-1)
                , dst(dst)
            {}
            virtual void read(char*, std::size_t) override
            {
                ThorsLogAndThrowCritical("ThorsAnvil::DB::MySQL::BindStream", "read", "Invalid operation");
            }
            virtual void write(char const* buffer, std::size_t len)  override
            {
                dst.insert(dst.end(), buffer, buffer + len);
            }
    };
    std::vector<RespPackageColumnDefinition> const& columns;
    std::vector<char>                               typeBuffer;
    std::vector<char>                               valueBuffer;
    std::size_t                                     currentCol;
    BindStream                                      bindStream;
    ConectWriter                                    bindWriter;

    public:
        BindBuffer(std::vector<RespPackageColumnDefinition> const& col);
        void bindToMySQL(ConectWriter& writer) const;
        void reset();
        std::size_t countBoundParameters() const;

        template<typename Src>
        void bindValue(Src const& value);
};

}

#include "BindBuffer.tpp"

#endif

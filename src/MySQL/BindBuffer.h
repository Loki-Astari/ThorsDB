#ifndef THORS_ANVIL_MYSQL_BIND_BUFFER_H
#define THORS_ANVIL_MYSQL_BIND_BUFFER_H

#include "MySQLStream.h"
#include "ConectWriter.h"
#include "RespPackageColumnDefinition.h"
#include <vector>

namespace ThorsAnvil
{
    namespace MySQL
    {

class BindBuffer
{
    class BindStream: public MySQLStream
    {
        std::vector<char>& dst;
        public:
            BindStream(std::vector<char>& dst)
                : MySQLStream(-1)
                , dst(dst)
            {}
            virtual void read(char*, std::size_t) override {}
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
}

#ifndef COVERAGE_MySQL
#include "BindBuffer.tpp"
#endif

#endif

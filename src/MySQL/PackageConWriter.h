
#ifndef THORS_ANVIL_MYSQL_PACKAGE_WRITER_H
#define THORS_ANVIL_MYSQL_PACKAGE_WRITER_H

#include "PackageStream.h"

#include "MySQLConfig.h"
#if     defined(THOR_ENDIAN_SML)
#define THOR_MYSQL_WRITE_INT(from,len)      stream.write(reinterpret_cast<char const*>(&from), len)
#elif   defined(THOR_ENDIAN_BIG)
#error  Have not defined this for large endian systems.
#else
#error  Unknow Endianess
#endif

namespace ThorsAnvil
{
    namespace MySQL
    {

class PackageConWriter
{
    protected:
    PackageStream&   stream;
    long             capabilities;
    long             charset;

    public:

        PackageConWriter(PackageStream& stream)
            : stream(stream)
            , capabilities(0)
            , charset(0)
        {}
        virtual ~PackageConWriter() {}

        void initFromHandshake(long capabilities, long charset);

        template<int len>
        void        writeFixedLengthInteger(long value);
        void        writeLengthEncodedInteger(long value);
        void        writeFixedLengthString(std::string const& value, long size);    // Makes sure the string is correct size
        void        writeNullTerminatedString(std::string const& value);            // Adds NULL terminator
        void        writeVariableLengthString(std::string const& value);            // Not NULL terminated.
        void        writeLengthEncodedString(std::string const& value);

        void        flush();
        void        reset();
};


    }
}



#ifndef COVERAGE_TEST
#include "PackageConWriter.tpp"
#endif

#endif


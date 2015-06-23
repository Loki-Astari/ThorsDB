
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

class ConectWriter
{
    protected:
    PackageStream&   stream;
    unsigned long    capabilities;
    unsigned long    charset;

    public:

        ConectWriter(PackageStream& stream)
            : stream(stream)
            , capabilities(0)
            , charset(0)
        {}
        virtual ~ConectWriter() {}

        void initFromHandshake(unsigned long capabilities, unsigned long charset);

        template<int len>
        void        writeFixedLengthInteger(unsigned long value);
        void        writeLengthEncodedInteger(unsigned long value);
        void        writeFixedLengthString(std::string const& value, std::size_t size); // Makes sure the string is correct size
        void        writeNullTerminatedString(std::string const& value);                // Adds NULL terminator
        void        writeVariableLengthString(std::string const& value);                // Not NULL terminated.
        void        writeLengthEncodedString(std::string const& value);

        void        flush();
        void        reset();
};


    }
}



#ifndef COVERAGE_TEST
#include "ConectWriter.tpp"
#endif

#endif


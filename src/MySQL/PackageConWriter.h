
#ifndef THORS_ANVIL_MYSQL_PACKAGE_WRITER_H
#define THORS_ANVIL_MYSQL_PACKAGE_WRITER_H

#include "PackageStream.h"

namespace ThorsAnvil
{
    namespace MySQL
    {

class PackageConWriter
{
    protected:
    PackageStream&   stream;

    public:

        PackageConWriter(PackageStream& stream)
            : stream(stream)
        {}
        virtual ~PackageConWriter() {}

        template<int len>
        void        writeFixedLengthInteger(long value);
        void        writeLengthEncodedInteger(long value);
        void        writeFixedLengthString(std::string const& value, long size);    // Makes sure the string is correct size
        void        writeNullTerminatedString(std::string const& value);            // Adds NULL terminator
        void        writeVariableLengthString(std::string const& value);            // Not NULL terminated.
        void        writeLengthEncodedString(std::string const& value);
};


    }
}

#if 1
#define THOR_MYSQL_WRITE_INT(from,len)      stream.write(reinterpret_cast<char const*>(&from), len)

#elif   BOOST_BIG_ENDIAN
#error  Not Tested
#elif   BOOST_PDP_ENDIAN
#error  Not Tested
#else
#error  Unknow Endianess
#endif


#ifndef COVERAGE_TEST
#include "PackageConWriter.tpp"
#endif

#endif


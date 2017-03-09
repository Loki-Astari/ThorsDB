#include "PackageStream.h"
#include "RespPackage.h"
#include "ThorSQL/SQLUtil.h"

namespace ThorsAnvil
{
    namespace MySQL
    {

inline std::unique_ptr<RespPackage> ConectReader::recvMessage(OKMap const& actions /*= {}*/, bool expectingEOF /*= false*/)
{
    std::unique_ptr<RespPackage>    resp = getNextPackage(actions);
    if (resp->isError())
    {
        throw std::runtime_error(
                errorMsg("ThorsAnvil::MySQL::ConectReader::recvMessage: ", "Error Message from Server: ", resp->message()
              ));
    }

    if (!expectingEOF && resp->isEOF())
    {
        // EOF is special case.
        // If we are getting a set of results then the end of the set is marked
        // by an EOF package. This is not an error. We release it here and
        // make the resp empty. Anybody that is looking for an end of sequence
        // will need to validate that the pointer returned is not nullptr
        resp    = nullptr;
    }

    // Now we know the dynamic_cast will work and not return a nullptr.
    // release it do the dynamic cast and store it in a new unique_ptr
    // for return.
    return resp;
}

template<int len>
inline unsigned long long ConectReader::fixedLengthInteger()
{
    unsigned long long result = 0;
    THOR_MYSQL_READ_INT(result, len);

    return result;
}


    }
}

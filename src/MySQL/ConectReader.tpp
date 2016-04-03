
#include "PackageStream.h"
#include "RespPackage.h"
#include "ThorSQL/SQLUtil.h"

namespace ThorsAnvil
{
    namespace MySQL
    {

template<typename Resp>
std::unique_ptr<Resp> ConectReader::recvMessage(OKMap const& actions /*= {}*/, bool expectingEOF /*= false*/)
{
    std::unique_ptr<RespPackage>    resp = getNextPackage(actions);
    if (resp->isError()) {
        throw std::runtime_error(
                errorMsg("ThorsAnvil::MySQL::ConectReader::recvMessage: ",
                         "Error Message from Server: ", resp->message()
              ));
    }

    if (!expectingEOF && resp->isEOF()) {
        // EOF is special case.
        // If we are getting a set of results then the end of the set is marked
        // by an EOF package. This is not an error. We release it here and
        // make the resp empty. Anybody that is looking for an end of sequence
        // will need to validate that the pointer returned is not nullptr
        resp    = nullptr;
    }
    else {
        // Throws exception if cast fails.
        Resp&                   resultRef = dynamic_cast<Resp&>(*resp);
        thorUnused(resultRef);
    }

    // Now we know the dynamic_cast will work and not return a nullptr.
    // release it do the dynamic cast and store it in a new unique_ptr
    // for return.
    std::unique_ptr<Resp>   result(dynamic_cast<Resp*>(resp.release()));
    return result;
}

template<int len>
inline unsigned long ConectReader::fixedLengthInteger()
{
    unsigned long result = 0;
    THOR_MYSQL_READ_INT(result, len);

    return result;
}


    }
}


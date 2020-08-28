#include "ThorMySQL.h"
#include "RequPackageString.h"

using namespace ThorsAnvil::DB::MySQL;

RequPackageString::RequPackageString(std::string const& message)
    : RequPackage("RequPackageMessage", "String Message")
    , message(message)
{}

void RequPackageString::build(ConectWriter& writer) const
{
    writer.writeNullTerminatedString(message);
}

std::ostream& RequPackageString::print(std::ostream& s) const
{
    return s << "RequPackageString: "
             << "message(" << message << ") ";
}

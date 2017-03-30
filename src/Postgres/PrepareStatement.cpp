#include "PrepareStatement.h"


using namespace ThorsAnvil::Postgres;

PrepareStatement::PrepareStatement(Connection& connection, std::string const& statement)
    : Statement(statement)
    , connection(connection)
{
    std::cout << statement << "\n";
}

PrepareStatement::~PrepareStatement()
{
}

#ifndef THORS_ANVIL_DB_SQL_TEST_SET_GOOD_TO_TRUE
#define THORS_ANVIL_DB_SQL_TEST_SET_GOOD_TO_TRUE

#include <string>

struct SetGoodToTrue
{
    bool&   good;
    SetGoodToTrue(bool& good)
        : good(good)
    {}
    void operator()(int /*id*/, std::string const& /*name*/, short /*age*/, char /*sex*/, double /*height*/) const
    {
        good = true;
    }
};
struct CountLines
{
    int&   count;
    CountLines(int& count)
        : count(count)
    {}
    void operator()(int /*id*/, std::string const& /*name*/, short /*age*/, char /*sex*/, double /*height*/) const
    {
        ++count;
    }
};

#endif




#include "ThorSerialize/Traits.h"
#include "ThorSerialize/Serialize.tpp"

struct Kind0SimpleStringNoConstructor
{
    std::string     message;
};
struct Kind0SimpleStringWithConstructor
{
    Kind0SimpleStringWithConstructor(std::string const& message)
        : message(message)
    {}
    std::string     message;
};
struct Kind0StringAndIntNoConstructor
{
    std::string     message;
    int             value;
};
struct Kind0StringAndIntWithConstructor
{
    Kind0StringAndIntWithConstructor(std::string const& message, int value)
        : message(message)
        , value(value)
    {}
    std::string     message;
    int             value;
};

ThorsAnvil_MakeTrait(Kind0SimpleStringNoConstructor, message);
ThorsAnvil_MakeTrait(Kind0SimpleStringWithConstructor, message);
ThorsAnvil_MakeTrait(Kind0StringAndIntNoConstructor, message, value);
ThorsAnvil_MakeTrait(Kind0StringAndIntWithConstructor, message, value);


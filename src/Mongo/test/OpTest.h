#ifndef THORS_ANVIL_DB_MONGO_TEST_OP_TEST_H
#define THORS_ANVIL_DB_MONGO_TEST_OP_TEST_H

#include "ThorSerialize/Traits.h"
#include "ThorSerialize/Serialize.tpp"

struct SimpleStringNoConstructor
{
    std::string     message;
};

struct SimpleStringWithConstructor
{
    SimpleStringWithConstructor(std::string const& message)
        : message(message)
    {}
    std::string     message;
};

struct StringAndIntNoConstructor
{
    std::string     message;
    int             value;
};

struct StringAndIntWithConstructor
{
    StringAndIntWithConstructor(std::string const& message, int value)
        : message(message)
        , value(value)
    {}
    std::string     message;
    int             value;
};

ThorsAnvil_MakeTrait(SimpleStringNoConstructor, message);
ThorsAnvil_MakeTrait(SimpleStringWithConstructor, message);
ThorsAnvil_MakeTrait(StringAndIntNoConstructor, message, value);
ThorsAnvil_MakeTrait(StringAndIntWithConstructor, message, value);

#endif

#ifndef THORSANVIL_DB_MONGO_TEST_COMMON_INSERT_TEST_H
#define THORSANVIL_DB_MONGO_TEST_COMMON_INSERT_TEST_H

struct User
{
    std::string     user;
    std::string     password;
    int             age;
};

ThorsAnvil_MakeTrait(User, user, password, age);

#endif


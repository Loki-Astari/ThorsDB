[![Build Status](https://travis-ci.org/Loki-Astari/ThorsDB.svg?branch=master)](https://travis-ci.org/Loki-Astari/ThorsDB)

![ThorStream](../img/stream.jpg)

## Namespace

````C++ Namespaces
    ThorsAnvil::SQL                     Where users can find useful classes.
    ThorsAnvil::SQ:::Details            Implementation details used by the library. Nobody should be using anthing from here.
    ThorsAnvil::SQL::Lib                Classes that can be used by DB specific implementations.
                                        See The MySQL Lib included for examples.
````
## Interfaces

````C++ Namespaces
    ThorsAnvil::SQL::Lib::ConnectionProxy
    ThorsAnvil::SQL::Lib::StatementProxy
````
## Classes

````C++ Namespaces
    ThorsAnvil::SQL::Connection
    ThorsAnvil::SQL::Statement

    ThorsAnvil::SQL::Lib::ConnectionCreator
    ThorsAnvil::SQL::Lib::ConnectionCreatorRegister
    ThrosAnvil::SQL::Lib::ValidationTmpError

    ThorsAnvil::SQL::Detail::Cursor
    ThorsAnvil::SQL::Detail::FunctionTraits
````
## Building a SQL Library

The `Connection`/`Statement` types are simple PIMPLE types. Internally these types hold a DB specific object that implements the interface `ConnectionProxy`/`StatementProxy`.

The `Connection` internally has a factory that creates DB specific object that implement the `ConnectionProxy` interface. The factory uses "schema" of the URI passed as the first argument to the constructor of the `Connection`.

You can registor creators with the Connection factory like this:

````C++
    namespace ThorsDB = ThorsAnvil::SQL;
    ThorsDB::Connection::registerConnectionType("<schema>",
                                                 [](std::string const& host, int port,
                                                    std::string const& username,
                                                    std::string const& password,
                                                    std::string const& dbname,
                                                    ThorsAnvil::SQL::Lib::Options const& options)
                                                 {
                                                     std::unique_ptr<ThorsAnvil::SQL::Lib::ConnectionProxy> result;
                                                     result.reset(new MyFunkyDB(host, port,
                                                                                username,
                                                                                password,
                                                                                dbname,
                                                                                options));
                                                     return result;
                                                 });
````

This basic pattern is so common that we provide a wrapper object that does the above for you. Simple declare a variable at global scope of type `ConnectionCreatorRegister`.

````C++
    // The constructor of this object does the above.
    ThorsDB::Lib::ConnectionCreatorRegister<MyFunkyDB>     registerMyLibType;
````

## ThorsAnvil::SQL::Connection

Represents a connection to a DB.   
It is a simplt PIMPLE type that internall delagates all calls to `ConnectionProxy`.

## ThorsAnvil::SQL::Statement

Represent an SQL statement.
It is a simplt PIMPLE type that internall delagates all calls to `StatementProxy`.

## ThorsAnvil::SQL::Lib::ConnectionCreator

A `std::function<>` type that allows us to wrap creator objects for storage in the the factory. In the above example we see an example of a lambda acing as a creator.

## ThorsAnvil::SQL::Lib::ConnectionCreatorRegister

A simple utility class that simplify the task of defining an object to represent a `ConnectionCreator`.   
See the above example.

## ThorsAnvil::SQL::Lib::ValidationTmpError

This is used by the MySQL library (Others may also use it) to stop further processing of the current lines in a `SELECT` statement.

### Usage:
If during a call to `StatementProxy::retrieve()` there is an situation that means the library wants to drop the row; then throw a `ValidationTmpError` and it will be caught (and dropped) by the SQL library. Processing will continue on the next row.

## ThorsAnvil::SQL::Detail::Cursor

This is a simple Iterator object. It is used by the SQL library to iterate over the lines returned by a `SELECT` statement and call the lambda with the values returned. Internally it uses `StatementProxy` to do the work. This object calls the `StatementProxy` methods `more()` and `retrieve()`. See StatementProxy for details.

## ThorsAnvil::SQL::Detail::FunctionTraits

This is template meta class that use SFINAE to correctly call the lambda provided for the `SELECT` statement.

## ThorsAnvil::SQL::Lib::ConnectionProxy

This is the a simple interface implement by an DB library implementation. It is basically used to create instances of `ThorsAnvil::SQL::Lib::StatementProxy` and do DB specific connection rituals.  
See the file src/SQL/Connection.h for details.

## ThorsAnvil::SQL::Lib::StatementProxy

This is the main interface implement by an DB library implementation.  
See the file src/SQL/Statement.h for details.


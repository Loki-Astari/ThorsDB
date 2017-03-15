![Build Status](https://travis-ci.org/Loki-Astari/ThorsSQL.svg?branch=master)](https://travis-ci.org/Loki-Astari/ThorsSQL)

![ThorStream](../img/stream.jpg)

## Classes

    ThorsAnvil::SQL::Connection
    ThorsAnvil::SQL::Statement
    ThorsAnvil::SQL::Options
    ThorsAnvil::SQL::BindArgs
    ThorsAnvil::SQL::UnixTimeStamp

### ThorsAnvil::SQL::Connection

Represents a connection to a DB. 
It is used to initialize `Statement Objects`.

It is a simple PIMPLE object were the actual functionality is provided by a ConnectionProxy object.

The Connection class also acts as a factory class for creating ConnectionProxy objects. Each library that defines an implementation should also register a ConnectionCreator object with the Connector so that it can automatically create the ConnectionProxy from the URI.

The class ConnectionCreatorRegister is useful as a simple way to register ConnectionCreator objects. Each library should declare a single object of this type in the global scope. Its instantiation will automatically register the creator when the shared library is loaded.

````C++ Example:

    Connextion conect("mysql://<host>[:<port]", "<username>", "<password>", "<DB Name>");
````



[![Build Status](https://travis-ci.org/Loki-Astari/ThorsDB.svg?branch=master)](https://travis-ci.org/Loki-Astari/ThorsDB)
![ThorStream](img/stream.jpg)

## Usage

To add MySQL functionality you need this object:

    ThorsAnvil::Access::Lib::ConnectionCreatorRegister<DefaultMySQLConnection>    mysqlTestConnection("mysql");

This is normally automatically added when you link with -lThorMySQL

## Normal Usage

To use this library see the documentation of (ThorsDB)[https://github.com/Loki-Astari/ThorsDB/blob/feature-AddSomeDocumentaionToSQL/README.md]


## Internal Documentation

The MySQL documentation goes into a lot of detail about the on wire protocol used by the MySQL server. This library provides a wrapper around this on-wire protocol talking directly to the MySQL server (i.e. it does not wrap the MySQL C library).

This protocol is several layers.

### Layer 1: Simple Stream

The lowest layer is the stream. This is simply an open bidirectional unix socket to the server. This is implemented by the `MySQLStream` class which implements the Interface `Common::StreamInterface`. By providing this layer of abstraction I can mock out the stream object for testing; thus allowing the testing framework to replay with a specific stream of bytes without actually needing to use a socket.

````C++
    Class:
    ==========
    MySQLStream:    Common::StreamInterface

    Interface:
    ==========
    Common::StreamInterface
````

### Layer 2: Package Stream

MySQL uses a package based protocol. Each request/response message is wrapped in a set of packages. These packages are numbered and sized to allow validation and some basic error checking of communication between the client and server. The class `PackageBuffer` implements the `Common::StreamInterface` interface; but it is used to wrap a Layer 1 stream thus hiding the packages from the next layer and still exposing a simple stream like interface.

````C++
    Class:
    ==========
    PackageBuffer:    Common::StreamInterface

    Interface:
    ==========
    Common::StreamInterface
````

It should be noted that the implementation of this layer is currently very basic. One of the improvement points is implementing a more efficient version of this package stream.

### Layer 3: Object Layer

This layer knows how to encode/de-code specific objects onto a stream. At this layer we have two classes `ConnectWriter`/`ConectReader`. These classes have methods to encode/de-code each specific type understood by the MySQL server onto a `Common::StreamInterface`. Objects of these type are constructed using a `Common::StreamInterface` and write/read data from this stream.

````C++
    Class:
    ==========
    ConectReader
    ConectWriter
````

### Layer 4: Request/Response Layer

The MySQL communications is a sequence of requests from the client written to a `ConectWriter` which results in a response from the MySQL server that can be read from a `ConectReader`. Each request has a specific one byte request code followed by the request specific body. Each response has a specific one byte response code followed by a response body. **But** some response objects use the same response code and the actual object returned will depend on the proceeding request.

The actual orchestration of the request/response cycle is performed by the `Connection` class. The `Connection` class is constructed with a `ConectReader` and `ConectWriter` objects.

The main interface to the `Connection` class is the `sendMessageGetResponse()` method which sends a specific request object and retrieves the corresponding reply object.
    
Note: There is an exception to the Request/Response cycle. When a stream is first opened to a MySQL server; the server immidiately writes a `RespPackageHandShake` onto the stream. The `Connection` object reads this response object to understand how to authenticate with the server and make sure the server supports the required features.

````C++
    Class:
    ==========
    Connection
````

### Layer 5: Request Response Objects

MySQL communicates by sending specific messages between the client and server.
These messages are represented by C++ objects that can be constructed.

Messages from client to server:

````C++
    RequPackage
        RequPackageHandShakeResp        Response to the initial handshake message sent by the server.
        RequPackagePrepare              Prepare an SQL statement.
        RequPackagePrepareClose         Close (destroy) an SQL prepared statement.
        RequPackagePrepareExecute       Execute an SQL prepared statement (including binding parameters)
        RequPackagePrepareReset         Reset an SQL prepared statement (so it can be re-used)
````

Messages from server to client:

````C++
    RespPackage
        RespPackageOK                   A generic OK.   Sent to indicate the end of response was good.
        RespPackageEOF                  A generic OK.   Sent to indicate the end of a stream was good.
                                                        This has been deprecated in favor of using OK.
        RespPackageERR                  A generic Error There was an error. No more data will be coming.

        RespPackageHandShake            Sent by server when a connection is first initiated.
        RespPackageAuthSwitchRequest    Sent during hand shake if we need to switch authentication protocol

        RespPackagePrepare              Sent as a response to RequPackagePrepare
        RespPackagePrepareExecute       Sent as a response to RequPackagePrepareExecute
        RespPackageResultSet            Sent as a part of RespPackagePrepareExecute
            RespPackageColumnDefinition Helper for a result set.
````

To send a message use:

        // If no response from the server is expected.
        connection.sendMessageGetResponse(<RequObject>);

        // If a response object from the server is required:
        // Then you need to provide a map of expected package ID to function that know how to build
        // RespPackages of that type (this is because package ID are Request specific and overlap.
        std::uniqe_ptr<RespType>  result = 
            connection.sendMessageGetResponse(
                <RequObject>,
                {{ 0x0A, [](int firstByte, ConectReader& reader){return new <RespObjctTypeA>(firstByte, reader();},
                 { 0x0B, [](int firstByte, ConectReader& reader){return new <RespObjctTypeB>(firstByte, reader();}
                 // OK/EOF/Error are detected automatically.
                 // Use -1 as key to match anything not explicitly mentioned.
                 // If no object is created an exception is thrown
                }
            );
### Layer 6: MySQL command layer

The ThorsMySQL library provides an implementation of the interfaces needed by ThorsDB library.

The class `DefaultMySQLConnection` implements the interface `ThorsAnvil::Access::Lib::ConnectionProxy`. Its main job is creating object of type `PrepareStatement` that implement the interface `ThorsAnvil::Access::Lib::StatementProxy`.

### Utility classes

````C++
    Classes
    ==============
    BindBuffer
    ThorCryptWrapper
    TypeReadWrite
    MySQLTimeBag
````







## Usage

To add MySQL functionality you need this object:

    ThorsAnvil::SQL::ConnectionCreatorRegister<DefaultMySQLConnection>    mysqlTestConnection("mysql");

This is normally automatically added when you link with -lThorMySQL

    SQL::Connection         connection(<stuff>);
    SQL::Statement          statement("SQL");



## Internal Documentation
### Types

    MySQL::Connection:          An object that represents a connection to the DB.
                                If this was created correctly then the handshake and authentication has completed.

                                This object is usually created via SQL::Connection. Which internally uses the
                                PIMPL idiom to hold a DB specific connection object.

                                SQL::Connection     connection("mysql://<url DB server>", <userName>, <password>, <DB on server>);

MySQL communicates by sending specific messages between the client and server.
These messages are represented by C++ objects that can be constructed.

Messages from client to server:

    RequPackage
        RequPackageHandShakeResp        Response to the initial handshake message sent by the server.
        RequPackagePrepare              Prepare an SQL statement.
        RequPackagePrepareClose         Close (destroy) an SQL prepared statement.
        RequPackagePrepareExecute       Execute an SQL prepared statement (including binding parameters)
        RequPackagePrepareReset         Reset an SQL prepared statement (so it can be re-used)

Messages from server to client:

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


To send a message use:

        // If no response from the server is expected.
        connection.sendMessageGetResponse(<RequObject>);

        // If a response object from the server is required:
        // Then you need to provide a map of expected package ID to function that know how to build
        // RespPackages of that type (this is becuase package ID are Request specific and overlap.
        std::uniqe_ptr<RespType>  connection.sendMessageGetResponse(<RequObject>, {{ 0xA1, std::function<RespPackage*(int byte, ConectReader&)> },
                                                                        { 0xA2, std::function<RespPackage*(int byte, ConectReader&)> }
                                                                       }
                                                                       // OK/EOF/Error are detected automatically.
                                                                       // -1 for anything that does not match.
                                                                       // If no object is created an exception is thrown
                                                        );

    MySQL::ConectReader         Used to read bytes across the connection from the DB.
    MySQL::ConectWriter         Used to send bytes across the connection to the DB.

    MySQL::PackageStream        An abstract streaming class for reading and writing objects.
                                The ConectReader/ConectWriter actualy use this base class for reading/writing

                                When connected to an actual server they use PackageBuffer but for
                                testing you can simply derive from PackageStream to get a much simpler
                                object.

    MySQL::PackageBuffer        Derived from MySQL::PackageStream.
                                Buffers the reads/writes by ConectReader/ConectWriter into a MySQL specific
                                on the wire package structure.





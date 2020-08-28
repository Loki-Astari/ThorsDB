[![Build Status](https://travis-ci.org/Loki-Astari/ThorsDB.svg?branch=master)](https://travis-ci.org/Loki-Astari/ThorsDB)

![ThorStream](../img/stream.jpg)

## Classes

    ThorsAnvil::SQL::Connection
    ThorsAnvil::SQL::Statement
    ThorsAnvil::SQL::Options
    ThorsAnvil::SQL::BindArgs<Args...>
    ThorsAnvil::SQL::UnixTimeStamp

## Functions

    ThorsAnvil::SQL::Bind(<Args>...)

## Exceptions

````
    std::runtime_error              Normal exceptions.
                                    Your application should treat them like you do other exceptions.

    std::domain_error               Errors that should not happen if DB Server and the library code are correct.
                                    If these happen then it is likely an error in the library.

                                    NEVER catch. Report a bug.

    std::logic_error                Error in library usage.
                                    Not all errors can be caught at compile time.
                                    These errors indicate the library is not being used correctly.

                                    NEVER catch. Use unit tests to find these bugs and fix.
````

### ThorsAnvil::SQL::Options

This is used to pass DB speciifc arguments to the DB.  
Check the DB specific documentation about options and meaning.

Example: [Thors MySQL Binding Docs](../src/MySQL/README.md)

### ThorsAnvil::SQL::BindArgs

This type is used to bind C++ types to the SQL bind points (The ? in a query string). Normally this type is not used directly but is created via a call to the `Bind()` function.

See below for example usage.

### ThorsAnvil::SQL::Connection

Represents a connection to a DB.  
It is used to initialize `Statement Objects`.

````C++ Example:
    // ThorsAnvil::SQL::Connection conect("<Db Schema>://<host>[:<port>]", "<username>", "<password>", "<DB Name>"[, <Option Map>]);

    namespace ThorSQL = ThorsAnvil::SQL;
    ThorSQL::Connection conect("mysql://asguard.com", "loki", "pantsonfire", "asguard");
````

* The first parameter is a URI that identifies the host running the DB and the type of instance.
* The other parameters are self explanatory.
* The optional parameter is sued to pass DB specific parameters

### ThorsAnvil::SQL::Statement

Represent an SQL statement.  
It has one "primary" method: `execute()` (in two veriations) which executes the SQL statement.

#### `execute()` Modification Statements:
If your SQL statement is INSERT/UPDATE or DELETE.  
Then you should use `execute()` or `execute(<BindArguments>)`. Note: Using a version of `execute()` that takes a lambda will generate a std::logic_error exception.

After a statement has been executes you can use the methods `rowsAffected()` and `lastInsertID()` to get basic information about the query.

````C++ Exmple
    // ThorsDB::Statement stat(<DB Connect>, <Query String>);
    // stat.execute([ThorsDB::Bind(Arg [,Arg]*),]?);

    ThorSQL::Statement  insert(conect, "INSERT INTO People(Name, Age, Sex, Description) VALUES(?, ?, ?, ?)");
    ThorSQL::Statement  update(conect, "UPDATE People SET Name = ? WHERE Id = ?");
    ThorSQL::Statement  delete(conect, "DELETE FROM People WHERE Id = ?");

    insert.execute(ThorsDB::Bind("Loki Was Bad", 456, 'M', "God of Bad Stuff"));
    long lokiId = insert.lastInsertID();

    update.exectue(ThorsDB::Bind("Loki", lokiId));
    delete.execute(ThorsDB::Bind(lokiId));
````

#### `execute()` Query Statements:
If your statement is SELECT.
Then you should use `execute(<lambda>)` or `execute(<BindArguments>, <lambda>)`. Note: Using a version of `execute()` without a lambda will generate a std::logic_error exception.

When the select is execute the lambda function will be called for each row retrieved from the DB.

````C++ Example
    // ThorsDB::Statement stat(<DB Connect>, <Query String>);
    // stat.execute([ThorsDB::Bind(Arg [,Arg]*),]? <Functor>);

    ThorsDB::Statement getPeople(conect, "SELECT Name, Age, Sex, Description FROM People");

    getPeople.execute([](std::string name const& name, int age, char sex, std::string const& description) {
        std::cout << "Person: " << name << " (" << age << ") (" << sex << ")  : " << description << "\n";
    });
````

#### Lambda Arguments:
For each column selected in the SQL query string, the lambda must have a matching parameter of the correct type. The order of the lambda parameters is the same as the order of the selected columns. Note 1: If the number of select parameters does not match the number of parameters in the lambda or the type of the column does not match the types of parameters then std::logic_error will be generated. Note 2: `SELECT *` in the query string retrieves all table columns, thus you must have a lambda expression with one parameter for every column in the table.

You need to check each specific DB implementation to see what column types bind to what C++ types.

Example: [Thors MySQL Binding Docs](../src/MySQL/README.md)

But the general guidance is that:

* DB Integer types bind to C++ int types `char/short/int/long/long long` (plus there unsigned versions)
* DB String types bind to C++ `std::string`
* DB Blobs types bind to C++ `std::vector<char>`
* DB Date/Time types bind to C++ `ThrosAnvil::SQL::UnixTimeStamp` (see below)

````C++ Example:
    ThorsDB::Statement getNames(conect, "SELECT Name FROM People");

    // This should be OK (assuming the column name has a type that will bind to std::string).
    getNames.execute([](std::string const& name) { /* STUFF */});

    // This will throw a std::logic_error as Name does not bind to a long.
    getNames.execute([](long id){ /* STUFF */});
````

### ThrosAnvil::SQL::UnixTimeStamp
Used as a Bind type for Date/Time fields in a DB.  
Basically to distinguish time objects from integer objects.

````C++ Example
    ThorsDB::Statement getDOB(conect, "SELECT Name, DateOfBirth from BirthRecord");

    getDOB.execute([](std::string const& name, ThrosAnvil::SQL::UnixTimeStamp const& dob) {
        std::cout << name << " : " << dob << "\n";      // Note: this is a simple wrapper around unix time stamp.
    }); 
````

This type is a simple wrapper around a `std::time_t` typedef. The DB field will be converted to an epoch time stamp (seconds since 1970/Jan/01 00:00:00). If you use the `ThrosAnvil::SQL::UnixTimeStamp` in any context that would take a `std::time_t` it will autoconvert (this includes integer context).



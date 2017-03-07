[![Build Status](https://travis-ci.org/Loki-Astari/ThorsSQL.svg?branch=master)](https://travis-ci.org/Loki-Astari/ThorsSQL)

![ThorStream](../img/stream.jpg)

##Objective:

* Write a platform/DB neutral SQL library.
* Do not wrap a C SQL library to achieve this (unless that is the only way).
* Remove the difference between normal and prepared statements.
* Remove the need for string escaping (C++ type system should cope with that).
* Queries are async by default.

This library is still pre-alpha.  


###Example Usage:

This is what I am trying to achieve.

````c++
    using ThorsSQL = ThorsAnvil::SQL;

    ThorsSQL::Connection    mysql("host", "username", "password");
    ThorsSQL::Statement     bigEarnerStat(mysql, ThorsSQL::OneTime, 
                                          "SELECT ID, Name, Salary FROM Employee WHERE Salary > %");

    // Bind variables.
    // Call function for every row returned.
    bigEarnerStat.execute(1000000, // parameter bound to % in statement.
        // Function executed for each row returned.
        // Parameters are matched against the SELECT in the statement.
        [](u64 id, std::string const& name, int salary){
            std::cout << name << " is a fat cat earning $" << salary/100 << "." << salary%100 << "\n";
        },
        // Function exectured if the server reports an error.
        [](int errorID, std::string const& message) {
            std::cout << "Error: " << errorID << "  Details: " << message << "\n";
        }
    );
````


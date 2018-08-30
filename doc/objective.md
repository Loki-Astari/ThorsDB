[![Build Status](https://travis-ci.org/Loki-Astari/ThorsSQL.svg?branch=master)](https://travis-ci.org/Loki-Astari/ThorsSQL)

![ThorStream](../img/stream.jpg)

## Objective:

* Write a platform/DB neutral SQL library.
* Do not wrap a C SQL library to achieve this (unless that is the only way).
* Remove the difference between normal and prepared statements.
* Remove the need for string escaping (C++ type system should cope with that).
* Queries are async by default. (Available via [ThorsNisse](https://github.com/Loki-Astari/ThorsNisse))

This library is Beta.


### Example Usage:

This is what I am trying to achieve.

````c++
    using ThorsSQL = ThorsAnvil::SQL;

    ThorsSQL::Connection    mysql("mysql:://host", "username", "password");
    ThorsSQL::Statement     bigEarnerStat(mysql, "SELECT ID, Name, Salary FROM Employee WHERE Salary > ?");

    // Bind variables.
    bigEarnerStat.execute(ThorsSQL::Bind(1000000),          // parameter bound to ? in statement.

        // Function executed for each row returned.
        [](u64 id, std::string const& name, int salary){
            // Note: Parameters are matched against the SELECT in the statement.
            std::cout << name << " is a fat cat earning $" << (salary / 100) << "." << (salary % 100) << "\n";
        }
    );
````


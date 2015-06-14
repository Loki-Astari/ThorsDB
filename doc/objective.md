[![endorse](http://api.coderwall.com/lokiastari/endorsecount.png)](http://coderwall.com/lokiastari)
[![Build Status](https://travis-ci.org/Loki-Astari/ThorsSQL.svg?branch=master)](https://travis-ci.org/Loki-Astari/ThorsSQL)

![ThorStream](../img/stream.jpg)

##Objective:

* Write a platform neutral SQL wrapper utilizing lambdas to make interacting with a SQL DB trivial.
* Do not wrap a C SQL library to achieve this.

This library is still pre-alpha.  


###Example Usage:

This is what I am trying to achieve.

````c++

    ThorsSQL::Connection    mysql("username", "password", "host");
    ThorsSQL::Statement     bigEarnerStat("SELECT ID, Name, Salary FROM Employee WHERE Salary > %");

    // Bind variables.
    // Call function for every row returned.
    bigEarnerStat.execute(1000000,[](u64 id, std::string const& name, int salary, ){
        std::cout << name << " is a fat cat earning $" << salary/100 << "." << salary%100 << "\n";
    });
````


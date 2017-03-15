[![Build Status](https://travis-ci.org/Loki-Astari/ThorsSQL.svg?branch=master)](https://travis-ci.org/Loki-Astari/ThorsSQL)

![ThorStream](../img/stream.jpg)


# Building Instructions:
````bash
    > git clone git@github.com:Loki-Astari/ThorsSQL.git
    > cd ThorsSQL
    > ./configure
    > make
    > sudo make install
````
## Description

By default installation will be in `/usr/local/[include/lib]`. You can override this with the normal auto-tools defaults. Use `./configure --help` to get details.

### What is installed:

* /usr/local/include/ThorSQL/*
* /usr/local/include/ThorMySQL/*
* /usr/local/lib/libThorSQL14.so
* /usr/local/lib/libThorMySQL14.so
* /usr/local/lib/libThorSQL14D.so
* /usr/local/lib/libThorMySQL14D.so


Note:
libThorMySQL14.so is build using -O3 and thus is fully optimized and debug symbols have been stripped.  
libThorMySQL14D.so is build using -g and is useful for debugging purposes.


### What is Downloaded

The configuration processes will download the generic makefiles (using git) from [ThorMaker](https://github.com/Loki-Astari/ThorMaker) which in turn will download and build google's gtest and vera++ library that is used in running the unit tests.

## Configure options

The test require a DB connection. By default the following are used:

    Host of mysql Server:       127.0.0.1
    mysql user                  root
    mysql user password         testPassword
    DB on server used           test

All these options can be modified using config arguments.

    --with-TestMySQLHost=<Host>
    --with-TestMySQLUser=<User>
    --with-TestMySQLPass=<Password>
    --with-TestMySQLDatabase=<DB>

## Requirements

This library uses features from C++14 so you will need a compiler that supports this. The generic makefile also does code coverage tests so your compiler will also need to support a code coverage tool that has an interface similar to gcov.

It has been tested on [travis-ci.org](https://travis-ci.org/Loki-Astari/ThorsSQL) using clang 3.5 and g++ 4.9 (on mac and ubuntu). Though clang 3.4 also supports C++14 its code coverage tool is very basic and the generic makefiles will fail when attempting to perform code coverage tests.



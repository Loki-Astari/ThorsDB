#ifndef THORS_ANVIL_POSTG_PREPARESTATEMENT_H
#define THORS_ANVIL_POSTG_PREPARESTATEMENT_H

#include "Statement.h"
#include "Connection.h"

namespace ThorsAnvil
{
    namespace Postgres
    {

using SQL::UnixTimeStamp;
class PrepareStatement: public Statement
{
    Connection&     connection;
    public:
        PrepareStatement(Connection& connection, std::string const& statement);
        ~PrepareStatement();
        // abort called if an exception is thrown by the lambda provided to SELECT
        // After abort returns the exception is re-thrown. So if an exception is
        // generated during abort() the application will terminate.
        virtual void   abort()                              {throw 1;}

        // bind() is called for each parameter provided by ThorsAnvil::SQL::Bind()
        virtual void   bind(char)                           {throw 2;}
        virtual void   bind(signed char)                    {throw 3;}
        virtual void   bind(signed short)                   {throw 4;}
        virtual void   bind(signed int)                     {throw 5;}
        virtual void   bind(signed long)                    {throw 6;}
        virtual void   bind(signed long long)               {throw 7;}
        virtual void   bind(unsigned char)                  {throw 8;}
        virtual void   bind(unsigned short)                 {throw 9;}
        virtual void   bind(unsigned int)                   {throw 10;}
        virtual void   bind(unsigned long)                  {throw 11;}
        virtual void   bind(unsigned long long)             {throw 12;}

        virtual void   bind(float)                          {throw 13;}
        virtual void   bind(double)                         {throw 14;}
        virtual void   bind(long double)                    {throw 15;}

        virtual void   bind(std::string const&)             {throw 16;}
        virtual void   bind(std::vector<char> const&)       {throw 17;}

        virtual void   bind(UnixTimeStamp const&)           {throw 18;}

        // -----

        SQL::Detail::Cursor execute();
        // After all the bind parameters have been bound with the above calls.
        // This method is called to initiate the communication with the DB
        virtual void doExecute()                            {throw 19;}

        // Called from the Detail::Cursor object to see if there are more rows
        // to be retrieved from the DB
        virtual bool more()                                 {throw 20;}

        // For each parameter in the lambda one of these functions is called
        // to retrieve a value returned by the DB. They are called in the order
        // of the parameters in the lambda
        virtual void   retrieve(char&)                      {throw 21;}
        virtual void   retrieve(signed char&)               {throw 22;}
        virtual void   retrieve(signed short&)              {throw 23;}
        virtual void   retrieve(signed int&)                {throw 24;}
        virtual void   retrieve(signed long&)               {throw 25;}
        virtual void   retrieve(signed long long&)          {throw 26;}
        virtual void   retrieve(unsigned char&)             {throw 27;}
        virtual void   retrieve(unsigned short&)            {throw 28;}
        virtual void   retrieve(unsigned int&)              {throw 29;}
        virtual void   retrieve(unsigned long&)             {throw 30;}
        virtual void   retrieve(unsigned long long&)        {throw 31;}

        virtual void   retrieve(float&)                     {throw 32;}
        virtual void   retrieve(double&)                    {throw 33;}
        virtual void   retrieve(long double&)               {throw 34;}

        virtual void   retrieve(std::string&)               {throw 35;}
        virtual void   retrieve(std::vector<char>&)         {throw 36;}

        virtual void   retrieve(UnixTimeStamp&)             {throw 37;}


        // Status Info
        // These functions can be called to get information about
        // the last call to execute() if this was a modification
        // statement.
        virtual bool isSelect() const                       {throw 38;}
        virtual long rowsAffected() const                   {throw 39;}
        virtual long lastInsertID() const                   {throw 40;}
};

    }
}

#endif


#ifndef MOCK_MYSQL_H
#define MOCK_MYSQL_H

#include "Connection.h"
#include "Statement.h"


using ThorsAnvil::SQL::StatementProxy;
using ThorsAnvil::SQL::StatementType;
template<typename T>
void getField(int& nextRow, int& nextField, T& value)
{
    switch(nextField)
    {
        case 0: value =     static_cast<T>(nextRow ? 15:32);        break;
        case 1: throw std::runtime_error("Invalid Conversion");
        case 2: value =     static_cast<T>(nextRow ? 32:29);        break;
        case 3: value =     static_cast<T>(nextRow ? 'M':'F');      break;
        case 4: value =     static_cast<T>(nextRow ? 34.9:33.543);  break;
    }
    ++nextField;
    if (nextField == 5) {
        nextField   = 0;
        ++nextRow;
    }
}
void getFieldString(int& nextRow, int& nextField, std::string& value)
{
    if (nextField != 1) {
        throw std::runtime_error("Invalid Conversion");
    }
    value = nextRow ? "John Smith" : "Mandy Twit";
    ++nextField;
    if (nextField == 5) {
        nextField   = 0;
        ++nextRow;
    }
}

class MockMySQLConnection: public ThorsAnvil::SQL::ConnectionProxy
{
    public:
        class MockMySQLStatement: public StatementProxy
        {
            // Returns two rows
            // ID(long)   Name(std::string)   Age(short)   Sex(char)   Height(double)  
            // 32         Mandy Twit          29           F           33.543
            // 15         John Smith          32           M           34.9
            int     nextRow;
            int     nextField;

            public:
            MockMySQLStatement()
                : nextRow(0)
                , nextField(0)
            {}
            virtual void   bind(char)                           {}
            virtual void   bind(signed char)                    {}
            virtual void   bind(signed short)                   {}
            virtual void   bind(signed int)                     {}
            virtual void   bind(signed long)                    {}
            virtual void   bind(signed long long)               {}
            virtual void   bind(unsigned char)                  {}
            virtual void   bind(unsigned short)                 {}
            virtual void   bind(unsigned int)                   {}
            virtual void   bind(unsigned long)                  {}
            virtual void   bind(unsigned long long)             {}

            virtual void   bind(float)                          {}
            virtual void   bind(double)                         {}
            virtual void   bind(long double)                    {}

            virtual void   bind(std::string const&)             {}

            // -----

            virtual void doExecute()                            {}
            virtual bool more()                                 {return nextRow < 2;}

            virtual void   retrieve(char& value)                {getField(nextRow, nextField, value);}
            virtual void   retrieve(signed char& value)         {getField(nextRow, nextField, value);}
            virtual void   retrieve(signed short& value)        {getField(nextRow, nextField, value);}
            virtual void   retrieve(signed int& value)          {getField(nextRow, nextField, value);}
            virtual void   retrieve(signed long& value)         {getField(nextRow, nextField, value);}
            virtual void   retrieve(signed long long& value)    {getField(nextRow, nextField, value);}
            virtual void   retrieve(unsigned char& value)       {getField(nextRow, nextField, value);}
            virtual void   retrieve(unsigned short& value)      {getField(nextRow, nextField, value);}
            virtual void   retrieve(unsigned int& value)        {getField(nextRow, nextField, value);}
            virtual void   retrieve(unsigned long& value)       {getField(nextRow, nextField, value);}
            virtual void   retrieve(unsigned long long& value)  {getField(nextRow, nextField, value);}

            virtual void   retrieve(float& value)               {getField(nextRow, nextField, value);}
            virtual void   retrieve(double& value)              {getField(nextRow, nextField, value);}
            virtual void   retrieve(long double& value)         {getField(nextRow, nextField, value);}

            virtual void   retrieve(std::string& value)         {getFieldString(nextRow, nextField, value);}
        };
        MockMySQLConnection(std::string const&, int, std::string const&, std::string const&, std::string const&, ThorsAnvil::SQL::Options const&)
        {
        }
        virtual std::unique_ptr<StatementProxy> createStatementProxy(std::string const& statement, StatementType type) override
        {
            return std::unique_ptr<StatementProxy>(new MockMySQLStatement);
        }
};

#endif


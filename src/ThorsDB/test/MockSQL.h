
#ifndef THORS_ANVIL_THHORDB_MOCK_MYSQL_H
#define THORS_ANVIL_THHORDB_MOCK_MYSQL_H

#include "Connection.h"
#include "Statement.h"
#include "ThorsLogging/ThorsLogging.h"
#include <string>


using ThorsAnvil::DB::Access::Lib::StatementProxy;
using ThorsAnvil::DB::Access::UnixTimeStamp;

void updateRow(int& nextRow, int& nextField)
{
    ++nextField;
    if (nextField == 5)
    {
        nextField   = 0;
        ++nextRow;
    }
}
template<typename T>
void getField(int& nextRow, int& nextField, T& value)
{
    if (nextRow > 1) {
        updateRow(nextRow, nextField);
        ThorsLogAndThrowCritical("ThorsAnvil::DB::Access:ThorsDB::Mock", "getField", "Mock throw");
    }
    switch(nextField)
    {
        case 0: value =     static_cast<T>(nextRow ? 15:32);        break;
        case 1: updateRow(nextRow, nextField); throw ThorsAnvil::DB::Access::Lib::ValidationTmpError("Invalid Conversion");
        case 2: value =     static_cast<T>(nextRow ? 32:29);        break;
        case 3: value =     static_cast<T>(nextRow ? 'M':'F');      break;
        case 4: value =     static_cast<T>(nextRow ? 34.9:33.543);  break;
    }
    updateRow(nextRow, nextField);
}
void getFieldString(int& nextRow, int& nextField, std::string& value)
{
    if (nextField != 1) {
        throw std::domain_error("Invalid Conversion");
    }
    value = nextRow ? "John Smith" : "Mandy Twit";
    ++nextField;
    if (nextField == 5) {
        nextField   = 0;
        ++nextRow;
    }
}

class MockSQLConnection: public ThorsAnvil::DB::Access::Lib::ConnectionProxy
{
    public:
        class MockSQLStatement: public StatementProxy
        {
            // Returns two rows
            // ID(long)   Name(std::string)   Age(short)   Sex(char)   Height(double)  
            // 32         Mandy Twit          29           F           33.543
            // 15         John Smith          32           M           34.9
            int     nextRow;
            int     nextField;
            bool    isSelectStatement;
            int     maxRows;

            public:
            MockSQLStatement(bool isSelectStatement, int maxRows)
                : nextRow(0)
                , nextField(0)
                , isSelectStatement(isSelectStatement)
                , maxRows(maxRows)
            {}
            virtual void   abort()                              override {}

            virtual void   bind(char)                           override {}
            virtual void   bind(signed char)                    override {}
            virtual void   bind(signed short)                   override {}
            virtual void   bind(signed int)                     override {}
            virtual void   bind(signed long)                    override {}
            virtual void   bind(signed long long)               override {}
            virtual void   bind(unsigned char)                  override {}
            virtual void   bind(unsigned short)                 override {}
            virtual void   bind(unsigned int)                   override {}
            virtual void   bind(unsigned long)                  override {}
            virtual void   bind(unsigned long long)             override {}

            virtual void   bind(float)                          override {}
            virtual void   bind(double)                         override {}
            virtual void   bind(long double)                    override {}

            virtual void   bind(std::string const&)             override {}

            virtual void   bind(std::vector<char> const&)       override {}
            virtual void   bind(UnixTimeStamp const&)           override {}

            // -----

            virtual void doExecute()                            override {}
            virtual bool more()                                 override {return nextRow < maxRows;}
            virtual bool isSelect() const                       override {return isSelectStatement;}
            virtual long rowsAffected() const                   override {return 0;}
            virtual long lastInsertID() const                   override {return 0;}

            virtual void   retrieve(char& value)                override {getField(nextRow, nextField, value);}
            virtual void   retrieve(signed char& value)         override {getField(nextRow, nextField, value);}
            virtual void   retrieve(signed short& value)        override {getField(nextRow, nextField, value);}
            virtual void   retrieve(signed int& value)          override {getField(nextRow, nextField, value);}
            virtual void   retrieve(signed long& value)         override {getField(nextRow, nextField, value);}
            virtual void   retrieve(signed long long& value)    override {getField(nextRow, nextField, value);}
            virtual void   retrieve(unsigned char& value)       override {getField(nextRow, nextField, value);}
            virtual void   retrieve(unsigned short& value)      override {getField(nextRow, nextField, value);}
            virtual void   retrieve(unsigned int& value)        override {getField(nextRow, nextField, value);}
            virtual void   retrieve(unsigned long& value)       override {getField(nextRow, nextField, value);}
            virtual void   retrieve(unsigned long long& value)  override {getField(nextRow, nextField, value);}

            virtual void   retrieve(float& value)               override {getField(nextRow, nextField, value);}
            virtual void   retrieve(double& value)              override {getField(nextRow, nextField, value);}
            virtual void   retrieve(long double& value)         override {getField(nextRow, nextField, value);}

            virtual void   retrieve(std::string& value)         override {getFieldString(nextRow, nextField, value);}

            virtual void   retrieve(std::vector<char>& value)   override {value=std::vector<char>();}

            virtual void   retrieve(UnixTimeStamp& value)       override {value=UnixTimeStamp(static_cast<std::time_t>(0));}
        };
        MockSQLConnection(std::string const&, int, std::string const&, std::string const&, std::string const&, ThorsAnvil::DB::Access::Options const&)
        {
        }
        virtual std::unique_ptr<StatementProxy> createStatementProxy(std::string const& statement) override
        {
            bool isSelect = statement.find("SELECT") != std::string::npos;
            std::string maxRowsString = isSelect ? statement.substr(6) : std::string("");
            int  maxRows  = maxRowsString.empty() ? 2 : std::stoi(maxRowsString);
            return std::unique_ptr<StatementProxy>(new MockSQLStatement(isSelect, maxRows));
        }
        virtual int    getSocketId() const                  override {return -1;}
        virtual void   setYield(std::function<void()>&&, std::function<void()>&&) override {}
};

#endif


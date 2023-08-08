#ifndef THORSANVIL_DB_POSTGRES_MESSAGES_H
#define THORSANVIL_DB_POSTGRES_MESSAGES_H

#include "PostgresConfig.h"
//#include "ThorsDB/MD5.h"
#include "ThorsCrypto/hash.h"
#include "ThorsIOUtil/Utility.h"
#include <iostream>

namespace ThorsAnvil::DB::Postgres
{

// --------
class Message
{
    public:
        virtual ~Message() {}
        virtual void send(ConectWriter& writer) = 0;
        virtual void print(std::ostream& stream) const = 0;
        virtual bool isError() const {return false;}
        friend std::ostream& operator<<(std::ostream& stream, Message const& message)
        {
            message.print(stream);
            return stream;
        }
};
// -------
class StartupMessage: public Message
{
    std::string username;
    std::string database;
    Options     options;
    public:
        StartupMessage(std::string const& username, std::string const& database, Options const& options)
            : username(username)
            , database(database)
            , options(options)
        {}
        virtual void send(ConectWriter& writer) override
        {
            writer.writeInt32(0x30000);

            writer.writeString("user");
            writer.writeString(username);

            writer.writeString("database");
            writer.writeString(database);

            for (auto const& val: options)
            {
                writer.writeString(val.first);
                writer.writeString(val.second);
            }

            writer.writeChar('\0');
            writer.sendMessage('\0');
        }
        virtual void print(std::ostream& stream) const override
        {
            stream << "StartupMessage: " << username << " : " << database << "\n";
        }
};
class PasswordMessage: public Message
{
    std::string password;

    public:
        PasswordMessage(std::string const& password)
            : password(password)
        {}
        virtual void send(ConectWriter& writer) override
        {
            writer.writeString(password);
            writer.sendMessage('p');
        }
        virtual void print(std::ostream& stream) const override
        {
            stream << "PasswordMessage: " << password << "\n";
        }
};
// -------
class ErrorMessage: public Message
{
    std::string type;
    std::string message;

    public:
        ErrorMessage(ConectReader& reader)
        {
            for (char code = reader.readByte(); code != '\0'; code = reader.readByte())
            {
                switch (code)
                {
                    case 'S':   // Always present.
                                // Severity: in error-message: ERROR, FATAL, or PANIC
                                //           in notice:        WARNING, NOTICE, DEBUG, INFO, or LOG
                                // or a localized translation of one of these.
                        type = "Severity:          ";
                        break;
                    case 'C':   // Always present.
                                // Code: the SQLSTATE code for the error (see Appendix A). Not localizable.
                        type = "SQLSTATE:          ";
                        break;
                    case 'M':   // Always present.
                                // Message: the primary human-readable error message. This should be accurate but terse (typically one line).
                        type = "Message:           ";
                        break;
                    case 'D':   // Detail: an optional secondary error message carrying more detail about the problem.
                                // Might run to multiple lines.
                        type = "Detail:            ";
                        break;
                    case 'H':   // Hint: an optional suggestion what to do about the problem.
                                // This is intended to differ from Detail in that it offers advice (potentially inappropriate) rather than hard facts.
                                // Might run to multiple lines.
                        type = "Hint:              ";
                        break;
                    case 'P':   // Position: the field value is a decimal ASCII integer,
                                // indicating an error cursor position as an index into the original query string.
                                // The first character has index 1, and positions are measured in characters not bytes.
                        type = "Position:          ";
                        break;
                    case 'p':   // Internal position: this is defined the same as the P field, but it is used when the cursor position refers
                                // to an internally generated command rather than the one submitted by the client.
                                // The q field will always appear when this field appears.
                        type = "Internal Position: ";
                        break;
                    case 'q':   // Internal query: the text of a failed internally-generated command.
                                // This could be, for example, a SQL query issued by a PL/pgSQL function.
                        type = "Internal Query:    ";
                        break;
                    case 'W':   // Where: an indication of the context in which the error occurred.
                                // Presently this includes a call stack traceback of active procedural language functions and internally-generated queries.
                                // The trace is one entry per line, most recent first.
                        type = "Where:             ";
                        break;
                    case 's':   // Schema name: if the error was associated with a specific database object, the name of the schema containing that object, if any.
                        type = "Schema Name:       ";
                        break;
                    case 't':   // Table name: if the error was associated with a specific table, the name of the table.
                                // (Refer to the schema name field for the name of the table's schema.)
                        type = "Table Name:        ";
                        break;
                    case 'c':   // Column name: if the error was associated with a specific table column, the name of the column.
                                // (Refer to the schema and table name fields to identify the table.)
                        type = "Column Name:       ";
                        break;
                    case 'd':   // Data type name: if the error was associated with a specific data type, the name of the data type.
                                // (Refer to the schema name field for the name of the data type's schema.)
                        type = "Data Type Name:    ";
                        break;
                    case 'n':   // Constraint name: if the error was associated with a specific constraint, the name of the constraint.
                                // Refer to fields listed above for the associated table or domain. (For this purpose,
                                // indexes are treated as constraints, even if they weren't created with constraint syntax.)
                        type = "Constraint Name:   ";
                        break;
                    case 'F':   // File: the file name of the source-code location where the error was reported.
                        type = "File:              ";
                        break;
                    case 'L':   // Line: the line number of the source-code location where the error was reported.
                        type = "Line:              ";
                        break;
                    case 'R':   // Routine: the name of the source-code routine reporting the error.
                        type = "Routine:           ";
                        break;
                    default:
                        type = "Unnknown:      ";
                        type += code;
                        type += " : ";
                }
                message = reader.readString();
            }
        }
        virtual void send(ConectWriter&) override {}
        virtual bool isError() const override
        {
            return true;
        }
        virtual void print(std::ostream& stream) const override
        {
            stream << "ErrorMessage: " << type << message;
        }
};
class Authentication: public Message
{
    public:
        virtual void send(ConectWriter&) override {}
        virtual std::unique_ptr<Message> authenticateWithServer(std::string const& /*username*/, std::string const& /*password*/)
        {
            throw std::runtime_error(ThorsAnvil::Utility::buildErrorMessage("ThorsAnvil::DB::Postgres::Authentication", "authenticateWithServer", *this, ": technique is not implemented"));
        }
};
class AuthenticationOk: public Authentication
{
    public:
        AuthenticationOk(ConectReader& /*reader*/)  {}
        virtual std::unique_ptr<Message> authenticateWithServer(std::string const&, std::string const&) override
        {
            return std::unique_ptr<Message>();
        }
        virtual void print(std::ostream& stream) const override
        {
            stream << "Authentication Ok:\n";
        }
};
class AuthenticationKerberosV5: public Authentication
{
    public:
        AuthenticationKerberosV5(ConectReader& /*reader*/)  {}
        virtual void print(std::ostream& stream) const override
        {
            stream << "Authentication Kerberos V5:\n";
        }
};
class AuthenticationCleartextPassword: public Authentication
{
    public:
        AuthenticationCleartextPassword(ConectReader& /*reader*/)  {}
        virtual void print(std::ostream& stream) const override
        {
            stream << "Authentication Cleartext Password:\n";
        }
};
class AuthenticationMD5Password: public Authentication
{
    std::vector<char> salt;
    public:
        AuthenticationMD5Password(ConectReader& reader)
            : salt(reader.readByte(4))
        {}
        virtual std::unique_ptr<Message> authenticateWithServer(std::string const& username, std::string const& password) override
        {
            using ThorsAnvil::Crypto::hexdigest;
            using ThorsAnvil::Crypto::Md5;
            // concat('md5', md5(concat(md5(concat(password, username)), random-salt)))
            std::string message = password + username;
            message = hexdigest<Md5>(message); // Util::md5(message);
            message += std::string(salt.begin(), salt.end());
            message = "md5" + hexdigest<Md5>(message); // Util::md5(message);
            return std::make_unique<PasswordMessage>(message);
        }
        virtual void print(std::ostream& stream) const override
        {
            stream << "Authentication MD5 Password:\n";
        }
};
class AuthenticationSCMCredential: public Authentication
{
    public:
        AuthenticationSCMCredential(ConectReader& /*reader*/)  {}
        virtual void print(std::ostream& stream) const override
        {
            stream << "Authentication SCM Credential:\n";
        }
};
class AuthenticationGSS: public Authentication
{
    public:
        AuthenticationGSS(ConectReader& /*reader*/)  {}
        virtual void print(std::ostream& stream) const override
        {
            stream << "Authentication GSS:\n";
        }
};
class AuthenticationGSSContinue: public Authentication
{
    std::string authData;
    public:
        AuthenticationGSSContinue(ConectReader& reader)
            : authData(reader.readStringToEnd())
        {}
        virtual void print(std::ostream& stream) const override
        {
            stream << "Authentication GSS Continue:\n";
        }
};
class AuthenticationSSPI: public Authentication
{
    public:
        AuthenticationSSPI(ConectReader& /*reader*/)  {}
        virtual void print(std::ostream& stream) const override
        {
            stream << "Authentication SSPI:\n";
        }
};
class ParseComplete: public Message
{
    public:
        ParseComplete(ConectReader& /*reader*/)  {}
        virtual void send(ConectWriter&) override {}
        virtual void print(std::ostream& stream) const override
        {
            stream << "ParseComplete:\n";
        }
};
// -------

}

#endif

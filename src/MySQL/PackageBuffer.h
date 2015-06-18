
#ifndef THORS_ANVIL_MYSQL_PACKAGE_BUFFER_H
#define THORS_ANVIL_MYSQL_PACKAGE_BUFFER_H

namespace ThorsAnvil
{
    namespace MySQL
    {

class PackageBufferMySQLDebugBuffer
{
    int socket;
    public:
        void setSocket(int sock)
        {
            socket  = sock;
        }
};

    }
}

#endif


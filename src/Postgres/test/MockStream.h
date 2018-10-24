
#include <sstream>
#include <stdexcept>
#include <iostream>

class MockStream: public ThorsAnvil::SQL::StreamInterface
{
    char const*   input;
    unsigned char*output;
    std::size_t   len;
    std::size_t   readSoFar;
    std::size_t   writSoFar;
    public:
        MockStream(char const* data, std::size_t len, unsigned char* output = nullptr)
            : input(data)
            , output(output)
            , len(len)
            , readSoFar(0)
            , writSoFar(0)
        {
        }
        virtual void        read(char* buffer, std::size_t size)        override {
                                                                                  if (readSoFar + size > len) {
                                                                                    std::stringstream msg;
                                                                                    msg << "Read too much: readSoFar(" << readSoFar << ") Size(" << size << ") Len(" << len << ")";
                                                                                    throw std::runtime_error(msg.str());
                                                                                  }
                                                                                  std::copy(input + readSoFar, input + readSoFar + size, buffer);readSoFar += size;
                                                                                 }
        virtual void        write(char const* buffer, std::size_t len)  override {std::copy(buffer, buffer + len, output + writSoFar); writSoFar += len;}
        virtual void        startNewConversation(bool)                  override {}
        virtual void        flush()                                     override {}
        virtual void        drop()                                      override {readSoFar = len;}
        virtual void        reset()                                     override {}
        virtual bool        isEmpty()                                   override {return len == readSoFar;}
        virtual std::string readRemainingData()                         override {return std::string(input + readSoFar, input + readSoFar + len);}
        virtual void        establishSSLConnection()                    override {}

        std::size_t         readLen() const {return readSoFar;}
        std::size_t         writLen() const {return writSoFar;}
};



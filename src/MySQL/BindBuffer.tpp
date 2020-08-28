namespace ThorsAnvil::DB::MySQL
{

template<typename Src>
void BindBuffer::bindValue(Src const& value)
{
    //  This is called once for each bound parameter.
    //  It build's up two chunks of data that are passed to the server.
    unsigned int type = writeParameterValue(bindWriter, value);
    typeBuffer.push_back(type);
    typeBuffer.push_back(std::is_unsigned<Src>::value ? '\x80' : '\x00');
    ++currentCol;
}

}

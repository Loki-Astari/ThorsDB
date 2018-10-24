#include "SSLUtil.h"

using namespace ThorsAnvil::SQL;

SSLUtil::SSLUtil()
{
    SSL_load_error_strings();
    SSL_library_init();
}

SSLUtil& SSLUtil::getInstance()
{
    static SSLUtil  instance;
    return instance;
}

std::string SSLUtil::errorMessage()
{
    getInstance();
    std::stringstream message;
    for (long code = ERR_get_error(); code != 0; code = ERR_get_error())
    {
        message << "\t" << ERR_error_string(code, nullptr) << "\n";
    }
    return message.str();
}

SSLMethod::SSLMethod(SSLMethodType type)
    : method(type == SSLMethodType::Client ? DTLS_client_method() : DTLS_server_method())
{
    SSLUtil::getInstance();
}

SSLctx::SSLctx(SSLMethod& method)
    : ctx(SSL_CTX_new(method.method))
{
    if (!ctx)
    {
        throw std::runtime_error(errorMsg("ThorsAnvil::SQL::SSLctx::SSLctx: SSL_CTX_new() failed: ", SSLUtil::errorMessage()));
    }
}

SSLctx::SSLctx(SSLMethod& method, std::string const& certFile, std::string const& keyFile)
    : SSLctx(method)
{
    if (SSL_CTX_set_ecdh_auto(ctx, 1) != 1)
    {
        SSL_CTX_free(ctx);
        throw std::runtime_error(errorMsg("ThorsAnvil::SQL::SSLctx::SSLctx: SSL_CTX_set_ecdh_auto() failed: ", SSLUtil::errorMessage()));
    }
    if (SSL_CTX_use_certificate_file(ctx, certFile.c_str(), SSL_FILETYPE_PEM) != 1)
    {
        SSL_CTX_free(ctx);
        throw std::runtime_error(errorMsg("ThorsAnvil::SQL::SSLctx::SSLctx: SSL_CTX_use_certificate_file() failed: ", SSLUtil::errorMessage()));
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, keyFile.c_str(), SSL_FILETYPE_PEM) != 1 )
    {
        SSL_CTX_free(ctx);
        throw std::runtime_error(errorMsg("ThorsAnvil::SQL::SSLctx::SSLctx: SSL_CTX_use_PrivateKey_file() failed: ", SSLUtil::errorMessage()));
    }
}

SSLctx::~SSLctx()
{
    SSL_CTX_free(ctx);
}

SSLObj::SSLObj(SSLctx const& ctx, int client)
    : ssl(SSL_new(ctx.ctx))
{
    if (!ssl)
    {
        throw std::runtime_error(errorMsg("ThorsAnvil::SQL::SSLObj::SSLObj: SSL_new() failed: ", SSLUtil::errorMessage()));
    }
    if (SSL_set_fd(ssl, client) != 1)
    {
        SSL_free(ssl);
        throw std::runtime_error(errorMsg("ThorsAnvil::SQL::SSLObj::SSLObj: SSL_set_fd() failed: ", SSLUtil::errorMessage()));
    }
}

SSLObj::~SSLObj()
{
    SSL_free(ssl);
}

void SSLObj::accept()
{
    if (SSL_accept(ssl) != 1)
    {
        throw std::runtime_error(errorMsg("ThorsAnvil::SQL::SSLObj::SSLObj: SSL_accept() failed: ", SSLUtil::errorMessage()));
    }
}
void SSLObj::connect()
{
    if (SSL_connect(ssl) != 1)
    {
        throw std::runtime_error(errorMsg("ThorsAnvil::SQL::SSLObj::SSLObj: SSL_connect() failed: ", SSLUtil::errorMessage()));
    }
}

int SSLObj::read(char* buffer, std::size_t len)
{
    return SSL_read(ssl, buffer, len);
}

int SSLObj::write(char const* buffer, std::size_t len)
{
    return SSL_write(ssl, buffer, len);
}

int SSLObj::errorCode(int ret)
{
    return SSL_get_error(ssl, ret);
}

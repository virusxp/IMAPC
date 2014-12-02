#ifndef IMAPC_H
#define IMAPC_H

#include <openssl/ssl.h>

class SSLSocket {
private:
    int socket;
    SLL* sslHandle;
    SSL_CTX* sslContext;

    bool openTCPConnection(char* hostname, int port);
    void disconnect();

public:
    SSLSocket();
    virtual ~SSLSocket();
}

class IMAPClient {
private:


public:
    IMAPClient();
    //virtual ~IMAPClient();
};

#endif //IMAPC_H

#ifndef IMAPC_H
#define IMAPC_H

#include <openssl/ssl.h>

class SSLConnection {
private:
    int socket;
    SLL* sslHandle;
    SSL_CTX* sslContext;
    char hostname[64];
    int port;

    char* rdBuffer;
    int bytesRead;

    char* wrBuffer;
    int bytesWrite;

    bool openTCPConnection(char* hostname, int port);

    void open();
    void disconnect();

public:
    SSLConnection(char* hostname, int port);
    SSLConnection(char* hostname, int port, size_t rwBuffer);
    virtual ~SSLConnection();

    int availableBytes();

    char readByte();
    char* readLine(char* buffer);
    char* readBuffer(char* buffer, int length);

    void writeByte(char chr);
    void writeLine(char* str);
    void writeBuffer(char* str);
}

class IMAPClient {
private:


public:
    IMAPClient();
    //virtual ~IMAPClient();
};

#endif //IMAPC_H

#ifndef IMAPC_H
#define IMAPC_H

#include <openssl/ssl.h>
#include <mutex>

#define ENOBYTSAVLBL    0xFFFE
#define EWRBFFROVRFLW   0xFFFD

class SSLConnection {
private:
    int sock;
    SSL* sslHandle;
    SSL_CTX* sslContext;
    char hostname[64];
    int port;

    volatile size_t maxBufferSize;

    char* rdBuffer;
    volatile int bytesRead;
    volatile int readPtr;
    std::mutex rdMtx;

    char* wrBuffer;
    volatile int bytesWrite;
    volatile int wrPtr;
    std::mutex wrMtx;

    bool openTCPConnection(char* hostname, int port);

    void open();
    void disconnect();

public:
    SSLConnection(char* hostname, int port);
    SSLConnection(char* hostname, int port, size_t rwBuffer);
    virtual ~SSLConnection();

    int availableBytes();
    bool bytesAvailable();

    char readByte();
    char* readByteN(char* buffer, int length);
    char* readLine(char* buffer);

    void writeByte(char chr);
    int writeByteN(char* chr, int length);
    void writeLine(char* str);
};

class IMAPClient {
private:


public:
    IMAPClient();
    //virtual ~IMAPClient();
};

#endif //IMAPC_H

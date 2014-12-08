#include "imapc.h"

#include <string>
#include <cstdlib>
#include <cstdio>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <errno.h>

#define IMAPC_DEFAULT_RW_BUFFER 2048

SSLConnection::SSLConnection(char* hostname, int port) {
    strcpy(this->hostname,hostname);
    this->port = port;

    this->open();

    rdBuffer = (char*)malloc(IMAPC_DEFAULT_RW_BUFFER * sizeof(char));
    if(rdBuffer == NULL) {/*Exception*/}
    bytesRead = 0;
    rdPtr = 0;

    wrBuffer = (char*)malloc(IMAPC_DEFAULT_RW_BUFFER * sizeof(char));
    if(wrBuffer == NULL) {/*Exception*/}
    bytesWrite = 0;
    wrPtr = 0;

    maxBufferSize = IMAPC_DEFAULT_RW_BUFFER;

    //start threading
    rdMtx.unlock();
    wrMtx.unlock();
}

SSLConnection::SSLConnection(char* hostname, int port, size_t rwBuffer) {
    strcpy(this->hostname,hostname);
    this->port = port;

    this->open();

    rdBuffer = (char*)malloc(IMAPC_DEFAULT_RW_BUFFER * sizeof(char));
    if(rdBuffer == NULL) {/*Exception*/}
    bytesRead = 0;
    rdPtr = 0;

    wrBuffer = (char*)malloc(IMAPC_DEFAULT_RW_BUFFER * sizeof(char));
    if(wrBuffer == NULL) {/*Exception*/}
    bytesWrite = 0;
    wrPtr = 0;

    maxBufferSize = rwBuffer;

    //start threading
    rdMtx.unlock();
    wrMtx.unlock();
}

SSLConnection::~SSLConnection() {
    free(rdBuffer);
    free(wrBuffer);

    disconnect();
}

bool SSLConnection::openTCPConnection(char* hostname, int port) {
    struct hostent* hostIP;
    struct sockaddr_in serverInfo;

    hostIP = gethostbyname(hostname);
    this->sock = socket(AF_INET,SOCK_STREAM,0);
    if(this->sock < 0) {
        this->sock = 0;
        return false;
    }

    serverInfo.sin_family = AF_INET;
    serverInfo.sin_port = htons(port);
    serverInfo.sin_addr = *((struct in_addr *) hostIP->h_addr);
    bzero(&(serverInfo.sin_zero),8);

    int error = connect(this->sock,(struct sockaddr*) &serverInfo,sizeof(sockaddr));
    if(erroŕ < 0) {
        this->sock = 0;
        return false;
    }

    return true;
}

void SSLConnection::open() {
    if(openTCPConnection(hostname,port)) {
        SSL_load_error_strings();
        SSL_library_init();
        this->sslContext = SSL_CTX_new(SSLv23_client_method());
        if(this->sslContext == NULL) ERR_print_errors_fp(stderr);
        this->sslHandle = SSL_new(this->sslContext);
        if(this->sslHandle == NULL) ERR_print_errors_fp(stderr);
        if (!SSL_set_fd(this->sslHandle,this->socket)) ERR_print_errors_fp(stderr);
        if (SSL_connect(this->sslHandle) != 1) ERR_print_errors_fp(stderr);
    } else {/*Exception*/}
}

void SSLConnection::disconnect() {
    SSL_shutdown(sslHandle);
    SSL_free(sslHandle);

    close (sock);

    SSL_CTX_free(sslContext);
}

int SSLConnection::availableBytes() {
    int buffer = 0;

    rdMtx.lock();
    buffer = bytesRead;
    rdMtx.unlock();

    return buffer;
}

bool SSLConnection::bytesAvailable() {
    int buffer = 0;

    rdMtx.lock();
    buffer = bytesRead;
    rdMtx.unlock();

    if(buffer > 0) return true;
    else return false;
}

char SSLConnection::readByte() {
    char buffer = '\0';

    if(this->bytesAvailable()) {
        errno = 0;
        rdMtx.lock();
        buffer = rdBuffer[rdPtr];
        rdPtr = (rdPtr < (maxBufferSize - 1)) ? (rdPtr + 1) : 0;
        bytesRead--;
        rdMtx.unlock();
        return buffer;

    } else {
        errno = ENOBYTSAVLBL;
        return '\0';
    }
}

char* SSLConnection::readByteN(char* buffer, int length) {
    if(!this->bytesAvailable) return NULL;

    rdMtx.lock();
    int rdC = (length < bytesRead) ? length : bytesRead;
    bytesRead -= rdC;
    for(int i = 0; i < length; i++) {
        buffer[i] = rdBuffer[rdPtr];
        rdPtr = (rdPtr < (maxBufferSize - 1)) ? (rdPtr + 1) : 0;
    }
    rdMtx.unlock();

    return buffer;
}

char* SSLConnection::readLine(char* buffer) {
    return NULL;
}

char* SSLConnection::readBuffer(char* buffer, int length) {
    return NULL;
}

void SSLConnection::writeByte(char chr) {

}

void SSLConnection::writeByteN(char* chr, int length) {

}

void SSLConnection::writeLine(char* str) {

}

void SSLConnection::writeBuffer(char* str) {

}


IMAPClient::IMAPClient() {

}


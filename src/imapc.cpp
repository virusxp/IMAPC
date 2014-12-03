#include "imapc.h"

#include <string>
#include <cstdlib>
#include <cstdio>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#define IMAPC_DEFAULT_RW_BUFFER 2048

SSLConnection::SSLConnection(char* hostname, int port) {
    strcpy(this->hostname,hostname);
    this->port = port;

    this->open();

    rdBuffer = (char*)malloc(IMAPC_DEFAULT_RW_BUFFER * sizeof(char));
    if(rdBuffer == NULL) {/*Exception*/}
    bytesRead = 0;

    wrBuffer = (char*)malloc(IMAPC_DEFAULT_RW_BUFFER * sizeof(char));
    if(wrBuffer == NULL) {/*Exception*/}
    bytesWrite = 0;
}

SSLConnection(char* hostname, int port, size_t rwBuffer) {
    strcpy(this->hostname,hostname);
    this->port = port;

    this->open();

    rdBuffer = (char*)malloc(IMAPC_DEFAULT_RW_BUFFER * sizeof(char));
    if(rdBuffer == NULL) {/*Exception*/}
    bytesRead = 0;

    wrBuffer = (char*)malloc(IMAPC_DEFAULT_RW_BUFFER * sizeof(char));
    if(wrBuffer == NULL) {/*Exception*/}
    bytesWrite = 0;
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
    this->socket = socket(AF_INET,SOCK_STREAM,0);
    if(this->socket < 0) {
        this->socket = 0;
        return false;
    }

    serverInfo.sin_family = AF_INET;
    serverInfo.sin_port = htons(port);
    serverInfo.sin_addr = *((struct in_addr *) hostIP->h_addr);
    bzero(&(serverInfo.sin_zero),8);

    int error = connect(this->socket,(struct sockaddr*) &serverInfo,sizeof(sockaddr));
    if(erro < 0) {
        this->socket = 0;
        return false;
    }
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
    SSL_shutdown(c->sslHandle);
    SSL_free(c->sslHandle);

    close (c->socket);

    SSL_CTX_free(c->sslContext);
}

int SSLConnection::availableBytes() {

}

char SSLConnection::readByte() {

}

char* SSLConnection::readLine(char* buffer) {

}

char* SSLConnection::readBuffer(char* buffer, int length) {

}

void SSLConnection::writeByte(char chr) {

}

void SSLConnection::writeLine(char* str) {

}

void SSLConnection::writeBuffer(char* str) {

}


IMAPClient::IMAPClient() {

}


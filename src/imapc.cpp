#include "imapc.h"

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

SSLSocket::SSLSocket(char* hostname, int port) {
    if(openTCPConnection(hostname,port)) {
        SSL_load_error_strings();
        SSL_library_init();
        this->sslContext = SSL_CTX_new(SSLv23_client_method());
        if(this->sslContext == NULL) ERR_print_errors_fp(stderr);
        this->sslHandle = SSL_new(this->sslContext);
        if(this->sslHandle == NULL) ERR_print_errors_fp(stderr);
        if (!SSL_set_fd(this->sslHandle,this->socket)) ERR_print_errors_fp(stderr);
        if (SSL_connect(this->sslHandle) != 1) ERR_print_errors_fp(stderr);
    } else {}
}

SSLSocket::~SSLSocket() {
    disconnect();
}

bool SSLSocket::openTCPConnection(char* hostname, int port) {
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

void SSLSocket::disconnect() {
    SSL_shutdown(c->sslHandle);
    SSL_free(c->sslHandle);

    close (c->socket);

    SSL_CTX_free(c->sslContext);
}

IMAPClient::IMAPClient() {

}


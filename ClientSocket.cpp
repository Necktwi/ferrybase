// Implementation of the ClientSocket class

#include "ClientSocket.h"
#include "SocketException.h"
#include "logger.h"
#include <malloc.h>

ClientSocket::ClientSocket(std::string host, int port, Socket::SOCKET_TYPE socketType, std::string trustedCA, std::string privatecert, std::string privatekey) :
host(host), port(port), Socket(socketType, trustedCA, privatecert, privatekey) {
    this->m_sock = -1;
    //soc = socketType == Socket::DEFAULT ? new Socket() : new Socket(socketType, trustedCA, privatecert, privatekey);
    if (!this->create()) {
        throw SocketException("Could not create client socket.");
    }

    if (!this->connect(host, port)) {
        throw SocketException("Could not bind to port.");
    }

}

void ClientSocket::asyncsend(std::string payload, AftermathObj* aftermath_obj) {
    aftermath_obj->payload = payload;
    aftermath_obj->cs = this;
    pthread_create(&aftermath_obj->t, NULL, (void*(*)(void*)) & ClientSocket::socsend, aftermath_obj);
}

void ClientSocket::asyncsend(std::string* payload, AftermathObj* aftermath_obj) {
    aftermath_obj->payloadPTR = payload;
    aftermath_obj->cs = this;
    pthread_create(&aftermath_obj->t, NULL, (void*(*)(void*)) & ClientSocket::socsend, aftermath_obj);
}

const ClientSocket& ClientSocket::operator<<(const std::string& s) const {
    if (!this->send(s)) {
        throw SocketException("Could not write to socket.");
    }
    return *this;
}

const ClientSocket& ClientSocket::operator>>(std::string& s) const {
    if (!this->recv(s)) {
        throw SocketException("Could not read from socket.");
    }

    return *this;
}

void * ClientSocket::socsend(void* args) {
    ClientSocket::AftermathObj* ssta = (ClientSocket::AftermathObj*) args;
    bool isSuccess = true;
    if (!ssta->cs->Socket::send(ssta->payloadPTR, ssta->__flags)) {
        ssta->error = "send failed";
        isSuccess = false;
    }
    ssta->aftermath(ssta->aftermathDS, isSuccess);
    delete ssta;
}

bool ClientSocket::send(const std::string s, int __flags) const {
    return this->Socket::send(&s, __flags);
}

bool ClientSocket::send(const std::string s) const {
    return send(&s, MSG_NOSIGNAL);
}

bool ClientSocket::send(const std::string* s, int __flags) const {
    if (!this->Socket::send(s, __flags)) {
        throw SocketException("Could not send.");
    }
    return true;
}

ClientSocket::~ClientSocket() {

}

/**
 * Reconnectes to the host:port
 * @return void
 */
void ClientSocket::reconnect() {
    if (is_valid())::close(m_sock);
    if (socketType == SOCKET_TYPE::TLS1_1) {
        ShutdownSSL(cSSL);
        DestroySSL();
    }
    if (!this->create()) {
        throw SocketException("Could not create client socket.");
    }

    if (!this->connect(host, port)) {
        throw SocketException("Could not bind to port.");
    }
}

/**
 * Disconnects from the host:port
 * @return void
 */
void ClientSocket::disconnect() {
    if (is_valid())::close(m_sock);
    if (socketType == SOCKET_TYPE::TLS1_1) {
        ShutdownSSL(cSSL);
        DestroySSL();
    }
}

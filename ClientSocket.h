// Definition of the ClientSocket class

#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include "Socket.h"

class ClientSocket : public Socket {
public:

    class AftermathObj {
    public:
        void* (*aftermath)(void* aftermathDS, bool isSuccess);
        void* aftermathDS;
        std::string payload;
        std::string* payloadPTR;
        std::string error;
        int __flags;
#if defined(unix) || defined(__unix__) || defined(__unix)
        pthread_t t;
#endif
        ClientSocket* cs;

        AftermathObj() {
        };

        ~AftermathObj() {
        };
    };

    ClientSocket();
    ClientSocket(std::string host, int port, Socket::SOCKET_TYPE socketType = Socket::DEFAULT, std::string trustedCA = "", std::string privatecert = "", std::string privatekey = "");
    std::string host;
    int port;
    void reconnect();
    void disconnect();
    bool send(const std::string s, int __flags) const;
    bool send(const std::string* s, int __flags) const;
    bool send(const std::string s) const;
    virtual ~ClientSocket();
    const ClientSocket& operator <<(const std::string&) const;
    void asyncsend(std::string payload, AftermathObj* after_math_obj);
    void asyncsend(std::string* payload, AftermathObj* aftermath_obj);
    const ClientSocket& operator >>(std::string&) const;


private:
    //Socket* soc;
    static void* socsend(void*);

    struct soc_send_t_args {
        std::string s;
        void* (&aftermath)(void* aftermathDS);
        void* aftermathDS;
    };
#if defined(unix) || defined(__unix__) || defined(__unix)
    pthread_key_t socket_thread_key;
#endif
};

#endif
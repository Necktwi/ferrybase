// Definition of the Socket class

#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <sys/types.h>
#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ossl_typ.h>
#endif

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int DMAXRECV = 500;

class Socket {
public:
   
   enum SOCKET_TYPE {
      DEFAULT, TLS1_1
   };
   
   bool is_valid() const {
      return m_sock != -1;
   }
   int MAXRECV = 500;
   Socket();
   Socket(SOCKET_TYPE socketType, std::string trustedCA, 
      std::string privatecert, std::string privatekey);
   virtual ~Socket();
   bool create(int timeout_sec = 10);
   bool bind(const int port);
   bool listen() const;
   int accept() const;
#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
   int accept(sockaddr* s_addr, SSL* cssl, SOCKET_TYPE socketType) const;
#endif
   bool connect(const std::string host, const int port);
   bool send(const std::string s, int __flags) const;
   bool send(const std::string* s, int __flags) const;
   bool send(const std::string s) const;
   int recv(std::string&, int size = DMAXRECV) const;
   void set_non_blocking(const bool);
   static void InitializeSSL();
   static void DestroySSL();
#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
   static void ShutdownSSL(SSL* ssl);
#endif
   static std::string getIpAddr(int fd);
   static int getPort(int fd);
   
protected:
   int m_sock = -1;
#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
   sockaddr_in m_addr;
   SSL_CTX *sslctx;
   SSL *cSSL;
#endif
   std::string trustedCA;
   std::string privatecert;
   std::string privatekey;
   SOCKET_TYPE socketType;
};

#endif

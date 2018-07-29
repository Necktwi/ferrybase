
// Implementation of the Socket class.

#include "Socket.h"
#include <string.h>
#include <string>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
#include <openssl/bio.h> 
#include <openssl/ssl.h> 
#include <openssl/err.h> 
#endif

#ifdef __APPLE__
#ifdef __MACH__
#define MSG_NOSIGNAL SO_NOSIGPIPE
#endif
#endif

std::string CA_FILE = "certs/ferryfair.cert";
std::string CLIENT_KEY = "certs/ferryport.ferryfair.key";
std::string CLIENT_CERT = "certs/ferryport.ferryfair.cert";

void Socket::InitializeSSL() {
#if defined(unix) || defined(__unix__) || defined(__unix)
	SSL_load_error_strings();
	SSL_library_init();
	OpenSSL_add_all_algorithms();
#endif
}

void Socket::DestroySSL() {
#if defined(unix) || defined(__unix__) || defined(__unix)
  ERR_free_strings();
	EVP_cleanup();
#endif
}

#if defined(unix) || defined(__unix__) || defined(__unix)
void Socket::ShutdownSSL(SSL* ssl) {
	SSL_shutdown(ssl);
	SSL_free(ssl);
}
#endif

Socket::Socket() :
m_sock(-1), socketType(DEFAULT) {
#if defined(unix) || defined(__unix__) || defined(__unix)
  memset(&m_addr,
			0,
			sizeof ( m_addr));
#endif
}

Socket::Socket(SOCKET_TYPE socketType, std::string trustedCA,
  std::string privatecert, std::string privatekey
) :
  m_sock(-1), socketType(socketType), trustedCA(trustedCA),
  privatecert(privatecert), privatekey(privatekey) 
{
#if defined(unix) || defined(__unix__) || defined(__unix)
  memset(&m_addr, 0, sizeof ( m_addr));
#endif
}

Socket::~Socket() {
#if defined(unix) || defined(__unix__) || defined(__unix)
  if (is_valid())::close(m_sock);
	if (socketType == Socket::TLS1_1) {
		ShutdownSSL(cSSL);
		DestroySSL();
	}
#endif
}

bool Socket::create(int timeout_sec) {
#if defined(unix) || defined(__unix__) || defined(__unix)
  m_sock = socket(AF_INET,
			SOCK_STREAM,
			0);
#endif
	if (!is_valid())
		return false;


	// TIME_WAIT - argh
	int on = 1;
#if defined(unix) || defined(__unix__) || defined(__unix)
	if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR,
			(const char*) &on, sizeof ( on)) == -1)
		return false;

	struct timeval timeout;
	timeout.tv_sec = timeout_sec;
	timeout.tv_usec = 0;

	if (setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof (timeout)) < 0)
		return false;

	if (setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof (timeout)) < 0)
		return false;
#endif
	if (socketType == Socket::TLS1_1) {
		InitializeSSL();
#if defined(__APPLE__)
		sslctx = SSL_CTX_new(TLSv1_1_method());
#elif defined(unix) || defined(__unix__) || defined(__unix)
		sslctx = SSL_CTX_new(TLSv1_1_method());
#endif  
#if defined(unix) || defined(__unix__) || defined(__unix)
    SSL_CTX_set_options(sslctx, SSL_OP_SINGLE_DH_USE);
		/*----- Load a client certificate into the SSL_CTX structure -----*/
		if (SSL_CTX_use_certificate_file(sslctx,
      (char*) CLIENT_CERT.c_str(), SSL_FILETYPE_PEM) <= 0
    ) {
			ERR_print_errors_fp(stderr);
			exit(1);
		}
		/*----- Load a private-key into the SSL_CTX structure -----*/
		if (SSL_CTX_use_PrivateKey_file(sslctx, (char*) CLIENT_KEY.c_str(), SSL_FILETYPE_PEM) <= 0) {
			ERR_print_errors_fp(stderr);
			exit(1);
		}
		int CA = SSL_CTX_load_verify_locations(sslctx, (char*) CA_FILE.c_str(), NULL);
		SSL_CTX_set_verify(sslctx, SSL_VERIFY_PEER, NULL);
		SSL_CTX_set_verify_depth(sslctx, 1);
		cSSL = SSL_new(sslctx);
		int sssfd = SSL_set_fd(cSSL, m_sock);
#endif
	}
	return true;

}

bool Socket::bind(const int port) {
	if (!is_valid()) {
		return false;
	}
#if defined(unix) || defined(__unix__) || defined(__unix)
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = INADDR_ANY;
	m_addr.sin_port = htons(port);

	int bind_return = ::bind(m_sock, (struct sockaddr *) &m_addr, sizeof ( m_addr));
	if (bind_return == -1) {
		return false;
	}
#endif
	return true;
}

bool Socket::listen() const {
	if (!is_valid()) {
		return false;
	}
#if defined(unix) || defined(__unix__) || defined(__unix)
	int listen_return = ::listen(m_sock, MAXCONNECTIONS);

	if (listen_return == -1) {
		return false;
	}
#endif
	return true;
}

int Socket::accept() const {
#if defined(unix) || defined(__unix__) || defined(__unix)
  int addr_length = sizeof ( m_addr);
	int sock 

    = ::accept(m_sock, (sockaddr *) & m_addr, (socklen_t *) & addr_length);

	if (sock <= 0) {
		return -1;
	} else {
		if (socketType != DEFAULT) {
			SSL_accept(cSSL);
		}
		return sock;
	}
#endif
  return 0;
}

#if defined(unix) || defined(__unix__) || defined(__unix)
int Socket::accept(sockaddr* s_addr, SSL* cssl, SOCKET_TYPE socketType) const {
	int addr_length = sizeof (*s_addr);
	int sock = ::accept(m_sock, s_addr, (socklen_t *) & addr_length);
	if (sock <= 0) {
		return -1;
	} else {
		if (socketType != DEFAULT) {
			SSL_accept(cssl);
		}
		return sock;
	}
}
#endif

bool Socket::send(const std::string s, int __flags) const {
	return send(&s, __flags);
}

bool Socket::send(const std::string* s, int __flags) const {
	int status;
#if defined(unix) || defined(__unix__) || defined(__unix)
  if (socketType != DEFAULT) {
		status = SSL_write(cSSL, s->c_str(), s->size());
	} else {
		status = ::send(m_sock, s->c_str(), s->size(), __flags);
	}
#endif
	if (status == -1) {
		return false;
	} else {
		return true;
	}
}

bool Socket::send(const std::string s) const {
#if defined(unix) || defined(__unix__) || defined(__unix)
  return send(&s, MSG_NOSIGNAL);
#endif
  return false;
}

int Socket::recv(std::string& s, int size) const {
	char buf [
#if defined(unix) || defined(__unix__) || defined(__unix)
    size + 
#endif
      1 
  ];

	s = "";

	memset(buf, 0, size + 1);

	int status = 0;
	if (socketType != DEFAULT) {
#if defined(unix) || defined(__unix__) || defined(__unix)
		status = SSL_read(cSSL, (char *) buf, size);
#endif
	} else {
#if defined(unix) || defined(__unix__) || defined(__unix)
		status = ::recv(m_sock, buf, size, 0);
#endif
	}
	if (status >= 0) {
		s.assign(buf, status);
	} else {
		std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
	}
	return status;
}

bool Socket::connect(const std::string host, const int port) {
	if (!is_valid()) return false;
#if defined(unix) || defined(__unix__) || defined(__unix)
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(port);
  hostent* hent;
	in_addr **addr_list;
	hent = gethostbyname(host.c_str());
	if (hent != NULL) {
		addr_list = (struct in_addr **) hent->h_addr_list;
#ifdef DEBUG
		if ((debug & 4) == 4) {
			int i = 0;
			std::cout << "\n" + getTime() + " Socket::connect gethostbyname : ";
			while (addr_list[i] != NULL) {
				printf("%s ", inet_ntoa(*addr_list[i]));
				i++;
			}
			std::cout << "\n";
			fflush(stdout);
		}
#endif
    int status =
#if defined(unix) || defined(__unix__) || defined(__unix)
      inet_pton(AF_INET, inet_ntoa(*addr_list[0]), &m_addr.sin_addr);
    if (errno == EAFNOSUPPORT) return false;
    status = ::connect(m_sock, (sockaddr *)& m_addr, sizeof(m_addr));
#else
      0;
#endif
		if (status == 0) {
			if (socketType != DEFAULT) {
#if defined(unix) || defined(__unix__) || defined(__unix)
        int err = SSL_connect(cSSL);
				if (SSL_get_peer_certificate(cSSL) != NULL) {
					if (SSL_get_verify_result(cSSL) == X509_V_OK) {
						return true;
					}
				}
				if (err <= 0) {
					//log and close down ssl
					int sslerr = SSL_get_error(cSSL, err);
					ShutdownSSL(cSSL);
				}
#endif
			}
			return true;
		} else {
			return false;
		}
	} else {
#ifdef DEBUG
		if ((debug & 4) == 4) {
			std::cout << "\n" + getTime() + " Socket::connect: unable to resolve hostname " << host << "\n";
			fflush(stdout);
			return false;
		}
#endif
	}
#endif
  return false;
}

void Socket::set_non_blocking(const bool b) {
	int opts;
#if defined(unix) || defined(__unix__) || defined(__unix)
	opts = fcntl(m_sock, F_GETFL);
	if (opts < 0) {
		return;
	}
	if (b)
		opts = (opts | O_NONBLOCK);
	else
		opts = (opts & ~O_NONBLOCK);

	fcntl(m_sock, F_SETFL, opts);
#endif
}

std::string Socket::getIpAddr(int fd) {
#if defined(unix) || defined(__unix__) || defined(__unix)
	socklen_t len;
	struct sockaddr_storage addr;
	char ipstr[INET6_ADDRSTRLEN];

	len = sizeof addr;
	getpeername(fd, (struct sockaddr*) &addr, &len);

	// deal with both IPv4 and IPv6:
	if (addr.ss_family == AF_INET) {
		struct sockaddr_in *s = (struct sockaddr_in *) &addr;
		inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	} else { // AF_INET6
		struct sockaddr_in6 *s = (struct sockaddr_in6 *) &addr;
		inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
	}
	return std::string((char*) ipstr);
#endif
  return std::string();
}

int Socket::getPort(int fd) {
#if defined(unix) || defined(__unix__) || defined(__unix)
  socklen_t len;
	struct sockaddr_storage addr;
	int port;

	len = sizeof addr;
	getpeername(fd, (struct sockaddr*) &addr, &len);

	// deal with both IPv4 and IPv6:
	if (addr.ss_family == AF_INET) {
		struct sockaddr_in *s = (struct sockaddr_in *) &addr;
		port = ntohs(s->sin_port);
	} else { // AF_INET6
		struct sockaddr_in6 *s = (struct sockaddr_in6 *) &addr;
		port = ntohs(s->sin6_port);
	}
	return port;
#endif 
  return 0;
}

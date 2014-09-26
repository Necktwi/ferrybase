/* 
 * File:   mycurl.h
<<<<<<< HEAD
 * Author: Satya Gowtham Kudupudi
=======
>>>>>>> 8fe40f3504389521f8093183796f838f2fc8f9a3
 *
 * Created on 20 March, 2013, 2:53 PM
 */

#ifndef MYCURL_H
#define	MYCURL_H

#include "Socket.h"
#include<string>

using std::string;

string SOAPReq(string hostname, string port, string requestPath, string SOAPAction, string content, Socket::SOCKET_TYPE socketType = Socket::DEFAULT, std::string trustedCA = "", std::string privatecert = "", std::string privatekey = "");
string HTTPReq(string hostname, string requestPath, string port = "80", string content = "", Socket::SOCKET_TYPE socketType = Socket::DEFAULT, std::string trustedCA = "", std::string privatecert = "", std::string privatekey = "");
#endif	/* MYCURL_H */


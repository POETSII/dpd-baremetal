#ifndef _EXTERNAL_SERVER_HH
#define _EXTERNAL_SERVER_HH
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <cstdint>
#include <thread>
#include "externals-common.h"

// ExternalServer
// used by the dpd simulation to expose an external connection for visualising

class ExternalServer {
  public:
    // constructor
    ExternalServer(std::string socket_name);
    ~ExternalServer(); // destructor

    // connection control 
    void connect(); // connect to pts-serve via the named pipes
    unsigned tryRecv(pts_to_extern_t *msg); // recv a message non-blocking 
    void send(pts_to_extern_t *msg); // send a message to the POETS server
    void waitForConnection(std::string socket_name); // blocks waiting for a connection
    void spawnConnector(std::string socket_name); // spawns a thread that waits for connection

    //! returns true if this socket has established a connection
    bool hasConnection(); 
    
  private:

    // socket
    boost::asio::io_service *backend_socket_ioservice_;
    boost::asio::local::stream_protocol::endpoint* backend_ep_;
    boost::asio::local::stream_protocol::acceptor* backend_acceptor_;
    boost::asio::local::stream_protocol::socket* backend_socket_;
   
    std::string socket_; // name for the socket to connect to pts-serve

    std::thread *connection_thread_; // thread used to wait for a connection

    //! when true a connection has been established
    bool connection_established_;
}; 

#endif /* _EXTERNAL_SERVER_HH */

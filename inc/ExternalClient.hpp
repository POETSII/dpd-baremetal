#ifndef _EXTERNAL_CLIENT_HH
#define _EXTERNAL_CLIENT_HH
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <cstdint>
#ifdef GALS
#include "gals.h"
#elif defined(SERIAL)
#else
#include "sync.h"
#endif
#include "externals-common.h"

// ExternalClient
// Used to connect to a remote POETS DPD simulation

class ExternalClient {
  public:
    // constructor
    ExternalClient(std::string socket_name);
    ~ExternalClient(); // destructor

    // connection control
    void connect(); // connect to pts-serve via the named pipes
    unsigned tryRecv(pts_to_extern_t *msg); // recv a message non-blocking
    void send(pts_to_extern_t *msg); // send a message to the POETS server

  private:

    // socket
    boost::asio::io_service *backend_socket_ioservice_;
    boost::asio::local::stream_protocol::endpoint* backend_ep_; // endpoint for the send channel
    boost::asio::local::stream_protocol::socket* backend_socket_; // socket for the send channel

    std::string socket_; // name for the socket to connect to pts-serve
};

#endif /* _EXTERNAL_CLIENT_HH */

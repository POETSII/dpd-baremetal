#ifndef _EXTERNALS_CLIENT_IMPL
#define _EXTERNALS_CLIENT_IMPL
#include "ExternalClient.hpp" 

// sends a message to hte poets server which injects it into the network
void ExternalClient::send(pts_to_extern_t *msg) {
  backend_socket_->send(boost::asio::buffer(msg, sizeof(pts_to_extern_t)));
}

// non-blocking receive from the poets server
// returns the number of bytes that were accepted
unsigned ExternalClient::tryRecv(pts_to_extern_t *msg) {
  if(backend_socket_->available() >= sizeof(pts_to_extern_t)) { // check if packet is available
    backend_socket_->receive(boost::asio::buffer(msg, sizeof(pts_to_extern_t)));
    return sizeof(pts_to_extern_t);
  }
  return 0;
} 

// Constructor
ExternalClient::ExternalClient(std::string socket_name) {
  std::string socket_ = socket_name;  

  backend_socket_ioservice_ = new boost::asio::io_service();
  backend_ep_ = new boost::asio::local::stream_protocol::endpoint(socket_name);
  backend_socket_= new boost::asio::local::stream_protocol::socket(*backend_socket_ioservice_);
  backend_socket_->connect(*backend_ep_); // send data down the send_pipe 

  backend_socket_ioservice_->run();

}

// destructor
ExternalClient::~ExternalClient()
{
  // close the pipes and file descriptors
  
}

#endif /* _EXTERNALS_CLIENT_IMPL */

#ifndef _EXTERNAL_SERVER_IMPL
#define _EXTERNAL_SERVER_IMPL

#include "ExternalServer.hpp" 

// ExternalServer
// Used to manage a connection from the external-daemon to the backend (either pts-serve or epoch_sim)

// sends a message to hte poets server which injects it into the network
void ExternalServer::send(pts_to_extern_t *msg) {
  backend_socket_->send(boost::asio::buffer(msg, sizeof(pts_to_extern_t)));
}

// non-blocking receive from the poets server
// returns the number of bytes that were accepted
unsigned ExternalServer::tryRecv(pts_to_extern_t *msg) {
  if(backend_socket_->available() >= sizeof(pts_to_extern_t)) { // check if packet is available
    backend_socket_->receive(boost::asio::buffer(msg, sizeof(pts_to_extern_t)));
    return sizeof(pts_to_extern_t);
  }
  return 0;
} 

//! blocks waiting for a connection
void ExternalServer::waitForConnection(std::string socket_name) {
  std::cout << "starting up a connection at: " << socket_name << "\n";
  backend_socket_ioservice_ = new boost::asio::io_service();
  backend_ep_ = new boost::asio::local::stream_protocol::endpoint(socket_name);
  backend_acceptor_ = new boost::asio::local::stream_protocol::acceptor(*backend_socket_ioservice_, *backend_ep_);
  backend_socket_ = new boost::asio::local::stream_protocol::socket(*backend_socket_ioservice_);

  backend_acceptor_->accept(*backend_socket_); // accept data from the recv_pipe
  backend_socket_ioservice_->run();
  connection_established_ = true;  
  return;
}

//! used to spawn a thread that waits for a connection
void ExternalServer::spawnConnector(std::string socket_name){
  connection_thread_ = new std::thread(&ExternalServer::waitForConnection, this, socket_name);
  return;
}

//! returns the state of the connection (true - has a connection. false - does not have a connection)
bool ExternalServer::hasConnection() { 
  return connection_established_; 
}

// Constructor
ExternalServer::ExternalServer(std::string socket_name) {
  std::string socket_ = socket_name;  

  // initially we don't have a connection
  connection_established_ = false;

  // setting up the socket
  ::unlink(socket_.c_str()); // remove the previous link

  // spawn thread to wait for connection
  spawnConnector(socket_name);

}

// destructor
ExternalServer::~ExternalServer()
{
  // close the pipes and file descriptors
  delete backend_socket_ioservice_;
  delete backend_ep_;
  delete backend_acceptor_;
  delete backend_socket_;
  
}

#endif /* _EXTERNAL_SERVER_H */

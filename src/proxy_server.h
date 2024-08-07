#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>

using boost::asio::ip::tcp;

class ProxyServer {
  public:
  ProxyServer(boost::asio::io_context& io_context, short port, const std::string& db_host, short db_port);

  private:
  void do_accept();
  void handle_client(std::shared_ptr<tcp::socket> client_socket);
  void handle_server(std::shared_ptr<tcp::socket> client_socket, std::shared_ptr<tcp::socket> server_socket);

  tcp::acceptor acceptor_;
  tcp::resolver resolver_;
  std::string db_host_;
  short db_port_;
  std::ofstream log_file_;
};

#endif // PROXY_SERVER_H

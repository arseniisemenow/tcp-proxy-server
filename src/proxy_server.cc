#include "proxy_server.h"

ProxyServer::ProxyServer(boost::asio::io_context& io_context, short port, const std::string& db_host, short db_port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
      resolver_(io_context),
      db_host_(db_host),
      db_port_(db_port),
      log_file_("sql_queries.log", std::ios_base::app) {
    do_accept();
}

void ProxyServer::do_accept() {
    auto client_socket = std::make_shared<tcp::socket>(acceptor_.get_io_context());
    acceptor_.async_accept(*client_socket, [this, client_socket](boost::system::error_code ec) {
        if (!ec) {
            handle_client(client_socket);
        }
        do_accept();
    });
}

void ProxyServer::handle_client(std::shared_ptr<tcp::socket> client_socket) {
    auto endpoints = resolver_.resolve(db_host_, std::to_string(db_port_));
    auto server_socket = std::make_shared<tcp::socket>(client_socket->get_io_context());

    boost::asio::async_connect(*server_socket, endpoints, [this, client_socket, server_socket](boost::system::error_code ec, tcp::endpoint) {
        if (!ec) {
            handle_server(client_socket, server_socket);
        }
    });
}

void ProxyServer::handle_server(std::shared_ptr<tcp::socket> client_socket, std::shared_ptr<tcp::socket> server_socket) {
    auto client_to_server_buffer = std::make_shared<boost::asio::streambuf>();
    auto server_to_client_buffer = std::make_shared<boost::asio::streambuf>();

    boost::asio::async_read_until(*client_socket, *client_to_server_buffer, '\0',
        [this, client_socket, server_socket, client_to_server_buffer](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                std::istream is(client_to_server_buffer.get());
                std::string sql_query(length, '\0');
                is.read(&sql_query[0], length);
                log_file_ << sql_query << std::endl;

                boost::asio::async_write(*server_socket, *client_to_server_buffer,
                    [this, client_socket, server_socket, client_to_server_buffer](boost::system::error_code ec, std::size_t) {
                        if (!ec) {
                            handle_server(client_socket, server_socket);
                        }
                    });
            }
        });

    boost::asio::async_read_until(*server_socket, *server_to_client_buffer, '\0',
        [this, client_socket, server_socket, server_to_client_buffer](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                boost::asio::async_write(*client_socket, *server_to_client_buffer,
                    [this, client_socket, server_socket, server_to_client_buffer](boost::system::error_code ec, std::size_t) {
                        if (!ec) {
                            handle_server(client_socket, server_socket);
                        }
                    });
            }
        });
}

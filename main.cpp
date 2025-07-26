#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include "asio.hpp"

using asio::ip::tcp;

class Amplifier {
public:
    std::string process_command(const std::string& cmd) {
        std::cout << "Received command: " << cmd << std::endl;
        if (cmd == "GET_VOLUME") {
            return "VOLUME=" + std::to_string(volume) + "\n";
        } else if (cmd.rfind("SET_VOLUME ", 0) == 0) {
            try {
                int new_vol = std::stoi(cmd.substr(11));
                if (new_vol >= 0 && new_vol <= 100) {
                    volume = new_vol;
                    return "ACK\n";
                } else {
                    return "ERR_VALUE_OUT_OF_RANGE\n";
                }
            } catch(...) {
                return "ERR_INVALID_ARG\n";
            }
        }
        return "ERR_UNKNOWN_COMMAND\n";
    }
private:
    int volume = 50;
};

// Manages a single client connection
class session : public std::enable_shared_from_this<session> {
public:
    session(tcp::socket socket, Amplifier& amp)
        : socket_(std::move(socket)), amplifier_(amp) {}

    void start() {
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(asio::buffer(data_, max_length),
            [this, self](std::error_code ec, std::size_t length) {
                if (!ec) {
                    std::string command(data_, length);
                    command.erase(std::remove_if(command.begin(), command.end(), 
                                    [](char c){ return c == '\n' || c == '\r'; }), 
                                  command.end());

                    std::string response = amplifier_.process_command(command);
                    do_write(response);
                }
            });
    }

    void do_write(const std::string& response) {
        auto self(shared_from_this());
        asio::async_write(socket_, asio::buffer(response),
            [this, self](std::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    do_read(); // Wait for the next command
                }
            });
    }

    tcp::socket socket_;
    Amplifier& amplifier_;
    enum { max_length = 1024 };
    char data_[max_length];
};

// Accepts incoming connections
class server {
public:
    server(asio::io_context& io_context, short port, Amplifier& amp)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), amplifier_(amp) {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](std::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<session>(std::move(socket), amplifier_)->start();
                }
                do_accept();
            });
    }

    tcp::acceptor acceptor_;
    Amplifier& amplifier_;
};

int main() {
    try {
        asio::io_context io_context;
        Amplifier amp;
        server s(io_context, 12345, amp);
        std::cout << "Asynchronous device simulator running on port 12345..." << std::endl;
        io_context.run(); // This call blocks and runs the server
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
//
// Created by PeachJames on 01/08/2018.
//

#include "uvw.hpp"
#include <memory>
#include <iostream>
#include <deque>
#include <sstream>

#include "protocol.h"

class server {

public:
    using client_ptr = std::shared_ptr<uvw::TcpHandle>;

    struct client_record {
        client_ptr d_client;
        std::optional<protocol::hello> d_hello;
    };

    server(){ std::cout << "Server started\n"; }
    ~server(){ std::cout << "Server stopped\n"; }

    void addConnection(client_ptr client)
    {
        std::cout << "New connection" << "\n";

        d_connections[(uv_os_fd_t)client->fileno()] = {client, {}};
    }

    void removeConnection(client_ptr client)
    {
        std::cout << "Client remove" << "\n";

        auto conit = d_connections.find((uv_os_fd_t)client->fileno());
        d_connections.erase(conit);
    }

    void send(client_ptr client, std::string_view content)
    {
        auto buffer = std::make_unique<char[]>(content.size());
        std::copy(content.begin(), content.end(), buffer.get());
        client->write(std::move(buffer), content.size());
    }

    void connectionSay(client_ptr client, protocol::client_input input)
    {
        std::cout << "Client say input: ";
        std::visit(protocol::input_print_visitor{}, input);

        if (std::holds_alternative<protocol::chat>(input)){
            auto& chat = std::get<protocol::chat>(input);
            std::ostringstream message;
            message << "CHAT " << chat.message() << "\n";
            for (auto &connection: d_connections) {
                send(connection.second.d_client, message.str());
            }
        } else if (std::holds_alternative<protocol::quit>(input)){
            client->close();
        } else if (std::holds_alternative<protocol::err>(input)){
            std::ostringstream message;
            message << "ERR unknown input\n";
            send(client, message.str());
        }
    }


private:
    std::unordered_map<int, client_record> d_connections;
};

void listen(uvw::Loop &loop) {
    std::shared_ptr<uvw::TcpHandle> tcp = loop.resource<uvw::TcpHandle>();

    tcp->data(std::make_shared<server>());

    tcp->on<uvw::ListenEvent>([tcp](const uvw::ListenEvent &, uvw::TcpHandle &srv) {
        std::cout << "Conn received\n";
        std::shared_ptr<uvw::TcpHandle> client = srv.loop().resource<uvw::TcpHandle>();

        {
            auto srv = tcp->data<server>();
            srv->addConnection(client);
        }

        client->on<uvw::CloseEvent>([tcp](const uvw::CloseEvent &, uvw::TcpHandle &client) {
            auto srv = tcp->data<server>();
            srv->removeConnection(client.shared_from_this());
            std::cout << "Close received\n";
        });

        client->on<uvw::EndEvent>([](const uvw::EndEvent &ee, uvw::TcpHandle &client) {
            std::cout << "End event\n";
            client.close();
        });

        client->on<uvw::DataEvent>([tcp](const uvw::DataEvent& de, uvw::TcpHandle& client){
            std::cout << "Data received: len=" << de.length << "\n";

            if (!de.data) return 0;

            auto isvalid = [](const char c){
                return !(std::iswprint(c) || c == '\n' || c == '\r');
            };

            if (std::find_if(de.data.get(), de.data.get() + de.length, isvalid) != de.data.get() + de.length) {
                std::cout << "Ignoring\n";
                return 0;
            }

            // assume ends with \r\n
            std::string dataStr{de.data.get(), de.length -2};

            if (0 == dataStr.find_first_of("exit")) {
                std::cout << "Exit received\n";
                client.close();
            }

            protocol::client_input input = protocol::parse_client_input(dataStr);

            auto srv = tcp->data<server>();
            srv->connectionSay(client.shared_from_this(), input);

            return 0;
        });

        srv.accept(*client);
        client->read();
    });

    std::cout << "Listening\n";
    tcp->bind("127.0.0.1", 4242);
    tcp->listen();
}

int main()
{
    auto loop = uvw::Loop::getDefault();

    listen(*loop);

    loop->run();
}
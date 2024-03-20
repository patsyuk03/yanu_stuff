#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <jsoncpp/json/json.h>

typedef websocketpp::client<websocketpp::config::asio> client;

void on_message(client* c, websocketpp::connection_hdl hdl, client::message_ptr msg) {
    std::cout << "2" << std::endl;
    std::cout << "Received: " << msg->get_payload() << std::endl;
}

void on_error(client* c, websocketpp::connection_hdl hdl) {
    std::cout << "3" << std::endl;
    std::cout << "Error: " << c->get_con_from_hdl(hdl)->get_ec().message() << std::endl;
}

void on_close(client* c, websocketpp::connection_hdl hdl) {
    std::cout << "4" << std::endl;
    std::cout << "Connection closed" << std::endl;
    std::cout << c << std::endl;
}

void on_open(client* c, websocketpp::connection_hdl hdl) {
    std::cout << "5" << std::endl;
    std::cout << "Connection established" << std::endl;

    // Send a message once the connection is open
    Json::Value parameters;
    parameters["a_iBtnNbr"] = 1;
    Json::Value message;
    message["function"] = "startBeverage";
    message["parameters"] = parameters;

    Json::StreamWriterBuilder wbuilder;
    std::string json_message = Json::writeString(wbuilder, message);

    c->send(hdl, json_message, websocketpp::frame::opcode::text);
}

int main() {
    std::string ws_url = "ws://192.168.1.10:25000";
    client ws_client;

    // Set up access channels to only log errors
    ws_client.clear_access_channels(websocketpp::log::alevel::all);
    ws_client.set_error_channels(websocketpp::log::elevel::all);

    // Initialize Asio
    ws_client.init_asio();

    // Register our message handler
    ws_client.set_message_handler(std::bind(&on_message, &ws_client, std::placeholders::_1, std::placeholders::_2));
    ws_client.set_fail_handler(std::bind(&on_error, &ws_client, std::placeholders::_1));
    ws_client.set_close_handler(std::bind(&on_close, &ws_client, std::placeholders::_1));
    ws_client.set_open_handler(std::bind(&on_open, &ws_client, std::placeholders::_1));

    websocketpp::lib::error_code ec;
    client::connection_ptr con = ws_client.get_connection(ws_url, ec);
    if (ec) {
        std::cout << "Connection error: " << ec.message() << std::endl;
        return -1;
    }

    // Queue a connection
    ws_client.connect(con);

    // Start the ASIO io_service run loop
    ws_client.run();
    return 0;
}

// #include "control_socket.h"

// #include <boost/beast/core.hpp>
// #include <boost/make_shared.hpp>
// #include <boost/bind.hpp>

// #include <iostream>

// using namespace std;

// namespace beast = boost::beast;
// namespace http  = beast::http;
// namespace websocket = beast::websocket;
// namespace net = boost::asio;

// control_socket::control_socket(empty_callback start, 
//                                 empty_callback stop) :
//     m_start_callback(start),
//     m_stop_callback(stop),
//     host( boost::asio::ip::make_address_v4("127.0.0.1"))
// {
    
// }

// control_socket::~control_socket()
// {
// }

// void control_socket::run()
// {
//     net::io_service ioc;
//     socket = boost::make_shared<net::ip::tcp::socket>(ioc);
//     m_ws = boost::make_shared<websocket::stream<net::ip::tcp::socket&>>(socket);
    
//     net::ip::tcp::endpoint ep{host, port};

//     net::ip::tcp::acceptor acpt(ioc);

//     beast::error_code err;

//     acpt.open(ep.protocol(), err);

//     if (err)
//     {
//         cout << "Error!(WebSocket) " << err.message() << endl;
//         return;
//     }

//     acpt.set_option(net::socket_base::reuse_address(true), err);
    
//     if (err)
//     {
//         cout << "Error!(WebSocket) " << err.message() << endl;
//         return;
//     }

//     acpt.bind(ep, err);

//     if (err)
//     {
//         cout << "Error!(WebSocket) " << err.message() << endl;
//         return;
//     }

//     acpt.listen(1, err);

//     if (err)
//     {
//         cout << "Error!(WebSocket) " << err.message() << endl;
//         return;
//     }

//     acpt.async_accept( ioc, boost::bind(
//         &control_socket::on_accept,
//         this));

//     ioc.run_one();
// }

// void control_socket::on_accept(boost::beast::error_code err)
// {
//     if (err)
//     {
//         cout << "Error!(WebSocket) " << err.message() << endl;
//         return;
//     }
//     // Accept the websocket handshake
//     m_ws->async_accept(
//         boost::bind(
//             &control_socket::on_handshake_accept,
//             this));
// }

// void control_socket::on_handshake_accept(boost::beast::error_code err)
// {
//     if (err)
//     {
//         cout << "Error!(WebSocket) " << err.message() << endl;
//         return;
//     }

//     m_ws->async_read(
//         m_buffer,
//         boost::bind(
//             &control_socket::on_read,
//             this));
// }

// void control_socket::on_read(boost::beast::error_code err,
//     std::size_t bytes_transferred)
// {
//     boost::ignore_unused(bytes_transferred);

//     // This indicates that the session was closed
//     if(err == websocket::error::closed)
//         return;

//     if (err)
//     {
//         cout << "Error!(WebSocket) " << err.message() << endl;
//         return;
//     }

//     void* data = const_cast<void*>(m_buffer.data().data());
    
//     cout << "Got msg " << std::string(reinterpret_cast<char*>(data)) << endl;
//     m_buffer.consume(m_buffer.size());

//     m_ws->async_read(
//         m_buffer,
//         boost::bind(
//             &control_socket::on_read,
//             this));
// }
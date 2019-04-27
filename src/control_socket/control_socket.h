// #ifndef CONTROL_SOCKET
// #define CONTROL_SOCKET

// #include <boost/beast/core/error.hpp>
// #include <boost/beast/websocket.hpp>
// #include <boost/beast/core/flat_buffer.hpp>
// #include <boost/shared_ptr.hpp>
// #include <boost/asio.hpp>
// #include <boost/function.hpp>


// typedef boost::function<void()> empty_callback;

// class control_socket
// {
// public:
//     control_socket(empty_callback, empty_callback);
    
//     ~control_socket();

//     void run();

// private:

//     void on_read(boost::beast::error_code,
//         std::size_t bytes_transferred);
        
//     void on_accept(boost::beast::error_code);
    
//     void on_handshake_accept(boost::beast::error_code);

// private:
//     boost::shared_ptr<boost::asio::ip::tcp::socket> socket;
//     boost::shared_ptr<boost::beast::websocket::stream<boost::asio::ip::tcp::socket&>> m_ws;
//     const empty_callback m_start_callback;
//     const empty_callback m_stop_callback;

//     const boost::asio::ip::address_v4 host;
//     const unsigned short port = 4545;
    
//     boost::beast::flat_buffer m_buffer;
    
// };

// #endif
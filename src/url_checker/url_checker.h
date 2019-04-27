#ifndef URL_CHECKER
#define URL_CHECKER

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/function.hpp>

#include <memory>
#include <string>

using tcp = boost::asio::ip::tcp;

class url_checker : public 
    boost::enable_shared_from_this<url_checker>, 
    boost::noncopyable
{
    explicit url_checker(boost::asio::io_context* ctx,
                        boost::function<void(url_checker*, std::string, unsigned)>);
public:
    typedef boost::shared_ptr<url_checker> ptr;

    static ptr init(boost::asio::io_context* ctx,
                        boost::function<void(url_checker*, std::string, unsigned)>);

    ~url_checker();

    void check(const std::string&);

private:
    void init_buffers();

    bool is_error(const boost::system::error_code& error,
                    const std::string& context = "") const;

    void shutdown(bool send_callback = false);

    // Async query
    void onResolve(boost::system::error_code, 
                   tcp::resolver::iterator iter);

    void onConnect(boost::system::error_code);

    void onWrite(boost::system::error_code,
                 std::size_t bytes_transferred);
                 
    void onRead(boost::system::error_code,
                 std::size_t bytes_readed);

    void onResponceTimeout(boost::system::error_code);
    
private:
    std::string m_host;
    const std::string m_target = "/";
    const unsigned m_http_version = 11;
    
    boost::function<void(url_checker*, std::string, unsigned)> m_callback;

    boost::asio::io_context* ioc;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::ip::tcp::resolver::query query;

    boost::beast::http::response_parser<boost::beast::http::buffer_body>* res;
    boost::beast::http::request<boost::beast::http::string_body>* req = nullptr;
    
    boost::asio::deadline_timer responce_timer;

    boost::beast::flat_buffer buffer;
    boost::asio::ip::tcp::socket socket;
};

#endif
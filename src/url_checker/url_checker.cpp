#include "url_checker.h"
#include "../traits.h"

// Boost
#include <boost/asio/connect.hpp>
#include <boost/bind.hpp>

// STL
#include <utility>
#include <strstream>

namespace http = boost::beast::http;

using namespace std;
using namespace boost::placeholders;

url_checker::url_checker(boost::asio::io_context* ctx, 
                        boost::function<void(url_checker*, std::string, unsigned)> callback) :
    ioc(ctx),
    socket(*ioc),
    resolver(*ctx),
    m_callback(callback),
    responce_timer(*ctx),
    query("", "80")
{
}

url_checker::ptr url_checker::init(boost::asio::io_context* ctx, 
                        boost::function<void(url_checker*, std::string, unsigned)> callback)
{
    url_checker::ptr new_(new url_checker(ctx, callback));
    return new_;
}

url_checker::~url_checker()
{
}

void url_checker::init_buffers()
{
    res = new http::response_parser<http::buffer_body>();
    req = new http::request<http::string_body>(http::verb::head, m_target, m_http_version);

    // Задаём поля HTTP заголовка
    req->set(http::field::host, m_host);
    req->set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    buffer.consume(buffer.size());
}

bool url_checker::is_error(const boost::system::error_code& error,
                           const std::string& context) const
{
    return error ? 
        [&error, &context]() -> bool 
        {
            std::ostringstream ss;
            ss << "[!] Error occured (" << context <<
                    ") : " << error.message() << endl; 
            debug::log(move(ss.str()));
            return true;
        }() 
        : false;
}

void url_checker::shutdown(bool send_callback) 
{
    unsigned status = 0;
    boost::system::error_code error;
    if (send_callback)
    {
            status = static_cast<unsigned>(res->get().result());
    }
    else 
    {
        if (socket.is_open())
        {
            socket.close(error);
            debug::log("Socket (" + m_host + ") close error: " +
                string(error.message()));
        }
    }
    socket.cancel(error);
    debug::log("Socket (" + m_host + ") cancel error: " +
        string(error.message()));
    m_callback(this, m_host, status);
}

void url_checker::onResolve(boost::system::error_code error, 
                            tcp::resolver::iterator iter) 
{
    debug::log("Resolve " + m_host);
    responce_timer.cancel();
    
    if (error == boost::asio::error::operation_aborted)
        return;

    if (is_error(error, "Resolve")) 
    {
        shutdown();
        return;
    }
    
    responce_timer.expires_from_now(boost::posix_time::seconds(2));

    responce_timer.async_wait(
        boost::bind(&url_checker::onResponceTimeout, this, _1));
    
    boost::asio::async_connect(socket, iter, 
                    boost::bind(&url_checker::onConnect, this, _1));
}


void url_checker::onConnect(boost::system::error_code error) 
{
    debug::log("Connect " + m_host);
    responce_timer.cancel();
    
    if (error == boost::asio::error::operation_aborted)
        return;

    if (is_error(error, "Connect")) 
    {
        shutdown();
        return;
    }
    // Отправляем реквест через приконекченный сокет
    http::async_write(socket, *req, 
                boost::bind(&url_checker::onWrite, this, _1, _2));
    // http::write(*socket, req, error);
}


void url_checker::onWrite(boost::system::error_code error,
                std::size_t bytes_transferred)
{
    debug::log("Write " + m_host);
    if (error == boost::asio::error::operation_aborted)
        return;

    if (is_error(error, "Write")) 
    {
        shutdown();
        return;
    }

    // Часть, отвечающая за чтение респонса

    responce_timer.expires_from_now(boost::posix_time::seconds(2));

    responce_timer.async_wait(
        boost::bind(&url_checker::onResponceTimeout, this, _1));

    http::async_read_some(socket, buffer, *res, 
        boost::bind(&url_checker::onRead, this, _1, _2));
}


void url_checker::onResponceTimeout(boost::system::error_code error)
{
    debug::log("Responce timed out " + m_host);
    if (error != boost::asio::error::operation_aborted)
    {
        is_error(error, "Responce timed out");
        shutdown();
    }
}


void url_checker::onRead(boost::system::error_code error,
                std::size_t bytes_readed)
{
    debug::log("Read " + m_host);

    if (error == boost::asio::error::operation_aborted)
        return;

    if (is_error(error, "Read")) 
    {
        shutdown();
        return;
    }

    if (!res->is_header_done())
    {
        http::async_read_some(socket, buffer, *res, 
            boost::bind(&url_checker::onRead, this, _1, _2));
    }
    else
    {
        responce_timer.cancel();
        shutdown(true);
    }
}

void url_checker::check(const string& host)
{
    m_host = host;

    debug::log("Checking " + m_host);

    init_buffers();

    // Tcp сокет, использующейся для соединения

    // Резолвим адрес
    query = tcp::resolver::query(m_host, "80");

    responce_timer.expires_from_now(boost::posix_time::seconds(2));

    responce_timer.async_wait(
        boost::bind(&url_checker::onResponceTimeout, this, _1));

    resolver.async_resolve(query, boost::bind(&url_checker::onResolve, this, _1, _2));
}

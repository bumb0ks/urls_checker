#ifndef CHECKER_MANAGER
#define CHECKER_MANAGER

#include <boost/asio.hpp>
#include <boost/container/vector.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/atomic.hpp>
#include <string>

#include "../url_checker/url_checker.h"
#include "../result_collector/result_collector.h"
#include "../data_collector/data_collector.h"
#include "../traits.h"

namespace container = boost::container;

class checkers_manager
{
public:
    checkers_manager();

    void start();

    void stop();

private:
    void run_ctx();

    void print() const;

    void set_data(url_vector& urls);

    void wait_to_checkup();

    void add_result(url_checker* sender, std::string url, unsigned st);

private:
    std::unique_ptr<result_collector> result_col;
    std::unique_ptr<data_collector> data_col;
    std::unique_ptr<boost::asio::io_context> ctx;
    std::vector<url_checker::ptr> chs;

    boost::thread_group thread_gr;
    boost::mutex result_mutex;

    const unsigned int checkers_size = 4;
    url_vector m_urls;

    boost::atomic<bool> m_need_to_stop = false;
};

#endif
#include "chekers_manager.h"

#include <boost/bind.hpp>

#include <algorithm>
#include <iostream>

using namespace std;

checkers_manager::checkers_manager() :
    ctx(boost::make_unique<boost::asio::io_context>()),
    result_col(boost::make_unique<result_collector>()),
    data_col(boost::make_unique<data_collector>())
{
    chs.reserve(checkers_size);
    for (unsigned i = 0; i < checkers_size * 2; ++i)
        chs.push_back(url_checker::init(ctx.get(), 
                    boost::bind(&checkers_manager::add_result, this, 
                    boost::placeholders::_1, 
                    boost::placeholders::_2, 
                    boost::placeholders::_3)));
}

void checkers_manager::wait_to_checkup()
{
    thread_gr.join_all();
}

void checkers_manager::add_result(url_checker* sender, std::string url, unsigned st)
{
    boost::lock_guard locker(result_mutex);
    debug::log("[+] " + url + " is " + to_string(st));
    result_col->add_result(url, st);
    if (!m_urls.empty() && !m_need_to_stop)
    {
        auto url = m_urls.back();
        m_urls.pop_back();
        ctx->post([url, sender](){
            sender->check(url);
        });
    }
    else
    {
        // if (data_col->rows_readed() == result_col->get_result_size())
        // {
        //     data_col->set_data(std::move(
        //         result_col->get_result()
        //     ));
        //     result_col->clear();

        //     set_data(std::move(data_col->get_data()));

        // }
        /*if (dc->rows_readed() == res_col->size())
            send update requests,
            select new part of data
                check if its not empty
            set data here
            and start
    */}
}

void checkers_manager::start()
{
    m_urls = std::move(data_col->get_data());

    while (m_urls.size() != 0)
    {
        for(url_checker::ptr ch : chs)
        {
            if (!m_urls.empty()) 
            { 
                auto url = m_urls.back();
                m_urls.pop_back();
                ch->check(url);
            }
        }

        for(unsigned int i = 0; i < checkers_size; ++i)
            thread_gr.create_thread(boost::bind(&checkers_manager::run_ctx, this));
        wait_to_checkup();

        data_col->set_data(std::move(result_col->get_result()));
        result_col->clear();

        if (m_need_to_stop)
        {
            m_need_to_stop = false;
            return;
        }    

        m_urls = std::move(data_col->get_data());
    }
}

void checkers_manager::stop()
{
    m_need_to_stop = true;
    cout << "CheckerManager: YEEEEEES!!!" << endl;
}

void checkers_manager::run_ctx()
{
    ctx->reset();
    ctx->run();
}

void checkers_manager::print() const
{
    result_col->print();
}

void checkers_manager::set_data(url_vector& urls)
{
    m_urls = urls;
}
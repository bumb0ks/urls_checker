#include "result_collector.h"

#include <iostream>

namespace container = boost::container;

using namespace std;

url_status_map result_collector::get_result() const 
{
    return res_map;
}

size_t result_collector::get_result_size() const
{
    return res_map.size();
}

void result_collector::clear()
{
    res_map.clear();
}

void result_collector::add_result(string url, unsigned res)
{
    res_map[url] = res;
}

void result_collector::print() const
{
    for (auto [url, status] : res_map)
    {
        std::cout << url << " is " << status << endl;
    }
}
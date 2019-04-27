#ifndef RESULT_COLLECTOR
#define RESULT_COLLECTOR

#include <boost/container/map.hpp>
#include <string>

#include "../traits.h"

class result_collector
{
public: 
    result_collector() = default;

    void add_result(std::string host, unsigned res);
    url_status_map get_result() const;
    size_t get_result_size() const;

    void clear();
    void print() const;

private:
    result_collector(const result_collector&) = delete;
    result_collector(result_collector&&) = delete;

    result_collector& operator=(const result_collector&) = delete;
    result_collector& operator=(result_collector&&) = delete;

private:
    url_status_map res_map;
};


#endif
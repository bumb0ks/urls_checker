#ifndef TRAITS
#define TRAITS

#include <boost/container/map.hpp>
#include <boost/container/vector.hpp>

#include <string>
#include <utility>

#define SERV_DEBUG true

namespace debug
{

    template <typename T>
    void log(const T& msg)
    {
        if (SERV_DEBUG)
            std::cout << msg << std::endl;
    }

}

typedef boost::container::map<std::string, unsigned> url_status_map;
typedef boost::container::vector<std::string> url_vector;

#endif
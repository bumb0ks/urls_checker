#ifndef DATA_COLLECTOR
#define DATA_COLLECTOR

#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <boost/scoped_ptr.hpp>
#include <string>

#include "../traits.h"

class data_collector
{
public:
    data_collector();

    url_vector get_data() const;
    void set_data(url_status_map&&);

    size_t rows_readed() const;

private:
    sql::Driver* driver;
    boost::scoped_ptr< sql::Connection > conn;

    mutable size_t current_offset = 0;
    mutable size_t rows_read = 0;
    size_t offset_size = 100;

    const sql::SQLString host  = "tcp://127.0.0.1";
    const sql::SQLString user  = "bumb0ks";
    const sql::SQLString pass  = "123456789";
    const sql::SQLString schema = "urls_db";
};

#endif
#include "data_collector.h"

#include <iostream>

using namespace std;

data_collector::data_collector()  :
    driver(sql::mysql::get_driver_instance())
{
    try
    {
    conn.reset(driver->connect(host,
                         user,
                         pass));
    conn->setSchema(schema);
    }
    catch(sql::SQLException &e)
    {
        debug::log("Error " + string(e.what()));
    }
    
}

size_t data_collector::rows_readed() const
{
    return rows_read;
}

void data_collector::set_data(url_status_map&& urls)
{
    boost::scoped_ptr< sql::Statement > stmt(conn->createStatement());

    for (const auto& [url, status] : urls)
    {
        string request(
            "UPDATE urls_table SET status = " + 
            to_string(status) + " WHERE url = '" + url + "';");
        debug::log(request);
        try
        {
            stmt->executeQuery(request.c_str());
        }
        catch(sql::SQLException &e)
        {
            debug::log("Error " + string(e.what()) + " : " + 
            to_string(e.getErrorCode()));
        }
    }
}

url_vector data_collector::get_data() const
{
    try
    {
        boost::scoped_ptr< sql::Statement > stmt(conn->createStatement());

        string request(
            "SELECT url AS _url FROM urls_table LIMIT " + 
            to_string(current_offset) + ", " + to_string(offset_size));

        boost::scoped_ptr< sql::ResultSet >
            res(stmt->executeQuery(request.c_str()));
        
        url_vector result;
        result.reserve(offset_size);

        rows_read = res->rowsCount();

        while(res->next())
        {
            result.push_back(res->getString("_url").c_str());
        }
        current_offset += offset_size;
        return result;
    }
    catch(sql::SQLException &e)
    {
        cout << "Error " << e.what() << endl;
        return url_vector();
    }
}
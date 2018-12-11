#pragma once

#include <string>
#include <boost/thread/mutex.hpp>

using namespace std;
class DataShare{
public:
    static DataShare* getInstance(){
        static DataShare* instance = new DataShare();
        return instance;
    }
    //client
    string client_data_package_;
    boost::mutex client_data_package_mtx_;

    string client_data_parse_;
    boost::mutex client_data_parse_mtx_;

    string client_control_command_;
    boost::mutex client_control_command_mtx_;

    //server
    string server_data_package_;
    boost::mutex server_data_package_mtx_;

    string server_data_parse_;
    boost::mutex server_data_parse_mtx_;

};
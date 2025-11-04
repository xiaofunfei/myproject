#ifndef __MYSQLCLIENT_H
#define __MYSQLCLIENT_H
#include <string>
#include <vector>
#include <mysql/mysql.h>
using std::string;
using std::vector;
namespace SOS{
class MySQLClient{
public:
    MySQLClient();
    ~MySQLClient();
    bool connect(const string&   host,
                 const string&   user,
                 const string &   pwd,
                 const string &    db,
                 unsigned short port);
    bool writeOperationQuery(const string &sql);
    vector<vector<string>> readOperationQuery(const string &sql);
    static void dump(const vector<vector<string>> & res);
private:
    void init();
private:
    MYSQL _conn;
};
}
#endif
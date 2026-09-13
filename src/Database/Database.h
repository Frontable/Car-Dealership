#pragma once

#include <postgresql/libpq-fe.h>
#include <string>

class Database
{
private:
    PGconn* m_connection;

public:
    Database();
    ~Database();

    PGconn* getConnection();
};
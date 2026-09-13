#include "Database.h"
#include <stdexcept>

Database::Database()
{
    m_connection = PQconnectdb(
        "host=localhost "
        "port=5432 "
        "dbname=cardealership "
        "user=postgres "
        "password=password"
    );

    if (PQstatus(m_connection) != CONNECTION_OK)
    {
        std::string error = PQerrorMessage(m_connection);

        PQfinish(m_connection);
        m_connection = nullptr;

        throw std::runtime_error(
            "Database connection failed: " + error
        );
    }
}

Database::~Database()
{
    if (m_connection != nullptr)
    {
        PQfinish(m_connection);
    }
}

PGconn* Database::getConnection()
{
    return m_connection;
}
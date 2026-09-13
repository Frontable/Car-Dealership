#include "UserRepository.h"
#include <iostream>
#include <algorithm>

UserRepository::UserRepository(Database& database)
:m_dataBase(database)
{
    
}

User UserRepository::add(const User& user)
{
    PGconn* connection = m_dataBase.getConnection();

    const char* params[] =
    {
        user.name.c_str(),
        user.email.c_str(),
        user.password.c_str(),
        user.role.c_str()
    };

    PGresult* result = PQexecParams(
        connection,
        "INSERT INTO users (name, email, password, role) "
        "VALUES ($1, $2, $3, $4) "
        "RETURNING id;",
        4,
        nullptr,
        params,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        std::string error = PQerrorMessage(connection);
        PQclear(result);

        throw std::runtime_error(
            "Failed to add user: " + error
        );
    }

    User newUser = user;

    newUser.id = std::stoi(
        PQgetvalue(result, 0, 0)
    );

    PQclear(result);

    return newUser;
}

bool UserRepository::update(int id, const User& updatedUser)
{
    PGconn* connection = m_dataBase.getConnection();

    std::string idString = std::to_string(id);

    const char* params[] =
    {
        updatedUser.name.c_str(),
        updatedUser.email.c_str(),
        updatedUser.password.c_str(),
        updatedUser.role.c_str(),
        idString.c_str()
    };

    PGresult* result = PQexecParams(
        connection,
        "UPDATE users "
        "SET name = $1, "
        "email = $2, "
        "password = $3, "
        "role = $4 "
        "WHERE id = $5;",
        5,
        nullptr,
        params,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        std::string error = PQerrorMessage(connection);
        PQclear(result);

        throw std::runtime_error(
            "Failed to update user: " + error
        );
    }

    int affectedRows = std::stoi(PQcmdTuples(result));

    PQclear(result);

    return affectedRows > 0;
}
    
bool UserRepository::remove(int id)
{
    PGconn* connection = m_dataBase.getConnection();

    std::string idString = std::to_string(id);

    const char* params[] =
    {
        idString.c_str()
    };

    PGresult* result = PQexecParams(
        connection,
        "DELETE FROM users "
        "WHERE id = $1;",
        1,
        nullptr,
        params,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        std::string error = PQerrorMessage(connection);
        PQclear(result);

        throw std::runtime_error(
            "Failed to remove user: " + error
        );
    }

    int affectedRows = std::stoi(PQcmdTuples(result));

    PQclear(result);

    return affectedRows > 0;
}

std::vector<User> UserRepository::getAll()
{
    PGconn* connection = m_dataBase.getConnection();

    PGresult* result = PQexecParams(
        connection,
        "SELECT id, name, email, password, role FROM users;",
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        std::string error = PQerrorMessage(connection);
        PQclear(result);

        throw std::runtime_error(
            "Failed to get users: " + error
        );
    }

    std::vector<User> users;

    int rowCount = PQntuples(result);

    for (int row = 0; row < rowCount; ++row)
    {
        User user;

        user.id = std::stoi(PQgetvalue(result, row, 0));
        user.name = PQgetvalue(result, row, 1);
        user.email = PQgetvalue(result, row, 2);
        user.password = PQgetvalue(result, row, 3);
        user.role = PQgetvalue(result, row, 4);

        users.push_back(user);
    }

    PQclear(result);

    return users;
}

std::optional<User> UserRepository::getById(int id)
{
    PGconn* connection = m_dataBase.getConnection();

    std::string idString = std::to_string(id);

    const char* params[] =
    {
        idString.c_str()
    };

    PGresult* result = PQexecParams(
        connection,
        "SELECT id, name, email, password, role "
        "FROM users "
        "WHERE id = $1;",
        1,
        nullptr,
        params,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        std::string error = PQerrorMessage(connection);
        PQclear(result);

        throw std::runtime_error(
            "Failed to get user: " + error
        );
    }

    if (PQntuples(result) == 0)
    {
        PQclear(result);
        return std::nullopt;
    }

    User user;

    user.id = std::stoi(PQgetvalue(result, 0, 0));
    user.name = PQgetvalue(result, 0, 1);
    user.email = PQgetvalue(result, 0, 2);
    user.password = PQgetvalue(result, 0, 3);
    user.role = PQgetvalue(result, 0, 4);

    PQclear(result);

    return user;
}

std::optional<User> UserRepository::getByEmail(const std::string& email)
{
    PGconn* connection = m_dataBase.getConnection();

    const char* params[] =
    {
        email.c_str()
    };

    PGresult* result = PQexecParams(
        connection,
        "SELECT id, name, email, password, role FROM users WHERE email = $1;",
        1,
        nullptr,
        params,
        nullptr,
        nullptr,
        0
    );

    if(PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        std::string error = PQerrorMessage(connection);
        PQclear(result);

        throw std::runtime_error("Failed to get user: " + error);
    }

    if(PQntuples(result) == 0)
    {
        PQclear(result);
        return std::nullopt;
    }

    User user;

    user.id = std::stoi(PQgetvalue(result, 0, 0));
    user.name = PQgetvalue(result, 0, 1);
    user.email = PQgetvalue(result, 0, 2);
    user.password = PQgetvalue(result, 0, 3);
    user.role = PQgetvalue(result, 0, 4);

    PQclear(result);

    return user;

}
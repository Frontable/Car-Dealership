#pragma once
#include <vector>
#include <optional>

#include "User.h"
#include "Database.h"

class UserRepository
{
    private:
    Database& m_dataBase;

    public:
    UserRepository(Database& database);

    User add(const User& user);

    bool update(int id, const User& updatedUser);
    
    bool remove(int id);

    std::vector<User> getAll();

    std::optional<User> getById(int it);

    std::optional<User> getByEmail(const std::string& email);

};
#pragma once
#include <vector>

#include "User.h"

class UserRepository
{
    private:
    std::vector<User> m_Users;
    int m_nextId;

    public:
    UserRepository();

    User add(const User& user);

    bool update(int id, const User& updatedUser);
    
    bool remove(int id);

    std::vector<User>& getAll();

    User* getById(int it);

    User* getByEmail(const std::string& email);

};
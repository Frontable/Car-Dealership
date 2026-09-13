#pragma once
#include "UserRepository.h"

#include <string>

class UserService
{
    private:
    UserRepository& repository;
    void validateUser(const User& user);

    public:

    UserService(UserRepository& repository);

    User add(const User& user);

    bool update(int id, const User& user);

    bool remove(int id);

    std::vector<User>& getAll();

    User* getById(int id);

    User* getByEmail(const std::string& email);

};
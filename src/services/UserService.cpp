#include "UserService.h"
#include <stdexcept>

UserService::UserService(UserRepository& repository)
:repository(repository)
{

}

User UserService::add(const User& user)
{
    validateUser(user);
    return repository.add(user);
}

bool UserService::update(int id, const User& user)
{
    validateUser(user);
    return repository.update(id, user);
}

bool UserService::remove(int id)
{
    return repository.remove(id);
}

std::vector<User>& UserService::getAll()
{
    return repository.getAll();
}

User* UserService::getById(int id)
{
    User* user = repository.getById(id);
    if(user == nullptr)
    {
        throw std::invalid_argument("User with this id doesn't exist");
    }

    return user;
}

User* UserService::getByEmail(const std::string& email)
{
    User* user = repository.getByEmail(email);
    if(user == nullptr)
    {
        throw std::invalid_argument("User with this email doesn't exist");
    }

    return user;
}

void UserService::validateUser(const User& user)
{
    if(user.name.empty())
    {
        throw std::invalid_argument("Name cannot be empty");
    }
    if(user.email.empty())
    {
        throw std::invalid_argument("Email cannot be empty");
    }
    if(user.password.empty())
    {
        throw std::invalid_argument("Password cannot be empty");
    }
    if(user.role.empty())
    {
        throw std::invalid_argument("Role cannot be empty");
    }
}
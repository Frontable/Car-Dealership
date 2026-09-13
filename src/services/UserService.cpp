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

std::vector<User> UserService::getAll()
{
    return repository.getAll();
}

std::optional<User> UserService::getById(int id)
{
    return repository.getById(id);
}

std::optional<User> UserService::getByEmail(const std::string& email)
{
    return repository.getByEmail(email);
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
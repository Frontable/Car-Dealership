#include "UserRepository.h"
#include <iostream>
#include <algorithm>

UserRepository::UserRepository()
{
    m_Users = {
        {1, "Joel", "Joel@CarDealership.com", "joel1234", "admin"},
        {2, "Simeon", "Simeon@CarDealership.com", "joel1234", "admin"},
        {3, "Jojo", "Jojo@CarDealership.com", "joel1234", "customer"},
    };
    m_nextId = 4;
    std::cout<<"users"<<std::endl;
}

User UserRepository::add(const User& user)
{
    User newUser = user;
    newUser.id = m_nextId++;

    m_Users.emplace_back(newUser);

    return newUser;

}

bool UserRepository::update(int id, const User& updatedUser)
{
    for(auto& user : m_Users)
    {
        if(user.id == id)
        {
            user.name = updatedUser.name;
            user.email = updatedUser.email;
            user.password = updatedUser.password;
            user.role= updatedUser.role;

            return true;
        }
    }
    return false;
}
    
bool UserRepository::remove(int id)
{
    auto iter = std::find_if(m_Users.begin(), m_Users.end(), [id](const User& user)
                { return user.id == id; });
    
    if(iter != m_Users.end())
    {
        m_Users.erase(iter);
        return true;
    }
    return false;
}

std::vector<User>& UserRepository::getAll()
{
    return m_Users;
}

User* UserRepository::getById(int id)
{
    for(User& user : m_Users)
    {
        if(user.id == id)
        {
            return &user;
        }
    }
    return nullptr;
}

User* UserRepository::getByEmail(const std::string& email)
{
    for(User& user : m_Users)
    {
        if(user.email == email)
        {
            return &user;
        } 
    }
    return nullptr;
}
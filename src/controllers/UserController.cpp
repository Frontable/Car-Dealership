#include "UserController.h"
#include <functional>

UserController::UserController(UserService& service)
:service(service)
{
}

void UserController::registerRoutes(Pistache::Rest::Router& router)
{
    Pistache::Rest::Routes::Post(
        router,
        "/users",
        std::bind(
            &UserController::add,
            this,
            std::placeholders::_1,
            std::placeholders::_2
        )
    );

    Pistache::Rest::Routes::Put(
        router,
        "/users/id/:id",
        std::bind(
            &UserController::update,
            this,
            std::placeholders::_1,
            std::placeholders::_2
        )
    );

    Pistache::Rest::Routes::Delete(
        router,
        "/users/id/:id",
        std::bind(
            &UserController::remove,
            this,
            std::placeholders::_1,
            std::placeholders::_2
        )
    );

    Pistache::Rest::Routes::Get(
        router,
        "/users",
        std::bind(
            &UserController::getAll,
            this,
            std::placeholders::_1,
            std::placeholders::_2
        )
    );

    Pistache::Rest::Routes::Get(
        router,
        "/users/id/:id",
        std::bind(
            &UserController::getById,
            this,
            std::placeholders::_1,
            std::placeholders::_2
        )
    );

    Pistache::Rest::Routes::Get(
        router,
        "/users/email/:email",
        std::bind(
            &UserController::getByEmail,
            this,
            std::placeholders::_1,
            std::placeholders::_2
        )
    );
}

pistacheResult UserController::add(const Pistache::Rest::Request& request,
        Pistache::Http::ResponseWriter response)
{
    try
    {
        std::string body = request.body();
        nlohmann::json j = nlohmann::json::parse(body);
        User user;
        user.name = j.at("name").get<std::string>();
        user.email = j.at("email").get<std::string>();
        user.password = j.at("password").get<std::string>();
        user.role = j.at("role").get<std::string>();

        User newUser = service.add(user);
        nlohmann::json newUserJson;
        userToJson(newUserJson, newUser);

        response.send(Pistache::Http::Code::Ok, newUserJson.dump(), MIME(Application, Json));
        return Pistache::Rest::Route::Result::Ok;     

    }
    catch(const std::invalid_argument& e)
    {
        nlohmann::json error = {{"error", e.what()}};
        response.send(Pistache::Http::Code::Bad_Request, error.dump(), MIME(Application, Json));
        return Pistache::Rest::Route::Result::Ok; 
    }
    
}

pistacheResult UserController::update(const Pistache::Rest::Request& request,
        Pistache::Http::ResponseWriter response)
{
    try
    {
        int id = std::stoi(request.param(":id").as<std::string>());

        std::string body = request.body();
        nlohmann::json j = nlohmann::json::parse(body);

        User updatedUser;
        updatedUser.id = id;
        updatedUser.name = j.at("name").get<std::string>();
        updatedUser.email = j.at("email").get<std::string>();
        updatedUser.password = j.at("password").get<std::string>();
        updatedUser.role = j.at("role").get<std::string>();

        service.update(id, updatedUser);

        nlohmann::json updatedUserJson;
        userToJson(updatedUserJson, updatedUser);

        response.send(Pistache::Http::Code::Ok, updatedUserJson.dump(), MIME(Application, Json));
        return Pistache::Rest::Route::Result::Ok;     

    }
    catch(const std::invalid_argument& e)
    {
        nlohmann::json error = {{"error", e.what()}};
        response.send(Pistache::Http::Code::Bad_Request, error.dump(), MIME(Application, Json));
        return Pistache::Rest::Route::Result::Ok; 
    }    
}

pistacheResult UserController::remove(const Pistache::Rest::Request& request,
        Pistache::Http::ResponseWriter response)
{
    try
    {
        int id = std::stoi(request.param(":id").as<std::string>());
        User* user = service.getById(id);
        User removedUser;
        removedUser.id = id;
        removedUser.name = user->name;
        removedUser.email = user->email;
        removedUser.password = "";
        removedUser.role = user->role;

        service.remove(id);

        nlohmann::json removedUserJson;
        userToJson(removedUserJson, removedUser);

        response.send(Pistache::Http::Code::Ok, removedUserJson.dump(), MIME(Application, Json));
        return Pistache::Rest::Route::Result::Ok;     

    }
    catch(const std::invalid_argument& e)
    {
        nlohmann::json error = {{"error", e.what()}};
        response.send(Pistache::Http::Code::Bad_Request, error.dump(), MIME(Application, Json));
        return Pistache::Rest::Route::Result::Ok; 
    }    
}

pistacheResult UserController::getAll(const Pistache::Rest::Request& request,
        Pistache::Http::ResponseWriter response)
{
    try
    {
        std::vector<User>& users = service.getAll();
        nlohmann::json j;

        for(auto& user : users)
        {
            userToJson(j, user);
        };

        response.send(Pistache::Http::Code::Ok, j.dump(), MIME(Application, Json));
        return Pistache::Rest::Route::Result::Ok;     

    }
    catch(const std::exception& e)
    {
        nlohmann::json error = {{"error", e.what()}};
        response.send(Pistache::Http::Code::Bad_Request, error.dump(), MIME(Application, Json));
        return Pistache::Rest::Route::Result::Ok; 
    }
    
}

pistacheResult UserController::getById(const Pistache::Rest::Request& request,
        Pistache::Http::ResponseWriter response)
{
    try
    {
        int id = std::stoi(request.param(":id").as<std::string>());

        User* user = service.getById(id);

        nlohmann::json j;
        userToJson(j, *user);

        response.send(Pistache::Http::Code::Ok, j.dump(), MIME(Application, Json));
        return Pistache::Rest::Route::Result::Ok; 
    }
    catch(const std::invalid_argument& e)
    {
        nlohmann::json error = {{"error", e.what()}};
        response.send(Pistache::Http::Code::Not_Found, error.dump(), MIME(Application, Json));
        return Pistache::Rest::Route::Result::Ok; 
    }
    
}

pistacheResult UserController::getByEmail(const Pistache::Rest::Request& request,
        Pistache::Http::ResponseWriter response)
{
    try
    {
        std::string email = request.param(":email").as<std::string>();

        User* user = service.getByEmail(email);

        nlohmann::json j;
        userToJson(j, *user);

        response.send(Pistache::Http::Code::Ok, j.dump(), MIME(Application, Json));
        return Pistache::Rest::Route::Result::Ok; 
    }
    catch(const std::invalid_argument& e)
    {
        nlohmann::json error = {{"error", e.what()}};
        response.send(Pistache::Http::Code::Not_Found, error.dump(), MIME(Application, Json));
        return Pistache::Rest::Route::Result::Ok; 
    }
}

void UserController::userToJson(nlohmann::json& j, const User& user)
{
    j.push_back({{"id", user.id}, {"name", user.name}, 
        {"email", user.email}, {"role", user.role}});
}

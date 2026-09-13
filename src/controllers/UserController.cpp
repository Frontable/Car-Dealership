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

pistacheResult UserController::add(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response)
{
    try
    {
        auto body = nlohmann::json::parse(request.body());

        User user;

        user.name = body.at("name");
        user.email = body.at("email");
        user.password = body.at("password");
        user.role = body.at("role");

        User newUser = service.add(user);

        nlohmann::json j;
        userToJson(j, newUser);

        response.send(
            Pistache::Http::Code::Created,
            j.dump(),
            MIME(Application, Json)
        );

        return Pistache::Rest::Route::Result::Ok;
    }
    catch (const std::invalid_argument& e)
    {
        nlohmann::json error = {
            {"error", e.what()}
        };

        response.send(
            Pistache::Http::Code::Bad_Request,
            error.dump(),
            MIME(Application, Json)
        );

        return Pistache::Rest::Route::Result::Ok;
    }
    catch (const std::runtime_error& e)
    {
        nlohmann::json error = {
            {"error", e.what()}
        };

        response.send(
            Pistache::Http::Code::Internal_Server_Error,
            error.dump(),
            MIME(Application, Json)
        );

        return Pistache::Rest::Route::Result::Ok;
    }
}

pistacheResult UserController::update(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response)
{
    try
    {
        int id = std::stoi(request.param(":id").as<std::string>());

        auto body = nlohmann::json::parse(request.body());

        User user;

        user.name = body.at("name");
        user.email = body.at("email");
        user.password = body.at("password");
        user.role = body.at("role");

        bool updated = service.update(id, user);

        if (!updated)
        {
            nlohmann::json error = {
                {"error", "User with this id doesn't exist"}
            };

            response.send(
                Pistache::Http::Code::Not_Found,
                error.dump(),
                MIME(Application, Json)
            );

            return Pistache::Rest::Route::Result::Ok;
        }

        std::optional<User> updatedUser = service.getById(id);

        nlohmann::json j;
        userToJson(j, *updatedUser);

        response.send(
            Pistache::Http::Code::Ok,
            j.dump(),
            MIME(Application, Json)
        );

        return Pistache::Rest::Route::Result::Ok;
    }
    catch (const std::invalid_argument& e)
    {
        nlohmann::json error = {
            {"error", "Invalid user id"}
        };

        response.send(
            Pistache::Http::Code::Bad_Request,
            error.dump(),
            MIME(Application, Json)
        );

        return Pistache::Rest::Route::Result::Ok;
    }
    catch (const std::runtime_error& e)
    {
        nlohmann::json error = {
            {"error", e.what()}
        };

        response.send(
            Pistache::Http::Code::Internal_Server_Error,
            error.dump(),
            MIME(Application, Json)
        );

        return Pistache::Rest::Route::Result::Ok;
    }
}

pistacheResult UserController::remove(const Pistache::Rest::Request& request,
        Pistache::Http::ResponseWriter response)
{
    try
    {
        int id = std::stoi(request.param(":id").as<std::string>());
        std::optional<User> user = service.getById(id);
        User removedUser;
        removedUser.id = id;
        removedUser.name = user->name;
        removedUser.email = user->email;
        removedUser.password = "";
        removedUser.role = user->role;

        bool remove = service.remove(id);
        if (!remove)
        {
            nlohmann::json error = {
                {"error", "User with this id doesn't exist"}
            };

            response.send(
                Pistache::Http::Code::Not_Found,
                error.dump(),
                MIME(Application, Json)
            );

            return Pistache::Rest::Route::Result::Ok;
        }

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
        std::vector<User> users = service.getAll();
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

pistacheResult UserController::getById(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response)
{
    try
    {
        int id = std::stoi(request.param(":id").as<std::string>());

        std::optional<User> user = service.getById(id);

        if (!user.has_value())
        {
            nlohmann::json error = {
                {"error", "User with this id doesn't exist"}
            };

            response.send(
                Pistache::Http::Code::Not_Found,
                error.dump(),
                MIME(Application, Json)
            );

            return Pistache::Rest::Route::Result::Ok;
        }

        nlohmann::json j;
        userToJson(j, *user);

        response.send(
            Pistache::Http::Code::Ok,
            j.dump(),
            MIME(Application, Json)
        );

        return Pistache::Rest::Route::Result::Ok;
    }
    catch (const std::invalid_argument& e)
    {
        nlohmann::json error = {
            {"error", e.what()}
        };

        response.send(
            Pistache::Http::Code::Not_Found,
            error.dump(),
            MIME(Application, Json)
        );

        return Pistache::Rest::Route::Result::Ok;
    }
    catch (const std::runtime_error& e)
    {
        nlohmann::json error = {
            {"error", e.what()}
        };

        response.send(
            Pistache::Http::Code::Internal_Server_Error,
            error.dump(),
            MIME(Application, Json)
        );

        return Pistache::Rest::Route::Result::Ok;
    }
}

pistacheResult UserController::getByEmail(const Pistache::Rest::Request& request,
        Pistache::Http::ResponseWriter response)
{
    try
    {
        std::string email = request.param(":email").as<std::string>();

        std::optional user = service.getByEmail(email);

        if(!user.has_value())
        {
            nlohmann::json error = {
                {"error", "User with this id doesn't exist"}
            };

            response.send(
                Pistache::Http::Code::Not_Found,
                error.dump(),
                MIME(Application, Json)
            );

            return Pistache::Rest::Route::Result::Ok;
        }

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
    catch (const std::runtime_error& e)
    {
        nlohmann::json error = {
            {"error", e.what()}
        };

        response.send(
            Pistache::Http::Code::Internal_Server_Error,
            error.dump(),
            MIME(Application, Json)
        );

        return Pistache::Rest::Route::Result::Ok;
    }
}

void UserController::userToJson(nlohmann::json& j, const User& user)
{
    j.push_back({{"id", user.id}, {"name", user.name}, 
        {"email", user.email}, {"role", user.role}});
}

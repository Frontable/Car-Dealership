#pragma once
#include <string>
#include "UserService.h"

#include <pistache/router.h>
#include <nlohmann/json.hpp>

using pistacheResult = Pistache::Rest::Route::Result;

class UserController
{
    private:
    UserService& service;
    void userToJson(nlohmann::json& j, const User& user);
    public:
    
    UserController(UserService& service);

    void registerRoutes(Pistache::Rest::Router& router);
    
    pistacheResult add(const Pistache::Rest::Request& request,
        Pistache::Http::ResponseWriter response);

    pistacheResult update(const Pistache::Rest::Request& request,
        Pistache::Http::ResponseWriter response);

    pistacheResult remove(const Pistache::Rest::Request& request,
        Pistache::Http::ResponseWriter response);

    pistacheResult getAll(const Pistache::Rest::Request& request,
        Pistache::Http::ResponseWriter response);

    pistacheResult getById(const Pistache::Rest::Request& request,
        Pistache::Http::ResponseWriter response);

    pistacheResult getByEmail(const Pistache::Rest::Request& request,
        Pistache::Http::ResponseWriter response);

};

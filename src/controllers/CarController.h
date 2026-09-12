#pragma once

#include <pistache/router.h>
#include <nlohmann/json.hpp>

#include "CarService.h"

using pistacheResult = Pistache::Rest::Route::Result;

class CarController
{
private:
    CarService& service;
    

public:
    CarController(CarService& service);

    void registerRoutes(Pistache::Rest::Router& router);

    pistacheResult add(const Pistache::Rest::Request& request,
                Pistache::Http::ResponseWriter response);

    pistacheResult getAll(const Pistache::Rest::Request& request,
                Pistache::Http::ResponseWriter response);
    
    pistacheResult getById(const Pistache::Rest::Request& request,
                Pistache::Http::ResponseWriter response);
    
    pistacheResult update(const Pistache::Rest::Request& request,
                Pistache::Http::ResponseWriter response);

    pistacheResult remove(const Pistache::Rest::Request& request,
                Pistache::Http::ResponseWriter response);

    private:
    inline void carToJson(nlohmann::json& j, const Car& car)
    {
        j.push_back({{"id", car.id}, {"brand", car.brand}, {"model", car.model},
        {"year", car.year}, {"price", car.price}});
    }

    inline void bodyToJson(nlohmann::json& j, 
                const Pistache::Rest::Request& request)
    {

    }

};
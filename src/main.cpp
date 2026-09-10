#include <iostream>
#include <vector>

#include <pistache/endpoint.h>
#include <pistache/router.h>

#include <nlohmann/json.hpp>

#include <models/Car.h>

using namespace Pistache;

int main()
{
    std::vector<Car> cars = {
            {1, "BMW", "320d", 2017, 22000.0},
            {2, "Audi", "A4", 2020, 28000.0},
            {3, "Mercedes", "C220", 2019, 30000.0}
        };

    Http::Endpoint endpoint(Address(Ipv4::any(), Port(9080)));

    Rest::Router router;

    Rest::Routes::Get(router, "/cars",
        [&cars](const Rest::Request& request, Http::ResponseWriter response)
        {
            nlohmann::json json = nlohmann::json::array();
            for(const Car& car : cars)
            {
                json.push_back({
                    {"id", car.id},
                    {"brand", car.brand},
                    {"model", car.model},
                    {"year", car.year},
                    {"price", car.price}
                });
            }
            response.send(Http::Code::Ok, json.dump(), MIME(Application, Json));
            return Rest::Route::Result::Ok;
        }
    );

    endpoint.init(Http::Endpoint::options().threads(1));

    endpoint.setHandler(router.handler());
    std::cout<<"Running";
    endpoint.serve();

    
}

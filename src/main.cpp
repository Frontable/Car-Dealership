#include <iostream>
#include <vector>

#include <pistache/endpoint.h>
#include <pistache/router.h>

#include <nlohmann/json.hpp>

#include "CarRepository.h"
#include "CarService.h"
#include "CarController.h"

using namespace Pistache;

int main()
{
    Http::Endpoint endpoint(Address(Ipv4::any(), Port(9080)));
    Rest::Router router;

    CarRepository carRepository;
    CarService carService(carRepository);
    CarController carController(carService);
    carController.registerRoutes(router);


    endpoint.init(Http::Endpoint::options().threads(1));

    std::cout<<"Running";
    endpoint.setHandler(router.handler());
    std::cout<<"Running";
    endpoint.serve();

    
}

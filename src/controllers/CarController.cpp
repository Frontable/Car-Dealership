#include "CarController.h"
#include <string>


CarController::CarController(CarService& service)
    : service(service)
{
}

void CarController::registerRoutes(Pistache::Rest::Router& router)
{
    Pistache::Rest::Routes::Post(
        router,
        "/cars",
        std::bind(
            &CarController::add,
            this,
            std::placeholders::_1,
            std::placeholders::_2
        )
    );

    Pistache::Rest::Routes::Get(
        router,
        "/cars",
        std::bind(
            &CarController::getAll,
            this,
            std::placeholders::_1,
            std::placeholders::_2
        )
    );

    Pistache::Rest::Routes::Get(
        router,
        "/cars/:id",
        std::bind(
            &CarController::getById,
            this,
            std::placeholders::_1,
            std::placeholders::_2
        )
    );

    Pistache::Rest::Routes::Put(
        router,
        "/cars/:id",
        std::bind(
            &CarController::update,
            this,
            std::placeholders::_1,
            std::placeholders::_2
        )
    );

    Pistache::Rest::Routes::Delete(
        router,
        "/cars/:id",
        std::bind(
            &CarController::remove,
            this,
            std::placeholders::_1,
            std::placeholders::_2
        )
    );
}

pistacheResult CarController::add(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response)
{
    std::string body = request.body();

    nlohmann::json b = nlohmann::json::parse(body);

    Car newCar;

    newCar.brand = b.at("brand").get<std::string>();
    newCar.model = b.at("model").get<std::string>();
    newCar.year = b.at("year").get<int>();
    newCar.price = b.at("price").get<double>();

    Car createdCar = service.add(newCar);

    nlohmann::json json;
    carToJson(json, createdCar);

    response.send(
        Pistache::Http::Code::Created,
        json.dump(),
        MIME(Application, Json)
    );

    return Pistache::Rest::Route::Result::Ok;
}

pistacheResult CarController::getAll(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response)
{
    std::vector<Car>& cars = service.getAllCars();

    nlohmann::json json = nlohmann::json::array();

    for (const Car& car : cars)
    {
        carToJson(json, car);
    }

    response.send(
        Pistache::Http::Code::Ok,
        json.dump(),
        MIME(Application, Json)
    );

    return Pistache::Rest::Route::Result::Ok;
}



pistacheResult CarController::getById(const Pistache::Rest::Request& request,
                Pistache::Http::ResponseWriter response)
{
    int id = std::stoi(request.param(":id").as<std::string>());
    Car* Car = service.getById(id);

    if(Car == nullptr)
    {
        nlohmann::json error = {{"error", "car not found!"}};
        response.send(Pistache::Http::Code::Not_Found, error.dump(), 
                MIME(Application, Json));
        return Pistache::Rest::Route::Result::Ok;
    }

    nlohmann::json car; carToJson(car, *Car);

    response.send(Pistache::Http::Code::Ok, car.dump(), MIME(Application,Json));

    return Pistache::Rest::Route::Result::Ok;

}

pistacheResult CarController::update(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response)
{
    int id = std::stoi(
        request.param(":id").as<std::string>()
    );

    Car* car = service.getById(id);

    if (car == nullptr)
    {
        nlohmann::json error = {
            {"error", "car not found!"}
        };

        response.send(
            Pistache::Http::Code::Not_Found,
            error.dump(),
            MIME(Application, Json)
        );

        return Pistache::Rest::Route::Result::Ok;
    }

    std::string body = request.body();

    nlohmann::json b = nlohmann::json::parse(body);

    Car updatedCar;

    updatedCar.id = id;
    updatedCar.brand = b.at("brand").get<std::string>();
    updatedCar.model = b.at("model").get<std::string>();
    updatedCar.year = b.at("year").get<int>();
    updatedCar.price = b.at("price").get<double>();

    service.update(id, updatedCar);

    nlohmann::json newCar;

    carToJson(newCar, updatedCar);

    response.send(
        Pistache::Http::Code::Ok,
        newCar.dump(),
        MIME(Application, Json)
    );

    return Pistache::Rest::Route::Result::Ok;
}

pistacheResult CarController::remove(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response)
{
    int id = std::stoi(request.param(":id").as<std::string>());

    Car* car = service.getById(id);

    if (car == nullptr)
    {
        nlohmann::json error = {
            {"error", "car not found!"}
        };

        response.send(
            Pistache::Http::Code::Not_Found,
            error.dump(),
            MIME(Application, Json)
        );

        return Pistache::Rest::Route::Result::Ok;
    }

    Car deletedCar = *car;

    service.remove(id);

    nlohmann::json json;
    carToJson(json, deletedCar);

    response.send(Pistache::Http::Code::Ok,json.dump(),MIME(Application, Json));

    return Pistache::Rest::Route::Result::Ok;
}



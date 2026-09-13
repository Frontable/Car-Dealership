#include "CarService.h"

#include <stdexcept>

CarService::CarService(CarRepository& repository)
:repository(repository)
{
    
}

std::vector<Car>& CarService::getAllCars()
{
    return repository.getCars();
}

Car* CarService::getById(int id)
{
    return repository.getById(id);
}


Car CarService::add(const Car& car)
{
    validateCar(car);

    return repository.add(car);
}

bool CarService::remove(int id)
{
    return repository.remove(id);
}

bool CarService::update(int id, const Car& updatedCar)
{
    validateCar(updatedCar);
    return repository.update(id, updatedCar);
}

void CarService::validateCar(const Car& car)
{
    if (car.brand.empty())
    {
        throw std::invalid_argument("Brand cannot be empty");
    }

    if (car.model.empty())
    {
        throw std::invalid_argument("Model cannot be empty");
    }

    if (car.year < 1900 || car.year > 2026)
    {
        throw std::invalid_argument("Invalid year");
    }

    if (car.price <= 0)
    {
        throw std::invalid_argument("Price must be greater than 0");
    }
}
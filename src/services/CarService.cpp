#include "CarService.h"

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
    return repository.add(car);
}

bool CarService::remove(int id)
{
    return repository.remove(id);
}

bool CarService::update(int id, const Car& updatedCar)
{
    return repository.update(id, updatedCar);
}
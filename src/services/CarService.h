#pragma once
#include "Car.h"
#include "CarRepository.h"

#include <vector>

class CarService
{
    private:
    CarRepository& repository;

    public:
    CarService(CarRepository& repository);

    std::vector<Car>& getAllCars();

    Car* getById(int id);

    Car add(const Car& car);

    bool remove(int id);

    bool update(int id, const Car& updatedCar);
};
#pragma once
#include <vector>
#include "Car.h"
class CarRepository
{
    private:
    int nextId;
    std::vector<Car> m_Cars;

    public:
    CarRepository();
    std::vector<Car>& getCars();

    Car* getById(int id);

    Car add(const Car& car);

    bool remove(int id);

    bool update(int id, const Car& updatedCar);

};

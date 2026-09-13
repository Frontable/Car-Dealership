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
    
    Car add(const Car& car);
    
    std::vector<Car>& getCars();
    
    Car* getById(int id);
    
    bool remove(int id);
    
    bool update(int id, const Car& updatedCar);
    
};
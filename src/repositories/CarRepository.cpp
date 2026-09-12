#include "CarRepository.h"
#include <algorithm>

CarRepository::CarRepository()
: nextId(4)
{
    m_Cars =
    {
        {1, "BMW", "320d", 2017, 22000.0},
        {2, "Audi", "A4", 2020, 28000.0},
        {3, "Mercedes", "C220", 2019, 30000.0}
    };
}

std::vector<Car>& CarRepository::getCars()
{
    return m_Cars;
}

Car* CarRepository::getById(int id)
{
    for(Car& car : m_Cars)
    {
        if(car.id == id)
        {
            return &car;
        }
    }
    return nullptr;
}

Car CarRepository::add(const Car& car)
{
    Car newCar = car;

    newCar.id = nextId++;

    m_Cars.push_back(newCar);

    return newCar;
}

bool CarRepository::remove(int id)
{
     auto it = std::find_if(m_Cars.begin(), m_Cars.end(),[id](const Car& car)
    {
        return car.id == id;
    });

    if(it != m_Cars.end())
    {
        m_Cars.erase(it);
        return true;
    }
    return false;
}

bool CarRepository::update(int id, const Car& updatedCar)
{
    Car* car = getById(id);

    if (car == nullptr)
    {
        return false;
    }

    *car = updatedCar;

    return true;
}
// car.h
#ifndef CAR_H
#define CAR_H

#include <QString> // Use QString for better Qt integration

class Car {
public:
    int id;
    QString name;
    QString model;
    int rent_per_day;
    int quantity;

    // Default constructor
    Car() : id(-1), rent_per_day(0), quantity(0) {}
};

#endif // CAR_H

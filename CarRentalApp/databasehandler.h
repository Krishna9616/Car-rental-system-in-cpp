// databasehandler.h
#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QString>
#include <QVector>
#include "sqlite3.h"
#include "car.h" // Assuming your Car class is in car.h
#include "customer.h" // Assuming your Customer class is in customer.h

// Forward declaration for BookingInfo if you have it
struct BookingInfo {
    QString carName;
    QString carModel;
    int days;
    int totalRent;
    QString bookingDate;
    int bookingId;
};


class DatabaseHandler {
public:
    DatabaseHandler(const QString& dbPath);
    ~DatabaseHandler();

    bool isOpen() const;

    QVector<Car> getAvailableCars();
    Car getCarById(int carId);
    bool storeBooking(const Customer& customer, int carId, int days, int totalRent);
    bool updateCarQuantity(int carId, int changeInQuantity); // e.g., -1 when booking
    QVector<BookingInfo> getBookingsByMobile(const QString& mobileNumber);

private:
    sqlite3* db;
    QString lastError;

    // You might move the static callback here or adapt it
    // static int carListCallback(void* data, int argc, char** argv, char** azColName);
};

#endif // DATABASEHANDLER_H

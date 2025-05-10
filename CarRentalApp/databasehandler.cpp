// databasehandler.cpp
#include "databasehandler.h"
#include <QDebug> // For error messages

// Implementation of Car and Customer classes (or include their .cpp if separate)
// You might need to adapt them to use QString instead of std::string, or provide converters.

// Example Car class modification for Qt
// car.h
// #include <string>
// class Car { public: std::string name; ... };
// becomes:
// #include <QString>
// class Car { public: int id; QString name; QString model; int rent_per_day; int quantity; };


// DatabaseHandler::DatabaseHandler(const QString& dbPath) : db(nullptr) {
//     if (sqlite3_open(dbPath.toUtf8().constData(), &db) != SQLITE_OK) {
//         lastError = sqlite3_errmsg(db);
//         qWarning() << "Can't open database:" << lastError;
//         sqlite3_close(db); // Close if partially opened
//         db = nullptr;
//     } else {
//         // Initialize tables if they don't exist (simplified)
//         const char* createCarsTableSQL =
//             "CREATE TABLE IF NOT EXISTS Cars ("
//             "id INTEGER PRIMARY KEY AUTOINCREMENT,"
//             "name TEXT NOT NULL,"
//             "model TEXT NOT NULL,"
//             "rent_per_day INTEGER NOT NULL,"
//             "quantity INTEGER NOT NULL CHECK(quantity >= 0)"
//             ");";
//         sqlite3_exec(db, createCarsTableSQL, 0, 0, 0);
//         // ... create Bookings table ...
//         const char* createBookingsTableSQL =
//             "CREATE TABLE IF NOT EXISTS Bookings ("
//             "booking_id INTEGER PRIMARY KEY AUTOINCREMENT,"
//             "customer_name TEXT NOT NULL,"
//             "mobile_number TEXT NOT NULL,"
//             "car_id INTEGER NOT NULL,"
//             "number_of_days INTEGER NOT NULL,"
//             "total_rent INTEGER NOT NULL,"
//             "booking_date DATETIME DEFAULT CURRENT_TIMESTAMP,"
//             "FOREIGN KEY (car_id) REFERENCES Cars(id)"
//             ");";
//         sqlite3_exec(db, createBookingsTableSQL, 0, 0, 0);
//     }
// }

DatabaseHandler::DatabaseHandler(const QString& dbPath) : db(nullptr) {
    qDebug() << "DatabaseHandler: Constructor called with path:" << dbPath; // ADD THIS
    int rc = sqlite3_open(dbPath.toUtf8().constData(), &db);
    if (rc != SQLITE_OK) {
        lastError = sqlite3_errmsg(db);
        qWarning() << "DatabaseHandler: Can't open database:" << dbPath << "Error:" << lastError;
        sqlite3_close(db); // Close if partially opened
        db = nullptr;
    } else {
        qDebug() << "DatabaseHandler: Database opened successfully:" << dbPath; // ADD THIS
        // ... (your table creation logic) ...
    }
}

DatabaseHandler::~DatabaseHandler() {
    if (db) {
        sqlite3_close(db);
    }
}

bool DatabaseHandler::isOpen() const {
    return db != nullptr;
}

// QVector<Car> DatabaseHandler::getAvailableCars() {
//     QVector<Car> cars;
//     if (!db) return cars;

//     sqlite3_stmt* stmt;
//     const char* sql = "SELECT id, name, model, rent_per_day, quantity FROM Cars WHERE quantity > 0;";
//     if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
//         while (sqlite3_step(stmt) == SQLITE_ROW) {
//             Car car;
//             car.id = sqlite3_column_int(stmt, 0);
//             car.name = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
//             car.model = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
//             car.rent_per_day = sqlite3_column_int(stmt, 3);
//             car.quantity = sqlite3_column_int(stmt, 4);
//             cars.append(car);
//         }
//         sqlite3_finalize(stmt);
//     } else {
//         lastError = sqlite3_errmsg(db);
//         qWarning() << "Failed to get available cars:" << lastError;
//     }
//     return cars;
// }


QVector<Car> DatabaseHandler::getAvailableCars() {
    qDebug() << "DatabaseHandler: getAvailableCars() called."; // ADD THIS
    QVector<Car> cars;
    if (!db) {
        qWarning() << "DatabaseHandler: getAvailableCars() - DB is null!"; // ADD THIS
        return cars;
    }

    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, name, model, rent_per_day, quantity FROM Cars WHERE quantity > 0;";
    qDebug() << "DatabaseHandler: Preparing SQL:" << sql; // ADD THIS

    int rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc_prepare == SQLITE_OK) {
        qDebug() << "DatabaseHandler: SQL prepared successfully."; // ADD THIS
        int step_count = 0; // ADD THIS
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            step_count++; // ADD THIS
            qDebug() << "DatabaseHandler: Found row " << step_count; // ADD THIS
            Car car;
            car.id = sqlite3_column_int(stmt, 0);
            // ... (rest of column reading) ...
            car.name = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            car.model = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            car.rent_per_day = sqlite3_column_int(stmt, 3);
            car.quantity = sqlite3_column_int(stmt, 4);
            qDebug() << "DatabaseHandler: Car data read - ID:" << car.id << "Name:" << car.name << "Qty:" << car.quantity; // ADD THIS
            cars.append(car);
        }
        if (step_count == 0) { // ADD THIS
            qDebug() << "DatabaseHandler: No rows returned by sqlite3_step.";
        }
        sqlite3_finalize(stmt);
    } else {
        lastError = sqlite3_errmsg(db);
        qWarning() << "DatabaseHandler: Failed to prepare SQL for getAvailableCars. RC:" << rc_prepare << "Error:" << lastError; // MODIFIED THIS
    }
    qDebug() << "DatabaseHandler: getAvailableCars() returning" << cars.size() << "cars."; // ADD THIS
    return cars;
}

Car DatabaseHandler::getCarById(int carId) {
    Car car;
    car.id = -1; // Indicates not found
    if (!db) return car;

    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, name, model, rent_per_day, quantity FROM Cars WHERE id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, carId);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            car.id = sqlite3_column_int(stmt, 0);
            car.name = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            car.model = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            car.rent_per_day = sqlite3_column_int(stmt, 3);
            car.quantity = sqlite3_column_int(stmt, 4);
        }
        sqlite3_finalize(stmt);
    } else {
        lastError = sqlite3_errmsg(db);
        qWarning() << "Failed to get car by ID:" << lastError;
    }
    return car;
}

bool DatabaseHandler::storeBooking(const Customer& customer, int carId, int days, int totalRent) {
    if (!db) return false;
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO Bookings (customer_name, car_id, mobile_number, number_of_days, total_rent) VALUES (?, ?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, customer.name.toUtf8().constData(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, carId);
        sqlite3_bind_text(stmt, 3, customer.mobile_number.toUtf8().constData(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, days);
        sqlite3_bind_int(stmt, 5, totalRent);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return true;
        } else {
            lastError = sqlite3_errmsg(db);
            qWarning() << "Failed to store booking:" << lastError;
        }
        sqlite3_finalize(stmt);
    } else {
        lastError = sqlite3_errmsg(db);
        qWarning() << "Failed to prepare storeBooking statement:" << lastError;
    }
    return false;
}

bool DatabaseHandler::updateCarQuantity(int carId, int changeInQuantity) {
    if (!db) return false;
    sqlite3_stmt* stmt;
    // Ensure quantity doesn't go below zero
    const char* sql = "UPDATE Cars SET quantity = quantity + ? WHERE id = ? AND quantity + ? >= 0;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, changeInQuantity);
        sqlite3_bind_int(stmt, 2, carId);
        sqlite3_bind_int(stmt, 3, changeInQuantity); // For the check

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            if (sqlite3_changes(db) > 0) { // Check if a row was actually updated
                sqlite3_finalize(stmt);
                return true;
            } else {
                lastError = "Car not found or quantity update would make it negative.";
                qWarning() << "Failed to update car quantity:" << lastError;
            }
        } else {
            lastError = sqlite3_errmsg(db);
            qWarning() << "Failed to execute updateCarQuantity:" << lastError;
        }
        sqlite3_finalize(stmt);
    } else {
        lastError = sqlite3_errmsg(db);
        qWarning() << "Failed to prepare updateCarQuantity statement:" << lastError;
    }
    return false;
}

QVector<BookingInfo> DatabaseHandler::getBookingsByMobile(const QString& mobileNumber) {
    QVector<BookingInfo> bookings;
    if (!db) return bookings;

    sqlite3_stmt* stmt;
    const char* sql = "SELECT C.name, C.model, B.number_of_days, B.total_rent, B.booking_date, B.booking_id "
                      "FROM Bookings B JOIN Cars C ON B.car_id = C.id "
                      "WHERE B.mobile_number = ? ORDER BY B.booking_date DESC;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, mobileNumber.toUtf8().constData(), -1, SQLITE_STATIC);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            BookingInfo info;
            info.carName = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
            info.carModel = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            info.days = sqlite3_column_int(stmt, 2);
            info.totalRent = sqlite3_column_int(stmt, 3);
            info.bookingDate = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
            info.bookingId = sqlite3_column_int(stmt, 5);
            bookings.append(info);
        }
        sqlite3_finalize(stmt);
    } else {
        lastError = sqlite3_errmsg(db);
        qWarning() << "Failed to get bookings by mobile:" << lastError;
    }
    return bookings;
}

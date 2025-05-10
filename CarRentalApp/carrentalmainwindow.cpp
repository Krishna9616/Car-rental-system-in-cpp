// carrentalmainwindow.cpp
#include "carrentalmainwindow.h"
#include <QApplication>         // For QApplication::applicationDirPath()
#include <QDir>                 // For QDir::toNativeSeparators()
#include <QDebug>               // For qDebug()
#include <QDateTime>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout> // Good for label-field pairs
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QSpinBox>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>      // For displaying bookings in a new window



CarRentalMainWindow::CarRentalMainWindow(QWidget *parent)
    : QMainWindow(parent),
    dbHandler(QApplication::applicationDirPath() + "/rental.db")
{
    // ADD THIS qDEBUG LINE IMMEDIATELY
    qDebug() << "DB Path Attempt:" << QDir::toNativeSeparators(QApplication::applicationDirPath() + "/rental.db");

    if (!dbHandler.isOpen()) {
        qWarning() << "DatabaseHandler reported DB is NOT open in MainWindow constructor."; // ADD THIS
        QMessageBox::critical(this, "Database Error", "Could not open or initialize the database. The application will now close.");
        // Potentially disable UI or close app here
    } else {
        qDebug() << "DatabaseHandler reported DB IS open in MainWindow constructor."; // ADD THIS
    }
    setupUi();
    loadAvailableCars(); // Load cars on startup
}

CarRentalMainWindow::~CarRentalMainWindow() {
    // dbHandler's destructor will close the database
}

void CarRentalMainWindow::setupUi() {
    setWindowTitle("Car Rental System");

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // --- Available Cars Section ---
    QLabel *carsLabel = new QLabel("Available Cars:");
    mainLayout->addWidget(carsLabel);
    carsTableWidget = new QTableWidget();
    carsTableWidget->setColumnCount(5);
    carsTableWidget->setHorizontalHeaderLabels({"ID", "Name", "Model", "Rent/Day", "Quantity"});
    carsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    carsTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    carsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // Read-only
    carsTableWidget->horizontalHeader()->setStretchLastSection(true);
    mainLayout->addWidget(carsTableWidget);

    refreshCarsButton = new QPushButton("Refresh Car List");
    mainLayout->addWidget(refreshCarsButton);
    connect(refreshCarsButton, &QPushButton::clicked, this, &CarRentalMainWindow::loadAvailableCars);
    connect(carsTableWidget, &QTableWidget::cellClicked, this, &CarRentalMainWindow::onCarSelectedInTable);


    // --- Booking Section ---
    QFormLayout *bookingFormLayout = new QFormLayout();
    customerNameLineEdit = new QLineEdit();
    customerMobileLineEdit = new QLineEdit();
    selectedCarIdLineEdit = new QLineEdit();
    selectedCarIdLineEdit->setReadOnly(true); // Filled by table selection
    rentalDaysSpinBox = new QSpinBox();
    rentalDaysSpinBox->setMinimum(1);
    rentalDaysSpinBox->setMaximum(30); // Example max

    bookingFormLayout->addRow("Customer Name:", customerNameLineEdit);
    bookingFormLayout->addRow("Mobile Number:", customerMobileLineEdit);
    bookingFormLayout->addRow("Selected Car ID:", selectedCarIdLineEdit);
    bookingFormLayout->addRow("Rental Days:", rentalDaysSpinBox);

    mainLayout->addLayout(bookingFormLayout);

    bookCarButton = new QPushButton("Book Car");
    mainLayout->addWidget(bookCarButton);
    connect(bookCarButton, &QPushButton::clicked, this, &CarRentalMainWindow::onBookCarClicked);

    // --- View My Bookings Section ---
    QHBoxLayout* viewBookingsLayout = new QHBoxLayout();
    viewBookingsMobileLineEdit = new QLineEdit();
    viewBookingsMobileLineEdit->setPlaceholderText("Enter Your Mobile Number");
    viewBookingsButton = new QPushButton("View My Bookings");
    viewBookingsLayout->addWidget(new QLabel("Your Mobile:"));
    viewBookingsLayout->addWidget(viewBookingsMobileLineEdit);
    viewBookingsLayout->addWidget(viewBookingsButton);
    mainLayout->addLayout(viewBookingsLayout);
    connect(viewBookingsButton, &QPushButton::clicked, this, &CarRentalMainWindow::onViewMyBookingsClicked);


    // --- Invoice / Display Area ---
    invoiceBookingDisplayArea = new QTextEdit();
    invoiceBookingDisplayArea->setReadOnly(true);
    mainLayout->addWidget(new QLabel("Details / Invoice:"));
    mainLayout->addWidget(invoiceBookingDisplayArea);


    setCentralWidget(centralWidget);
    resize(800, 600); // Set initial size
}

void CarRentalMainWindow::loadAvailableCars() {
    if (!dbHandler.isOpen()) {
        invoiceBookingDisplayArea->setText("Database not connected. Cannot load cars.");
        return;
    }
    QVector<Car> cars = dbHandler.getAvailableCars();
    carsTableWidget->setRowCount(0); // Clear existing rows

    for (const Car& car : cars) {
        int row = carsTableWidget->rowCount();
        carsTableWidget->insertRow(row);
        carsTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(car.id)));
        carsTableWidget->setItem(row, 1, new QTableWidgetItem(car.name));
        carsTableWidget->setItem(row, 2, new QTableWidgetItem(car.model));
        carsTableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(car.rent_per_day)));
        carsTableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(car.quantity)));
    }
}

// void CarRentalMainWindow::onCarSelectedInTable(int row, int column) {
//     Q_UNUSED(column);
//     if (row >= 0 && row < carsTableWidget->rowCount()) {
//         selectedCarIdLineEdit->setText(carsTableWidget->item(row, 0)->text());
//     }
// }

void CarRentalMainWindow::onCarSelectedInTable(int row, int column) {
    // ADD THESE qDebug LINES
    qDebug() << "onCarSelectedInTable called. Row:" << row << "Column:" << column;
    Q_UNUSED(column);

    if (row >= 0 && row < carsTableWidget->rowCount()) {
        QTableWidgetItem *idItem = carsTableWidget->item(row, 0); // Column 0 is Car ID
        if (idItem) {
            QString carIdText = idItem->text();
            qDebug() << "Car ID item found. Text:" << carIdText; // ADD THIS
            selectedCarIdLineEdit->setText(carIdText);
        } else {
            qDebug() << "Car ID item at row" << row << "col 0 is NULL."; // ADD THIS
        }
    } else {
        qDebug() << "Invalid row selected (row =" << row << ") or table empty (rowCount =" << carsTableWidget->rowCount() << ")."; // MODIFIED THIS for more info
    }
}

void CarRentalMainWindow::onBookCarClicked() {
    if (!dbHandler.isOpen()) {
        QMessageBox::warning(this, "Error", "Database not connected.");
        return;
    }

    Customer customer;
    customer.name = customerNameLineEdit->text().trimmed();
    customer.mobile_number = customerMobileLineEdit->text().trimmed();

    bool okCarId;
    int carId = selectedCarIdLineEdit->text().toInt(&okCarId);
    int days = rentalDaysSpinBox->value();

    if (customer.name.isEmpty() || customer.mobile_number.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Customer name and mobile number cannot be empty.");
        return;
    }
    if (!okCarId || carId <= 0) {
        QMessageBox::warning(this, "Input Error", "Please select a valid car from the list.");
        return;
    }

    Car selectedCar = dbHandler.getCarById(carId);
    if (selectedCar.id == -1) {
        QMessageBox::warning(this, "Error", "Selected car not found in database.");
        return;
    }
    if (selectedCar.quantity <= 0) {
        QMessageBox::warning(this, "Error", "Selected car is out of stock.");
        loadAvailableCars(); // Refresh list
        return;
    }

    int totalRent = selectedCar.rent_per_day * days;

    // Confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Booking",
                                  QString("Book %1 %2 for %3 days?\nTotal Rent: Rs. %4")
                                      .arg(selectedCar.name).arg(selectedCar.model).arg(days).arg(totalRent),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (dbHandler.updateCarQuantity(carId, -1)) { // Attempt to decrement quantity
            if (dbHandler.storeBooking(customer, carId, days, totalRent)) {
                QMessageBox::information(this, "Success", "Car booked successfully!");
                generateAndShowInvoice(customer, selectedCar, days, totalRent);
                loadAvailableCars(); // Refresh car list
                // Clear input fields
                customerNameLineEdit->clear();
                customerMobileLineEdit->clear();
                selectedCarIdLineEdit->clear();
                rentalDaysSpinBox->setValue(1);
            } else {
                QMessageBox::critical(this, "Error", "Failed to store booking details. Car quantity was not reserved.");
                // Attempt to revert quantity change (though this could also fail)
                dbHandler.updateCarQuantity(carId, 1);
            }
        } else {
            QMessageBox::critical(this, "Error", "Failed to reserve the car (e.g., became unavailable). Booking cancelled.");
            loadAvailableCars(); // Refresh to show updated quantities
        }
    }
}

void CarRentalMainWindow::generateAndShowInvoice(const Customer& customer, const Car& car, int days, int totalRent) {
    QString invoiceText;
    invoiceText += "\t\tCAR RENTAL - CUSTOMER INVOICE\n";
    invoiceText += "\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    invoiceText += QString("Date: %1\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    invoiceText += QString("Customer Name: %1\n").arg(customer.name);
    invoiceText += QString("Mobile Number: %1\n").arg(customer.mobile_number);
    invoiceText += QString("Car Selected: %1 (%2)\n").arg(car.name).arg(car.model);
    invoiceText += QString("Number of Days: %1\n").arg(days);
    invoiceText += QString("Rent per Day: Rs. %1\n").arg(car.rent_per_day);
    invoiceText += QString("Total Rent: Rs. %1\n").arg(totalRent);
    invoiceText += "\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    invoiceBookingDisplayArea->setText(invoiceText);
}

void CarRentalMainWindow::onViewMyBookingsClicked() {
    if (!dbHandler.isOpen()) {
        QMessageBox::warning(this, "Error", "Database not connected.");
        return;
    }
    QString mobile = viewBookingsMobileLineEdit->text().trimmed();
    if (mobile.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a mobile number to view bookings.");
        return;
    }

    QVector<BookingInfo> bookings = dbHandler.getBookingsByMobile(mobile);

    if (bookings.isEmpty()) {
        invoiceBookingDisplayArea->setText(QString("No bookings found for mobile number: %1").arg(mobile));
        // QMessageBox::information(this, "No Bookings", QString("No bookings found for mobile number: %1").arg(mobile));
        return;
    }

    QString bookingsText = QString("Bookings for Mobile: %1\n").arg(mobile);
    bookingsText += "-----------------------------------------------------------------\n";
    bookingsText += QString("%1 | %2 | %3 | %4 | %5 | %6\n")
                        .arg("ID", -5)
                        .arg("Car Name", -15)
                        .arg("Model", -10)
                        .arg("Days", -5)
                        .arg("Rent", -7)
                        .arg("Date", -20);
    bookingsText += "-----------------------------------------------------------------\n";


    for (const auto& booking : bookings) {
        bookingsText += QString("%1 | %2 | %3 | %4 | %5 | %6\n")
        .arg(booking.bookingId, -5)
            .arg(booking.carName, -15)
            .arg(booking.carModel, -10)
            .arg(booking.days, -5)
            .arg(booking.totalRent, -7)
            .arg(booking.bookingDate, -20);
    }
    invoiceBookingDisplayArea->setText(bookingsText);

    // Optional: Display in a separate dialog
    /*
    QDialog *bookingsDialog = new QDialog(this);
    bookingsDialog->setWindowTitle("My Bookings");
    QVBoxLayout *dialogLayout = new QVBoxLayout(bookingsDialog);
    QTextEdit *bookingsDisplay = new QTextEdit(bookingsDialog);
    bookingsDisplay->setReadOnly(true);
    bookingsDisplay->setText(bookingsText);
    dialogLayout->addWidget(bookingsDisplay);
    QPushButton *closeButton = new QPushButton("Close", bookingsDialog);
    connect(closeButton, &QPushButton::clicked, bookingsDialog, &QDialog::accept);
    dialogLayout->addWidget(closeButton);
    bookingsDialog->setLayout(dialogLayout);
    bookingsDialog->exec(); // Show as modal dialog
    */
}

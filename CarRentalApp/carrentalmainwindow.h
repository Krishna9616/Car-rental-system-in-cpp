// carrentalmainwindow.h
#ifndef CARRENTALMAINWINDOW_H
#define CARRENTALMAINWINDOW_H

#include <QMainWindow>
#include "databasehandler.h" // Include your database handler

// Forward declarations of Qt UI elements
QT_BEGIN_NAMESPACE
namespace Ui { class CarRentalMainWindow; } // If using .ui file from Qt Designer
QT_END_NAMESPACE

class QTableWidget;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QSpinBox; // For number of days
class QLabel;

class CarRentalMainWindow : public QMainWindow {
    Q_OBJECT

public:
    CarRentalMainWindow(QWidget *parent = nullptr);
    ~CarRentalMainWindow();

private slots:
    void loadAvailableCars();
    void onBookCarClicked();
    void onViewMyBookingsClicked();
    void onCarSelectedInTable(int row, int column); // Optional: auto-fill car ID

private:
    void setupUi(); // To manually create UI or load from .ui file
    void generateAndShowInvoice(const Customer& customer, const Car& car, int days, int totalRent);


    // If not using .ui file from Qt Designer, declare widgets here:
    QTableWidget *carsTableWidget;
    QLineEdit *customerNameLineEdit;
    QLineEdit *customerMobileLineEdit;
    QLineEdit *selectedCarIdLineEdit; // Or make it read-only and fill from table selection
    QSpinBox *rentalDaysSpinBox;
    QPushButton *bookCarButton;
    QPushButton *refreshCarsButton;
    QPushButton *viewBookingsButton; // New button
    QLineEdit *viewBookingsMobileLineEdit; // Input for mobile number for viewing bookings
    QTextEdit *invoiceBookingDisplayArea; // For displaying invoices or booking lists

    DatabaseHandler dbHandler;
};

#endif // CARRENTALMAINWINDOW_H

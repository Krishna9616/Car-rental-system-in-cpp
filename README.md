# 🚗 Car Rental System

A desktop-based **Car Rental System** application built with **C++**, **Qt (Widgets)** for the GUI, and **SQLite** as the backend database. This project allows rental operations by letting users choose a car, input customer information, and generate a rental invoice.

---

## 🧭 Application Flow

1. Display a list of available cars.
2. Prompt the user to enter:
   - Customer Name
   - Mobile Number
3. Ask the user to select a Car ID from the list.
4. Input the number of days to rent the car.
5. Automatically generate an invoice with calculated cost.
6. Store the transaction details (customer + rental info) in the SQLite database.

---

## ✨ Features

- View all available cars
- Customer information entry
- Rent car with dynamic charge calculation
- Generate rental invoice (basic summary view)
- Save all data (car, customer, rental info) in SQLite database
- Intuitive Qt GUI

---

## 🛠️ Tech Stack

| Component  | Technology Used         |
|------------|-------------------------|
| Language   | C++                     |
| GUI        | Qt (Qt Widgets)         |
| Database   | SQLite                  |
| Build Tool | qmake / Qt Creator      |

---

## 🧑‍💻 How to Run

### Prerequisites

- C++ compiler (e.g., g++, MSVC)
- Qt 5/6 installed (Qt Creator recommended)
- SQLite3 (generally bundled with Qt)

### Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/car-rental-system.git
   cd car-rental-system

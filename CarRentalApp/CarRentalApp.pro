QT += core gui widgets

CONFIG += c++11 // Or c++14, c++17 if needed

TARGET = CarRentalApp
TEMPLATE = app

# Default PCH use. Reduce CONFIG+=pch if you want to manage it manually.
# CONFIG +=PCH_ creación_de_archivos_PCH

SOURCES += main.cpp \
           carrentalmainwindow.cpp \
           databasehandler.cpp \
           sqlite3.c   # Make sure sqlite3.c is in your project directory
           # car.cpp (if you create it)
           # customer.cpp (if you create it)

HEADERS  += carrentalmainwindow.h \
            databasehandler.h \
            car.h \
            customer.h \
            sqlite3.h   # Make sure sqlite3.h is in your project directory

# FORMS += carrentalmainwindow.ui # Only if you created a .ui file

# For SQLite
# Option 1: Link against system SQLite library (common on Linux/macOS)
# LIBS += -lsqlite3  // <<<< COMMENTED OUT

# Option 2: Include sqlite3.c amalgamation directly in your project
# (More portable, especially for Windows, ensures specific SQLite version)
# 1. Download sqlite3.c and sqlite3.h from sqlite.org
# 2. Add them to your project directory (e.g., a 'sqlite' subfolder or root)
# 3. Uncomment the following lines and comment out LIBS += -lsqlite3
# The SOURCES and HEADERS lines for sqlite3 are now above, uncommented.
DEFINES += SQLITE_OMIT_LOAD_EXTENSION # Optional
# DEFINES += SQLITE_THREADSAFE=0 # If you're sure it's single-threaded only (reduces overhead)

# --- Rest of your .pro file (deployment rules) ---
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

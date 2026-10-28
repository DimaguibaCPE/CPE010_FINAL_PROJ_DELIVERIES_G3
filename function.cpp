#include "function.h"
#include <iostream>
#include <limits>
#include <cstdlib>

// Login settings
const std::string DISPATCHER_USERNAME = "dispatcher";
const std::string DISPATCHER_PASSWORD = "12345";
const int MAX_LOGIN_ATTEMPTS = 3;

// SCREEN AND INPUT FUNCTIONS
void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[1;1H";
#endif
}

void line() {
    std::cout << "============================================================\n";
}

void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

int readMenuChoice(int minChoice, int maxChoice) {
    int choice;

    while (true) {
        if (std::cin >> choice &&
            choice >= minChoice &&
            choice <= maxChoice) {

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n'
            );

            return choice;
        }

        std::cout << "\nInvalid input. Enter a number from "
                  << minChoice << " to " << maxChoice << ".\n";
        std::cout << "Enter your selection: ";

        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n'
        );
    }
}

// LOGIN SYSTEM
bool login() {
    for (int attempt = 1;
         attempt <= MAX_LOGIN_ATTEMPTS;
         ++attempt) {

        clearScreen();

        std::cout << "ROUTE OPTIMIZER FOR DELIVERY DRIVERS\n";
        line();
        std::cout << "                         LOGIN\n";
        line();

        std::string username;
        std::string password;

        std::cout << "Username: ";
        std::getline(std::cin, username);

        std::cout << "Password: ";
        std::getline(std::cin, password);

        if (username == DISPATCHER_USERNAME &&
            password == DISPATCHER_PASSWORD) {

            std::cout << "\nLogin successful! Welcome, Dispatcher.\n";
            pauseScreen();
            return true;
        }

        std::cout << "\nInvalid username or password.\n";
        std::cout << "Attempts remaining: "
                  << MAX_LOGIN_ATTEMPTS - attempt << "\n";

        if (attempt < MAX_LOGIN_ATTEMPTS) {
            pauseScreen();
        }
    }

    return false;
}

// MAIN APPLICATION NAVIGATION
void runApplication() {
    bool running = true;

    while (running) {
        if (!login()) {
            clearScreen();

            std::cout << "Too many unsuccessful login attempts.\n";
            std::cout << "The program will now close.\n";

            pauseScreen();
            return;
        }

        bool loggedIn = true;

        while (loggedIn) {
            clearScreen();
            showMainMenu();

            int choice = readMenuChoice(0, 8);

            if (choice == 0) {
                clearScreen();

                std::cout << "You have logged out successfully.\n";
                pauseScreen();

                loggedIn = false;
            } else {
                openFeature(choice);
            }
        }

        clearScreen();

        std::cout << "ROUTE OPTIMIZER FOR DELIVERY DRIVERS\n";
        line();
        std::cout << "(1) Log In Again\n";
        std::cout << "(0) Exit Program\n";
        line();
        std::cout << "Enter your selection: ";

        int choice = readMenuChoice(0, 1);

        if (choice == 0) {
            running = false;
        }
    }

    clearScreen();

    std::cout
        << "Thank you for using Route Optimizer for Delivery Drivers!\n";
}

// MAIN MENU
void showMainMenu() {
    std::cout << "ROUTE OPTIMIZER FOR DELIVERY DRIVERS\n";
    line();

    std::cout << "                         MAIN MENU\n";
    line();

    std::cout << "(1) Add Delivery Record\n";
    std::cout << "(2) Manage Delivery Records\n";
    std::cout << "(3) Rider Management\n";
    std::cout << "(4) Delivery Assignment\n";
    std::cout << "(5) Delivery Tracking\n";
    std::cout << "(6) Route Optimization\n";
    std::cout << "(0) Log Out\n";

    line();
    std::cout << "Enter your selection: ";
}

void openFeature(int choice) {
    clearScreen();

    switch (choice) {
        case 1:
            showAddDeliveryScreen();
            break;

        case 2:
            showManageDeliveriesScreen();
            break;

        case 3:
            showRiderManagementScreen();
            break;

        case 4:
            showDeliveryAssignmentScreen();
            break;

        case 5:
            showDeliveryTrackingScreen();
            break;

        case 6:
            showRouteOptimizationScreen();
            break;

        default:
            break;
    }
}

// ADD DELIVERY RECORD
void showAddDeliveryScreen() {
    std::cout << "ADD DELIVERY RECORD\n";
    line();

    std::cout << "Planned actions:\n";
    std::cout << "- Enter customer name and contact number\n";
    std::cout << "- Enter parcel details and destination\n";
    std::cout << "- Select delivery priority\n";
    std::cout << "- Generate a delivery ID and confirm the record\n";

    std::cout << "\nThis feature is not implemented yet.\n";
    pauseScreen();
}

//  MANAGE DELIVERY RECORDS
void showManageDeliveriesScreen() {
    bool inMenu = true;

    while (inMenu) {
        clearScreen();
        showManageDeliveriesMenu();

        int choice = readMenuChoice(0, 4);

        switch (choice) {
            case 1:
                clearScreen();
                std::cout << "VIEW DELIVERY RECORDS\n";
                line();
                std::cout << "Viewing records will be implemented later.\n";
                pauseScreen();
                break;

            case 2:
                clearScreen();
                std::cout << "SEARCH DELIVERY RECORD\n";
                line();
                std::cout << "Searching records will be implemented later.\n";
                pauseScreen();
                break;

            case 3:
                clearScreen();
                std::cout << "EDIT DELIVERY RECORD\n";
                line();
                std::cout << "Editing records will be implemented later.\n";
                pauseScreen();
                break;

            case 4:
                clearScreen();
                std::cout << "CANCEL DELIVERY RECORD\n";
                line();
                std::cout << "Cancelling records will be implemented later.\n";
                pauseScreen();
                break;

            case 0:
                inMenu = false;
                break;
        }
    }
}

void showManageDeliveriesMenu() {
    std::cout << "MANAGE DELIVERY RECORDS\n";
    line();

    std::cout << "(1) View Delivery Records\n";
    std::cout << "(2) Search Delivery Record\n";
    std::cout << "(3) Edit Delivery Record\n";
    std::cout << "(4) Cancel Delivery Record\n";
    std::cout << "(0) Return to Main Menu\n";

    line();
    std::cout << "Enter your selection: ";
}

// RIDER MANAGEMENT
void showRiderManagementScreen() {
    bool inMenu = true;

    while (inMenu) {
        clearScreen();
        showRiderManagementMenu();

        int choice = readMenuChoice(0, 4);

        switch (choice) {
            case 1:
                clearScreen();
                std::cout << "ADD RIDER\n";
                line();
                std::cout << "Adding riders will be implemented later.\n";
                pauseScreen();
                break;

            case 2:
                clearScreen();
                std::cout << "VIEW RIDERS\n";
                line();
                std::cout << "Viewing riders will be implemented later.\n";
                pauseScreen();
                break;

            case 3:
                clearScreen();
                std::cout << "UPDATE RIDER INFORMATION\n";
                line();
                std::cout << "Updating riders will be implemented later.\n";
                pauseScreen();
                break;

            case 4:
                clearScreen();
                std::cout << "MANAGE RIDER AVAILABILITY\n";
                line();
                std::cout << "Managing availability will be implemented later.\n";
                pauseScreen();
                break;

            case 0:
                inMenu = false;
                break;
        }
    }
}

void showRiderManagementMenu() {
    std::cout << "RIDER MANAGEMENT\n";
    line();

    std::cout << "(1) Add Rider\n";
    std::cout << "(2) View Riders\n";
    std::cout << "(3) Update Rider Information\n";
    std::cout << "(4) Manage Rider Availability\n";
    std::cout << "(0) Return to Main Menu\n";

    line();
    std::cout << "Enter your selection: ";
}

// DELIVERY ASSIGNMENT
void showDeliveryAssignmentScreen() {
    std::cout << "DELIVERY ASSIGNMENT\n";
    line();

    std::cout << "Planned actions:\n";
    std::cout << "- View pending deliveries\n";
    std::cout << "- View available riders and their workloads\n";
    std::cout << "- Assign deliveries based on priority and workload\n";

    std::cout << "\nThis feature is not implemented yet.\n";
    pauseScreen();
}

// DELIVERY TRACKING
void showDeliveryTrackingScreen() {
    std::cout << "DELIVERY TRACKING\n";
    line();

    std::cout << "Planned actions:\n";
    std::cout << "- View delivery statuses\n";
    std::cout << "- Search for a delivery by ID\n";
    std::cout << "- Update a delivery status\n";

    std::cout << "\nThis feature is not implemented yet.\n";
    pauseScreen();
}

// ROUTE OPTIMIZATION
void showRouteOptimizationScreen() {
    std::cout << "ROUTE OPTIMIZATION\n";
    line();

    std::cout << "Planned actions:\n";
    std::cout << "- View available locations and roads\n";
    std::cout << "- Find the shortest route to a destination\n";
    std::cout << "- Optimize the order of multiple delivery stops\n";

    std::cout << "\nThis feature is not implemented yet.\n";
    pauseScreen();
}
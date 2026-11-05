#include "delivery.h"
#include <iostream>
#include <random>
#include <limits>
#include <iomanip> 


const std::string MAIN_WAREHOUSE = "Main Warehouse";

DeliveryManager& getDeliveryManager() {
    static DeliveryManager manager;
    return manager;
}

// generates a unique randomized Delivery ID
std::string DeliveryManager::generateDeliveryID() {

    static std::random_device rd;
    static std::mt19937 gen(rd());

    static std::uniform_int_distribution<int> dist(10000, 99999);

    std::string id;

    do {
        id = "DEL-" + std::to_string(dist(gen));

    } while (usedDeliveryIDs.find(id) != usedDeliveryIDs.end());

    usedDeliveryIDs.insert(id);

    return id;
}

// registers a new delivery record
void DeliveryManager::addDeliveryRecord() {

    Delivery newDelivery;

    std::cout << "\n============================================\n";
    std::cout << "             ADD DELIVERY RECORD\n";
    std::cout << "============================================\n";

    // this automatically generate delivery id
    newDelivery.deliveryID = generateDeliveryID();

    std::cout << "\nGenerated Delivery ID: "
              << newDelivery.deliveryID << "\n";

    // customer information to be filled up
    std::cout << "\n--- CUSTOMER INFORMATION ---\n";

    do {
        std::cout << "Customer Name: ";
        std::getline(std::cin, newDelivery.customerName);

        if (newDelivery.customerName.empty()) {
            std::cout << "Customer name cannot be empty.\n";
        }

    } while (newDelivery.customerName.empty());

    do {
        std::cout << "Contact Number: ";
        std::getline(std::cin, newDelivery.contactNumber);

        if (newDelivery.contactNumber.empty()) {
            std::cout << "Contact number cannot be empty.\n";
        }

    } while (newDelivery.contactNumber.empty());

    // parcel information
    std::cout << "\n--- PARCEL INFORMATION ---\n";

    do {
        std::cout << "Parcel Description: ";
        std::getline(std::cin, newDelivery.parcelDescription);

        if (newDelivery.parcelDescription.empty()) {
            std::cout << "Parcel description cannot be empty.\n";
        }

    } while (newDelivery.parcelDescription.empty());

    // delivery priority
    int priorityChoice;

    std::cout << "\n--- DELIVERY PRIORITY ---\n";
    std::cout << "[1] High\n";
    std::cout << "[2] Medium\n";
    std::cout << "[3] Low\n";

    while (true) {

        std::cout << "Select Priority: ";

        if (std::cin >> priorityChoice &&
            priorityChoice >= 1 &&
            priorityChoice <= 3) {

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n'
            );

            break;
        }

        std::cout << "Invalid selection. Please enter 1-3.\n";

        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n'
        );
    }

    switch (priorityChoice) {
        case 1:
            newDelivery.priority = "High";
            break;

        case 2:
            newDelivery.priority = "Medium";
            break;

        case 3:
            newDelivery.priority = "Low";
            break;
    }

    // delivery route information
    std::cout << "\n--- DELIVERY ROUTE INFORMATION ---\n";

    // this automatically assign warehouse as origin
    newDelivery.origin = MAIN_WAREHOUSE;

    std::cout << "FROM (Origin): "
              << newDelivery.origin << "\n";

    // enters destination of the order
    do {
        std::cout << "TO (Destination): ";

        std::getline(std::cin, newDelivery.destination);

        if (newDelivery.destination.empty()) {
            std::cout << "Destination cannot be empty.\n";
        }

    } while (newDelivery.destination.empty());

    newDelivery.status = "Pending";

    deliveries.push_back(newDelivery);

    pendingDeliveries.push(newDelivery.deliveryID);

    // displays confirmation of order
    std::cout << "\n============================================\n";
    std::cout << "       DELIVERY SUCCESSFULLY REGISTERED!\n";
    std::cout << "============================================\n";

    std::cout << "Delivery ID: " << newDelivery.deliveryID << "\n";
    std::cout << "Customer Name: " << newDelivery.customerName << "\n";
    std::cout << "Contact Number: " << newDelivery.contactNumber << "\n";
    std::cout << "Parcel: " << newDelivery.parcelDescription << "\n";

    std::cout << "FROM: " << newDelivery.origin << "\n";
    std::cout << "TO: " << newDelivery.destination << "\n";

    std::cout << "Priority: " << newDelivery.priority << "\n";
    std::cout << "Status: " << newDelivery.status << "\n";

    std::cout << "============================================\n";

    std::cout << "\nTotal Registered Deliveries: "
              << deliveries.size() << "\n";

    std::cout << "Pending Deliveries: "
              << pendingDeliveries.size() << "\n";
}

// dsipalys all registered delivery records
void DeliveryManager::viewDeliveryRecords() const {

    std::cout << "\n============================================\n";
    std::cout << "       REGISTERED DELIVERY ORDERS\n";
    std::cout << "============================================\n";

    if (deliveries.empty()) {
        std::cout << "\nNo delivery records found.\n";
        std::cout << "Register a new delivery to get started.\n";
        std::cout << "============================================\n";
        return;
    }

    std::cout << "\nTotal Registered Deliveries: "
              << deliveries.size() << "\n";

    int recordNumber = 1;

    for (const Delivery& delivery : deliveries) {

        std::cout << "\nDELIVERY RECORD #" << recordNumber << "\n";
        std::cout << "--------------------------------------------\n";

        std::cout << "Delivery ID:        " << delivery.deliveryID << "\n";
        std::cout << "Customer Name:      " << delivery.customerName << "\n";
        std::cout << "Contact Number:     " << delivery.contactNumber << "\n";
        std::cout << "Parcel Description: " << delivery.parcelDescription << "\n";

        std::cout << "\nDELIVERY ROUTE\n";
        std::cout << "FROM:               " << delivery.origin << "\n";
        std::cout << "TO:                 " << delivery.destination << "\n";

        std::cout << "\nDELIVERY DETAILS\n";
        std::cout << "Priority:           " << delivery.priority << "\n";
        std::cout << "Status:             " << delivery.status << "\n";

        std::cout << "--------------------------------------------\n";

        recordNumber++;
    }

    std::cout << "\nEnd of Registered Delivery Records.\n";
    std::cout << "============================================\n";
}

const std::vector<Delivery>&
DeliveryManager::getAllDeliveries() const {

    return deliveries;
}

bool DeliveryManager::peekNextPendingDeliveryID(
    std::string& id) const {

    if (pendingDeliveries.empty()) {
        return false;
    }

    id = pendingDeliveries.front();

    return true;
}

bool DeliveryManager::popNextPendingDeliveryID(
    std::string& id) {

    if (pendingDeliveries.empty()) {
        return false;
    }

    id = pendingDeliveries.front();

    pendingDeliveries.pop();

    return true;
}

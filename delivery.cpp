#include "delivery.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <limits>
#include <iomanip>
#include <chrono> 

const std::string MAIN_WAREHOUSE = "Main Warehouse";

namespace {
    // partitions delivery records around the last record's delivery ID
    int partitionDeliveries(std::vector<Delivery>& records, int low, int high) {
        const std::string pivot = records[high].deliveryID;
        int smallerIndex = low - 1;

        for (int current = low; current < high; ++current) {
            if (records[current].deliveryID <= pivot) {
                ++smallerIndex;
                std::swap(records[smallerIndex], records[current]);
            }
        }

        std::swap(records[smallerIndex + 1], records[high]);
        return smallerIndex + 1;
    }

    // recursively sorts delivery records in ascending delivery ID order
    void quickSortDeliveries(std::vector<Delivery>& records, int low, int high) {
        if (low < high) {
            const int pivotIndex = partitionDeliveries(records, low, high);
            quickSortDeliveries(records, low, pivotIndex - 1);
            quickSortDeliveries(records, pivotIndex + 1, high);
        }
    }
}

// exposes the production Quick Sort implementation for the performance benchmark
void quickSortDeliveryVector(std::vector<Delivery>& records) {
    if (!records.empty()) {
        quickSortDeliveries(records, 0, static_cast<int>(records.size()) - 1);
    }
}

// returns the shared delivery manager instance
DeliveryManager& getDeliveryManager() {
    static DeliveryManager manager;
    return manager;
}

// generates a unique delivery ID that is not yet in use
std::string DeliveryManager::generateDeliveryID() {

    // initializes the random number generator
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

std::cout << "\n============================================================\n";
std::cout << "                    ADD DELIVERY RECORD\n";
std::cout << "============================================================\n";

    // generates a unique ID for the new delivery
    newDelivery.deliveryID = generateDeliveryID();

    std::cout << "\nGenerated Delivery ID: "
              << newDelivery.deliveryID << "\n";

    // collects the customer's name and contact number
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

    // collects the parcel description
    std::cout << "\n--- PARCEL INFORMATION ---\n";

    do {
        std::cout << "Parcel Description: ";
        std::getline(std::cin, newDelivery.parcelDescription);

        if (newDelivery.parcelDescription.empty()) {
            std::cout << "Parcel description cannot be empty.\n";
        }

    } while (newDelivery.parcelDescription.empty());

    // allows the dispatcher to select the delivery priority
    int priorityChoice;

    std::cout << "\n--- DELIVERY PRIORITY ---\n";
    std::cout << "(1) High\n";
    std::cout << "(2) Medium\n";
    std::cout << "(3) Low\n";

    // validates the selected priority
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

    // assigns the selected priority to the delivery record
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

    // collects the origin and destination of the delivery
    std::cout << "\n--- DELIVERY ROUTE INFORMATION ---\n";

    // sets the main warehouse as the delivery origin
    newDelivery.origin = MAIN_WAREHOUSE;

    std::cout << "FROM (Origin): "
              << newDelivery.origin << "\n";

    // collects the delivery destination from the dispatcher
    do {
        std::cout << "TO (Destination): ";

        std::getline(std::cin, newDelivery.destination);

        if (newDelivery.destination.empty()) {
            std::cout << "Destination cannot be empty.\n";
        }

    } while (newDelivery.destination.empty());

    
    // sets the initial delivery status to pending
    newDelivery.status = "Pending";

    // stores the delivery record in the delivery list
    deliveries.push_back(newDelivery);

    // automatically sorts delivery records using Quick Sort
    quickSortDeliveryVector(deliveries);

    // adds the delivery ID to the pending delivery queue
    pendingDeliveries.push(newDelivery.deliveryID);

    // displays the details of the successfully registered delivery
    std::cout << "\n============================================================\n";
    std::cout << "             DELIVERY SUCCESSFULLY REGISTERED!\n";
    std::cout << "============================================================\n";

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

// displays all registered delivery records
void DeliveryManager::viewDeliveryRecords() const {

    std::cout << "\n============================================================\n";
    std::cout << "                 REGISTERED DELIVERY ORDERS\n";
    std::cout << "============================================================\n";

    // checks if there are no registered delivery records
    if (deliveries.empty()) {
        std::cout << "\nNo delivery records found.\n";
        std::cout << "Register a new delivery to get started.\n";
        std::cout << "============================================================\n";
        return;
    }

    std::cout << "\nTotal Registered Deliveries: "
              << deliveries.size() << "\n";

    // starts the numbering of delivery records
    int recordNumber = 1;

    // displays each registered delivery record
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

// searches for a delivery record using its ID
void DeliveryManager::searchDeliveryRecord() const {

    std::string searchID;

    std::cout << "\n============================================================\n";
    std::cout << "                  SEARCH DELIVERY RECORD\n";
    std::cout << "============================================================\n";

    std::cout << "Enter Delivery ID: ";
    std::getline(std::cin, searchID);

    // checks each delivery record for the entered ID
    for (const Delivery& delivery : deliveries) {

        if (delivery.deliveryID == searchID) {

            std::cout << "\nDELIVERY RECORD FOUND!\n";
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
            std::cout << "Assigned Rider:     ";

            // displays the assigned rider or none if unassigned
            if (delivery.assignedRider.empty()) {
                std::cout << "None\n";
            }
            else {
                std::cout << delivery.assignedRider << "\n";
            }

            std::cout << "--------------------------------------------\n";

            return;
        }
    }

    std::cout << "\nNo delivery record found with ID: "
              << searchID << "\n";
}

// sorts all delivery records by delivery ID using Quick Sort
void DeliveryManager::quickSortDeliveryRecords() {
    if (deliveries.empty()) {
        std::cout << "\nNo delivery records available to sort.\n";
        return;
    }

    quickSortDeliveryVector(deliveries);

    std::cout << "\nDelivery records successfully sorted by Delivery ID (ascending).\n";
    std::cout << "Sorting algorithm: Quick Sort\n";
}

// edits an existing pending delivery record
void DeliveryManager::editDeliveryRecord() {

    std::string searchID;

    std::cout << "\n============================================================\n";
    std::cout << "                    EDIT DELIVERY RECORD\n";
    std::cout << "============================================================\n";

    std::cout << "Enter Delivery ID: ";
    std::getline(std::cin, searchID);

    for (Delivery& delivery : deliveries) {

        if (delivery.deliveryID == searchID) {
            
            // allows editing only when the delivery is pending
            if (delivery.status != "Pending") {
                std::cout << "\nOnly pending deliveries can be edited.\n";
                return;
            }

            int choice;

            std::cout << "\nDELIVERY RECORD FOUND!\n";
            std::cout << "Customer Name: " << delivery.customerName << "\n";
            std::cout << "Destination: " << delivery.destination << "\n";

            std::cout << "\nSELECT INFORMATION TO EDIT\n";
            std::cout << "(1) Customer Name\n";
            std::cout << "(2) Contact Number\n";
            std::cout << "(3) Parcel Description\n";
            std::cout << "(4) Destination\n";
            std::cout << "(5) Priority\n";
            std::cout << "(0) Cancel\n";

            std::cout << "Enter your selection: ";
            
            // validates the selected field to edit
            while (!(std::cin >> choice) || choice < 0 || choice > 5) {
                std::cout << "Invalid selection. Enter 0-5: ";
                std::cin.clear();
                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(), '\n'
                );
            }

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n'
            );

            std::string newValue;

            switch (choice) {

                case 1:
                    do {
                        std::cout << "Enter New Customer Name: ";
                        std::getline(std::cin, newValue);

                        if (newValue.empty()) {
                            std::cout << "Customer name cannot be empty.\n";
                        }

                    } while (newValue.empty());

                    delivery.customerName = newValue;
                    break;

                case 2:
                    do {
                        std::cout << "Enter New Contact Number: ";
                        std::getline(std::cin, newValue);

                        if (newValue.empty()) {
                            std::cout << "Contact number cannot be empty.\n";
                        }

                    } while (newValue.empty());

                    delivery.contactNumber = newValue;
                    break;

                case 3:
                    do {
                        std::cout << "Enter New Parcel Description: ";
                        std::getline(std::cin, newValue);

                        if (newValue.empty()) {
                            std::cout << "Parcel description cannot be empty.\n";
                        }

                    } while (newValue.empty());

                    delivery.parcelDescription = newValue;
                    break;

                case 4:
                    do {
                        std::cout << "Enter New Destination: ";
                        std::getline(std::cin, newValue);

                        if (newValue.empty()) {
                            std::cout << "Destination cannot be empty.\n";
                        }

                    } while (newValue.empty());

                    delivery.destination = newValue;
                    break;

                case 5: {
                    int priorityChoice;

                    std::cout << "\n(1) High\n";
                    std::cout << "(2) Medium\n";
                    std::cout << "(3) Low\n";

                    std::cout << "Select New Priority: ";

                    while (!(std::cin >> priorityChoice) ||
                           priorityChoice < 1 ||
                           priorityChoice > 3) {

                        std::cout << "Invalid selection. Enter 1-3: ";

                        std::cin.clear();

                        std::cin.ignore(
                            std::numeric_limits<std::streamsize>::max(), '\n'
                        );
                    }

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(), '\n'
                    );

                    // updates the delivery priority based on the selected option
                    switch (priorityChoice) {
                        case 1:
                            delivery.priority = "High";
                            break;

                        case 2:
                            delivery.priority = "Medium";
                            break;

                        case 3:
                            delivery.priority = "Low";
                            break;
                    }

                    break;
                }

                case 0:
                    std::cout << "\nEditing cancelled.\n";
                    return;
            }

            if (choice >= 1 && choice <= 5) {
                std::cout << "\nDelivery record successfully updated!\n";
            }

            return;
        }
    }

    std::cout << "\nNo delivery record found with ID: "
              << searchID << "\n";
}

// cancels an existing pending delivery record
void DeliveryManager::cancelDeliveryRecord() {

    std::string searchID;

    std::cout << "\n============================================================\n";
    std::cout << "                  CANCEL DELIVERY RECORD\n";
    std::cout << "============================================================\n";

    std::cout << "Enter Delivery ID: ";
    std::getline(std::cin, searchID);

    for (Delivery& delivery : deliveries) {

        if (delivery.deliveryID == searchID) {
            
            // allows cancellation only when the delivery is pending
            if (delivery.status != "Pending") {
                std::cout << "\nOnly pending deliveries can be cancelled.\n";
                return;
            }

            std::cout << "\nDELIVERY RECORD FOUND!\n";
            std::cout << "Customer Name: " << delivery.customerName << "\n";
            std::cout << "Destination: " << delivery.destination << "\n";
            std::cout << "Status: " << delivery.status << "\n";

            char confirmation;

            std::cout << "\nAre you sure you want to cancel this delivery? (Y/N): ";
            std::cin >> confirmation;

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n'
            );
            
            // changes the delivery status when cancellation is confirmed
            if (confirmation == 'Y' || confirmation == 'y') {

                delivery.status = "Cancelled";

                std::cout << "\nDelivery record successfully cancelled.\n";
            }
            else {
                std::cout << "\nCancellation aborted.\n";
            }

            return;
        }
    }

    std::cout << "\nNo delivery record found with ID: "
              << searchID << "\n";
}

// returns all registered delivery records
const std::vector<Delivery>&
DeliveryManager::getAllDeliveries() const {

    return deliveries;
}

// retrieves the next pending delivery ID without removing it
bool DeliveryManager::peekNextPendingDeliveryID(
    std::string& id) const {

    if (pendingDeliveries.empty()) {
        return false;
    }

    id = pendingDeliveries.front();

    return true;
}

// retrieves and removes the next pending delivery ID
bool DeliveryManager::popNextPendingDeliveryID(
    std::string& id) {

    if (pendingDeliveries.empty()) {
        return false;
    }

    id = pendingDeliveries.front();

    pendingDeliveries.pop();

    return true;
}

// assigns a pending delivery to a rider
bool DeliveryManager::assignDelivery(
    const std::string& deliveryID,
    const std::string& riderName
) {
    for (int i = 0; i < deliveries.size(); i++) {

        if (deliveries[i].deliveryID == deliveryID) {

            // prevents assignment if the delivery is no longer pending
            if (deliveries[i].status != "Pending") {
                return false;
            }

            // stores the assigned rider and updates the delivery status
            deliveries[i].assignedRider = riderName;
            deliveries[i].status = "Assigned";

            return true;
        }
    }

    return false;
}

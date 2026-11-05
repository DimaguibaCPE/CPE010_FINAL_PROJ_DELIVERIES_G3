#include "function.h"
#include "delivery.h"
#include "deliverytrack.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <fstream>
#include <sstream>

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
    loadRidersFromFile();
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

// RIDER RECORDS

const int MAX_RIDERS = 100;

struct Rider {
    int id;
    std::string name;
    std::string contact;
    bool available;
    bool onDelivery;
    int workload;

    std::string item;
    std::string address;
};

// RIDER RECORD STORAGE

Rider riders[MAX_RIDERS];
int riderCount = 0;
int nextRiderID = 1;

void saveRidersToFile() {

    std::ofstream file("riders.txt");

    if (!file.is_open()) {
        std::cout << "Error saving rider records.\n";
        return;
    }

    for (int i = 0; i < riderCount; i++) {

        file << "ID: #" << riders[i].id
             << ", Name: " << riders[i].name
             << ", Contact: " << riders[i].contact
             << ", Workload: " << riders[i].workload
             << ", Status: ";

        if (riders[i].onDelivery) {
            file << "On-Delivery";
        }
        else if (riders[i].available) {
            file << "Ready";
        }
        else {
            file << "Unavailable";
        }

        file << ", Item: " << riders[i].item<< ", Address: " << riders[i].address << "\n";
    }


    
    file.close();
}


void loadRidersFromFile() {

    std::ifstream file("riders.txt");

    if (!file.is_open()) {
        return;
    }

    riderCount = 0;
    nextRiderID = 1;

    std::string line;

    while (std::getline(file, line) && riderCount < MAX_RIDERS) {

        std::stringstream ss(line);

        std::string idPart;
        std::string namePart;
        std::string contactPart;
        std::string workloadPart;
        std::string statusPart;
        std::string itemPart;
        std::string addressPart;

        std::getline(ss, idPart, ',');
        std::getline(ss, namePart, ',');
        std::getline(ss, contactPart, ',');
        std::getline(ss, workloadPart, ',');
        std::getline(ss, statusPart, ',');
        std::getline(ss, itemPart, ',');
        std::getline(ss, addressPart);

        try {
            riders[riderCount].id =
                std::stoi(idPart.substr(idPart.find('#') + 1));

            riders[riderCount].name =
                namePart.substr(namePart.find(": ") + 2);

            riders[riderCount].contact =
                contactPart.substr(contactPart.find(": ") + 2);

            riders[riderCount].workload =
                std::stoi(workloadPart.substr(workloadPart.find(": ") + 2));

            std::string status = statusPart.substr(statusPart.find(": ") + 2);

            riders[riderCount].available = (status == "Ready");
            riders[riderCount].onDelivery = (status == "On-Delivery");
            riders[riderCount].item = itemPart.substr(itemPart.find(": ") + 2);
            riders[riderCount].address = addressPart.substr(addressPart.find(": ") + 2);

            if (riders[riderCount].id >= nextRiderID) {
                nextRiderID = riders[riderCount].id + 1;
            }

            riderCount++;
        }
        catch (...) {
            continue;
        }
    }

    file.close();
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

    getDeliveryManager().addDeliveryRecord();

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
                getDeliveryManager().viewDeliveryRecords();
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

// ADD RIDER
void addRider() {

    clearScreen();

    std::cout << "ADD RIDER\n";
    line();

    if (riderCount >= MAX_RIDERS) {
        std::cout << "Rider limit has been reached.\n";
        pauseScreen();
        return;
    }

    std::string name;
    std::string contact;

    // GET RIDER NAME
    std::cout << "Enter Rider Name: ";
    std::getline(std::cin, name);

    if (name.empty()) {
        std::cout << "\nName cannot be empty.\n";
        pauseScreen();
        return;
    }

    // GET CONTACT NUMBER
    std::cout << "Enter Contact Number: ";
    std::getline(std::cin, contact);

    if (contact.empty()) {
        std::cout << "\nContact number cannot be empty.\n";
        pauseScreen();
        return;
    }

    // STORE RIDER INFORMATION
    riders[riderCount].id = nextRiderID;
    riders[riderCount].name = name;
    riders[riderCount].contact = contact;
    riders[riderCount].available = true;
    riders[riderCount].onDelivery = false;
    riders[riderCount].workload = 0;
    riders[riderCount].item = "None";
    riders[riderCount].address = "None";

    std::cout << "\nRider successfully added!\n";

    std::cout << "Rider ID: "
              << nextRiderID << "\n";

    std::cout << "Name: "
              << name << "\n";

    std::cout << "Contact: "
              << contact << "\n";

    std::cout << "Availability: Available\n";

    // UPDATE COUNTERS
    riderCount++;
    nextRiderID++;

    saveRidersToFile();

    pauseScreen();
}

// VIEW RIDERS
void viewRiders() {

    clearScreen();

    std::cout << "VIEW RIDERS\n";
    line();

    if (riderCount == 0) {
        std::cout << "No rider records found.\n";
        pauseScreen();
        return;
    }

    for (int i = 0; i < riderCount; i++) {

        std::cout << "\nRider ID: "
                  << riders[i].id << "\n";

        std::cout << "Name: "
                  << riders[i].name << "\n";

        std::cout << "Contact: "
                  << riders[i].contact << "\n";

        std::cout << "Availability: ";

        if (riders[i].available == true) {
            std::cout << "Available\n";
        }
        else {
            std::cout << "Unavailable\n";
        }

        std::cout << "Workload: "
                  << riders[i].workload << "\n";

        line();
    }

    std::cout << "\nTotal Riders: "
              << riderCount << "\n";

    pauseScreen();
}

// UPDATE RIDER INFORMATION
void updateRider() {

    clearScreen();

    int id;
    int found = -1;

    std::cout << "UPDATE RIDER INFORMATION\n";
    line();

    if (riderCount == 0) {
        std::cout << "No rider records found.\n";
        pauseScreen();
        return;
    }

    std::cout << "Enter Rider ID: ";
    std::cin >> id;

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(), '\n'
    );

    // SEARCH FOR RIDER
    for (int i = 0; i < riderCount; i++) {

        if (riders[i].id == id) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        std::cout << "\nRider not found.\n";
        pauseScreen();
        return;
    }

    int choice;

    std::cout << "\nRider Found!\n";

    std::cout << "Name: "
              << riders[found].name << "\n";

    std::cout << "Contact: "
              << riders[found].contact << "\n";

    line();

    std::cout << "(1) Update Name\n";
    std::cout << "(2) Update Contact Number\n";
    std::cout << "(0) Cancel\n";

    line();

    std::cout << "Enter your selection: ";
    choice = readMenuChoice(0, 2);

    if (choice == 1) {

        std::string newName;

        std::cout << "Enter New Name: ";
        std::getline(std::cin, newName);

        if (newName.empty()) {
            std::cout << "\nName cannot be empty.\n";
        }
        else {
            riders[found].name = newName;
            std::cout << "\nName successfully updated!\n";
        }
    }

    else if (choice == 2) {

        std::string newContact;

        std::cout << "Enter New Contact Number: ";
        std::getline(std::cin, newContact);

        if (newContact.empty()) {
            std::cout << "\nContact number cannot be empty.\n";
        }
        else {
            riders[found].contact = newContact;
            std::cout << "\nContact number successfully updated!\n";
        }
    }

    else {
        return;
    }

    saveRidersToFile();

    pauseScreen();
}

// MANAGE RIDER AVAILABILITY
void manageRiderAvailability() {

    clearScreen();

    int id;
    int found = -1;

    std::cout << "MANAGE RIDER AVAILABILITY\n";
    line();

    if (riderCount == 0) {
        std::cout << "No rider records found.\n";
        pauseScreen();
        return;
    }

    std::cout << "Enter Rider ID: ";
    std::cin >> id;

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(), '\n'
    );

    // SEARCH FOR RIDER
    for (int i = 0; i < riderCount; i++) {

        if (riders[i].id == id) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        std::cout << "\nRider not found.\n";
        pauseScreen();
        return;
    }

    std::cout << "\nRider: "
              << riders[found].name << "\n";

    std::cout << "Current Availability: ";

    if (riders[found].available == true) {
        std::cout << "Available\n";
    }
    else {
        std::cout << "Unavailable\n";
    }

    line();

    std::cout << "(1) Set as Available\n";
    std::cout << "(2) Set as Unavailable\n";
    std::cout << "(0) Cancel\n";

    line();

    std::cout << "Enter your selection: ";

    int choice = readMenuChoice(0, 2);

    if (choice == 1) {

        riders[found].available = true;

        std::cout << "\nRider is now Available.\n";
    }

    else if (choice == 2) {

        riders[found].available = false;

        std::cout << "\nRider is now Unavailable.\n";
    }

    else {
        return;
    }

    saveRidersToFile();

    pauseScreen();
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
                addRider();
                break;

            case 2:
                viewRiders();
                break;

            case 3:
                updateRider();
                break;

            case 4:
                manageRiderAvailability();
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


int getPriorityValue(const std::string& priority) {
    if (priority == "High") {
        return 3;
    }

    if (priority == "Medium") {
        return 2;
    }

    return 1;
}

void viewPendingDeliveries() {

    const std::vector<Delivery>& deliveries =
        getDeliveryManager().getAllDeliveries();

    std::cout << "\nPENDING DELIVERIES\n";
    line();

    bool found = false;

    for (int i = 0; i < deliveries.size(); i++) {

        if (deliveries[i].status == "Pending") {

            found = true;

            std::cout << "Delivery ID: "
                      << deliveries[i].deliveryID << "\n";

            std::cout << "Customer: "
                      << deliveries[i].customerName << "\n";

            std::cout << "Destination: "
                      << deliveries[i].destination << "\n";

            std::cout << "Priority: "
                      << deliveries[i].priority << "\n";

            line();
        }
    }

    if (!found) {
        std::cout << "No pending deliveries found.\n";
    }
}

void viewAvailableRiders() {

    std::cout << "\nAVAILABLE RIDERS\n";
    line();

    bool found = false;

    for (int i = 0; i < riderCount; i++) {

        if (riders[i].available && !riders[i].onDelivery) {

            found = true;

            std::cout << "Rider ID: "
                      << riders[i].id << "\n";

            std::cout << "Name: "
                      << riders[i].name << "\n";

            std::cout << "Workload: "
                      << riders[i].workload << "\n";

            line();
        }
    }

    if (!found) {
        std::cout << "No available riders.\n";
    }
}

int findAvailableRiderWithLowestWorkload() {

    int selected = -1;

    for (int i = 0; i < riderCount; i++) {

        if (riders[i].available && !riders[i].onDelivery) {

            if (selected == -1 ||
                riders[i].workload < riders[selected].workload) {

                selected = i;
            }
        }
    }

    return selected;
}


void assignDeliveryManually() {

    const std::vector<Delivery>& deliveries =
        getDeliveryManager().getAllDeliveries();

    std::vector<int> pendingIndexes;

    std::cout << "\nSELECT A PENDING DELIVERY\n";
    line();

    for (int i = 0; i < deliveries.size(); i++) {

        if (deliveries[i].status == "Pending") {

            pendingIndexes.push_back(i);

            std::cout << pendingIndexes.size() << ". "
                      << deliveries[i].deliveryID << " | "
                      << deliveries[i].customerName << " | "
                      << deliveries[i].destination << " | Priority: "
                      << deliveries[i].priority << "\n";
        }
    }

    if (pendingIndexes.empty()) {
        std::cout << "No pending deliveries to assign.\n";
        return;
    }

    std::cout << "0. Cancel\n";
    line();
    std::cout << "Choose delivery: ";

    int deliveryChoice = readMenuChoice(
        0, pendingIndexes.size()
    );

    if (deliveryChoice == 0) return;

    std::vector<int> availableIndexes;

    for (int i = 0; i < riderCount; i++) {

        if (riders[i].available && !riders[i].onDelivery) {
            availableIndexes.push_back(i);
        }
    }

    if (availableIndexes.empty()) {
        std::cout << "No available riders.\n";
        return;
    }

    std::cout << "\nSELECT A RIDER\n";
    line();

    for (int i = 0; i < availableIndexes.size(); i++) {

        int index = availableIndexes[i];

        std::cout << i + 1 << ". "
                  << "ID: " << riders[index].id
                  << " | " << riders[index].name
                  << " | Workload: " << riders[index].workload
                  << "\n";
    }

    std::cout << "0. Cancel\n";
    line();
    std::cout << "Choose rider: ";

    int riderChoice = readMenuChoice(
        0, availableIndexes.size()
    );

    if (riderChoice == 0) return;

    int deliveryIndex = pendingIndexes[deliveryChoice - 1];
    int riderIndex = availableIndexes[riderChoice - 1];

    if (getDeliveryManager().assignDelivery(
        deliveries[deliveryIndex].deliveryID,
        riders[riderIndex].name
    )) {

        riders[riderIndex].available = false;
        riders[riderIndex].onDelivery = true;
        riders[riderIndex].workload++;

        // Get parcel and destination from the selected delivery
        riders[riderIndex].item = deliveries[deliveryIndex].parcelDescription;
        riders[riderIndex].address = deliveries[deliveryIndex].destination;

        saveRidersToFile();

        std::cout << "\nDelivery successfully assigned!\n";

        std::cout << "Delivery ID: "
                << deliveries[deliveryIndex].deliveryID << "\n";

        std::cout << "Assigned Rider: "
                << riders[riderIndex].name << "\n";

        std::cout << "Item: "
                << riders[riderIndex].item << "\n";

        std::cout << "Address: "
                << riders[riderIndex].address << "\n";

    } else {
        std::cout << "Unable to assign delivery.\n";
    }
}

void autoAssignDeliveries() {

    std::cout << "\nAUTOMATIC DELIVERY ASSIGNMENT\n";
    line();

    int assignedCount = 0;

    while (true) {

        const std::vector<Delivery>& deliveries =
            getDeliveryManager().getAllDeliveries();

        int deliveryIndex = -1;

        for (int i = 0; i < deliveries.size(); i++) {

            if (deliveries[i].status == "Pending") {

                if (deliveryIndex == -1 ||
                    getPriorityValue(deliveries[i].priority) >
                    getPriorityValue(deliveries[deliveryIndex].priority)) {

                    deliveryIndex = i;
                }
            }
        }

        if (deliveryIndex == -1) {
            break;
        }

        int riderIndex = findAvailableRiderWithLowestWorkload();

        if (riderIndex == -1) {
            break;
        }

        if (getDeliveryManager().assignDelivery(
            deliveries[deliveryIndex].deliveryID,
            riders[riderIndex].name
        )) {

            riders[riderIndex].available = false;
            riders[riderIndex].onDelivery = true;
            riders[riderIndex].workload++;
            
            riders[riderIndex].address = deliveries[deliveryIndex].destination;
            riders[riderIndex].item = deliveries[deliveryIndex].parcelDescription;

            assignedCount++;

            std::cout << "Assigned "
                      << deliveries[deliveryIndex].deliveryID
                      << " to "
                      << riders[riderIndex].name << "\n";
        }
    }

    if (assignedCount > 0) {
    saveRidersToFile();
}

    std::cout << "\nTotal deliveries assigned: "
              << assignedCount << "\n";
}



void showDeliveryAssignmentScreen() {

    bool inMenu = true;

    while (inMenu) {

        clearScreen();

        std::cout << "DELIVERY ASSIGNMENT\n";
        line();

        std::cout << "(1) View Pending Deliveries\n";
        std::cout << "(2) View Available Riders\n";
        std::cout << "(3) Manually Assign Delivery\n";
        std::cout << "(4) Automatically Assign Deliveries\n";
        std::cout << "(0) Return to Main Menu\n";

        line();
        std::cout << "Enter your selection: ";

        int choice = readMenuChoice(0, 4);

        switch (choice) {

            case 1:
                clearScreen();
                viewPendingDeliveries();
                pauseScreen();
                break;

            case 2:
                clearScreen();
                viewAvailableRiders();
                pauseScreen();
                break;

            case 3:
                clearScreen();
                assignDeliveryManually();
                pauseScreen();
                break;

            case 4:
                clearScreen();
                autoAssignDeliveries();
                pauseScreen();
                break;

            case 0:
                inMenu = false;
                break;
        }
    }
}

// DELIVERY TRACKING

void showDeliveryTrackingScreen() {

    bool inMenu = true;

    const std::string fileName = "riders.txt";

    while (inMenu) {

        clearScreen();
        printMenu();

        int select = readMenuChoice(0, 5);
        std::string id;

        switch (select) {

            case 0:
                inMenu = false;
                break;

            case 1:
                std::cout << "\nAll Riders:\n";
                viewAll(fileName);
                pauseScreen();
                break;

            case 2:
                std::cout << "\nRiders on Standby:\n";
                viewReady(fileName);
                pauseScreen();
                break;

            case 3:
                std::cout << "\nRiders on Delivery:\n";
                viewOnDelivery(fileName);
                pauseScreen();
                break;

            case 4:
                std::cout << "\nComplete a Delivery:\n";
                std::cout << "Enter ID number: ";
                std::getline(std::cin, id);

                deliverComplete(fileName, id);

                loadRidersFromFile();
                pauseScreen();
                break;

            case 5:
                std::cout << "\nStart a Delivery:\n";
                std::cout << "Enter ID number: ";
                std::getline(std::cin, id);

                std::string address, item;

                std::cout << "Enter Item: ";
                getline(std::cin, item);

                std::cout << "Enter Address: ";
                getline(std::cin, address);

                deliverStart(fileName, id, address, item);

                loadRidersFromFile();
                pauseScreen();
                break;
        }
    }
}


// ROUTE OPTIMIZATION
void showRouteOptimizationScreen();

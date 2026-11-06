#include "function.h"
#include "delivery.h"
#include "deliverytrack.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <fstream>
#include <sstream>

// stores the dispatcher login credentials and maximum login attempts
const std::string DISPATCHER_USERNAME = "dispatcher";
const std::string DISPATCHER_PASSWORD = "12345";
const int MAX_LOGIN_ATTEMPTS = 3;

//===================== SCREEN AND INPUT FUNCTIONS =====================

// clears the console screen
void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[1;1H";
#endif
}

// displays a horizontal separator line
void line() {
    std::cout << "============================================================\n";
}

// pauses the program until the user presses Enter
void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

// reads and validates a menu choice within the specified range
int readMenuChoice(int minChoice, int maxChoice) {
    int choice;

    while (true) {
        
        // checks if the input is a valid number within the range
        if (std::cin >> choice &&
            choice >= minChoice &&
            choice <= maxChoice) {

            // clears the remaining input from the line
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n'
            );

            return choice;
        }

        // displays an error message for invalid input
        std::cout << "\nInvalid input. Enter a number from "
                  << minChoice << " to " << maxChoice << ".\n";
        std::cout << "Enter your selection: ";

        // resets the input stream and removes invalid input
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n'
        );
    }
}

//===================== LOGIN SYSTEM =====================

// verifies the dispatcher credentials with a limited number of attempts
bool login() {
    for (int attempt = 1;
         attempt <= MAX_LOGIN_ATTEMPTS;
         ++attempt) {

        // clears the screen before displaying the login menu
        clearScreen();

        std::cout << "ROUTE OPTIMIZER FOR DELIVERY DRIVERS\n";
        line();
        std::cout << "                         LOGIN\n";
        line();

        std::string username;
        std::string password;

        // asks the user to enter their username
        std::cout << "Username: ";
        std::getline(std::cin, username);

        // asks the user to enter their password
        std::cout << "Password: ";
        std::getline(std::cin, password);

        // checks if the entered credentials match the dispatcher account
        if (username == DISPATCHER_USERNAME &&
            password == DISPATCHER_PASSWORD) {

            // confirms successful login
            std::cout << "\nLogin successful! Welcome, Dispatcher.\n";
            pauseScreen();
            return true;
        }

        // displays an error message and the remaining login attempts
        std::cout << "\nInvalid username or password.\n";
        std::cout << "Attempts remaining: "
                  << MAX_LOGIN_ATTEMPTS - attempt << "\n";

        // pauses before the next login attempt
        if (attempt < MAX_LOGIN_ATTEMPTS) {
            pauseScreen();
        }
    }

    // returns false after all login attempts fail
    return false;
}


// ===================== MAIN APPLICATION NAVIGATION =====================

// controls the main application flow, login, and menu navigation
void runApplication() {
    
    // loads saved rider records when the application starts
    loadRidersFromFile();
    bool running = true;

    // keeps the application running until the user exits
    while (running) {
        
        // checks if the dispatcher successfully logs in
        if (!login()) {
            clearScreen();

            // displays a message after all login attempts fail
            std::cout << "Too many unsuccessful login attempts.\n";
            std::cout << "The program will now close.\n";

            pauseScreen();
            return;
        }

        bool loggedIn = true;

        // displays the main menu while the dispatcher is logged in
        while (loggedIn) {
            clearScreen();
            showMainMenu();

            // reads the selected main menu option
            int choice = readMenuChoice(0, 6);

            // logs out the dispatcher when option 0 is selected
            if (choice == 0) {
                clearScreen();

                std::cout << "You have logged out successfully.\n";
                pauseScreen();

                loggedIn = false;
            } else {
                // opens the selected application feature
                openFeature(choice);
            }
        }

        // displays the options after logout
        clearScreen();

        std::cout << "ROUTE OPTIMIZER FOR DELIVERY DRIVERS\n";
        line();
        std::cout << "(1) Log In Again\n";
        std::cout << "(0) Exit Program\n";
        line();
        std::cout << "Enter your selection: ";

        // reads the user's choice to log in again or exit
        int choice = readMenuChoice(0, 1);

        // stops the application when option 0 is selected
        if (choice == 0) {
            running = false;
        }
    }

    // clears the screen before displaying the exit message
    clearScreen();

    std::cout
        << "Thank you for using Route Optimizer for Delivery Drivers!\n";
}

//===================== RIDER RECORDS =====================

// sets the maximum number of riders that can be stored
const int MAX_RIDERS = 100;

// stores the information of a rider
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

//=====================RIDER RECORD STORAGE=====================

// stores rider records in an array
Rider riders[MAX_RIDERS];

// tracks the current number of registered riders
int riderCount = 0;

// generates the next rider ID
int nextRiderID = 1;

// saves all rider records to the file
void saveRidersToFile() {

    // opens the rider file in write mode
    std::ofstream file("riders.txt");

    // stops the function if the file cannot be opened
    if (!file.is_open()) {
        std::cout << "Error saving rider records.\n";
        return;
    }

    // writes each rider record to the file
    for (int i = 0; i < riderCount; i++) {

        // writes the rider's basic information
        file << "ID: #" << riders[i].id
             << ", Name: " << riders[i].name
             << ", Contact: " << riders[i].contact
             << ", Workload: " << riders[i].workload
             << ", Status: ";

        // writes the rider's current status
        if (riders[i].onDelivery) {
            file << "On-Delivery";
        }
        else if (riders[i].available) {
            file << "Ready";
        }
        else {
            file << "Unavailable";
        }

        // writes the rider's assigned item and address
        file << ", Item: " << riders[i].item<< ", Address: " << riders[i].address << "\n";
    }

    // closes the file after saving the records
    file.close();
}

// loads rider records from the file
void loadRidersFromFile() {

    // opens the rider file in read mode
    std::ifstream file("riders.txt");

    // stops the function if the file does not exist or cannot be opened
    if (!file.is_open()) {
        return;
    }

    // resets the rider count and ID counter before loading
    riderCount = 0;
    nextRiderID = 1;

    std::string line;

    // reads each rider record while there is available array space
    while (std::getline(file, line) && riderCount < MAX_RIDERS) {

        // separates the record into individual fields
        std::stringstream ss(line);

        std::string idPart;
        std::string namePart;
        std::string contactPart;
        std::string workloadPart;
        std::string statusPart;
        std::string itemPart;
        std::string addressPart;

        // extracts each field from the record
        std::getline(ss, idPart, ',');
        std::getline(ss, namePart, ',');
        std::getline(ss, contactPart, ',');
        std::getline(ss, workloadPart, ',');
        std::getline(ss, statusPart, ',');
        std::getline(ss, itemPart, ',');
        std::getline(ss, addressPart);

        // converts and assigns the extracted values to the rider structure
        try {
            riders[riderCount].id =
                std::stoi(idPart.substr(idPart.find('#') + 1));

            riders[riderCount].name =
                namePart.substr(namePart.find(": ") + 2);

            riders[riderCount].contact =
                contactPart.substr(contactPart.find(": ") + 2);

            riders[riderCount].workload =
                std::stoi(workloadPart.substr(workloadPart.find(": ") + 2));

            // determines the rider's availability from the saved status
            std::string status = statusPart.substr(statusPart.find(": ") + 2);

            riders[riderCount].available = (status == "Ready");
            riders[riderCount].onDelivery = (status == "On-Delivery");

            // extracts the rider's assigned item and address
            riders[riderCount].item = itemPart.substr(itemPart.find(": ") + 2);
            riders[riderCount].address = addressPart.substr(addressPart.find(": ") + 2);

            // updates the next rider ID based on the loaded record
            if (riders[riderCount].id >= nextRiderID) {
                nextRiderID = riders[riderCount].id + 1;
            }

            // increases the rider count after loading a valid record
            riderCount++;
        }
        catch (...) {
            
            // skips records that contain invalid data
            continue;
        }
    }

    // closes the file after loading the records
    file.close();
}

// ===================== MAIN MENU =====================

// displays the main application menu
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

// opens the selected application feature
void openFeature(int choice) {
    
    // clears the screen before opening the selected feature
    clearScreen();

    // directs the user to the selected feature
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

        // exits the switch if the choice does not match a feature
        default:
            break;
    }
}

// ===================== ADD DELIVERY RECORD =====================

// displays the Add Delivery Record screen
void showAddDeliveryScreen() {

    // calls the function to register a new delivery
    getDeliveryManager().addDeliveryRecord();

    // waits for the user before returning to the menu
    pauseScreen();
}

// ===================== MANAGE DELIVERY RECORDS =====================

// controls the Manage Delivery Records submenu
void showManageDeliveriesScreen() {
    bool inMenu = true;

    // keeps the submenu open until the user returns to the main menu
    while (inMenu) {
        clearScreen();
        showManageDeliveriesMenu();

        // reads and validates the submenu selection
        int choice = readMenuChoice(0, 4);

        // executes the selected delivery record operation
        switch (choice) {
            case 1:
                // displays all delivery records
                clearScreen();
                getDeliveryManager().viewDeliveryRecords();
                pauseScreen();
                break;

            case 2:
                // searches for a delivery record using its ID
                clearScreen();
                getDeliveryManager().searchDeliveryRecord();
                pauseScreen();
                break;

            case 3:
                // edits an existing pending delivery record
                clearScreen();
                getDeliveryManager().editDeliveryRecord();
                pauseScreen();
                break;

            case 4:
                // cancels an existing pending delivery record
                clearScreen();
                getDeliveryManager().cancelDeliveryRecord();
                pauseScreen();
                break;

            case 0:
                // returns to the main menu
                inMenu = false;
                break;
        }
    }
}

// displays the Manage Delivery Records submenu options
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


// ===================== ADD RIDER =====================

// registers a new rider in the system
void addRider() {

    // clears the screen before displaying the add rider menu
    clearScreen();

    std::cout << "\n============================================================\n";
    std::cout << "                          ADD RIDER\n";
    std::cout << "============================================================\n";

    // checks if the rider limit has been reached
    if (riderCount >= MAX_RIDERS) {
        std::cout << "Rider limit has been reached.\n";
        pauseScreen();
        return;
    }

    std::string name;
    std::string contact;

    // asks the user to enter the rider's name
    std::cout << "Enter Rider Name: ";
    std::getline(std::cin, name);

    // checks if the rider name is empty
    if (name.empty()) {
        std::cout << "\nName cannot be empty.\n";
        pauseScreen();
        return;
    }

    // asks the user to enter the rider's contact number
    std::cout << "Enter Contact Number: ";
    std::getline(std::cin, contact);

    // checks if the contact number is empty
    if (contact.empty()) {
        std::cout << "\nContact number cannot be empty.\n";
        pauseScreen();
        return;
    }

    // stores the entered information in the rider array
    riders[riderCount].id = nextRiderID;
    riders[riderCount].name = name;
    riders[riderCount].contact = contact;
    riders[riderCount].available = true;
    riders[riderCount].onDelivery = false;
    riders[riderCount].workload = 0;
    riders[riderCount].item = "None";
    riders[riderCount].address = "None";

    // confirms that the rider was successfully added
    std::cout << "\nRider successfully added!\n";

    // displays the assigned rider ID
    std::cout << "Rider ID: "
              << nextRiderID << "\n";

    // displays the registered rider name
    std::cout << "Rider Name: "
              << name << "\n";

    // displays the registered contact number
    std::cout << "Contact Number: "
              << contact << "\n";

    // displays the rider's initial availability
    std::cout << "Availability: Available\n";

    // updates the rider count and next available ID
    riderCount++;
    nextRiderID++;

    // saves the updated rider records to the file
    saveRidersToFile();

    // waits for the user before returning to the menu
    pauseScreen();
}

// ===================== VIEW RIDERS =====================

// displays all registered rider records
void viewRiders() {

    // clears the screen before displaying the rider list
    clearScreen();

    std::cout << "\n============================================================\n";
    std::cout << "                          VIEW RIDERS\n";
    std::cout << "============================================================\n";

    // checks if there are no registered riders
    if (riderCount == 0) {
        std::cout << "No rider records found.\n";
        pauseScreen();
        return;
    }

    // displays the information of each registered rider
    for (int i = 0; i < riderCount; i++) {

        // displays the rider's ID
        std::cout << "\nRider ID: "
                  << riders[i].id << "\n";

        // displays the rider's name
        std::cout << "Rider Name: "
                  << riders[i].name << "\n";

        // displays the rider's contact number
        std::cout << "Contact Number: "
                  << riders[i].contact << "\n";

        // displays the rider's availability status
        std::cout << "Availability: ";

        if (riders[i].available == true) {
            std::cout << "Available\n";
        }
        else {
            std::cout << "Unavailable\n";
        }

        // displays the rider's current workload
        std::cout << "Workload: "
                  << riders[i].workload << "\n";

        // separates each rider record
        line();
    }

    // displays the total number of registered riders
    std::cout << "\nTotal Riders: "
              << riderCount << "\n";

    // waits for the user before returning to the menu
    pauseScreen();
}


// ===================== UPDATE RIDER INFORMATION =====================

// updates the name or contact number of an existing rider
void updateRider() {

    // clears the screen before displaying the update menu
    clearScreen();

    int id;
    int found = -1;

    std::cout << "\n============================================================\n";
    std::cout << "                     UPDATE RIDER INFORMATION\n";
    std::cout << "============================================================\n";

    // checks if there are no registered riders
    if (riderCount == 0) {
        std::cout << "No rider records found.\n";
        pauseScreen();
        return;
    }

    // asks the user to enter the rider ID
    std::cout << "Enter Rider ID: ";
    std::cin >> id;

    // clears the remaining input from the line
    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(), '\n'
    );

    // searches for the rider using their ID
    for (int i = 0; i < riderCount; i++) {

        // stores the index of the matching rider
        if (riders[i].id == id) {
            found = i;
            break;
        }
    }

    // stops the update if the rider ID is not found
    if (found == -1) {
        std::cout << "\nRider not found.\n";
        pauseScreen();
        return;
    }

    int choice;

    // displays the information of the selected rider
    std::cout << "\nRider Found!\n";

    std::cout << "Rider Name: "
              << riders[found].name << "\n";

    std::cout << "Contact Number: "
              << riders[found].contact << "\n";

    line();

    // displays the available update options
    std::cout << "(1) Update Name\n";
    std::cout << "(2) Update Contact Number\n";
    std::cout << "(0) Cancel\n";

    line();

    // reads and validates the user's selection
    std::cout << "Enter your selection: ";
    choice = readMenuChoice(0, 2);

    // updates the rider's name
    if (choice == 1) {

        std::string newName;

        // asks the user to enter the new name
        std::cout << "Enter New Name: ";
        std::getline(std::cin, newName);

        // checks if the new name is empty
        if (newName.empty()) {
            std::cout << "\nName cannot be empty.\n";
        }
        else {
            // saves the new rider name
            riders[found].name = newName;
            std::cout << "\nName successfully updated!\n";
        }
    }

    // updates the rider's contact number
    else if (choice == 2) {

        std::string newContact;

        // asks the user to enter the new contact number
        std::cout << "Enter New Contact Number: ";
        std::getline(std::cin, newContact);

        // checks if the new contact number is empty
        if (newContact.empty()) {
            std::cout << "\nContact number cannot be empty.\n";
        }
        else {
            // saves the new contact number
            riders[found].contact = newContact;
            std::cout << "\nContact number successfully updated!\n";
        }
    }

    // returns without updating if the user cancels
    else {
        return;
    }

    // saves the updated rider records to the file
    saveRidersToFile();

    // waits for the user before returning to the menu
    pauseScreen();
}

// ===================== MANAGE RIDER AVAILABILITY =====================

// changes the availability status of an existing rider
void manageRiderAvailability() {

    // clears the screen before displaying the availability menu
    clearScreen();

    int id;
    int found = -1;

    std::cout << "\n============================================================\n";
    std::cout << "                  MANAGE RIDER AVAILABILITY\n";
    std::cout << "============================================================\n";

    // checks if there are no registered riders
    if (riderCount == 0) {
        std::cout << "No rider records found.\n";
        pauseScreen();
        return;
    }

    // asks the user to enter the rider ID
    std::cout << "Enter Rider ID: ";
    std::cin >> id;

    // clears the remaining input from the line
    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(), '\n'
    );

    // searches for the rider using their ID
    for (int i = 0; i < riderCount; i++) {

        // stores the index of the matching rider
        if (riders[i].id == id) {
            found = i;
            break;
        }
    }

    // stops the update if the rider ID is not found
    if (found == -1) {
        std::cout << "\nRider not found.\n";
        pauseScreen();
        return;
    }

    // displays the selected rider's name
    std::cout << "\nRider: "
              << riders[found].name << "\n";

    // displays the rider's current availability
    std::cout << "Current Availability: ";

    if (riders[found].available == true) {
        std::cout << "Available\n";
    }
    else {
        std::cout << "Unavailable\n";
    }

    line();

    // displays the available status update options
    std::cout << "(1) Set as Available\n";
    std::cout << "(2) Set as Unavailable\n";
    std::cout << "(0) Cancel\n";

    line();

    // reads and validates the user's selection
    std::cout << "Enter your selection: ";

    int choice = readMenuChoice(0, 2);

    // sets the rider's availability to Available
    if (choice == 1) {

        riders[found].available = true;

        std::cout << "\nRider is now Available.\n";
    }

    // sets the rider's availability to Unavailable
    else if (choice == 2) {

        riders[found].available = false;

        std::cout << "\nRider is now Unavailable.\n";
    }

    // returns without changing the status if the user cancels
    else {
        return;
    }

    // saves the updated rider records to the file
    saveRidersToFile();

    // waits for the user before returning to the menu
    pauseScreen();
}

// ===================== RIDER MANAGEMENT =====================

// controls the Rider Management submenu
void showRiderManagementScreen() {

    bool inMenu = true;

    // keeps the submenu open until the user returns to the main menu
    while (inMenu) {

        clearScreen();
        showRiderManagementMenu();

        // reads and validates the submenu selection
        int choice = readMenuChoice(0, 4);

        // executes the selected rider management operation
        switch (choice) {

            case 1:
                // registers a new rider
                addRider();
                break;

            case 2:
                // displays all registered riders
                viewRiders();
                break;

            case 3:
                // updates the information of an existing rider
                updateRider();
                break;

            case 4:
                // changes the availability status of a rider
                manageRiderAvailability();
                break;

            case 0:
                // returns to the main menu
                inMenu = false;
                break;
        }
    }
}

// displays the Rider Management submenu options
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

// returns the numerical value of a delivery priority
int getPriorityValue(const std::string& priority) {

    // assigns the highest value to High priority
    if (priority == "High") {
        return 3;
    }

    // assigns the middle value to Medium priority
    if (priority == "Medium") {
        return 2;
    }

    // returns the lowest value for other priorities
    return 1;
}

// ===================== DELIVERY ASSIGNMENT HELPERS =====================

// displays all deliveries with Pending status
void viewPendingDeliveries() {

    // retrieves all registered delivery records
    const std::vector<Delivery>& deliveries =
        getDeliveryManager().getAllDeliveries();

    std::cout << "\n============================================================\n";
    std::cout << "                     PENDING DELIVERIES\n";
    std::cout << "============================================================\n";

    bool found = false;

    // checks each delivery record for Pending status
    for (int i = 0; i < deliveries.size(); i++) {

        // displays the delivery if it is still pending
        if (deliveries[i].status == "Pending") {

            found = true;

            // displays the delivery ID
            std::cout << "Delivery ID: "
                      << deliveries[i].deliveryID << "\n";

            // displays the customer's name
            std::cout << "Customer: "
                      << deliveries[i].customerName << "\n";

            // displays the delivery destination
            std::cout << "Destination: "
                      << deliveries[i].destination << "\n";

            // displays the delivery priority
            std::cout << "Priority: "
                      << deliveries[i].priority << "\n";

            // separates each delivery record
            line();
        }
    }

    // displays a message if no pending deliveries are found
    if (!found) {
        std::cout << "No pending deliveries found.\n";
    }
}

// displays riders who are available for assignment
void viewAvailableRiders() {

    std::cout << "\n============================================================\n";
    std::cout << "                     AVAILABLE RIDERS\n";
    std::cout << "============================================================\n";

    bool found = false;

    // checks each registered rider
    for (int i = 0; i < riderCount; i++) {

        // displays riders who are available and not on delivery
        if (riders[i].available && !riders[i].onDelivery) {

            found = true;

            // displays the rider ID
            std::cout << "Rider ID: "
                      << riders[i].id << "\n";

            // displays the rider's name
            std::cout << "Name: "
                      << riders[i].name << "\n";

            // displays the rider's current workload
            std::cout << "Workload: "
                      << riders[i].workload << "\n";

            // separates each rider record
            line();
        }
    }

    // displays a message if no riders are available
    if (!found) {
        std::cout << "No available riders.\n";
    }
}

// finds the available rider with the lowest workload
int findAvailableRiderWithLowestWorkload() {

    // stores the index of the selected rider
    int selected = -1;

    // checks each registered rider
    for (int i = 0; i < riderCount; i++) {

        // considers riders who are available and not on delivery
        if (riders[i].available && !riders[i].onDelivery) {

            // selects the rider with the lowest workload
            if (selected == -1 ||
                riders[i].workload < riders[selected].workload) {

                selected = i;
            }
        }
    }

    // returns the selected rider's index or -1 if none is available
    return selected;
}

// manually assigns a pending delivery to a selected available rider
void assignDeliveryManually() {

    // gets all delivery records
    const std::vector<Delivery>& deliveries =
        getDeliveryManager().getAllDeliveries();

    // stores the indexes of pending deliveries
    std::vector<int> pendingIndexes;

    // displays the pending delivery selection screen
    std::cout << "\n============================================================\n";
    std::cout << "               SELECT A PENDING DELIVERY\n";
    std::cout << "============================================================\n";

    // checks each delivery for pending status
    for (int i = 0; i < deliveries.size(); i++) {

        if (deliveries[i].status == "Pending") {

            // stores the index of the pending delivery
            pendingIndexes.push_back(i);

            // displays the pending delivery details
            std::cout << "(" << pendingIndexes.size() << ") "
                      << deliveries[i].deliveryID << " | "
                      << deliveries[i].customerName << " | "
                      << deliveries[i].destination << " | Priority: "
                      << deliveries[i].priority << "\n";
        }
    }

    // stops the process if no pending deliveries exist
    if (pendingIndexes.empty()) {
        std::cout << "No pending deliveries to assign.\n";
        return;
    }

    // provides an option to cancel the assignment
    std::cout << "(0) Cancel\n";
    line();
    std::cout << "Choose delivery: ";

    // gets the selected delivery choice
    int deliveryChoice = readMenuChoice(
        0, pendingIndexes.size()
    );

    // cancels the assignment if zero is selected
    if (deliveryChoice == 0) return;

    // stores the indexes of available riders
    std::vector<int> availableIndexes;

    // checks each rider for availability
    for (int i = 0; i < riderCount; i++) {

        if (riders[i].available && !riders[i].onDelivery) {

            // stores the index of the available rider
            availableIndexes.push_back(i);
        }
    }

    // stops the process if no available riders exist
    if (availableIndexes.empty()) {
        std::cout << "No available riders.\n";
        return;
    }

    // displays the available rider selection screen
    std::cout << "\nSELECT A RIDER\n";
    line();

    // displays the details of available riders
    for (int i = 0; i < availableIndexes.size(); i++) {

        // gets the actual rider index
        int index = availableIndexes[i];

        std::cout << "(" << i + 1 << ") "
                  << "ID: " << riders[index].id
                  << " | " << riders[index].name
                  << " | Workload: " << riders[index].workload
                  << "\n";
    }

    // provides an option to cancel the assignment
    std::cout << "(0) Cancel\n";
    line();
    std::cout << "Choose rider: ";

    // gets the selected rider choice
    int riderChoice = readMenuChoice(
        0, availableIndexes.size()
    );

    // cancels the assignment if zero is selected
    if (riderChoice == 0) return;

    // gets the actual index of the selected delivery
    int deliveryIndex = pendingIndexes[deliveryChoice - 1];

    // gets the actual index of the selected rider
    int riderIndex = availableIndexes[riderChoice - 1];

    // assigns the selected delivery to the chosen rider
    if (getDeliveryManager().assignDelivery(
        deliveries[deliveryIndex].deliveryID,
        riders[riderIndex].name
    )) {

        // updates the rider's availability and delivery status
        riders[riderIndex].available = false;
        riders[riderIndex].onDelivery = true;

        // increases the rider's workload
        riders[riderIndex].workload++;

        // gets the parcel and destination from the selected delivery
        riders[riderIndex].item = deliveries[deliveryIndex].parcelDescription;
        riders[riderIndex].address = deliveries[deliveryIndex].destination;

        // saves the updated rider information
        saveRidersToFile();

        // displays the assignment success message
        std::cout << "\nDelivery successfully assigned!\n";

        // displays the assigned delivery ID
        std::cout << "Delivery ID: "
                << deliveries[deliveryIndex].deliveryID << "\n";

        // displays the assigned rider
        std::cout << "Assigned Rider: "
                << riders[riderIndex].name << "\n";

        // displays the assigned parcel
        std::cout << "Item: "
                << riders[riderIndex].item << "\n";

        // displays the delivery address
        std::cout << "Address: "
                << riders[riderIndex].address << "\n";

    } else {

        // displays an error message if assignment fails
        std::cout << "Unable to assign delivery.\n";
    }
}

// automatically assigns pending deliveries based on priority and rider workload
void autoAssignDeliveries() {

    // displays the automatic delivery assignment screen
    std::cout << "\n============================================================\n";
    std::cout << "             AUTOMATIC DELIVERY ASSIGNMENT\n";
    std::cout << "============================================================\n";

    // counts the successfully assigned deliveries
    int assignedCount = 0;

    // continues assigning deliveries while possible
    while (true) {

        // gets all delivery records
        const std::vector<Delivery>& deliveries =
            getDeliveryManager().getAllDeliveries();

        // initializes the selected delivery index
        int deliveryIndex = -1;

        // searches for the highest-priority pending delivery
        for (int i = 0; i < deliveries.size(); i++) {

            if (deliveries[i].status == "Pending") {

                // compares delivery priorities to select the highest one
                if (deliveryIndex == -1 ||
                    getPriorityValue(deliveries[i].priority) >
                    getPriorityValue(deliveries[deliveryIndex].priority)) {

                    // stores the index of the selected delivery
                    deliveryIndex = i;
                }
            }
        }

        // stops the process if no pending deliveries remain
        if (deliveryIndex == -1) {
            break;
        }

        // finds the available rider with the lowest workload
        int riderIndex = findAvailableRiderWithLowestWorkload();

        // stops the process if no available rider is found
        if (riderIndex == -1) {
            break;
        }

        // assigns the selected delivery to the chosen rider
        if (getDeliveryManager().assignDelivery(
            deliveries[deliveryIndex].deliveryID,
            riders[riderIndex].name
        )) {

            // updates the rider's availability and delivery status
            riders[riderIndex].available = false;
            riders[riderIndex].onDelivery = true;

            // increases the rider's workload
            riders[riderIndex].workload++;

            // updates the rider's delivery address and parcel
            riders[riderIndex].address = deliveries[deliveryIndex].destination;
            riders[riderIndex].item = deliveries[deliveryIndex].parcelDescription;

            // increases the total number of assigned deliveries
            assignedCount++;

            // displays the delivery assignment result
            std::cout << "Assigned "
                      << deliveries[deliveryIndex].deliveryID
                      << " to "
                      << riders[riderIndex].name << "\n";
        }
    }

    // saves rider information if deliveries were assigned
    if (assignedCount > 0) {
        saveRidersToFile();
    }

    // displays the total number of assigned deliveries
    std::cout << "\nTotal deliveries assigned: "
              << assignedCount << "\n";
}

// displays the delivery assignment menu and handles user selections
void showDeliveryAssignmentScreen() {

    // controls the menu loop
    bool inMenu = true;

    // continues displaying the menu while active
    while (inMenu) {

        // clears the screen before displaying the menu
        clearScreen();

        // displays the delivery assignment menu title
        std::cout << "DELIVERY ASSIGNMENT\n";
        line();

        // displays the available menu options
        std::cout << "(1) View Pending Deliveries\n";
        std::cout << "(2) View Available Riders\n";
        std::cout << "(3) Manually Assign Delivery\n";
        std::cout << "(4) Automatically Assign Deliveries\n";
        std::cout << "(0) Return to Main Menu\n";

        line();
        std::cout << "Enter your selection: ";

        // gets the user's menu choice
        int choice = readMenuChoice(0, 4);

        // handles the selected menu option
        switch (choice) {

            case 1:
                // displays pending deliveries
                clearScreen();
                viewPendingDeliveries();
                pauseScreen();
                break;

            case 2:
                // displays available riders
                clearScreen();
                viewAvailableRiders();
                pauseScreen();
                break;

            case 3:
                // starts manual delivery assignment
                clearScreen();
                assignDeliveryManually();
                pauseScreen();
                break;

            case 4:
                // starts automatic delivery assignment
                clearScreen();
                autoAssignDeliveries();
                pauseScreen();
                break;

            case 0:
                // exits the delivery assignment menu
                inMenu = false;
                break;
        }
    }
}

// ===================== DELIVERY TRACKING =====================

// displays the delivery tracking menu and manages rider delivery activities
void showDeliveryTrackingScreen() {

    // controls the menu loop
    bool inMenu = true;

    // sets the rider data file name
    const std::string fileName = "riders.txt";

    // continues displaying the menu while active
    while (inMenu) {

        // clears the screen and displays the tracking menu
        clearScreen();
        printMenu();

        // gets the user's menu selection
        int select = readMenuChoice(0, 4);

        // stores the rider ID entered by the user
        std::string id;

        // handles the selected tracking menu option
        switch (select) {

            case 0:
                // exits the delivery tracking menu
                inMenu = false;
                break;

            case 1:
                // displays all riders
                std::cout << "\nAll Riders:\n";
                viewAll(fileName);
                pauseScreen();
                break;

            case 2:
                // displays riders who are on standby
                std::cout << "\nRiders on Standby:\n";
                viewReady(fileName);
                pauseScreen();
                break;

            case 3:
                // displays riders currently on delivery
                std::cout << "\nRiders on Delivery:\n";
                viewOnDelivery(fileName);
                pauseScreen();
                break;

            case 4:
                // completes a delivery using the entered rider ID
                std::cout << "\nComplete a Delivery:\n";
                std::cout << "Enter ID number: ";
                std::getline(std::cin, id);

                // updates the rider's delivery completion status
                deliverComplete(fileName, id);

                // reloads the updated rider information
                loadRidersFromFile();
                pauseScreen();
                break;

        }
    }
}

// ===================== ROUTE OPTIMIZATION =====================

// displays the route optimization screen
void showRouteOptimizationScreen();

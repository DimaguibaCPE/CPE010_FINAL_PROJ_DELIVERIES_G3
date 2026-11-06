#include <iostream>
#include <string>
#include "deliverytrack.h"

// sets the file name used to store rider delivery status
const std::string fileName = "status.txt"; // can be changed

int main() {

    // initializes the menu selection and delivery information variables
    int select = 0;
    std::string id, address, item;

    while (select != 6) {

        // displays the delivery tracking menu
        printMenu();

        std::cout << "\n\nEnter Option: ";
        std::cin >> select;

        switch (select) {
            case 1:
                // displays all rider records
                std::cout << "All Riders:\n";
                viewAll(fileName);
                break;

            case 2:
                // displays riders who are on standby
                std::cout << "Riders on Standby:\n";
                viewReady(fileName);
                break;

            case 3:
                // displays riders currently on delivery
                std::cout << "All Riders:\n\n";
                viewOnDelivery(fileName);
                break;

            case 4:
                // completes a delivery using the entered rider ID
                std::cout << "Complete a Delivery:\n\n";
                std::cout << "Enter ID number (****/no need for #): ";
                std::cin >> id;

                // updates the rider's delivery completion status
                deliverComplete(fileName, id);
                break;

            case 5:
                // starts a delivery using the entered rider ID
                std::cout << "Start a Delivery:\n\n";
                std::cout << "Enter ID number (****/no need for #): ";
                std::cin >> id;

                // gets the delivery destination address
                std::cout << "Enter Address: ";
                std::cin.ignore();
                std::getline(std::cin, address);

                // gets the name of the item being delivered
                std::cout << "Enter Item Name: ";
                std::getline(std::cin, item);

                // updates the rider's delivery information
                deliverStart(fileName, id, address, item);
                break;

            case 6:
                // displays the exit message
                std::cout << "Exiting..." << std::endl;
                break;

            default:
                // displays a message for an invalid menu selection
                std::cout << "Invalid choice!" << std::endl;
        }
    }

    return 0;
}
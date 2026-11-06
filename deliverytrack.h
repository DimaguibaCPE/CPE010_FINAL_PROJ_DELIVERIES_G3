#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "function.h"

#ifndef DELIVERYTRACK_H
#define DELIVERYTRACK_H

/*
How the delivery tracker works:

View functions:
- Displays all riders regardless of status
- Displays riders with Ready status
- Displays riders with On-Delivery status
- Checks a specific rider's status using their ID

Update functions:
- Updates rider information in the file
- Changes rider status to On-Delivery when a delivery starts
- Changes rider status to Ready when a delivery is completed
- Tracks delivery completion

The tracker uses linear search to find and update rider records.

Note: Enter rider IDs and statuses without adding "#" or "Status:".
Example: "0001", "Ready"
*/

// displays the delivery tracker menu
void printMenu(){

    std::cout<<"DELIVERY TRACKER\n";
    line();
    std::cout<<"(1) Rider Directory\n";
    
    std::cout<<"(2) Riders on Standby\n";
    
    std::cout<<"(3) Riders on Delivery\n";
    
    std::cout<<"(4) Complete a Delivery\n";

    std::cout<<"(0) Return to Main Menu\n";

    line();
    std::cout << "Enter your selection: ";

    return;
}

// =================== VIEW FUNCTIONS =================

// displays all rider records from the file
void viewAll(std::string filelappelle){
    
    std::ifstream file(filelappelle);

    if(!file.is_open()){
        std::cout << "Error: Failed to open file.\n";
        return;
    }

    // reads each line from the file
    std::string toView;
    bool found = false;
    while(std::getline(file, toView)){
        if (!toView.empty()) {
            std::cout << toView << std::endl;
            found = true;
        }
    }

    // displays a message if no rider records are found
    if (!found) {
        std::cout << "No drivers found." << std::endl;
    }
    file.close();
}

// displays riders with Ready status
void viewReady(std::string filelappelle){
    std::ifstream file(filelappelle);

    if(!file.is_open()){
        std::cout<<"Failed to open file!"<<std::endl;
        return;
    }

    // filters records with Ready status
    std::string readyLine;
    std::string readTill = ", Status:";
    std::string status = "Status: Ready";

    bool found = false;
    
    while(std::getline(file, readyLine)){
        if(readyLine.find(status) != std::string::npos){
            // displays the rider information before the status field
            std::cout << readyLine.substr(0, readyLine.find(readTill)) << std::endl;
            found = true;
            }
        }       

    // displays a message if no riders are on standby
    if (!found) {
        std::cout << "No drivers on standby." << std::endl;
    }

    file.close();
}

// displays riders with On-Delivery status
void viewOnDelivery(std::string filelappelle){ 
    std::ifstream file(filelappelle);

    if(!file.is_open()){
        std::cout<<"Failed to open file!"<<std::endl;
        return;
    }

    // filters records with On-Delivery status
    std::string onDel;
    std::string readTill = ", Status:";
    std::string readFrom = "Item: ";
    std::string status = "Status: On-Delivery";

    bool found = false;

    while(std::getline(file, onDel)){    
        if (onDel.find(status) != std::string::npos) {

            // displays rider information before the status field
            std::cout << onDel.substr(
                0, onDel.find(readTill)
            ) << " ";

            // displays the item information
            std::cout << onDel.substr(
                onDel.find(readFrom), std::string::npos
            ) << std::endl;

            found = true;
            }
        }   
                
    // displays a message if no riders are on delivery
    if (!found) {
        std::cout << "No drivers currently on delivery." << std::endl;
    }

    file.close();
}

// ================= END OF VIEW FUNCTIONS ==========

// ===================== UPDATE FUNCTIONS =========================

// updates a rider's status, item, and address in the file
void updateStats(std::string filelappelle, std::string id, std::string stat, std::string address, std::string item){
    std::ifstream reader(filelappelle);

    if(!reader.is_open()){
        std::cout<<"Failed to open file!"<<std::endl;
        return;
    }
    
    // stores the file contents before rewriting
    std::vector<std::string> heldLines;
    std::string holdLine;
    std::string search = "ID: #" + id + ",";
    bool foundIt = false;

    while(std::getline(reader, holdLine)){
        
        // searches for the rider ID
        if(holdLine.find(search) != std::string::npos){
            std::size_t posStat = holdLine.find("Status:");
            std::size_t posItem = holdLine.find("Item: ");
            std::size_t posAdd = holdLine.find("Address: ");

            // checks if all required fields exist
            if(posStat != std::string::npos && posItem != std::string::npos && posAdd != std::string::npos){
                holdLine.replace(posStat, posItem - posStat - 2, "Status: " + stat);

                // updates field positions after replacing the status
                posItem = holdLine.find("Item: ");
                posAdd = holdLine.find("Address: ");

                holdLine.replace(posItem, posAdd - posItem - 2, "Item: " + item);
                posAdd = holdLine.find("Address: ");

                // updates the address field
                holdLine.replace(posAdd, std::string::npos, "Address: " + address);
                foundIt = true;
            }
        }

        // stores the current line for rewriting
        heldLines.push_back(holdLine);
    }

    reader.close();

    // stops the update if the rider ID is not found
    if(!foundIt){
        std::cout << "Error: Rider not found.\n";
        return;
    }
   
    // rewrites the updated file contents
    std::ofstream writer(filelappelle);

    if(!writer.is_open()){
        std::cout<< "Error for Writing Step"<<std::endl;
        return;
    }

    // writes each stored line back to the file
    for(const std::string& update : heldLines){
        writer<< update << '\n';
    }

    writer.close();

    // confirms that the rider information was updated
    std::cout << "Rider " << id << " status successfully updated.\n";

}

// updates a rider's status when a delivery is completed
void deliverComplete(std::string filelappelle, std::string id){

    // checks if the rider has On-Delivery status
    std::ifstream reader(filelappelle);

    if(!reader.is_open()){
        std::cout<<"Failed to open file!"<<std::endl;
        return;
    }
    
    std::string line;
    std::string search = "ID: #" + id + ",";
    bool foundIt = false;

    while(std::getline(reader, line)){
        
        // searches for the rider ID
        if(line.find(search) != std::string::npos){
            foundIt = true;

            // resets the rider's status if they are on delivery
            if(line.find("Status: On-Delivery") != std::string::npos){
                reader.close(); 
                updateStats(filelappelle, id, "Ready", "None", "None");
                return; 
            }
            else{
                
                // prevents completion if the rider is not on delivery
                std::cout<<"Rider is not doing a delivery!"<<std::endl;
                reader.close();
                return; 
            }
            
        }
    }   

    reader.close(); 

    // displays an error if the rider ID is not found
    if(!foundIt){
        std::cout<<"Rider not found or Missing!"<<std::endl;
        return;
    }
}

#endif

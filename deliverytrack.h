#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#ifndef DELIVERYTRACK_H
#define DELIVERYTRACK_H

/*Algo/How it works:

Tracker Updater:

View functions:
- View all riders (Regardless of status)
-View all Riders available(Status: Ready)
-View all Riders on delivery(Status: On Delivery)


Update functions:
-System updates files
-If rider assigned, change status(Status: On Delivery)
-If delivery finished, change status (Status: Ready)
- Delivery completion tracking
IN SHORT: It's basically linear search with extra steps lol

NOTE: When calling Status or ID no need to add "#" or "Status:". Type as is ex.("0001", "Ready"). 
*/

//Menu

void printMenu(){

    std::cout<<"<==========DELIVERY TRACKER==========>\n\n";

    std::cout<<"1. Rider Directory\n\n";
    
    std::cout<<"2.Riders on Standby\n\n";
    
    std::cout<<"3. Riders on Delivery\n\n";
    
    std::cout<<"4. Complete a Delivery\n\n";

    std::cout<<"5. Start a Delivery\n\n";

    std::cout<<"6. Exit\n\n";

    return;
}

//===================VIEW FUNCTIONS=================

//readAll (Bound to change)
void viewAll(std::string filelappelle){ //l'apple("to call") >:D
    
    std::ifstream file(filelappelle);

    if(!file.is_open()){
        std::cout<<"Failed to open file!"<<std::endl;
        return;
    }

    //read each line
    std::string toView;
    while(std::getline(file, toView)){
        std::cout<<toView<<std::endl;
    }
    file.close();
}

//read those with status "Ready" (Bound to change)
void viewReady(std::string filelappelle){
    std::ifstream file(filelappelle);

    if(!file.is_open()){
        std::cout<<"Failed to open file!"<<std::endl;
        return;
    }

    //read only those with "Ready" status:
    std::string readyLine;
    std::string readTill = ", Status:";
    std::string status = "Status: Ready";
    
    while(std::getline(file, readyLine)){
        if(readyLine.find(status) != std::string::npos){ //
            std::cout<<readyLine.substr(0, readyLine.find(readTill))<<std::endl;
            }
        }       

    file.close();
}

void viewOnDelivery(std::string filelappelle){ 
    std::ifstream file(filelappelle);

    if(!file.is_open()){
        std::cout<<"Failed to open file!"<<std::endl;
        return;
    }

    //read only those with "On-Delivery" status:
    std::string onDel;
    std::string readTill = ", Status:";
    std::string readFrom = "Item: ";
    std::string status = "Status: On-Delivery"; //reads only till ID

    while(std::getline(file, onDel)){    
        if(onDel.find(status) != std::string::npos){ //
            std::cout<<onDel.substr(0, onDel.find(readTill))<<" ";
            std::cout<<onDel.substr(onDel.find(readFrom), std::string::npos)<<std::endl;
            }
        }           


    file.close();
}

//=================END OF VIEW FUNCTIONS==========

//=====================UPDATE FUNCTIONS=========================

//update helper function
void updateStats(std::string filelappelle, std::string id, std::string stat, std::string address, std::string item){
    std::ifstream reader(filelappelle);

    if(!reader.is_open()){
        std::cout<<"Failed to open file!"<<std::endl;
        return;
    }
    
    std::vector<std::string> heldLines; //Scales automatically much dynamic than using new file
    std::string holdLine;
    std::string search = "ID: #" + id + ",";
    bool foundIt = false;

    while(std::getline(reader, holdLine)){
        //ID search
        if(holdLine.find(search) != std::string::npos){
            std::size_t posStat = holdLine.find("Status:");
            std::size_t posItem = holdLine.find("Item: ");
            std::size_t posAdd = holdLine.find("Address: ");
            //checks if there are missing variables
            if(posStat != std::string::npos && posItem != std::string::npos && posAdd != std::string::npos){
                holdLine.replace(posStat, posItem - posStat - 2, "Status: " + stat);
                //resets the positions, because they keep doing weird things
                posItem = holdLine.find("Item: ");
                posAdd = holdLine.find("Address: ");

                holdLine.replace(posItem, posAdd - posItem - 2, "Item: " + item);
                posAdd = holdLine.find("Address: ");

                holdLine.replace(posAdd, std::string::npos, "Address: " + address);
                foundIt = true; // if found
            }
        }
        heldLines.push_back(holdLine);
    }

     reader.close(); //close it to switch into write mode.

    if(!foundIt){
        std::cout<<"Rider not found or Missing!"<<std::endl;
        return;
    }
   
    //Rewrite part

    //open in write mode this time
    std::ofstream writer(filelappelle);
    if(!writer.is_open()){
        std::cout<< "Error for Writing Step"<<std::endl;
        return;
    }

    for(const std::string& update : heldLines){ //called a range based for loop iterates all lines vector to write
        writer<< update << '\n';
    }

    writer.close();

    std::cout<<"Rider "<<id<< " Status updated."<<std::endl;

}

void deliverStart(std::string filelappelle, std::string id, std::string address, std::string item){

    //checks if rider status is status : ready
    std::ifstream reader(filelappelle);

    if(!reader.is_open()){
        std::cout<<"Failed to open file!"<<std::endl;
        return;
    }
    
    std::string line;
    std::string search = "ID: #" + id + ",";
    bool foundIt = false;

    while(std::getline(reader, line)){
        //ID search
        if(line.find(search) != std::string::npos){
            foundIt = true;
            if(line.find("Status: Ready") != std::string::npos){
                reader.close(); 
                updateStats(filelappelle, id, "On-Delivery", address, item);
            }
            else{
                std::cout<<"Rider already on delivery!"<<std::endl;
                reader.close();
            }
            return;   
        }
    }   

     reader.close(); 

    if(!foundIt){
        std::cout<<"Rider not found or Missing!"<<std::endl;
        return;
    }
}

void deliverComplete(std::string filelappelle, std::string id){ //no need for other parameters since set to "None"

    //checks if rider status is status : on-delivery
    std::ifstream reader(filelappelle);

    if(!reader.is_open()){
        std::cout<<"Failed to open file!"<<std::endl;
        return;
    }
    
    std::string line;
    std::string search = "ID: #" + id + ",";
    bool foundIt = false;

    while(std::getline(reader, line)){
        //ID search
        if(line.find(search) != std::string::npos){
            foundIt = true;
            if(line.find("Status: On-Delivery") != std::string::npos){
                reader.close(); 
                updateStats(filelappelle, id, "Ready", "None", "None");
                return; 
            }
            else{
                std::cout<<"Rider is not doing a delivery!"<<std::endl;
                reader.close();
                return; 
            }
            
        }
    }   
     reader.close(); 

    if(!foundIt){
        std::cout<<"Rider not found or Missing!"<<std::endl;
        return;
    }
}

#endif

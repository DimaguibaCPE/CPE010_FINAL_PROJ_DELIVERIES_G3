#include <iostream>
#include <string>
#include "deliverytrack.h"


const std::string fileName = "test.txt"; // can be changed

int main(){
    int select = 0;
    std::string id;

    while(select != 6){
    printMenu();

    std::cout<<"\n\nEnter Option: ";
    std::cin>>select;

    switch(select){
        case 1:
            std::cout<<"All Riders:\n";
            viewAll(fileName);
            break;
        
        case 2:
            std::cout<<"Riders on Standby:\n";
            viewReady(fileName);
            break;
        
        case 3:
            std::cout<<"All Riders:\n\n";
            viewOnDelivery(fileName);
            break;
        
        case 4:
        std::cout<<"Complete a Delivery:\n\n";
        std::cout<<"Enter ID number (****/no need for #): ";
        std::cin>>id;
            deliverComplete(fileName, id);
            break;
        
        case 5:
            std::cout<<"Start a Delivery:\n\n";
            std::cout<<"Enter ID number (****/no need for #): ";
            std::cin>>id;
            deliverStart(fileName, id);
            break;
        
        case 6:
        std::cout<<"Exiting..."<<std::endl;
            break;

        default:
            std::cout<<"Invalid choice!"<<std::endl;

    }

    }
    
    return 0;
}
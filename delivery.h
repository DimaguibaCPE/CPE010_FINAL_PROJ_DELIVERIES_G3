#ifndef DELIVERY_H
#define DELIVERY_H

#include <string>
#include <vector>
#include <queue>
#include <unordered_set>

// information lists
struct Delivery {
    std::string deliveryID;
    std::string customerName;
    std::string contactNumber;
    std::string parcelDescription;

    std::string origin;
    std::string destination;

    std::string priority;
    std::string status;

    std::string assignedRider;
};

class DeliveryManager {
private:
    // this stores all registered deliveries
    std::vector<Delivery> deliveries;

    // stores the id of all existing delivery record
    std::queue<std::string> pendingDeliveries;

    // prevents duplicate delivery id
    std::unordered_set<std::string> usedDeliveryIDs;

    // this generates unique delivery id
    std::string generateDeliveryID();

public:
    // registers a new delivery record
    void addDeliveryRecord();
    // displays alll registered delivery records
    void viewDeliveryRecords() const;

    // access existing record
    const std::vector<Delivery>& getAllDeliveries() const;

    // gets the next pending delivery without removing it
    bool peekNextPendingDeliveryID(std::string& id) const;

    // gets and removes the next pending delivery
    bool popNextPendingDeliveryID(std::string& id);

    bool assignDelivery(const std::string& deliveryID, const std::string& riderName);
};

// provides access to the shared delivery manager
DeliveryManager& getDeliveryManager();

#endif

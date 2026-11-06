#ifndef DELIVERY_H
#define DELIVERY_H

#include <string>
#include <vector>
#include <queue>
#include <unordered_set>

// stores the information of a delivery record
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
    // stores all registered deliveries
    std::vector<Delivery> deliveries;

    // stores the IDs of pending deliveries in FIFO order
    std::queue<std::string> pendingDeliveries;

    // prevents duplicate delivery id
    std::unordered_set<std::string> usedDeliveryIDs;

    // generates a unique ID for a new delivery record
    std::string generateDeliveryID();

public:
    // registers a new delivery record
    void addDeliveryRecord();

    // displays all registered delivery records
    void viewDeliveryRecords() const;

    // searches for a delivery record using its ID
    void searchDeliveryRecord() const;

    // edits an existing pending delivery record
    void editDeliveryRecord();

    // cancels an existing pending delivery record
    void cancelDeliveryRecord();

    // returns all registered delivery records
    const std::vector<Delivery>& getAllDeliveries() const;

    // gets the next pending delivery without removing it
    bool peekNextPendingDeliveryID(std::string& id) const;

    // gets and removes the next pending delivery
    bool popNextPendingDeliveryID(std::string& id);

    // assigns a pending delivery to a rider
    bool assignDelivery(const std::string& deliveryID, const std::string& riderName);
};

// provides access to the shared delivery manager
DeliveryManager& getDeliveryManager();

#endif
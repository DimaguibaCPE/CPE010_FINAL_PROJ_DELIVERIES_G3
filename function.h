#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>

// declares the dispatcher login settings
extern const std::string DISPATCHER_USERNAME;
extern const std::string DISPATCHER_PASSWORD;
extern const int MAX_LOGIN_ATTEMPTS;

// declares screen display and user input functions
void clearScreen();
void line();
void pauseScreen();
int readMenuChoice(int minChoice, int maxChoice);

// declares login and main application navigation functions
bool login();
void runApplication();
void showMainMenu();
void openFeature(int choice);

// declares the main screens for each application feature
void showAddDeliveryScreen();
void showManageDeliveriesScreen();
void showRiderManagementScreen();
void showDeliveryAssignmentScreen();
void showDeliveryTrackingScreen();
void showRouteOptimizationScreen();

// declares delivery and rider management submenu functions
void showManageDeliveriesMenu();
void showRiderManagementMenu();

// declares functions for saving and loading rider records
void saveRidersToFile();
void loadRidersFromFile();

// declares delivery assignment functions for viewing and assigning deliveries
void viewPendingDeliveries();
void viewAvailableRiders();
void assignDeliveryManually();
void autoAssignDeliveries();

// declares functions for rider workload and delivery priority evaluation
int findAvailableRiderWithLowestWorkload();
int getPriorityValue(const std::string& priority);

#endif
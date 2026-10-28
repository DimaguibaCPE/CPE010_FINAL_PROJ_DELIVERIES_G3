#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>

// Login settings
extern const std::string DISPATCHER_USERNAME;
extern const std::string DISPATCHER_PASSWORD;
extern const int MAX_LOGIN_ATTEMPTS;

// Screen and input functions
void clearScreen();
void line();
void pauseScreen();
int readMenuChoice(int minChoice, int maxChoice);

// Login and application navigation
bool login();
void runApplication();
void showMainMenu();
void openFeature(int choice);

// Feature screens
void showAddDeliveryScreen();
void showManageDeliveriesScreen();
void showRiderManagementScreen();
void showDeliveryAssignmentScreen();
void showDeliveryTrackingScreen();
void showRouteOptimizationScreen();

// Feature submenus
void showManageDeliveriesMenu();
void showRiderManagementMenu();

#endif
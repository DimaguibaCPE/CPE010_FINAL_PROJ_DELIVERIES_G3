ROUTE OPTIMIZER FOR DELIVERY DRIVERS:


TEST LOGIN CREDENTIALS:

- Username: dispatcher
- Password: 12345


HOW TO COMPILE:
- Run: g++ -std=c++11 main.cpp function.cpp delivery.cpp routeOptimization.cpp -o RouteOptimizer

HOW TO RUN:
- Run: ./RouteOptimizer


PROGRESS 1: LOGIN SYSTEM AND INITIAL MENU LAYOUT – ASAJAR

- Developed a dispatcher login system using predefined username and password credentials.
- Implemented a maximum of three login attempts for account access.
- Designed the main menu with navigation to the planned system features.
- Added basic input validation, logout, and exit functionality.


PROGRESS 2: DELIVERY TRACKING – DIMAGUIBA

- Implemented a delivery tracking feature that allows users to view rider information.
- Added functionality to check rider availability and current delivery status.
- Allows users to start a delivery by updating the rider's status.
- Allows users to mark a delivery as completed by returning the rider's status to available.


PROGRESS 3: RIDER MANAGEMENT – GRESOLA, GARBOSA

- Implemented functionality to add new riders to the system.
- Added functionality to view rider information using their assigned ID.
- Allows users to update existing rider information.
- Added functionality to update rider availability.
- Implemented automatic rider ID generation and file-based storage for rider records.


PROGRESS 4: DELIVERY INFORMATION REGISTRATION – GRESOLA

- Implemented functionality to create and register new delivery records.
- Records customer information, contact number, parcel description, destination, and delivery priority.
- Added automatic delivery ID generation for each registered delivery.
- Allows users to view existing delivery records.
- Implemented initial delivery status tracking, with newly registered deliveries marked as pending.


PROGRESS 5: DELIVERY ASSIGNMENT – GARBOSA

- Implemented functionality to view pending deliveries and available riders.
- Allows users to manually assign pending deliveries to available riders.
- Added automatic delivery assignment based on delivery priority and rider workload.
- Prioritizes high-priority deliveries during automatic assignment.
- Integrates rider workload tracking to help distribute deliveries among available riders.
- Workload adjustment upon delivery completion still requires further improvement.


PROGRESS 6: ROUTE OPTIMIZATION – GODINO

- Implemented a graph-based system to represent road connections between predefined locations in Quezon City.
- Applied Dijkstra's algorithm to calculate the shortest paths between delivery locations.
- Added a nearest-stop approach to recommend an efficient order for multiple delivery destinations.
- Integrated delivery records to identify pending deliveries and their corresponding destinations.
- Calculates and displays the recommended delivery sequence, individual travel distances, and total route distance.
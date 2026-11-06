**ROUTE OPTIMIZER FOR DELIVERY DRIVERS:**


**TEST LOGIN CREDENTIALS:**

- Username: dispatcher
- Password: 12345


**HOW TO COMPILE:**
- Run: g++ -std=c++11 main.cpp function.cpp delivery.cpp routeOptimization.cpp -o RouteOptimizer

**HOW TO RUN:**
- Run: ./RouteOptimizer


**PROGRESS 1: LOGIN SYSTEM AND INITIAL MENU LAYOUT – ASAJAR**

- Developed a dispatcher login system using predefined username and password credentials.
- Implemented a maximum of three login attempts for account access.
- Designed the main menu with navigation to the planned system features.
- Added basic input validation, logout, and exit functionality.


**PROGRESS 2: DELIVERY TRACKING – DIMAGUIBA**

- Implemented a delivery tracking feature that allows users to view rider information.
- Added functionality to check rider availability and current delivery status.
- Allows users to start a delivery by updating the rider's status.
- Allows users to mark a delivery as completed by returning the rider's status to available.


**PROGRESS 3: RIDER MANAGEMENT – GGARBOSA**

- Implemented functionality to add new riders to the system.
- Added functionality to view rider information using their assigned ID.
- Allows users to update existing rider information.
- Added functionality to update rider availability.
- Implemented automatic rider ID generation and file-based storage for rider records.


**PROGRESS 4: DELIVERY INFORMATION REGISTRATION – GRESOLA**

- Implemented functionality to create and register new delivery records.
- Records customer information, contact number, parcel description, destination, and delivery priority.
- Added automatic delivery ID generation for each registered delivery.
- Allows users to view existing delivery records.
- Implemented initial delivery status tracking, with newly registered deliveries marked as pending.


**PROGRESS 5: ROUTE OPTIMIZATION – GODINO**

- Implemented a graph-based system to represent road connections between predefined locations in Quezon City.
- Applied Dijkstra's algorithm to calculate the shortest paths between delivery locations.
- Added a nearest-stop approach to recommend an efficient order for multiple delivery destinations.
- Integrated delivery records to identify pending deliveries and their corresponding destinations.


**PROGRESS 6: DELIVERY ASSIGNMENT – GARBOSA**

- Implemented functionality to view pending deliveries and available riders.
- Allows users to manually assign pending deliveries to available riders.
- Added automatic delivery assignment based on delivery priority and rider workload.
- Prioritizes high-priority deliveries during automatic assignment.
- Integrates rider workload tracking to help distribute deliveries among available riders.
- Workload adjustment upon delivery completion still requires further improvement.


**PROGRESS 7: DELIVERY RECORD MANAGEMENT – ASAJAR**

- Implemented functionality to search for existing delivery records using their assigned Delivery ID.
- Added functionality to update delivery information, including customer name, contact number, parcel description, destination, and priority.
- Allows users to cancel pending delivery records by updating their status to “Cancelled” instead of permanently deleting them.
- Added restrictions to allow editing and cancellation only for deliveries with Pending status.
- Integrated the search, edit, and cancel functions into the Manage Delivery Records submenu.
- Added descriptive code comments and improved code formatting and indentation for better readability and maintainability.

**ADDITIONAL NOTES:**

- **View Pending Deliveries and Available Riders** – The dispatcher can view pending deliveries (deliveries that have not yet been picked up) and check which riders are currently available.

- **Manual Assignment** – The dispatcher can manually select a delivery and assign an available rider to it.

- **Automatic Assignment** – The system automatically assigns riders based on the delivery priority and the riders' current workloads.

- **Workload Calculation** – The workload is based on the number of deliveries each rider has completed. For high-priority deliveries, the system prioritizes riders with the lowest workload when assigning deliveries.

- **Delivery Completion** – The dispatcher can mark an order as completed through the Delivery Tracker once the delivery has been successfully completed.

- **Rider Availability** – Riders marked as unavailable cannot be assigned new deliveries. This is different from the On Delivery status. The unavailable status is intended for situations where a rider is temporarily not available for work or cannot accept new delivery assignments.

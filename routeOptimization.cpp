#include "routeOptimization.h"
#include "delivery.h"
#include "function.h"

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <functional>
#include <limits>
#include <string>
#include <cctype>

using namespace std;

namespace {
    // sets the maximum possible distance as infinity
    const double INF = numeric_limits<double>::max();

    // converts a location name to lowercase for case-insensitive comparison
    string normalizeName(string name) {
        for (size_t i = 0; i < name.length(); ++i) {
            name[i] = static_cast<char>(
                tolower(static_cast<unsigned char>(name[i]))
            );
        }
        return name;
    }
}

// initializes the predefined Quezon City locations and sample road connections
RouteOptimizer::RouteOptimizer() {
    
    // defines the simulated delivery locations
    locations = {
        "Central",          // 0 - simulated main warehouse
        "Pinyahan",         // 1
        "Kamuning",         // 2
        "Sacred Heart",     // 3
        "UP Campus",        // 4
        "Krus na Ligas",    // 5
        "Loyola Heights",   // 6
        "Bagumbayan"        // 7
    };

    // creates a graph entry for each predefined location
    graph.resize(locations.size());

    // adds demonstration road connections with sample distances in kilometers
    // these distances are illustrative and are not verified road distances
    addRoad(0, 1, 2.0);
    addRoad(0, 2, 3.0);
    addRoad(1, 2, 2.5);
    addRoad(1, 3, 2.0);
    addRoad(1, 4, 4.0);
    addRoad(2, 3, 1.5);
    addRoad(2, 5, 4.0);
    addRoad(3, 5, 3.0);
    addRoad(4, 5, 2.5);
    addRoad(4, 6, 3.0);
    addRoad(5, 6, 2.0);
    addRoad(6, 7, 4.0);
    addRoad(5, 7, 5.0);
}

// adds a road connection between two valid locations
void RouteOptimizer::addRoad(int from, int to, double distance) {
    // checks whether the locations and distance are valid
    if (from < 0 || to < 0 ||
        from >= static_cast<int>(locations.size()) ||
        to >= static_cast<int>(locations.size()) ||
        distance < 0.0) {
        return;
    }

    // adds the road connection in both directions
    graph[from].push_back(Road{to, distance});
    graph[to].push_back(Road{from, distance});
}

// finds a location by name using case-insensitive comparison
int RouteOptimizer::findLocation(const string& name) const {
    // normalizes the searched location name
    const string searchName = normalizeName(name);

    // checks each predefined location for a matching name
    for (size_t i = 0; i < locations.size(); ++i) {
        if (normalizeName(locations[i]) == searchName) {
            return static_cast<int>(i);
        }
    }

    // returns -1 if the location is not found
    return -1;
}

// calculates the shortest distances from a starting location using Dijkstra's algorithm
void RouteOptimizer::dijkstra(
    int start,
    vector<double>& distances,
    vector<int>& parents
) const {
    
    // gets the total number of locations
    const int n = static_cast<int>(locations.size());

    // initializes all distances to infinity and parents to -1
    distances.assign(n, INF);
    parents.assign(n, -1);

    // defines the priority queue node format
    typedef pair<double, int> Node;

    // creates a min-priority queue based on distance
    priority_queue<Node, vector<Node>, greater<Node> > pq;

    // sets the starting location distance to zero
    distances[start] = 0.0;

    // adds the starting location to the priority queue
    pq.push(Node(0.0, start));

    // processes locations until the priority queue is empty
    while (!pq.empty()) {
        
        // gets the current shortest distance and location
        const double currentDistance = pq.top().first;
        const int current = pq.top().second;
        pq.pop();

        // skips the node if a shorter distance was already found
        if (currentDistance > distances[current]) {
            continue;
        }

        // checks all roads connected to the current location
        for (size_t i = 0; i < graph[current].size(); ++i) {
            const Road& road = graph[current][i];

            // gets the next location and calculates the new distance
            const int next = road.destination;
            const double newDistance = currentDistance + road.distance;

            // updates the shortest distance if a better path is found
            if (newDistance < distances[next]) {
                distances[next] = newDistance;

                // records the previous location in the shortest path
                parents[next] = current;

                // adds the updated location to the priority queue
                pq.push(Node(newDistance, next));
            }
        }
    }
}

// creates a recommended delivery route using the nearest-stop approach
OptimizedRoute RouteOptimizer::optimize(
    const vector<int>& deliveryStops,
    const vector<vector<string> >& deliveryIDs
) const {
    
    // initializes the route result
    OptimizedRoute result;
    result.totalDistance = 0.0;
    result.complete = true;

    // copies the delivery stops that still need to be visited
    vector<int> remaining = deliveryStops;

    // sets the simulated Main Warehouse as the starting location
    int current = 0; // Simulated Main Warehouse: Barangay Central

    // continues selecting the nearest remaining delivery stop
    while (!remaining.empty()) {
        
        // calculates shortest distances from the current location
        vector<double> distances;
        vector<int> parents;
        dijkstra(current, distances, parents);

        // initializes the nearest stop selection
        int nearestIndex = -1;
        double nearestDistance = INF;

        // searches for the nearest reachable remaining delivery stop
        for (size_t i = 0; i < remaining.size(); ++i) {
            const int stop = remaining[i];

            // checks whether the stop is valid and closer than the current nearest stop
            if (stop >= 0 && stop < static_cast<int>(distances.size()) &&
                distances[stop] < nearestDistance) {
                nearestDistance = distances[stop];
                nearestIndex = static_cast<int>(i);
            }
        }

        // stops route generation if no reachable stop is found
        if (nearestIndex == -1 || nearestDistance == INF) {
            result.complete = false;
            break;
        }

        // gets the selected next delivery stop
        const int nextStop = remaining[nearestIndex];

        // stores the shortest path to the selected stop
        vector<int> path;
        int node = nextStop;

        // traces the path backward using the parent locations
        while (node != -1) {
            path.push_back(node);

            // stops tracing when the current starting location is reached
            if (node == current) {
                break;
            }

            // moves to the previous location in the shortest path
            node = parents[node];
        }

        // reverses the path to display it from start to destination
        reverse(path.begin(), path.end());

        // creates the result information for the selected stop
        OptimizedStop stopResult;
        stopResult.locationID = nextStop;
        stopResult.distanceFromPrevious = nearestDistance;
        stopResult.path = path;

        // matches the selected stop with its corresponding delivery IDs
        for (size_t i = 0; i < deliveryStops.size() && i < deliveryIDs.size(); ++i) {
            if (deliveryStops[i] == nextStop) {
                stopResult.deliveryIDs = deliveryIDs[i];
                break;
            }
        }

        // adds the selected stop to the optimized route
        result.stops.push_back(stopResult);

        // adds the distance to the total route distance
        result.totalDistance += nearestDistance;

        // updates the current location to the selected stop
        current = nextStop;

        // removes the visited stop from the remaining stops
        remaining.erase(remaining.begin() + nearestIndex);
    }

    // returns the generated route result
    return result;
}

// displays the predefined delivery locations
void RouteOptimizer::displayLocations() const {
    
    // displays the location list heading
    std::cout << "\n============================================================\n";
    std::cout << "           PREDEFINED DELIVERY BARANGAYS\n";
    std::cout << "============================================================\n";

    // displays each location with its assigned index
    for (size_t i = 0; i < locations.size(); ++i) {
        cout << "(" << i << ") " << locations[i];

        // identifies the simulated warehouse location
        if (i == 0) {
            cout << " (Simulated Main Warehouse)";
        }

        cout << "\n";
    }

    // displays the separator line
    line();
}

// displays the predefined road connections and their distances
void RouteOptimizer::displayRoads() const {
    
    // displays the road connection heading
    cout << "\nSAMPLE ROAD CONNECTIONS\n";

    // checks each location's connected roads
    for (size_t i = 0; i < graph.size(); ++i) {
        for (size_t j = 0; j < graph[i].size(); ++j) {
            const int destination = graph[i][j].destination;

            // displays each undirected road connection only once
            if (static_cast<int>(i) < destination) {
                cout << locations[i] << " <-> "
                     << locations[destination] << " : "
                     << graph[i][j].distance << " km\n";
            }
        }
    }
}

// returns the name of a location based on its ID
string RouteOptimizer::getLocationName(int id) const {
    
    // checks whether the location ID is valid
    if (id < 0 || id >= static_cast<int>(locations.size())) {
        return "Unknown Location";
    }

    // returns the corresponding location name
    return locations[id];
}

// returns the total number of predefined locations
int RouteOptimizer::getLocationCount() const {
    
    // converts the location count to an integer
    return static_cast<int>(locations.size());
}

// displays the route optimization menu and handles route generation
void showRouteOptimizationScreen() {
    
    // creates the route optimizer object
    RouteOptimizer optimizer;

    // stores the user's menu selection
    int choice = 0;

    // continues displaying the menu until the user returns
    do {
        
        // clears the screen before displaying the menu
        clearScreen();
        cout << "\n";
        line();

        // displays the route optimization menu
        cout << "              ROUTE OPTIMIZATION\n";
        line();
        cout << "(1) View Locations\n";
        cout << "(2) View Road Connections\n";
        cout << "(3) Optimize Pending Deliveries\n";
        cout << "(0) Return to Main Menu\n";
        line();
        cout << "Enter your selection: ";

        // gets the user's menu choice
        choice = readMenuChoice(0, 3);

        // displays the predefined locations
        if (choice == 1) {
            clearScreen();
            optimizer.displayLocations();
            pauseScreen();

        // displays the sample road connections
        } else if (choice == 2) {
            clearScreen();
            optimizer.displayRoads();
            pauseScreen();

        // generates a recommended route for pending deliveries
        } else if (choice == 3) {
            clearScreen();

            // displays the optimized route heading
            std::cout << "\n============================================================\n";
            std::cout << "           OPTIMIzE DELIVERY ROUTE\n";
            std::cout << "============================================================\n";

            // gets all delivery records
            const vector<Delivery>& deliveries =
                getDeliveryManager().getAllDeliveries();

            // stores the delivery location IDs and their associated delivery IDs
            vector<int> stops;
            vector<vector<string> > deliveryIDs;

            // checks each delivery record
            for (size_t i = 0; i < deliveries.size(); ++i) {
                
                // skips deliveries that are not pending
                if (normalizeName(deliveries[i].status) != "pending") {
                    continue;
                }

                // initializes the destination location ID
                int locationID = -1;

                // matches the delivery destination with a predefined location
                for (int j = 1; j < optimizer.getLocationCount(); ++j) {
                    if (normalizeName(deliveries[i].destination) ==
                        normalizeName(optimizer.getLocationName(j))) {
                        locationID = j;
                        break;
                    }
                }

                // displays a message if the destination is not recognized
                if (locationID <= 0) {
                    cout << "Delivery " << deliveries[i].deliveryID
                         << " has an unrecognized destination: "
                         << deliveries[i].destination << "\n";
                    continue;
                }

                // checks whether the destination is already included in the stops
                bool alreadyAdded = false;
                for (size_t j = 0; j < stops.size(); ++j) {
                    if (stops[j] == locationID) {
                        // adds the delivery ID to the existing destination
                        deliveryIDs[j].push_back(deliveries[i].deliveryID);
                        alreadyAdded = true;
                        break;
                    }
                }

                // adds a new destination if it has not been included yet
                if (!alreadyAdded) {
                    stops.push_back(locationID);

                    // stores the delivery ID for the new destination
                    vector<string> ids;
                    ids.push_back(deliveries[i].deliveryID);
                    deliveryIDs.push_back(ids);
                }
            }

            // checks whether any matching pending deliveries were found
            if (stops.empty()) {
                cout << "No matching pending deliveries found.\n";
                cout << "Use one of the predefined barangay names as the destination.\n";
            } else {
                
                // displays the route starting point and number of locations
                cout << "Starting Point: Central (Simulated Main Warehouse)\n";
                cout << "Pending Delivery Locations: " << stops.size() << "\n";

                // generates the recommended delivery route
                const OptimizedRoute plan = optimizer.optimize(stops, deliveryIDs);

                // displays the recommended delivery order
                cout << "\nRECOMMENDED DELIVERY ORDER\n";
                line();

                // displays each stop included in the route
                for (size_t i = 0; i < plan.stops.size(); ++i) {
                    const OptimizedStop& stop = plan.stops[i];

                    // displays the stop number and location name
                    cout << "\nStop " << i + 1 << ": "
                         << optimizer.getLocationName(stop.locationID) << "\n";

                    // displays the delivery IDs assigned to the stop
                    cout << "Delivery ID(s): ";

                    for (size_t j = 0; j < stop.deliveryIDs.size(); ++j) {
                        if (j > 0) cout << ", ";
                        cout << stop.deliveryIDs[j];
                    }

                    // displays the shortest path to the current stop
                    cout << "\nShortest path: ";
                    
                    for (size_t j = 0; j < stop.path.size(); ++j) {
                        if (j > 0) cout << " -> ";
                        cout << optimizer.getLocationName(stop.path[j]);
                    }

                    // displays the distance from the previous stop
                    cout << "\nDistance from previous stop: "
                         << stop.distanceFromPrevious << " km\n";
                }

                // displays the total route distance and completion status
                cout << "\n";
                line();
                cout << "Total Route Distance: " << plan.totalDistance << " km\n";

                // displays whether all matching deliveries were included
                if (plan.complete) {
                    cout << "All matching deliveries have been included in the route.\n";
                } else {
                    cout << "Some delivery locations could not be reached.\n";
                }
            }

            // pauses the screen before returning to the menu
            pauseScreen();
        }
    } while (choice != 0);
}

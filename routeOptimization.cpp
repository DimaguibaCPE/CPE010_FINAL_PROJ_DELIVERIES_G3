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
    const double INF = numeric_limits<double>::max();

    string normalizeName(string name) {
        for (size_t i = 0; i < name.length(); ++i) {
            name[i] = static_cast<char>(
                tolower(static_cast<unsigned char>(name[i]))
            );
        }
        return name;
    }
}

// Predefined Quezon City barangays and illustrative road distances.
RouteOptimizer::RouteOptimizer() {
    locations = {
        "Central",          // 0 - simulated Main Warehouse
        "Pinyahan",         // 1
        "Kamuning",         // 2
        "Sacred Heart",     // 3
        "UP Campus",        // 4
        "Krus na Ligas",    // 5
        "Loyola Heights",   // 6
        "Bagumbayan"        // 7
    };

    graph.resize(locations.size());

    // Demonstration weights in kilometers; not verified road distances.
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

void RouteOptimizer::addRoad(int from, int to, double distance) {
    if (from < 0 || to < 0 ||
        from >= static_cast<int>(locations.size()) ||
        to >= static_cast<int>(locations.size()) ||
        distance < 0.0) {
        return;
    }

    graph[from].push_back(Road{to, distance});
    graph[to].push_back(Road{from, distance});
}

int RouteOptimizer::findLocation(const string& name) const {
    const string searchName = normalizeName(name);

    for (size_t i = 0; i < locations.size(); ++i) {
        if (normalizeName(locations[i]) == searchName) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void RouteOptimizer::dijkstra(
    int start,
    vector<double>& distances,
    vector<int>& parents
) const {
    const int n = static_cast<int>(locations.size());
    distances.assign(n, INF);
    parents.assign(n, -1);

    typedef pair<double, int> Node;
    priority_queue<Node, vector<Node>, greater<Node> > pq;

    distances[start] = 0.0;
    pq.push(Node(0.0, start));

    while (!pq.empty()) {
        const double currentDistance = pq.top().first;
        const int current = pq.top().second;
        pq.pop();

        if (currentDistance > distances[current]) {
            continue;
        }

        for (size_t i = 0; i < graph[current].size(); ++i) {
            const Road& road = graph[current][i];
            const int next = road.destination;
            const double newDistance = currentDistance + road.distance;

            if (newDistance < distances[next]) {
                distances[next] = newDistance;
                parents[next] = current;
                pq.push(Node(newDistance, next));
            }
        }
    }
}

OptimizedRoute RouteOptimizer::optimize(
    const vector<int>& deliveryStops,
    const vector<vector<string> >& deliveryIDs
) const {
    OptimizedRoute result;
    result.totalDistance = 0.0;
    result.complete = true;

    vector<int> remaining = deliveryStops;
    int current = 0; // Simulated Main Warehouse: Barangay Central

    while (!remaining.empty()) {
        vector<double> distances;
        vector<int> parents;
        dijkstra(current, distances, parents);

        int nearestIndex = -1;
        double nearestDistance = INF;

        for (size_t i = 0; i < remaining.size(); ++i) {
            const int stop = remaining[i];
            if (stop >= 0 && stop < static_cast<int>(distances.size()) &&
                distances[stop] < nearestDistance) {
                nearestDistance = distances[stop];
                nearestIndex = static_cast<int>(i);
            }
        }

        if (nearestIndex == -1 || nearestDistance == INF) {
            result.complete = false;
            break;
        }

        const int nextStop = remaining[nearestIndex];
        vector<int> path;
        int node = nextStop;

        while (node != -1) {
            path.push_back(node);
            if (node == current) {
                break;
            }
            node = parents[node];
        }

        reverse(path.begin(), path.end());

        OptimizedStop stopResult;
        stopResult.locationID = nextStop;
        stopResult.distanceFromPrevious = nearestDistance;
        stopResult.path = path;

        for (size_t i = 0; i < deliveryStops.size() && i < deliveryIDs.size(); ++i) {
            if (deliveryStops[i] == nextStop) {
                stopResult.deliveryIDs = deliveryIDs[i];
                break;
            }
        }

        result.stops.push_back(stopResult);
        result.totalDistance += nearestDistance;
        current = nextStop;
        remaining.erase(remaining.begin() + nearestIndex);
    }

    return result;
}

void RouteOptimizer::displayLocations() const {
    cout << "\nPREDEFINED DELIVERY BARANGAYS\n";
    for (size_t i = 0; i < locations.size(); ++i) {
        cout << "[" << i << "] " << locations[i];
        if (i == 0) {
            cout << " (Simulated Main Warehouse)";
        }
        cout << "\n";
    }
}

void RouteOptimizer::displayRoads() const {
    cout << "\nSAMPLE ROAD CONNECTIONS\n";
    for (size_t i = 0; i < graph.size(); ++i) {
        for (size_t j = 0; j < graph[i].size(); ++j) {
            const int destination = graph[i][j].destination;
            if (static_cast<int>(i) < destination) {
                cout << locations[i] << " <-> "
                     << locations[destination] << " : "
                     << graph[i][j].distance << " km\n";
            }
        }
    }
}

string RouteOptimizer::getLocationName(int id) const {
    if (id < 0 || id >= static_cast<int>(locations.size())) {
        return "Unknown Location";
    }
    return locations[id];
}

int RouteOptimizer::getLocationCount() const {
    return static_cast<int>(locations.size());
}

void showRouteOptimizationScreen() {
    RouteOptimizer optimizer;
    int choice = 0;

    do {
        clearScreen();
        cout << "\n";
        line();
        cout << "              ROUTE OPTIMIZATION\n";
        line();
        cout << "(1) View Locations\n";
        cout << "(2) View Road Connections\n";
        cout << "(3) Optimize Pending Deliveries\n";
        cout << "(0) Return to Main Menu\n";
        line();
        cout << "Enter your selection: ";

        choice = readMenuChoice(0, 3);

        if (choice == 1) {
            clearScreen();
            optimizer.displayLocations();
            pauseScreen();
        } else if (choice == 2) {
            clearScreen();
            optimizer.displayRoads();
            pauseScreen();
        } else if (choice == 3) {
            clearScreen();
            cout << "OPTIMIZED DELIVERY ROUTE\n";
            line();

            const vector<Delivery>& deliveries =
                getDeliveryManager().getAllDeliveries();

            vector<int> stops;
            vector<vector<string> > deliveryIDs;

            for (size_t i = 0; i < deliveries.size(); ++i) {
                if (normalizeName(deliveries[i].status) != "pending") {
                    continue;
                }

                int locationID = -1;
                for (int j = 1; j < optimizer.getLocationCount(); ++j) {
                    if (normalizeName(deliveries[i].destination) ==
                        normalizeName(optimizer.getLocationName(j))) {
                        locationID = j;
                        break;
                    }
                }

                if (locationID <= 0) {
                    cout << "Delivery " << deliveries[i].deliveryID
                         << " has an unrecognized destination: "
                         << deliveries[i].destination << "\n";
                    continue;
                }

                bool alreadyAdded = false;
                for (size_t j = 0; j < stops.size(); ++j) {
                    if (stops[j] == locationID) {
                        deliveryIDs[j].push_back(deliveries[i].deliveryID);
                        alreadyAdded = true;
                        break;
                    }
                }

                if (!alreadyAdded) {
                    stops.push_back(locationID);
                    vector<string> ids;
                    ids.push_back(deliveries[i].deliveryID);
                    deliveryIDs.push_back(ids);
                }
            }

            if (stops.empty()) {
                cout << "No matching pending deliveries found.\n";
                cout << "Use one of the predefined barangay names as the destination.\n";
            } else {
                cout << "Starting Point: Central (Simulated Main Warehouse)\n";
                cout << "Pending Delivery Locations: " << stops.size() << "\n";

                const OptimizedRoute plan = optimizer.optimize(stops, deliveryIDs);
                cout << "\nRECOMMENDED DELIVERY ORDER\n";
                line();

                for (size_t i = 0; i < plan.stops.size(); ++i) {
                    const OptimizedStop& stop = plan.stops[i];

                    cout << "\nStop " << i + 1 << ": "
                         << optimizer.getLocationName(stop.locationID) << "\n";
                    cout << "Delivery ID(s): ";

                    for (size_t j = 0; j < stop.deliveryIDs.size(); ++j) {
                        if (j > 0) cout << ", ";
                        cout << stop.deliveryIDs[j];
                    }

                    cout << "\nShortest path: ";
                    for (size_t j = 0; j < stop.path.size(); ++j) {
                        if (j > 0) cout << " -> ";
                        cout << optimizer.getLocationName(stop.path[j]);
                    }

                    cout << "\nDistance from previous stop: "
                         << stop.distanceFromPrevious << " km\n";
                }

                cout << "\n";
                line();
                cout << "Total Route Distance: " << plan.totalDistance << " km\n";
                if (plan.complete) {
                    cout << "All matching deliveries have been included in the route.\n";
                } else {
                    cout << "Some delivery locations could not be reached.\n";
                }
            }
            pauseScreen();
        }
    } while (choice != 0);
}

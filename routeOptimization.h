#ifndef ROUTEOPTIMIZATION_H
#define ROUTEOPTIMIZATION_H

#include <string>
#include <vector>

// stores the destination and distance of a road connection
struct Road {
    int destination;
    double distance;
};

// stores the details of an optimized delivery stop
struct OptimizedStop {
    int locationID;
    double distanceFromPrevious;
    std::vector<int> path;
    std::vector<std::string> deliveryIDs;
};

// stores the complete optimized route information
struct OptimizedRoute {
    double totalDistance;
    bool complete;
    std::vector<OptimizedStop> stops;
};

// manages route optimization and location information
class RouteOptimizer {
private:
    // stores the predefined delivery location names
    std::vector<std::string> locations;

    // stores the road connections between locations
    std::vector<std::vector<Road> > graph;

    // finds a location index using its name
    int findLocation(const std::string& name) const;

    // calculates the shortest distances using Dijkstra's algorithm
    void dijkstra(
        int start,
        std::vector<double>& distances,
        std::vector<int>& parents
    ) const;

public:
    // initializes the predefined locations and road connections
    RouteOptimizer();

    // adds a road connection between two locations
    void addRoad(int from, int to, double distance);

    // displays the predefined delivery locations
    void displayLocations() const;

    // displays the road connections and their distances
    void displayRoads() const;

    // generates a recommended route for the given delivery stops
    OptimizedRoute optimize(
        const std::vector<int>& deliveryStops,
        const std::vector<std::vector<std::string> >& deliveryIDs
    ) const;

    // returns the name of a location using its ID
    std::string getLocationName(int id) const;

    // returns the total number of predefined locations
    int getLocationCount() const;
};

// displays the route optimization screen and handles user selections
void showRouteOptimizationScreen();

#endif
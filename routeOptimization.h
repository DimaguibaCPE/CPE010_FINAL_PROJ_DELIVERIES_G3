#ifndef ROUTEOPTIMIZATION_H
#define ROUTEOPTIMIZATION_H

#include <string>
#include <vector>

struct Road {
    int destination;
    double distance;
};

struct OptimizedStop {
    int locationID;
    double distanceFromPrevious;
    std::vector<int> path;
    std::vector<std::string> deliveryIDs;
};

struct OptimizedRoute {
    double totalDistance;
    bool complete;
    std::vector<OptimizedStop> stops;
};

class RouteOptimizer {
private:
    std::vector<std::string> locations;
    std::vector<std::vector<Road> > graph;

    int findLocation(const std::string& name) const;
    void dijkstra(
        int start,
        std::vector<double>& distances,
        std::vector<int>& parents
    ) const;

public:
    RouteOptimizer();

    void addRoad(int from, int to, double distance);
    void displayLocations() const;
    void displayRoads() const;

    OptimizedRoute optimize(
        const std::vector<int>& deliveryStops,
        const std::vector<std::vector<std::string> >& deliveryIDs
    ) const;

    std::string getLocationName(int id) const;
    int getLocationCount() const;
};

void showRouteOptimizationScreen();

#endif

//
//  Route.h
//  EVERouter
//
//  Created by Sario O. Alvey on 17/01/2013.
//  Copyright (c) 2013 Lars Kroll. All rights reserved.
//

#ifndef __EVERouter__Route__
#define __EVERouter__Route__

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#import "SolarSystem.h"

using namespace std;

class Route {
private:
    vector<SolarSystem*> path;
    set<int> waypoints;
public:
    Route(const vector<SolarSystem*> p) { path = p; }
    Route(const vector<SolarSystem*> p, const set<int> wps) { path = p; waypoints = wps; }
    Route() {}
    const vector<SolarSystem*> getPath() { return path; }
    const set<int> getWaypoints() { return waypoints; }
    void setWaypoints(const set<int> wps) { waypoints = wps; }
    void addWaypoint(const int wp) { waypoints.insert(wp); }
    SolarSystem* getStart() { return path[0]; }
    SolarSystem* getEnd() { return path[path.size()-1]; }
    const vector<SolarSystem*>::size_type length() { return path.size(); }
    
    Route* operator+(Route *other);
    void operator+=(Route *other);
    //void swap(Route & other);
    Route& operator=(Route &other);
    Route* reverse();
    
    friend std::ostream& operator<< (std::ostream& stream, const Route& route);
    
};

#endif /* defined(__EVERouter__Route__) */

//
//  Route.cpp
//  EVERouter
//
//  Created by Sario O. Alvey on 17/01/2013.
//  Copyright (c) 2013 Lars Kroll. All rights reserved.
//

#include "Route.h"

void Route::operator+=(Route *other) {
    SolarSystem* myEnd = path.back();
    SolarSystem* otherFirst = other->path.front();
    if (myEnd->systemId == otherFirst->systemId) {
        path.insert(path.end(), ++(other->path.begin()), other->path.end());
    } else {
        path.insert(path.end(), other->path.begin(), other->path.end());
    }
}

//void Route::swap(Route & other) {
//    //std::swap(path, other.path);
//}

Route& Route::operator=(Route &other) {
    
    path.clear();
    path.insert(path.end(), other.path.begin(), other.path.end());
    
    return *this;
}

Route* Route::operator+(Route *other) {
    vector<SolarSystem*> newpath (path.size()+other->path.size());
    
    newpath.insert(newpath.begin(), path.begin(), path.end());
    newpath.insert(newpath.end(), other->path.begin(), other->path.end());
    
    return new Route(newpath);
}

Route* Route::reverse() {
    vector<SolarSystem*> newPath;
    copy(path.begin(), path.end(), back_inserter(newPath));
    std::reverse(newPath.begin(), newPath.end());
    
    return new Route(newPath);
}

std::ostream& operator<< (std::ostream& stream, const Route& route) {
    
    for (int i = 0; i < route.path.size(); ++i) {
        SolarSystem *sys = route.path[i];
        if (sys !=  NULL) {
            if (route.waypoints.count(sys->systemId) == 1 ) {
                stream << i << ". - " << (*sys) << " - " << endl;
            } else {
                stream << i << ". " << (*sys) << endl;
            }
        } else {
            stream << "ERROR: System is NULL" << endl;
        }
    }
    return stream;
}
//
//  SolarSystem.h
//  EVERouter
//
//  Created by Sario O. Alvey on 16/01/2013.
//  Copyright (c) 2013 Lars Kroll. All rights reserved.
//

#ifndef __EVERouter__SolarSystem__
#define __EVERouter__SolarSystem__

#include <iostream>
#include <vector>
#include <set>
#import "SpacePoint.h"

using namespace std;

class SolarSystem {
private:
    SpacePoint _position;
    vector<int> gates;
public:
    const int systemId;
    const double security;
    const std::string systemName;
    
    
    SolarSystem(int sysId, std::string name, double dX, double dY, double dZ, double sec) : systemId( sysId ), systemName( name ), security( sec ) {
        _position.set(dX, dY, dY);
    }
    
    void addGate(const int targetId);
    const vector<int> getGates() { return gates; }
    
    double distance(const SolarSystem *other);
    const double getDisplaySecurity() const;
    
    friend std::ostream& operator<< (std::ostream& stream, const SolarSystem& sys);
    friend std::ostream& operator<< (std::ostream& stream, const set<SolarSystem*>& sysSet);
};

#endif /* defined(__EVERouter__SolarSystem__) */

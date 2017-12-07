//
//  SolarSystem.cpp
//  EVERouter
//
//  Created by Sario O. Alvey on 16/01/2013.
//  Copyright (c) 2013 Lars Kroll. All rights reserved.
//

#include "SolarSystem.h"

void SolarSystem::addGate(const int targetId) {
    gates.push_back(targetId);
}

double SolarSystem::distance(const SolarSystem *other) {
    return _position.distance(other->_position);
}

const double SolarSystem::getDisplaySecurity() const {
    double sec = security;
    double tensec = 10.0*sec;
    int tentruesec = (int)(tensec+0.5); //round
    return ((double)tentruesec)/10.0;
}

std::ostream& operator<< (std::ostream& stream, const SolarSystem& sys) {
    
    stream << sys.systemName << " (";
    stream << sys.getDisplaySecurity() << ")";
    
//    for (int i = 0; i < route.path.size(); ++i) {
//        SolarSystem *sys = route.path[i];
//        if (sys !=  NULL) {
//            if (route.waypoints.count(sys->systemId) == 1 ) {
//                stream << i << ". - " << (*sys) << " - " << endl;
//            } else {
//                stream << i << ". " << (*sys) << endl;
//            }
//        } else {
//            stream << "ERROR: System is NULL" << endl;
//        }
//    }
    return stream;
}

std::ostream& operator<< (std::ostream& stream, const set<SolarSystem*>& sysSet) {
    set<SolarSystem*>::iterator it;
    stream << "(";
    for (it = sysSet.begin(); it != sysSet.end(); ++it) {
        SolarSystem *sys = *it;
        if (sys !=  NULL) {
           stream << (*sys) << ", ";
        } else {
            stream << "ERROR: System is NULL" << endl;
        }
    }
    stream << ")";
    return stream;
}
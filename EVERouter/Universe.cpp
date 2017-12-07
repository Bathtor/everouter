//
//  Universe.cpp
//  EVERouter
//
//  Created by Sario O. Alvey on 16/01/2013.
//  Copyright (c) 2013 Lars Kroll. All rights reserved.
//

#include "Universe.h"
#include "TopKSet.h"

bool isHighSec(SolarSystem* sys) {
    double sec = sys->security;
    double tensec = 10.0*sec;
    int tentruesec = (int)(tensec+0.5); //round
    return tentruesec >= 5;
}

void Universe::addSystem(SolarSystem *sys) {
    systems.push_back(sys);
    systemMap[sys->systemId] = sys;
    systemNameMap[sys->systemName] = sys;
    if (isHighSec(sys)) {
        highsec.insert(sys->systemId);
    }
}

SolarSystem* Universe::getSystem(const int sysId) {
    try {
        return systemMap.at(sysId);
    } catch (out_of_range) {
        return NULL;
    }
}

void Universe::addJumpGate(const int sysId1, const int sysId2) {
    try {
        SolarSystem* sys1 = systemMap.at(sysId1);
        if (systemMap.count(sysId2) == 1) {
            //SolarSystem* sys2 = systemMap.at(sysId2);
            
            sys1->addGate(sysId2);
        } else {
            printf("ERROR: Could not find system for %d", sysId2);
        }
        //sys2->addGate(sysId1);
    } catch (out_of_range) {
        printf("ERROR: Could not find system for %d", sysId1);
    }
}

vector<SolarSystem*> *Universe::findSystems(const string namePrefix) {
    map<string, SolarSystem*>::const_iterator lower = systemNameMap.lower_bound(namePrefix);
    vector<SolarSystem*> *sysvec = new vector<SolarSystem*>();
    while (true) {
        if (lower != systemNameMap.end()) {
            const string& key = lower->first;
            if (key.compare(0, namePrefix.size(), namePrefix) == 0) {
                sysvec->push_back(lower->second);
                lower++;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    return sysvec;
}

Route* Universe::route(const int startId, const int endId) {
    if (preferHighsec) {
        //just use the other function so not to maintain the code twice
        set<int> endPoint;
        endPoint.insert(endId);
        map<int, Route*> route = routeTo(startId, endPoint);
        return route[endId];
    }
    map<int, int> dist;
    set<int> last;
    int distance;
     
    dist[startId] = 0;
    
    last.insert(startId);
    distance = 0;
    
    while (last.count(endId) == 0) {
        distance++;
        set<int> next;
        for (set<int>::iterator it = last.begin(); it != last.end(); ++it) {
            int cur = *it;
            SolarSystem *current = getSystem(cur);
            vector<int> gates = current->getGates();
            for (int i = 0; i < gates.size(); ++i) {
                int neigh = gates[i];
                //SolarSystem *neighbour = getSystem(neigh);
                if (dist.count(neigh) == 0) {
                    dist[neigh] = distance;
                    next.insert(neigh);
                }
            }
        }
        last = next;
    }
    
    // Start reconstructing the path backwards
    SolarSystem *current = getSystem(endId);
    vector<SolarSystem*> path;
    int curDist = dist[current->systemId];
    path.push_back(current);
    
    while (curDist > 0) {
        vector<int> neighbours = current->getGates();
        int leastDist = INT_MAX;
        int leastNeigh = -1;
        for (int i = 0; i < neighbours.size(); ++i) {
            int neighbour = neighbours[i];
            if (dist.count(neighbour) == 1) {
                int neighDist = dist[neighbour];
                if (neighDist < leastDist) {
                    leastDist = neighDist;
                    leastNeigh = neighbour;
                }
            }
        }
        if (leastNeigh == -1) {
            cout << "ERROR: Could not find path!" << endl;
            return NULL;
        }
        current = getSystem(leastNeigh);
        path.push_back(current);
        curDist = leastDist;
    }
    
    reverse(path.begin(), path.end());
    
    return new Route(path);
    

}

map<int, Route*> Universe::routeTo(const int startId, set<int> endPoints) {
    bool tmpPrefHS = preferHighsec;
    if (tmpPrefHS) {
        if (highsec.count(startId) == 0) {
            tmpPrefHS = false;
            cout << "Can't find a route in Highsec from a non-highsec system: ";
            cout << getSystem(startId)->systemName << endl;            
        } else {
            for (set<int>::iterator it = endPoints.begin(); it != endPoints.end(); ++it) {
                int epId = *it;
                if (highsec.count(epId) == 0) {
                    tmpPrefHS = false;
                    cout << "Can't find a route in Highsec to a non-highsec system: ";
                    cout << getSystem(epId)->systemName << endl;
                }
            }
        }
    }
    map<int, int> dist;
    set<int> last;
    int distance;
    map<int, int>::size_type lastDistSize;
    
    dist[startId] = 0;
    lastDistSize = 0;
    
    last.insert(startId);
    distance = 0;
    
    
    set<int> todo;
    set_difference(endPoints.begin(), endPoints.end(), last.begin(), last.end(), inserter(todo, todo.begin()));
    
    
    while (todo.size() > 0) {
        if (dist.size() == lastDistSize) { //We didn't make any progress last round
                                           //cout << "Couldn't find a route in pure highsec." << endl;
            //Start over including low- and null-sec
            tmpPrefHS = false;
            last.clear();
            last.insert(startId);
            distance = 0;
            dist.clear();
            dist[startId] = 0;
            lastDistSize = 0;
            todo.clear();
            set_difference(endPoints.begin(), endPoints.end(), last.begin(), last.end(), inserter(todo, todo.begin()));            
        }
        lastDistSize = dist.size();
        
        distance++;
        set<int> next;
        for (set<int>::iterator it = last.begin(); it != last.end(); ++it) {
            int cur = *it;
            SolarSystem *current = getSystem(cur);
            if (tmpPrefHS && !isHighSec(current)) {
                continue;
            }
            vector<int> gates = current->getGates();
            for (int i = 0; i < gates.size(); ++i) {
                int neigh = gates[i];
                //SolarSystem *neighbour = getSystem(neigh);
                if (dist.count(neigh) == 0) {
                    dist[neigh] = distance;
                    next.insert(neigh);
                }
            }
        }
        last = next;
        set<int> temp_todo;
        set_difference(todo.begin(), todo.end(), last.begin(), last.end(), inserter(temp_todo, temp_todo.begin()));
        temp_todo.swap(todo);
    }
    
    // Start reconstructing the path backwards
    map<int, Route*> routes;
    for (set<int>::iterator it = endPoints.begin(); it != endPoints.end(); ++it) {
        int endId = *it;
        SolarSystem *current = getSystem(endId);
        vector<SolarSystem*> path;
        int curDist = dist[current->systemId];
        path.push_back(current);
        
        while (curDist > 0) {
            vector<int> neighbours = current->getGates();
            int leastDist = INT_MAX;
            int leastNeigh = -1;
            for (int i = 0; i < neighbours.size(); ++i) {
                int neighbour = neighbours[i];
                if (dist.count(neighbour) == 1) {
                    int neighDist = dist[neighbour];
                    if (neighDist < leastDist) {
                        leastDist = neighDist;
                        leastNeigh = neighbour;
                    }
                }
            }
            if (leastNeigh == -1) {
                cout << "ERROR: Could not find path!" << endl;
                routes.clear();
                return routes;
            }
            current = getSystem(leastNeigh);
            path.push_back(current);
            curDist = leastDist;
        }
        
        reverse(path.begin(), path.end());
        
        routes[endId] = new Route(path);
        routes[endId]->addWaypoint(endId);
    }
    return routes;
}

Route* Universe::routeVia(const int startId, const set<int> midPoints, const int endId) {
    map<int, Route*> startRoutes = routeTo(startId, midPoints);
    map<int, Route*> endRoutes = routeTo(endId, midPoints);
    map<int, map<int, Route*> > midRoutes;
    for (set<int>::iterator it = midPoints.begin(); it != midPoints.end(); ++it) {
        int cur = *it;
        set<int> otherPoints;
        copy(midPoints.begin(), midPoints.end(), inserter(otherPoints, otherPoints.begin()));
        otherPoints.erase(cur);
        //map<int, Route*> curRoutes = routeTo(cur, otherPoints);
        midRoutes[cur] = routeTo(cur, otherPoints);
    }
    //cout << "Calculated all route combinations of " << midPoints.size() << " Midpoints" << endl;
    // Now to the n! complexity part...Make this shit efficient da fuq!
    int minLength = INT_MAX;
    Route *minRoute = new Route();
    vector<int> tempMidPoints;
    copy(midPoints.begin(), midPoints.end(), back_inserter(tempMidPoints));
    sort(tempMidPoints.begin(), tempMidPoints.end());
    //cout << "Trying all " << tempMidPoints.size() << "! combinations" << endl;
    do {
        //cout << "Trying order: ";
//        for (int i = 0; i < tempMidPoints.size(); ++i) {
//            cout << getSystem(tempMidPoints[i])->systemName << " ";
//        }
        //cout << "...";
        vector<int>::iterator firstIt = tempMidPoints.begin();
        int curPoint = *firstIt;
        Route *curRoute = startRoutes[curPoint];        
        for (vector<int>::iterator it = ++firstIt; it != tempMidPoints.end(); ++it) {
            int nextPoint = *it;
            (*curRoute) += midRoutes[curPoint][nextPoint];
            if (curRoute->length() >= minLength) {
                break;
            }
            curPoint = nextPoint;
        }
        if (curRoute->length() >= minLength) {
            //cout << "already too long" << endl;
            continue;
        }
        Route *endRoute = endRoutes[curPoint]->reverse();
        (*curRoute) += endRoute;
        if (curRoute->length() < minLength) {
            minLength = (int) curRoute->length(); //Unlikely that a route will be longer than an int
            (*minRoute) = (*curRoute);
        } 
        //cout << "found route of length " << curRoute->length() << "(min route length: " << minRoute->length() << '/' << minLength << ")" << endl;

    } while (next_permutation(tempMidPoints.begin(), tempMidPoints.end()));
    
    minRoute->setWaypoints(midPoints);
    minRoute->addWaypoint(endId);
    
    return minRoute;
}

Route* Universe::route(vector<int> sysIds) {
    if (sysIds.size() < 2) {
        cout << "Can't route to self!" << endl;
        return NULL;
    }
    
    Route *accRoute = new Route();
    
    for (int i = 1; i < sysIds.size(); i++) {
        int src = sysIds[i-1];
        int dest = sysIds[i];
        Route *nextRoute = route(src, dest);
        if (nextRoute == NULL) {
            return NULL;
        }
        Route *newRoute = (*accRoute) + nextRoute;
        delete accRoute;
        delete nextRoute;
        accRoute = newRoute;
        
    }
    return accRoute;
}

void Universe::findCenter(const bool lowsec, set<SolarSystem*> *systems95, set<SolarSystem*> *systems90,
                          unsigned long* maxJumps95, unsigned long* maxJumps90) {
    TopKSet<unsigned long>::size_type perc95 = highsec.size()*0.05;
    TopKSet<unsigned long>::size_type perc90 = highsec.size()*0.10;
    cout << "Taking top-" << perc95 << " for 95% and top-" << perc90 << " for 90% (" << highsec.size() << "highsec systems)" << endl;
    unsigned long minMaxJumps95 = INT_MAX;
    unsigned long minMaxJumps90 = INT_MAX;
    unsigned long counter = 1;
    set<int>::iterator it;
    for (it = highsec.begin(); it != highsec.end(); ++it) {
        cout << "Processing system " << counter << " of " << highsec.size() << endl;
        TopKSet<unsigned long> top95(perc95);
        TopKSet<unsigned long> top90(perc90);
        int sysId = *it;
        set<int>::iterator jt;
        for (jt = highsec.begin(); jt != highsec.end(); ++jt) {
            int otherSysId = *jt;
            if (otherSysId == sysId) {
                continue;
            }
            Route* r = route(sysId, otherSysId);
            top95.insert(r->length());
            top90.insert(r->length());
            delete r;
            
            if (top90.full() && (top95.min() > minMaxJumps95) && (top90.min() > minMaxJumps90)) {
                break;
            }
        }
        SolarSystem* sys = systemMap.at(sysId);
        if (top95.min() < minMaxJumps95) {
            minMaxJumps95 = top95.min();
            systems95->clear();
            systems95->insert(sys);
        }
        if (top95.min() == minMaxJumps95) {
            systems95->insert(sys);
        }
        if (top90.min() < minMaxJumps90) {
            minMaxJumps90 = top90.min();
            systems90->clear();
            systems90->insert(sys);
        }
        if (top90.min() == minMaxJumps90) {
            systems90->insert(sys);
        }

        cout << sys->systemName << " is max. " << top95.min() << " jumps from 95% of all other highsec systems." << endl;
        cout << sys->systemName << " is max. " << top90.min() << " jumps from 90% of all other highsec systems." << endl;
        cout << *systems95 << " are max. " << minMaxJumps95 << " jumps from 95% of all other highsec systems." << endl;
        cout << *systems90 << " are max. " << minMaxJumps90 << " jumps from 90% of all other highsec systems." << endl;
        counter++;
    }

    *maxJumps90 = minMaxJumps90;
    *maxJumps95 = minMaxJumps95;
}
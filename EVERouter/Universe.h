//
//  Universe.h
//  EVERouter
//
//  Created by Sario O. Alvey on 16/01/2013.
//  Copyright (c) 2013 Lars Kroll. All rights reserved.
//

#ifndef __EVERouter__Universe__
#define __EVERouter__Universe__

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#import "SolarSystem.h"
#import "Route.h"

using namespace std;

class Universe {
private:
    vector<SolarSystem*> systems;
    map<int, SolarSystem*> systemMap;
    map<string, SolarSystem*> systemNameMap;
    set<int> highsec;
    bool preferHighsec;

public:
    Universe() : preferHighsec(true) {}
    void setPreferHighsec(bool val) { preferHighsec = val; }
    void addSystem(SolarSystem* sys);
    SolarSystem* getSystem(const int sysId);
    vector<SolarSystem*> *findSystems(const string namePrefix);
    void addJumpGate(const int sysId1, const int sysId2);
    Route* route(const int startId, const int endId);
    map<int, Route*> routeTo(const int startId, set<int> endPoints);
    Route* routeVia(const int startId, const set<int> midPoints, const int endId);
    Route* route(vector<int> sysIds);
    void findCenter(const bool lowsec, set<SolarSystem*> *systems95, set<SolarSystem*> *systems90,
                    unsigned long* maxJumps95, unsigned long* maxJumps90);
};

#endif /* defined(__EVERouter__Universe__) */

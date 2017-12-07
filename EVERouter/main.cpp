//
//  main.cpp
//  EVERouter
//
//  Created by Sario O. Alvey on 16/01/2013.
//  Copyright (c) 2013 Lars Kroll. All rights reserved.
//

// STD Stuff
#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <stdexcept>
#include <ctime>
// MySQL Driver Stuff
#include <mysql.h>
//#include <driver.h>
//#include <connection.h>
//#include <statement.h>
//#include <prepared_statement.h>
//#include <resultset.h>
//#include <metadata.h>
//#include <resultset_metadata.h>
//#include <exception.h>
//#include <warning.h>

// My Stuff
#import "SpacePoint.h"
#import "Universe.h"

#define DBHOST "localhost"
#define USER "eve"
#define PASSWORD "853Soupi"
#define DATABASE "EVE"

using namespace std;

int load_data(Universe *verse) {

    MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL *connection, mysql;
	
	int query_state;
    
    mysql_init(&mysql);
    //connection = mysql_real_connect(&mysql,"host","user","password","database",0,0,0);
    connection = mysql_real_connect(&mysql,DBHOST,USER,PASSWORD,DATABASE,0,0,0);
    if (connection == NULL) {
        cout << mysql_error(&mysql) << endl;
        return 1;
    }
    
    query_state = mysql_query(connection, "SELECT solarSystemID,solarSystemName,x,y,z,security FROM mapSolarSystems");
    if (query_state !=0) {
        cout << mysql_error(connection) << endl;
        return 1;
    }
    
    result = mysql_store_result(connection);
    while ( ( row = mysql_fetch_row(result)) != NULL ) {
        int sysId = atoi(row[0]);
        string sysName = row[1];
        double dX = atof(row[2]);
        double dY = atof(row[3]);
        double dZ = atof(row[4]);
        double sec = atof(row[5]);
        
        SolarSystem *sys = new SolarSystem(sysId, sysName, dX, dY, dZ, sec);
        
        verse->addSystem(sys);
        
//        cout << "Solar System: ";
//        cout << sysId << ',';
//        cout << sysName << ',';
//        cout << '(' << dX << ',';
//        cout << dY << ',';
//        cout << dZ << ')'  << ',';
//        cout << sec << endl;
    }
    
    query_state = mysql_query(connection, "SELECT fromSolarSystemID,toSolarSystemID FROM mapSolarSystemJumps");
    if (query_state !=0) {
        cout << mysql_error(connection) << endl;
        return 1;
    }
    
    result = mysql_store_result(connection);
    while ( ( row = mysql_fetch_row(result)) != NULL ) {
        int sysId1 = atoi(row[0]);
        int sysId2 = atoi(row[1]);
        
        
        verse->addJumpGate(sysId1, sysId2);
        SolarSystem *s1, *s2;
        s1 = verse->getSystem(sysId1);
        s2 = verse->getSystem(sysId2);
        //double dist = s1->distance(s2);
        //cout << "Jumps from " << s1->systemName << " to " << s2->systemName << '(' << dist << ')' << endl;
        
    }
    
    mysql_free_result(result);
    mysql_close(connection);
    
    return 0;
}

bool readSystem(Universe *verse, SolarSystem** system) {
    string sysName = "";
    while (true) {
        string input;
        cout << ">" << sysName;
        getline(cin, input);
        sysName = sysName + input;
        if (sysName == "") {
            return false;
        }
        vector<SolarSystem*> *searchRes = verse->findSystems(sysName);
        if (searchRes->size() == 1) {
            (*system) = (*searchRes)[0];
            return true;
        }
        if (searchRes->size() == 0) {
            cout << "No Results. Start over: " << endl;
            sysName = "";
            continue;
        }
        cout << "Input abiguous. Possibilities: " << endl;
        for (int i = 0; i < searchRes->size(); ++i) {
            cout << (*searchRes)[i]->systemName << endl;
        }
    }
}

int main(int argc, const char * argv[])
{
    
    
    cout << "Loading...";
    
    Universe verse;
    
    int ld = load_data(&verse);
    if (ld != 0) {
        return ld;
    }
    cout << "done." << endl << endl;
    
    cout << "****** WELCOME TO EVE ROUTING FOR PROS ******" << endl << endl;
    
//    set<SolarSystem*> centers90;
//    set<SolarSystem*> centers95;
//    unsigned long maxJumps90 = 0;
//    unsigned long maxJumps95 = 0;
//    verse.findCenter(false, &centers90, &centers95, &maxJumps95, &maxJumps90);
//    
//    cout << "Final Result: " << endl;
//    cout << centers95 << " are max. " << maxJumps95 << " jumps from 95% of all other highsec systems." << endl;
//    cout << centers90 << " are max. " << maxJumps90 << " jumps from 90% of all other highsec systems." << endl << endl;
    
    cout << "Please choose a start-system: " << endl;
    SolarSystem *start = nullptr;
    if (!readSystem(&verse, &start)) {
        cout << "No start system - no route. Exiting." << endl;
        return 1;
    }
    cout << "Chosen start-system: " << start->systemName << endl;
    
    cout << "Please choose an end-system: " << endl;
    SolarSystem *end = nullptr;
    if (!readSystem(&verse, &end)) {
        cout << "No end system - no route. Exiting." << endl;
        return 1;
    }
    cout << "Chosen end-system: " << end->systemName << endl;
    
    set<int> midpoints;
    while(true) {
        cout << "Do you wish to add more midpoints?(y/n)" << endl;
        string input;
        getline(cin, input);
        if (input == "n") {
            break;
        } else if (input == "y") {
            cout << "Please choose an midpoint-system: " << endl;
            SolarSystem *mp = nullptr;
            if (!readSystem(&verse, &mp)) {
                cout << "No end system - no route. Exiting." << endl;
                return 1;
            }
            cout << "Chosen end-system: " << mp->systemName << endl;
            midpoints.insert(mp->systemId);
        } else {
            cout << "Invalid input: " << input << endl;
        }
    }
    
    cout << endl << "Routing..." << endl;
    
    Route *route;
    if (midpoints.size() == 0) {
        route = verse.route(start->systemId, end->systemId);
    } else {
        route = verse.routeVia(start->systemId, midpoints, end->systemId);
    }
    cout << (*route) << endl;
    
//    cout << "Searching for Amarr...";
//    vector<SolarSystem*> *searchRes = verse.findSystems("Amam");
//    cout << "Got " << searchRes->size() << " results:" << endl;
//    for (int i = 0; i < searchRes->size(); i++) {
//        cout << (*searchRes)[i]->systemName << endl;
//    }
//    
//    cout << "Trying to route from Amarr to Jita and Rens" << endl;
//    vector<SolarSystem*> *searchResAmarr = verse.findSystems("Amarr");
//    SolarSystem *amarr = (*searchResAmarr)[0];
//    vector<SolarSystem*> *searchResJita = verse.findSystems("Jita");
//    SolarSystem *jita = (*searchResJita)[0];
//    vector<SolarSystem*> *searchResRens = verse.findSystems("Rens");
//    SolarSystem *rens = (*searchResRens)[0];
//    set<int> endPoints;
//    endPoints.insert(jita->systemId);
//    endPoints.insert(rens->systemId);
//    
//    map<int, Route*> routes = verse.routeTo(amarr->systemId, endPoints);
//    for (map<int, Route*>::iterator it = routes.begin(); it != routes.end(); ++it) {
//        Route *route = it->second;
//        cout << "Found route of length " << route->length() << " to " << route->getEnd()->systemName << endl;
//        vector<SolarSystem*> path = route->getPath();
//        
//        for (int i = 0; i < path.size(); ++i) {
//            cout << i << ". " << path[i]->systemName << endl;
//        }
//    }
//    
//    cout << "Trying to route from Amarr to Jita via Rens, Hek, Parses and Akhmoh" << endl;
//    vector<SolarSystem*> *searchResHek = verse.findSystems("Hek");
//    SolarSystem *hek = (*searchResHek)[0];
//    vector<SolarSystem*> *searchResParses= verse.findSystems("Parses");
//    SolarSystem *parses = (*searchResParses)[0];
//    vector<SolarSystem*> *searchResAkhmoh = verse.findSystems("Akhmoh");
//    SolarSystem *akhmoh = (*searchResAkhmoh)[0];
//    set<int> midPoints;
//    midPoints.insert(rens->systemId);
//    midPoints.insert(hek->systemId);
//    midPoints.insert(parses->systemId);
//    midPoints.insert(akhmoh->systemId);
//    
//    clock_t begin = clock();
//    Route* shortestRoute = verse.routeVia(amarr->systemId, midPoints, jita->systemId);
//    clock_t end = clock();
//    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
//    
//    cout << "Found route of length " << shortestRoute->length() << " in " << elapsed_secs << "s" << endl;
//
//    cout << (*shortestRoute) << endl;
    //    vector<SolarSystem*> shortestPath = shortestRoute->getPath();
//    
//    for (int i = 0; i < shortestPath.size(); ++i) {
//        SolarSystem *sys = shortestPath[i];
//        if (sys !=  NULL) {
//        cout << i << ". " << sys->systemName << endl;
//        } else {
//            cout << "ERROR: System is NULL" << endl;
//        }
//    }
    return 0;
}






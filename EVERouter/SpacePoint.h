//
//  Point.h
//  EVERouter
//
//  Created by Sario O. Alvey on 16/01/2013.
//  Copyright (c) 2013 Lars Kroll. All rights reserved.
//

#ifndef __EVERouter__Point__
#define __EVERouter__Point__

#include <iostream>

class SpacePoint {
    
private:
    double m_dX;
    double m_dY;
    double m_dZ;
        
    
public:
    SpacePoint() { m_dX = 0.0; m_dY = 0.0; m_dZ = 0.0; }
    SpacePoint(double dX, double dY, double dZ);
    
    void set(double dX, double dY, double dZ) { m_dX = dX; m_dY = dY; m_dZ = dZ; }
    
    double distance(const SpacePoint &other);
    bool operator==(const SpacePoint &other);
    
};

#endif /* defined(__EVERouter__Point__) */

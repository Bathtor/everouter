//
//  Point.cpp
//  EVERouter
//
//  Created by Sario O. Alvey on 16/01/2013.
//  Copyright (c) 2013 Lars Kroll. All rights reserved.
//

#include "SpacePoint.h"
#include <math.h>

SpacePoint::SpacePoint(double dX, double dY, double dZ) {
    m_dX = dX;
    m_dY = dY;
    m_dZ = dZ;
}

double SpacePoint::distance(const SpacePoint &other) {
    double diffSqX, diffSqY, diffSqZ, sum, root;
    diffSqX = pow(m_dX-other.m_dX, 2);
    diffSqY = pow(m_dY-other.m_dY, 2);
    diffSqZ = pow(m_dZ-other.m_dZ, 2);
    sum = diffSqX + diffSqY + diffSqZ;
    root = sqrt(sum);
    
    return root;
}

bool SpacePoint::operator==(const SpacePoint &other) {
    return (m_dX == other.m_dX) && (m_dY == other.m_dY) && (m_dZ == other.m_dZ);
}
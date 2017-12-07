//
//  Tests.m
//  Tests
//
//  Created by Sario O. Alvey on 16/01/2013.
//  Copyright (c) 2013 Lars Kroll. All rights reserved.
//

#import "Tests.h"
#include <iostream>

@implementation Tests

- (void)setUp
{
    [super setUp];
    
    // Set-up code here.
}

- (void)tearDown
{
    // Tear-down code here.
    
    [super tearDown];
}

- (void)testExample
{
    SpacePoint p1(1.0, 1.0, 1.0);
    SpacePoint p11(1.0, 1.0, 1.0);
    SpacePoint p2(1.0, 1.0, 4.0);

    STAssertTrue(!(&p1 == NULL), @"Point 1 should not be nil.");
    STAssertTrue(p1 == p1, @"A point should equal itself.");
    STAssertTrue(p1 == p11, @"A point should equal a point at the same position.");
    STAssertFalse(p1 == p2, @"A point should not equal a point at another position.");
    
    STAssertEquals(p1.distance(p2), 3.0, @"Distance between (1, 1, 1) and (1, 1, 4) should be 3.");
    STAssertEquals(p1.distance(p2), p2.distance(p1), @"SpacePoint distance should be commutative");
}

@end

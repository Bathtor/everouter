//
//  TopKSet.h
//  EVERouter
//
//  Created by Sario O. Alvey on 26/10/2013.
//  Copyright (c) 2013 Lars Kroll. All rights reserved.
//

#ifndef __EVERouter__TopKSet__
#define __EVERouter__TopKSet__

#include <iostream>
#include <vector>
#include <algorithm>

#endif /* defined(__EVERouter__TopKSet__) */

using namespace std;

template <typename T>
class TopKSet {
    
private:
    vector<T> _vals;
public:
    typedef typename vector<T>::size_type size_type;
    const size_type k;
    
    TopKSet(size_type kVal) : k(kVal) {}
    bool insert(T val) {
        _vals.push_back(val);
        sort(_vals.begin(), _vals.end(), greater<T>());
        if (_vals.size() > k) {
            _vals.pop_back();
            return true;
        }
        return false;
    }
    T min() {return _vals.back();}
    T max() {return _vals.front();}
    size_type size() {return _vals.size();};
    bool full() {return (_vals.size() == k);};
};
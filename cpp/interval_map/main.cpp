#include <cstdio>
#include <iostream>
#include <map>

template<typename K, typename V>

class interval_map {
    friend void IntervalMapTest();
    V m_valBegin;
    std::map<K,V> m_map;
public:
    // constructor associates whole range of K with val
    interval_map(V const& val)
    : m_valBegin(val)
    {}

    // Assign value val to interval [keyBegin, keyEnd).
    // Overwrite previous values in this interval.
    // Conforming to the C++ Standard Library conventions, the interval
    // includes keyBegin, but excludes keyEnd.
    // If !( keyBegin < keyEnd ), this designates an empty interval,
    // and assign must do nothing.
    void assign( K const& keyBegin, K const& keyEnd, V const& val ) {

        // consecutive map entries must not contain the same value:

        // 1- Update end values:
        auto uppBound = m_map.lower_bound(keyEnd);
        if ( uppBound == m_map.begin() ) {
             m_map.insert(std::make_pair(keyEnd, m_valBegin)); 
        }
        else { m_map.insert(std::make_pair(keyEnd, (--uppBound)->second)); }

        // 2- Update begin values:
        auto lowBound = m_map.lower_bound(keyBegin);
        if ( lowBound == m_map.begin() || (--lowBound)->second != val ) {
            m_map.insert(std::make_pair(keyBegin, val)); 
        }

        // 3- remove entries in between
        lowBound = m_map.lower_bound(keyBegin);
        if ( m_map[keyBegin] != val ) lowBound--;
        auto midBound = m_map.lower_bound(keyEnd);
        if ( m_map[keyEnd] != val ) midBound--;

        while ( (midBound != lowBound) ){
            m_map.erase(midBound);
            midBound = m_map.lower_bound(keyEnd);
            midBound--;
        }
    }

    // look-up of the value associated with key
    V const& operator[]( K const& key ) const {
        auto it=m_map.upper_bound(key);
        if(it==m_map.begin()) {
            return m_valBegin;
        } else {
            return (--it)->second;
        }
    }

    void print_map(){
        std::cout << "all entries: " << std::endl;
        for (const auto& pair : m_map) {
            std::cout << "(" << K(pair.first) << "," << V(pair.second) << ")" << std::endl;
        }
    }
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.


int main(){
    interval_map<int, char> mymap('A');

    mymap.assign(2, 5, 'C');
    mymap.assign(1, 9, 'B');
    mymap.assign(2, 5, 'C');
    mymap.assign(3, 4, 'D');
    mymap.assign(4, 6, 'D');
    mymap.assign(2, 4, 'D');

    for (int i=0; i<=10; i++){
        printf("%d->%c\n", i, mymap[i]);
    }

    mymap.print_map();
    return 0;
}
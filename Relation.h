//
//  Relation.h
//  Relational_Database_p3_part_1
//
//  Created by William Pinkston on 3/20/17.
//  Copyright © 2017 Xunil_programming. All rights reserved.
//

#ifndef Relation_h
#define Relation_h

#include <string>
#include <vector>
#include <set>

#include "Tuple.h"

class relation
{
public:
    struct tuple_compare {
        bool operator() (const tuple& leftT, const tuple& rightT) const{
            std::string lVal = leftT.tuple::toString();
            std::string rVal = rightT.tuple::toString();
            return lVal < rVal;
        }
    };

    relation(std::string setName, std::vector<std::string> setAttributeMap);
    std::string getName();
    bool addTuple(std::vector<std::string> attributes);
    bool checkTuple(tuple* check);
    bool helperCheck(tuple* check, std::set<tuple, tuple_compare> checkAgainst);
    
    std::set<tuple, tuple_compare> project(std::vector<std::string> columns);
    int findInMap(std::string find);
    
    int getSize();
    
    std::vector<tuple> select(std::string target, std::string criterion);
    
    bool rename(int index, std::string newName);
    
    bool startInner();
    bool innerProject(std::vector<std::string> columns);
    bool innerSelect(int targetIndex, std::string criterion);
    bool innerSelectParallel(std::string map);
    std::set<tuple, tuple_compare> endInner();
    std::vector<std::string> getMap();
    std::vector<std::string> getFullMap();
    bool clearProjectionMap();
    bool resetBaseMap();
protected:
    std::string _name;
    int _numAttributes;
    std::set<tuple, tuple_compare> _tuples;
    std::set<tuple, tuple_compare> _tempTuples;
    std::vector<std::string> attributeMap;
    std::vector<std::string> projectionAttributeMap;
};

#endif /* Relation_h */









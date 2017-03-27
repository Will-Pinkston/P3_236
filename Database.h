//
//  Database.h
//  Relational_database_p3_part_2
//
//  Created by William Pinkston on 3/25/17.
//  Copyright © 2017 Xunil_programming. All rights reserved.
//

#ifndef Database_h
#define Database_h

#include <vector>

#include "Relation.h"
#include "Parser.h"

class database
{
public:
    database(datalogProgram Program);
    std::vector<std::string> getParameters(std::queue<parameter*> holdParameterList);
    
    std::string answerQueries();
    std::string evalQuery(int relationSelect, std::vector<parameter*> parameterList);
    bool evalHelper(std::vector<parameter*>& parameterList, int i, int relationSelect, std::vector<std::string>& projectionHolder);
    
    std::string answerToString(std::set<tuple, relation::tuple_compare> response, std::vector<std::string> responseMap);
private:
    std::vector<relation*> _relations;
    std::queue<predicate> _queries;
};

#endif /* Database_h */

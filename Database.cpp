//
//  Database.cpp
//  Relational_database_p3_part_2
//
//  Created by William Pinkston on 3/25/17.
//  Copyright © 2017 Xunil_programming. All rights reserved.
//

#include "Database.h"

database::database(datalogProgram Program)
{
    std::queue<predicate> Schemes = Program.getSchemes();
    int numSchemes = (int)Schemes.size();
    for (int i = 0; i < numSchemes; i++)
    {
        std::queue<parameter*> holdParameterList = Schemes.front().getParameterList();
        std::vector<std::string> schemeIDs = getParameters(holdParameterList);
        relation* holdRelation = new relation(Schemes.front().getId(), schemeIDs);
        _relations.push_back(holdRelation);
        Schemes.pop();
    }
    
    std::queue<predicate> Facts = Program.getFacts();
    int numFacts = (int)Facts.size();
    for (int i = 0; i < numFacts; i++)
    {
        std::queue<parameter*> holdParameterList = Facts.front().getParameterList();
        std::vector<std::string> factStrings = getParameters(holdParameterList);
        
        std::string factName = Facts.front().getId();
        for (int i = 0; i < _relations.size(); i++)
        {
            if (factName == _relations[i]->getName())
            {
                _relations[i]->addTuple(factStrings);
            }
        }
        Facts.pop();
    }
    
    _queries = Program.getQueries();
}

std::vector<std::string> database::getParameters(std::queue<parameter*> holdParameterList)
{
    std::vector<std::string> parameters;
    int numParams = (int)holdParameterList.size();
    for (int j = 0; j < numParams; j++)
    {
        parameters.push_back(holdParameterList.front()->toString());
        holdParameterList.pop();
    }
    return parameters;
}


std::string database::answerQueries()
{
    std::stringstream retVal;
//    retVal << "\n";
    int numQueries = (int)_queries.size();
    for (int i = 0; i < numQueries; i++)
    {
        std::queue<parameter*> queryParametersQ = _queries.front().getParameterList();
        std::vector<parameter*> queryParameters;
        int numParam = (int)queryParametersQ.size();
        for (int i = 0; i < numParam; i++)
        {
            queryParameters.push_back(queryParametersQ.front());
            queryParametersQ.pop();
        }
        
        retVal << _queries.front().toString() << "?";
        std::string queryName = _queries.front().getId();
        for (int j = 0; j < _relations.size(); j++)
        {
            if (queryName == _relations[j]->getName())
            {
                retVal << evalQuery(j, queryParameters);
            }
        }
        _queries.pop();
    }
    std::string retValStr = retVal.str();
    retValStr = retValStr.substr(0, retValStr.length() - 1);
    return retValStr;
}

std::string database::evalQuery(int relationSelect, std::vector<parameter*> parameterList)
{
    _relations[relationSelect]->clearProjectionMap();
    _relations[relationSelect]->resetBaseMap();
    _relations[relationSelect]->startInner();
    
    std::vector<std::string> projectionHolder;
    
    for (int i = 0; i < parameterList.size(); i++)
    {
        evalHelper(parameterList, i, relationSelect, projectionHolder);
//        std::string paramType = parameterList[i]->getType();
//        if (paramType == "ID")
//        {
//            _relations[relationSelect]->rename(i, parameterList[i]->toString());
//            bool parallel = false;
//            for (int j = 0; j < projectionHolder.size(); j++)
//            {
//                if (parameterList[i]->toString() == projectionHolder[j])
//                {
//                    _relations[relationSelect]->innerSelectParallel(parameterList[i]->toString());
//                    parallel = true;
//                }
//            }
//            if (!parallel) projectionHolder.push_back(parameterList[i]->toString());
//        }
//        else if (paramType == "STRING")
//        {
//            _relations[relationSelect]->innerSelect(i, parameterList[i]->toString());
//        }
    }
    if (projectionHolder.size() != 0)
    {
        _relations[relationSelect]->innerProject(projectionHolder);
    }
    
    std::set<tuple, relation::tuple_compare> response(_relations[relationSelect]->endInner());
//    response = _relations[relationSelect]->endInner();
    std::vector<std::string> responseMap = _relations[relationSelect]->getMap();
    std::string retVal;
    if (responseMap.size() == 0)
    {
        if (response.size() == 0)
        {
            retVal = " No\n";
        }
        else retVal = " Yes(1)\n";
    }
    else retVal = answerToString(response,responseMap);
    return retVal;
}

bool database::evalHelper(std::vector<parameter*>& parameterList, int i, int relationSelect, std::vector<std::string>& projectionHolder)
{
    //
    //
    std::string paramType = parameterList[i]->getType();
    if (paramType == "ID")
    {
        _relations[relationSelect]->rename(i, parameterList[i]->toString());
        bool parallel = false;
        for (int j = 0; j < projectionHolder.size(); j++)
        {
            if (parameterList[i]->toString() == projectionHolder[j])
            {
                _relations[relationSelect]->innerSelectParallel(parameterList[i]->toString());
                parallel = true;
            }
        }
        if (!parallel) projectionHolder.push_back(parameterList[i]->toString());
    }
    else if (paramType == "STRING")
    {
        _relations[relationSelect]->innerSelect(i, parameterList[i]->toString());
    }
    //
    //
    return true;
}

std::string database::answerToString(std::set<tuple, relation::tuple_compare> response, std::vector<std::string> responseMap)
{
    std::stringstream retVal;
    
    if (response.size() == 0)
    {
        retVal << " No\n";
    }
    else
    {
        std::set<tuple, relation::tuple_compare>::iterator tupleCounter = response.begin();
        retVal << " Yes(" << response.size() << ")\n";
        for (int i = 0; i < response.size(); i++)
        {
            retVal << "  ";
            retVal << responseMap[0] << "=" << tupleCounter->getAttribute(0);
            for (int j = 1; j < responseMap.size(); j++)
            {
                retVal << ", " << responseMap[j] << "=" << tupleCounter->getAttribute(j);
            }
            retVal << "\n";
            tupleCounter++;
        }
    }
    
    return retVal.str();
}











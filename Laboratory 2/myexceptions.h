//
// Created by 1285166 on 24.04.2020.
//

#ifndef INC_2NDLABCOMPUTERGRAPHIC_MYEXCEPTIONS_H
#define INC_2NDLABCOMPUTERGRAPHIC_MYEXCEPTIONS_H

#include <iostream>

using namespace std;

class myexception : public exception{
public:
    myexception(char *omg){
        excepinfo = omg;
    }
    const char* what() const throw() override{
        return excepinfo;
    }
    char *excepinfo;
};


#endif //INC_2NDLABCOMPUTERGRAPHIC_MYEXCEPTIONS_H

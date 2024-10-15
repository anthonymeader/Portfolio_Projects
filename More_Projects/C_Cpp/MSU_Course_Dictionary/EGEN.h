#ifndef EGEN_H
#define EGEN_H
#include "UNV.h"
#include <iostream>
using namespace std;

class EGEN : public UNV {
    std::string title, professor;

    public:
    EGEN(std::string sn, std::string name, std::string teach) : UNV(sn) {
        title = name;
        professor = teach;
    }

    std::string getTitle() {
        return title;
    }

    std::string getProfessor() {
        return professor;
    }


};

ostream& operator <<(ostream &os, EGEN e) {
    os << e.getType() << "-" << e.getProfessor() << "-" << e.getTitle() << "-" << e.getNumber();
    return os;
}

#endif

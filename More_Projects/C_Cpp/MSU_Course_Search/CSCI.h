#ifndef CSCI_H
#define CSCI_H
#include "UNV.h"
#include <iostream>
using namespace std;

class CSCI : public UNV {
    std::string title, professor;

    public:
    CSCI(std::string sn, std::string name, std::string teach) : UNV(sn) {
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

ostream& operator <<(ostream &os, CSCI e) {
    os << e.getType() << "-" << e.getTitle() << "-" << e.getNumber() << "-" << e.getProfessor();
    return os;
}

#endif

#ifndef BIO_H
#define BIO_H
#include "UNV.h"
#include <iostream>
using namespace std;

class BIO : public UNV {
    std::string title, professor;

    public:
    BIO(std::string sn, std::string name, std::string teach) : UNV(sn) {
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

ostream& operator <<(ostream &os, BIO e) {
    os << e.getType() << "-" << e.getNumber() << "-" << e.getTitle() << "-" << e.getProfessor();
    return os;
}

#endif

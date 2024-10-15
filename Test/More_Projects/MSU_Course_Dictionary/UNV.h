#ifndef UNV_H
#define UNV_H
#include <string>
#include <sstream>
// header file
class UNV {
    std::string type;
    std::string number;

    public:
    UNV(std::string sn) {
        std::stringstream ss(sn);
        std::string data[2];
        int i = 0;
        while (ss.good()) { //for array
            std::string substr;
            getline(ss, substr, ' ');
            data[i++] = substr;
        }
        type = data[0];
        number = data[1];
    }

    std::string getType() {
        return type; //returns
    }

    std::string getNumber() {
        return number; //returns
    }
};

#endif

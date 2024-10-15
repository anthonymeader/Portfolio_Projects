#include "EGEN.h"
#include "CSCI.h"
#include "BIO.h"
#include <fstream>
#include <vector>

using namespace std;

void menu() {
    cout << "Choices:\n"; //sets a format and options
    cout << "n - print class given number\n";
    cout << "t - print all classes for a given type\n";
    cout << "q - quit\n";
}

int main(int argc, char** argv) { //takes from directed file
    ifstream infile;
    if(argc < 2) { //if errors in the file
        cout << "Input file name required!\n"; 
        exit(0);
    }
    infile.open(argv[1]);
    if(!infile) {
        cout << argv[1] << " file could not be opened.\n";
        exit(0);
    }
    string line;
    vector<EGEN> e; //declared vectors, also puts file into array
    vector<CSCI> c;
    vector<BIO> b;
    while(!infile.eof()) {
        getline(infile, line);
        if(line.length() == 0) break;
        stringstream ss(line);
        string data[3];
        int i = 0;
        while (ss.good()) {
            string substr;
            getline(ss, substr, ',');
            data[i++] = substr;
        }
        if(data[0].find("EGEN") != std::string::npos)
            e.push_back(EGEN(data[0], data[1], data[2]));
        else if(data[0].find("CSCI") != std::string::npos)
            c.push_back(CSCI(data[0], data[1], data[2]));
        else if(data[0].find("BIOB") != std::string::npos)
            b.push_back(BIO(data[0], data[1], data[2]));
        
    }
    char choice;
    do {
        menu();
        cin >> choice;
        string input1, input2;
        switch(choice) {
            case 'n' : //makes it so it prints all the information from the specified class
                cout << "Enter class type and class number (ex. CSCI 112): ";
                cin >> input1;
                cin >> input2;
                if(input1.compare("EGEN") == 0) {
                    for(int i = 0; i < e.size(); i++) {
                        if(input2.compare(e[i].getNumber()) == 0) {
                            cout << e[i] << endl;
                        }
                    }
                }
                else if(input1.compare("CSCI") == 0) {
                    for(int i = 0; i < c.size(); i++) {
                        if(input2.compare(c[i].getNumber()) == 0) {
                            cout << c[i] << endl;
                        }
                    }
                }
                else if (input1.compare("BIOB") == 0) {
                    for(int i = 0; i < b.size(); i++) {
                        if(input2.compare(b[i].getNumber()) == 0) {
                            cout << b[i] << endl;
                        }
                    }
                }
                else 
                    cout << "Not found!\n";
                break;
            case 't': //makes it so it prints all the informations from the specified subject
                cout << "Enter type (CSCI, EGEN, or BIOB): ";  
                cin >> input1;
                if(input1.compare("EGEN") == 0) {
                    for(int i = 0; i < e.size(); i++) {
                            cout << e[i] << endl;
                    }
                }
                else if(input1.compare("CSCI") == 0) {
                    for(int i = 0; i < c.size(); i++) {
                            cout << c[i] << endl;
                    }
                }
                else if (input1.compare("BIOB") == 0) { //prints all from BIOB array
                    for(int i = 0; i < b.size(); i++) {
                            cout << b[i] << endl;
                    }
                }
                else // if anything else prints an error
                    cout << "Invalid type!\n";
                break;
            case 'q':
                break;
            default:
                cout << "Invalid choice\n";
                break;
        }
    }while(choice != 'q');
    return 0;
}


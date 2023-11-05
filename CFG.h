//
// Created by Siebe Mees on 05/11/2023.
//

#ifndef CYK_CFG_H
#define CYK_CFG_H

#include <string>
#include <vector>

using namespace std;

class CFG {
private:
    vector<string> V; // Set of variables
    vector<string> T; // Set of terminals
    string S; // Start symbol
    vector<pair<string, vector<string>>> P; // Productions

public:
    CFG();

    CFG(const string &filename);

    void accepts(const string &inputString);

    void print();

    // Setters
    void setV(const vector<string> &v);
    void setT(const vector<string> &t);
    void setS(const string &s);
    void setP(const vector<pair<string, vector<string>>> &p);

    // Getters
    const vector<string> &getV() const;
};



#endif //CYK_CFG_H

//
// Created by Siebe Mees on 05/11/2023.
//

#include "CFG.h"
#include <iostream>
#include <fstream>
#include "json.hpp"
using namespace std;

using json = nlohmann::json;

CFG::CFG() {}

CFG::CFG(const string &filename) {
    // inlezen uit file
    ifstream input(filename);

    json j;
    input >> j;

    // Access the elements of the "Start" array
    string start;
    for (const auto& startvar : j["Start"]) {
        start += startvar;
    }
    S = start;

    // Access the elements of the "Variables" array
    vector<string> variables;
    for (const auto& var : j["Variables"]) {
        variables.push_back(var);
    }
    sort(variables.begin(), variables.end());
    V = variables;

    // Access the elements of the "Terminals" array
    vector<string> terminals;
    for (const auto& terminal : j["Terminals"]) {
        terminals.push_back(terminal);
    }
    sort(terminals.begin(), terminals.end());
    T = terminals;

    // Access the elements of the "Productions" array
    vector<pair<string, vector<string>>> productions;
    for (const auto& rule : j["Productions"]) {
        string head = rule["head"];
        vector<string> body = rule["body"];
        productions.push_back(make_pair(head, body));
    }
    sort(productions.begin(), productions.end());
    P = productions;
}

void CFG::accepts(const string &inputString) {
    // CYK algorithm p.303
    vector<vector<string>> table;
    for (int i = 1; i <= inputString.length(); i++) {
        std::vector<string> row;
        for (int j = 1; j <= i; j++) {
            row.push_back("| |");
        }
        table.push_back(row);
    }

    for (int len = 1; len <= inputString.length(); len++) {
        for (int i = 0; i + len <= inputString.length(); i++) {
            for (int j = i; j < i + len; j++) {
                std::cout << inputString[j];
                /*for (int l = 0; l < P.size(); l++) {
                    for (int m = 0; m < P[l].second.size(); ++m) {
                        //cout << P[l].second[m];
                        string str(1, inputString[j]);
                        if (P[l].second[m] == str) {
                            table[len][i] = P[l].first;
                        }
                    }
                }*/
            }
            std::cout << std::endl;
        }
    }

    // Printing the table
    for (const auto& row : table) {
        for (auto character : row) {
            std::cout << character << " ";
        }
        std::cout << std::endl;
    }
}

void CFG::print() {
    // V, Set of variables
    cout << "V = {";
    for (int i = 0; i < V.size(); ++i) {
        cout << V[i];
        if (i == V.size() - 1) {
            cout << "}" << endl;
            break;
        } else {
            cout << ", ";
        }
    }

    // T, Set of terminals
    cout << "T = {";
    for (int i = 0; i < T.size(); ++i) {
        cout << T[i];
        if (i == T.size() - 1) {
            cout << "}" << endl;
            break;
        } else {
            cout << ", ";
        }
    }

    // P, Productions
    cout << "P = {" << endl;
    for (int i = 0; i < P.size(); ++i) {
        cout << "    " << P[i].first << "   -> `";
        for (int j = 0; j <  P[i].second.size(); ++j) {
            cout << P[i].second[j];
            if (j == P[i].second.size()-1) {
                cout << "`" << endl;
            } else {
                cout << " ";
            }
        }
    }
    cout << "}" << endl;


    // S, Start symbol
    cout << "S = " << S << endl;
}

// Setters
void CFG::setV(const vector<string> &v) {
    V = v;
}

void CFG::setT(const vector<string> &t) {
    T = t;
}

void CFG::setS(const string &s) {
    S = s;
}

void CFG::setP(const vector<pair<string, vector<string>>> &p) {
    P = p;
    sort(P.begin(), P.end());
}

// Getters
const vector<string> &CFG::getV() const {
    return V;
}

//
// Created by Siebe Mees on 05/11/2023.
//

#include "CFG.h"
#include <iostream>
#include <set>
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

set<string> concatenate(set<string> &set1, set<string> &set2) {
    set<string> result;
    for (const string& str1 : set1) {
        for (const string& str2 : set2) {
            result.insert(str1 + str2);
        }
    }
    return result;
}

void CFG::accepts(const string &inputString) {
    int n = inputString.length();

    // Create table
    vector<vector<set<string>>> table(n);
    for (int i = 0; i < n; i++) {
        table[i].resize(i + 1);
    }

    // CYK algorithm p.303
    /*queue<string> unitStrings;
    for (int len = 1; len <= n; len++) {
        for (int i = 0; i + len <= n; i++) {
            string unitString;
            for (int j = i; j < i + len; j++) {
                string str (1, inputString[j]);
                unitString += str;
            }
            unitStrings.push(unitString);
        }
    }*/


    // Basis case
    for (int i = 0; i < n; i++) {
        char terminal =  inputString[i];
        string terminalString(1, terminal);
        for (const auto& production : P) {
            for (int j = 0; j < production.second.size(); ++j) {
                if (production.second[j] == terminalString) {
                    table[n-1][i].insert(production.first);
                }
            }
        }
    }

    // Inductive case https://web.cs.ucdavis.edu/~rogaway/classes/120/winter12/CYK.pdf
    for (int i = n - 2; i >= 0; i--) {
        for (int j = 0; j <= i; j++) {
            cout << "Table[" << i << "][" << j << "]" << endl;

            vector<set<string>> pairs;
            if (i > n/2) {
                cout << "i > n/2" << endl;
                for (int k = n-1; k > i; k--) {
                    for (int l = j; l < n-i+j; l += (k-i)) {
                        cout << "k: " << k << " l: " << l << endl;
                        //pairs.push_back(concatenate(table[k][l], table[k][l]));
                    }

                }
            } else if (i == 2) {
                cout << "i == 2" << endl;
                for (int k = n-1; k > i; k--) {
                    for (int l = 0; l < table[k].size(); l++) {
                        cout << "k: " << k << " l: " << l << endl;
                        //pairs.push_back(concatenate(table[k][l], table[k][l]));
                    }
                }
            } else {
                cout << "i < n/2" << endl;
                for (int k = n-1; k > i; k--) {
                    for (int l = 0; l < table[k].size(); l++) {
                        cout << "k: " << k << " l: " << l << endl;
                        //pairs.push_back(concatenate(table[k][l], table[k][l]));
                    }
                }
            }



            /*for (const auto& pair : pairs) {
                for (const auto& production : P) {
                    string body = "";
                    for (const string& str : production.second) {
                        body += str;
                    }
                    if (body == pair) {
                        table[i][j].insert(production.first);
                    }
                }
            }*/

        }
    }


    // Print table
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i + 1; j++) {
            // Print the elements in the set
            cout << "| ";
            if (!table[i][j].empty()) {
                bool first = true;
                cout << "{";
                for (const string& nt : table[i][j]) {
                    if (!first) {
                        cout << ", ";
                    }
                    cout << nt;
                    first = false;
                }
                cout << "} ";
            } else {
                cout << "{} ";
            }
        }
        cout << "|" << endl;
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

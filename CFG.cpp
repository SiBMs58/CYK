//
// Created by Siebe Mees on 05/11/2023.
//

#include "CFG.h"
#include <iostream>
#include <set>
#include <queue>
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
    int n = inputString.length();
    vector<string> w;
    for (int i = 0; i < n; i++) {
        string str (1, inputString[i]);
        w.push_back(str);
    }

    // Initialize the table
    map<int, map<int, set<string>>> T;

    // Filling in the table
    for (int j = 0; j < n; j++) {
        // Iterate over the productions
        for (const auto& production : P) {
            string lhs = production.first;
            const vector<string>& rhs = production.second;

            // If a terminal is found
            if (rhs.size() == 1 && rhs[0] == w[j])
                T[j][j].insert(lhs);
        }

        for (int i = j; i >= 0; i--) {
            // Iterate over the range from i to j
            for (int k = i; k <= j; k++) {
                // Iterate over the productions
                for (const auto& production : P) {
                    string lhs = production.first;
                    const vector<string>& rhs = production.second;

                    // If a non-terminal is found
                    if (rhs.size() == 2 && T[i][k].count(rhs[0]) && T[k + 1][j].count(rhs[1]))
                        T[i][j].insert(lhs);
                }
            }
        }
    }

    // Print the CYK parsing table with a newline for each column
    vector<vector<set<string>>> table(n);
    for (int i = 0; i < n; i++) {
        table[i].resize(i + 1);
    }

    for (int i = 0; i < n; i++) {
        queue<set<string>> col;
         for (int j = 0; j < n; j++) {
            set<string> symbols = T[i][j];
            col.push(symbols);
        }
        for (int l = 0; l < i; l++) {
            col.pop();
        }
        for (int k = n-1; k >= i; k--) {
            table[k][i] = col.front();
            col.pop();
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



    // If the word can be formed by the given grammar
    if (T[0][n - 1].size() != 0)
        cout << "True\n";
    else
        cout << "False\n";

    /*// Create table
    vector<vector<set<string>>> table(n);
    for (int i = 0; i < n; i++) {
        table[i].resize(i + 1);
    }

    // CYK algorithm p.303
    queue<string> unitStrings;
    for (int len = 1; len <= n; len++) {
        for (int i = 0; i + len <= n; i++) {
            string unitString;
            for (int j = i; j < i + len; j++) {
                string str (1, inputString[j]);
                unitString += str;
            }
            unitStrings.push(unitString);
        }
    }


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



            for (const auto& pair : pairs) {
                for (const auto& production : P) {
                    string body = "";
                    for (const string& str : production.second) {
                        body += str;
                    }
                    if (body == pair) {
                        table[i][j].insert(production.first);
                    }
                }
            }

        }
    }

    for (int l = 2; l <= n; l++) { // Length of span
        for (int s = 1; s <= n-l+1; s++) { // Start of span
            for (int p = 1; p <= l-1; p++) { // Partition of span
                set<string> body = produce(table[s][p], table[s+1][s+l-1], *this);
                if (!body.empty()) {
                }
                cout << "l: " << l << " s: " << s << " p: " << p << endl;
            }
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
    }*/

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

const vector<pair<string, vector<string>>> &CFG::getP() const {
    return P;
}

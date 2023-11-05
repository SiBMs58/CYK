//
// Created by Siebe Mees on 05/11/2023.
//

#include "CFG.h"

using namespace std;

int main() {
    CFG cfg("../input-cyk1.json");
    cfg.print();
    /*cfg.accepts("baaba"); //moet true geven
    cfg.accepts("abba"); //moet false geven*/
    return 0;
}
//
// Created by Siebe Mees on 05/11/2023.
//

#include "CFG.h"

using namespace std;

int main() {
    /*CFG cfg("../input-cyk1.json");
    cfg.accepts("baaba"); //moet true geven
    cfg.accepts("abba"); //moet false geven*/
    CFG cfg("../input-cyk2.json");
    cfg.accepts("aaabb"); //moet false geven
    cfg.accepts("aaabbb"); //moet true geven
    return 0;
}
#ifndef OXY_AUTOTEST_H
#define OXY_AUTOTEST_H

#include "define.h"


void printAutoTestsResults(char* testName, float mark[], int coeff[], int nTests);
void testBlocAffichage();
void testBlocFIR();
void testBlocIIR();
void testBlocMesure();


#endif //OXY_AUTOTEST_H
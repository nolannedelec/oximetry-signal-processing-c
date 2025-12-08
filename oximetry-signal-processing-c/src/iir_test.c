#include <stdio.h>
#include "iir.h"

int main() {
    absorp result = iirTest("log1_fir.dat");
    printf("Résultat IIR - ACR: %f, DCR: %f, ACIR: %f, DCIR: %f\n", 
           result.acr, result.dcr, result.acir, result.dcir);
    return 0;
}

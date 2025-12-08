#include <stdio.h>
#include "fir.h"

int main() {
    absorp result = firTest("log1.dat");
    printf("Résultat FIR - ACR: %f, DCR: %f, ACIR: %f, DCIR: %f\n", 
           result.acr, result.dcr, result.acir, result.dcir);
    return 0;
}


#include "define.h"
#include "fichiers.h"
#include "autotests.h"
#include "affichage.h"
#include "iir.h"
#include "fir.h"
#include "mesure.h"
#include <stdio.h>

int main(){
	testBlocFIR();
	testBlocIIR();
	testBlocMesure();
	testBlocAffichage();


	return 0;
}
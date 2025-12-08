
#include "define.h"
#include "fichiers.h"
#include "autotests.h"
#include "affichage.h"
#include "iir.h"
#include "fir.h"
#include "mesure.h"
#include <stdio.h>

#define FIR_TEST_COUNT 4
#define AFF_TEST_COUNT 2


// fonction de base pour afficher les résultats de tests
void printAutoTestsResults(char* testName, float mark[], int coeff[], int nTests){
	int i=0;
	int cpt=0;
	int max=0;
	printf("---- Autotest results of block %s ----\n",testName);
	for(i=0; i<nTests; i++){
		printf("--> test %d : %.0f/%d\n",i,mark[i]*coeff[i],coeff[i]);
		cpt += mark[i]*coeff[i];
		max += coeff[i];
	}
	printf("Finish autotest of block %s => total score : %.1f %%\n",testName,(float)cpt/((float)max)*100);

}

void testBlocAffichage(){	
	int N_mark = AFF_TEST_COUNT;
	float mark[AFF_TEST_COUNT] = {0.};
	int coeff[AFF_TEST_COUNT] = {1, 1};

	FILE* pf;
	oxy	mes, mes2;
	int spo2data;
	int poulsdata;
	int lock_test  = 0; // 0 lock file not present, 1 lock file present
	int mark_index = 0;

	// Start test one
	mes.spo2  = 80;
	mes.pouls = 120;

	if (access(".verrouData", F_OK ) == 0) remove(".verrouData");

	affichage(mes);
	pf = fopen("data.txt","r");
	if(pf == NULL){
		mark[mark_index++] = 0.0;
		printAutoTestsResults("Affichage", mark,coeff, AFF_TEST_COUNT);
		return; // STOP HERE: 0%
	}

	fscanf(pf,"%d\n%d",&spo2data,&poulsdata);
	fclose(pf);
	if (spo2data  == mes.spo2)	mark[mark_index] += 0.5;
	if (poulsdata == mes.pouls) mark[mark_index] += 0.5;	
	mark_index++;
	
	if (access(".verrouData", F_OK ) == 0) remove(".verrouData");

	// Start test two
	// Simulate data written but .verrouData still here
	pf=fopen("data.txt","w");
	if(pf == NULL){
		mark[mark_index++] = 0.0;
		remove(".verrouData");
		printAutoTestsResults("Affichage", mark,coeff, AFF_TEST_COUNT);
		return; // STOP HERE
	}

	fprintf(pf,"%d\n%d\n", 60, 100);
	fclose(pf);
	// Create lock file
	pf = fopen(".verrouData","w");
	fclose(pf);

	// We try, it should not modify data.txt
	affichage(mes);
	if( access(".verrouData", F_OK ) == 0) { 
		lock_test = 1;
		remove(".verrouData");
	}
	pf = fopen("data.txt", "r");
	if(pf == NULL){
		mark[mark_index++] = 0.0;
		remove(".verrouData");
		printAutoTestsResults("Affichage", mark,coeff, AFF_TEST_COUNT);
		return; // STOP HERE
	}
	fscanf(pf, "%d\n%d", &spo2data, &poulsdata);
	fclose(pf);

	if (spo2data == 60)		mark[mark_index] += 0.4;
	if (poulsdata == 100)	mark[mark_index] += 0.4;
	if (lock_test)			mark[mark_index] += 0.2;
	mark_index++;
	
	remove(".verrouData");

	printAutoTestsResults("Affichage", mark,coeff, AFF_TEST_COUNT);
}

void testBlocFIR(){
	float mark[FIR_TEST_COUNT] = { 0.f, 0.f, 0.f, 0.f };
	int coeff[FIR_TEST_COUNT] = { 1, 1, 1, 1 };

	absorp expectedOutput = {
		.acr = 2688.f,
		.dcr = 2048.f,
		.acir = 3381.f,
		.dcir = 2000.f,
	};
	
	char* filename = "data/log1.dat";
	absorp actualOutput = firTest(filename);

	if (fabsf(actualOutput.acr - expectedOutput.acr) < 2.f) {
		mark[0] = 1;
	}
	#ifdef DEBUG_AUTOTESTS
	else{
		
		printf("ACR=%f [%f]\n",actualOutput.acr,expectedOutput.acr);
	}
	#endif
	if (fabsf(actualOutput.dcr - expectedOutput.dcr) < 2.f) {
		mark[1] = 1;
	}
	#ifdef DEBUG_AUTOTESTS
	else{
		
		printf("DCR=%f [%f]\n",actualOutput.dcr,expectedOutput.dcr);
	}
	#endif
	if (fabsf(actualOutput.acir - expectedOutput.acir) < 2.f) {
		mark[2] = 1;
	}
	#ifdef DEBUG_AUTOTESTS
	else{
		
		printf("ACIR=%f [%f]\n",actualOutput.acir,expectedOutput.acir);
	}
	#endif
	if (fabsf(actualOutput.dcir - expectedOutput.dcir) < 2.f) {
		mark[3] = 1;
	}
	#ifdef DEBUG_AUTOTESTS
	else{
		
		printf("DCIR=%f [%f]\n",actualOutput.dcir,expectedOutput.dcir);
	}
	#endif
	printAutoTestsResults("FIR", mark, coeff, FIR_TEST_COUNT);
}

void testBlocMesure(){
	oxy myOxyRef;
	float mark[2]={0,0};
	int coeff[2]={1,1};
	char* filename = "data/log1_iir.dat";
	FILE* myFile_pf=initFichier(filename);
	oxy myOxy = mesureTest(filename);
	myOxyRef.spo2=98;
	myOxyRef.pouls=80;
	if(abs(myOxy.spo2-myOxyRef.spo2)<=2){
		mark[0]=1;
	}else{
		mark[0]=0;
	}
	if(abs(myOxy.pouls-myOxyRef.pouls)<=2){
		mark[1]=1;
	}else{
		mark[1]=0;
	}

	printAutoTestsResults("Mesure",mark,coeff,2);
	/*printf("spo2=%d [ref=%d]\n",myOxy.spo2,myOxyRef.spo2);
	printf("pouls=%d [ref=%d]\n",myOxy.pouls,myOxyRef.pouls);*/
	

}

void testBlocIIR(){
	
	float mark[4]={0,0,0,0};
	int coeff[4]={1,1,1,1};
	char* filename = "data/log1_fir.dat";
	int ref_oxy[4] = {732,2048,1525,2000};
	FILE* myFile_pf=initFichier(filename);
	absorp myAbsorp = iirTest(filename);
	if(fabs(myAbsorp.acr-ref_oxy[0])<2){
		mark[0]=1;
	}else{
		mark[0]=0;
	}
	if(fabs(myAbsorp.dcr-ref_oxy[1])<2){
		mark[1]=1;
	}else{
		mark[1]=0;
	}
	if(fabs(myAbsorp.acir-ref_oxy[2])<2){
		mark[2]=1;
	}else{
		mark[2]=0;
	}
	if(fabs(myAbsorp.dcir-ref_oxy[3])<2){
		mark[3]=1;
	}else{
		mark[3]=0;
	}

	printAutoTestsResults("IIR",mark,coeff,4);

}

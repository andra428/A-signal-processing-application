#include <utility.h>
#include <analysis.h>
#include <ansi_c.h>
#include <advanlys.h>
#include <formatio.h>
#include <cvirte.h>		
#include <userint.h>
#include "Intefata.h"
#include "toolbox.h"

// Constants
#define SAMPLE_RATE		0
#define NPOINTS			1

//==============================================================================
// Global variables

int waveInfo[2]; //waveInfo[0] = sampleRate
				//waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;
double *waveData = 0;

double *equirpl;
double *chebysehv;


int start = 0;
int stop = 0;
double * filt = NULL;
double* anvelope = 0;
WindowConst winConst1;
WindowConst winConst2;
WindowConst winConst3;
static int panelHandle;
int freqHandle;

int main (int argc, char *argv[])
{
	if(InitCVIRTE(0,argv,0)==0)
		return -1;
	if((panelHandle=LoadPanel(0,"Intefata.uir",PANEL))<0)
		return -1;
	if((freqHandle=LoadPanel(0,"Intefata.uir",FREQ_PANEL))<0)
		return -1;
	DisplayPanel(panelHandle);
	
	RunUserInterface();
	DiscardPanel(panelHandle);
	return 0;
}

int CVICALLBACK OnLoadButtonCB (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{
	double min = 0.0;
	double max = 0.0;
	int MaxIndex = 0;
	int MinIndex = 0;
	double mediana = 0.0;
	double media=0.0;
	double intersectii = 0;
	double m2=0.0;
	static ssize_t hist[101];
    static double axis[101];
	double *waveDataS=0;
	int imgh;
	switch (event)
	{
		case EVENT_COMMIT:
			
			//GENERARE
			//executa script python pentru conversia unui fisierului .wav in .txt
			//LaunchExecutable("python main.py");
			
			//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar
			Delay(4);
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			filt=(double *) calloc(npoints, sizeof(double));
			anvelope = (double *) calloc(npoints,sizeof(double));
			
			
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			FileToArray("anvelope.txt", anvelope, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			//afisare pe grapf
			PlotY(panel, PANEL_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			
			// MAX,MIN,MEDIE,MEDIANA,DISPERSIE,INTERSECTII
			
			MaxMin1D(waveData, npoints, &max, &MaxIndex, &min, &MinIndex);
			Mean(waveData, npoints, &media);
			SetCtrlVal(panel, PANEL_IDC_MIN, min);
			SetCtrlVal(panel, PANEL_IDC_MAX, max);
			SetCtrlVal(panel, PANEL_NUMERIC_2, media);
			
			waveDataS = (double *) calloc(npoints, sizeof(double));
			double suma2=0;
			for(int i=0;i<npoints;i++){
				if(waveData[i] == 0)
					intersectii++;
				
				suma2=suma2+(waveData[i] - media)*(waveData[i] - media);
				waveDataS[i]=waveData[i];
			}
			
			m2=suma2/npoints;
			m2=sqrt(m2);
			Sort(waveDataS,npoints,ANALYSIS_SORT_ASCENDING,waveDataS);
			mediana = waveDataS[npoints/2];
			SetCtrlVal(panel, PANEL_NUMERIC, mediana);
			SetCtrlVal(panel, PANEL_DISPERSIA, m2);
			SetCtrlVal(panel,PANEL_NUMERIC_3,intersectii);
			SetCtrlVal(panel,PANEL_MIN_INDEX,MinIndex);
			SetCtrlVal(panel,PANEL_MAX_INDEX,MaxIndex);
			
			//plotare semnal + envelope
			DeleteGraphPlot (panel,PANEL_GRAPH_RAW_DATA, -1, VAL_IMMEDIATE_DRAW);
			PlotY(panel, PANEL_GRAPH_RAW_DATA, waveData, npoints , VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			PlotY(panel, PANEL_GRAPH_RAW_DATA, anvelope, npoints , VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_GREEN);
			
			//Histograma
			
			Histogram(waveData,npoints,min,max,hist,axis,1+3.3*log(npoints));
			PlotXY (panel, PANEL_GRAPH, axis,  hist,1+3.3*log(npoints), VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1,VAL_RED);
			//salvam poza
			GetCtrlDisplayBitmap(panel,PANEL_GRAPH_RAW_DATA,1,&imgh);
			SaveBitmapToJPEGFile(imgh,"D:\\facultate\\Anul III\\Sem I\\APD-P\\etapa1_moodle\\poze\\raw_data.jpg",JPEG_DCTFAST,100);
			break;
	}
	return 0;
}

int CVICALLBACK OnPanel (int panel, int event, void *callbackData,int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK ON_NEXT (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(start==0 && stop==0){
				stop++;
			}
			if(start==0 && stop!=0){
				start++;
			}
			if(start!=0 && stop!=0)
			{
				start = stop;
				stop++;
			}
			SetCtrlVal(panel,PANEL_STOP,stop);
			SetCtrlVal(panel,PANEL_START,start);
			break;
	}
	return 0;
}

int CVICALLBACK ON_PREV (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			if(start!=0)
			{
				start--;
				stop--;
			}
			
			SetCtrlVal(panel,PANEL_START,start);
			SetCtrlVal(panel,PANEL_STOP,stop);
			break;
	}
	return 0;
}

double * ordin1(double array [], int npoints, double alpha)
{
	int i;
	filt=(double*)calloc(npoints,sizeof(double));
	filt[0]=array[0];
	for(i=1;i<npoints;i++)
	{
		filt[i]=(1-alpha)*filt[i-1]+alpha*array[i];
	}
	return filt;
}

int CVICALLBACK On_Filtrare (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{	
	int val=0;
	int npointsMediere=0;
	double alpha;
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,PANEL_FILTER_BUTTON,&val);
			if(val==0)
			{
				GetCtrlVal(panel,PANEL_RING,&npointsMediere);
				filt=(double*)calloc(npoints,sizeof(double));
				DeleteGraphPlot(panel,PANEL_GRAPH_FILTERED_DATA,-1,VAL_IMMEDIATE_DRAW);
				if(npointsMediere==0){
					MedianFilter(waveData, npoints, 32, 31, filt);
				}else{
					MedianFilter(waveData, npoints, 16, 15, filt);
				}
				
				PlotY(panel, PANEL_GRAPH_FILTERED_DATA, filt,npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			}
			else{
				if(val==1){
					
					filt=(double*)calloc(npoints,sizeof(double));
					DeleteGraphPlot(panel,PANEL_GRAPH_FILTERED_DATA,-1,VAL_IMMEDIATE_DRAW);
					GetCtrlVal(panel, PANEL_ALPHA,&alpha);
					filt=ordin1(waveData,npoints,alpha);
					PlotY(panel,PANEL_GRAPH_FILTERED_DATA, filt,npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS,VAL_DK_YELLOW);
				}
			}
		break;
	}
	return 0;
}
int CVICALLBACK OnInterval (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{
	int start,stop;
	char buffer[256];
	int imgh;
	static int name=1;

	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(panel,PANEL_START,&start);
			GetCtrlVal(panel,PANEL_STOP,&stop);
			
			SetAxisRange (panel, PANEL_GRAPH_FILTERED_DATA, VAL_MANUAL,  start*waveInfo[0], stop*waveInfo[0], VAL_AUTOSCALE, 1, 2);
			sprintf(buffer,"D:\\facultate\\Anul III\\Sem I\\APD-P\\etapa1_moodle\\poze\\%d.jpg",name);
			name=name+1;
			GetCtrlDisplayBitmap(panel,PANEL_GRAPH_FILTERED_DATA,1,&imgh);
			SaveBitmapToJPEGFile(imgh,buffer,JPEG_DCTFAST,100);
			DiscardBitmap(imgh);
			break;
	}
	return 0;
}

int CVICALLBACK On_Freq (int panel, int event, void *callbackData,
						 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK Switch_Panels (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(panel == panelHandle)
			{
				SetCtrlVal(freqHandle,FREQ_PANEL_BINARYSWITCH, 1);
				DisplayPanel(freqHandle);
				HidePanel(panel);
			}
			else
			{
				SetCtrlVal(panelHandle, PANEL_BINARYSWITCH, 0);
				DisplayPanel(panelHandle);
				HidePanel(panel);
			}
			break;
	}
	return 0;
}

int CVICALLBACK Panel2_Load (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	int windowtype=0;
	int puncte;
	
	switch (event)
	{
		case EVENT_COMMIT:	
			GetCtrlVal(panel,FREQ_PANEL_RING,&puncte);
			GetCtrlVal(panel,FREQ_PANEL_RING_2,&windowtype);
			if(windowtype==1){
			//ferestruire
				ScaledWindowEx(waveData,puncte,BLKMAN,0,&winConst1);
				
			}
			else{
			//ferestruire
				ScaledWindowEx(waveData,puncte,BLKHARRIS,0,&winConst1);
			}
			DeleteGraphPlot(panel,FREQ_PANEL_GRAPH_1,-1,VAL_IMMEDIATE_DRAW);
			PlotY(panel,FREQ_PANEL_GRAPH_1,waveData,puncte,VAL_DOUBLE,VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,VAL_CONNECTED_POINTS,VAL_BLUE);
			break;
	}
	return 0;
}
int CVICALLBACK Panel2_Filtrare (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	
	int puncte=0;

	int tip_ferestruire;
	//pentru EquiRPL
	double frequency=6000;
	double fpass=1700;
	double fstop=1500;
	double hCoef[45];
	double delta;
	
	
	//pt chebyshev
	
	double cutoff=2000;
	double lowercut=1500;
	double uppercut=1700;
	int filter_order=5;
	double a_coef[2*filter_order+1];
	double b_coef[2*filter_order+1];
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,FREQ_PANEL_RING,&puncte);
			equirpl=(double *) malloc((puncte*sizeof(double))*45);
			GetCtrlVal(panel,FREQ_PANEL_RING_2,&tip_ferestruire);
			//filtru EquiRPL
			EquiRpl_HPF(frequency, fstop,fpass, 45, hCoef, &delta); 
			Convolve (hCoef, 45, waveData,puncte,equirpl);
			//DeleteGraphPlot(panel,FREQ_PANEL_GRAPH_2,-1,VAL_IMMEDIATE_DRAW);
			//PlotY(panel,FREQ_PANEL_GRAPH_2,equirpl,puncte,VAL_DOUBLE,VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,VAL_CONNECTED_POINTS,VAL_BLUE);
			
			//CHEBYSHEV
			chebysehv=(double *) malloc(puncte*sizeof(double)*45);
			Ch_HPF(waveData,puncte,frequency,cutoff,0.1,filter_order,chebysehv);
			Ch_Coef(HIGHPASS,filter_order,frequency,lowercut,uppercut,0.1,a_coef,filter_order+1,b_coef,filter_order+1);
			//DeleteGraphPlot(panel,FREQ_PANEL_CHEBYSHEV,-1,VAL_IMMEDIATE_DRAW);
			//PlotY(panel,FREQ_PANEL_CHEBYSHEV, chebysehv,puncte,VAL_DOUBLE,VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,1,VAL_BLUE);
			
			if(tip_ferestruire==1){
			//ferestruire
				ScaledWindowEx(equirpl,puncte,BLKMAN,0,&winConst2);
				ScaledWindowEx(chebysehv,puncte,BLKMAN,0,&winConst3);
			}
			else{
			//ferestruire
				ScaledWindowEx(equirpl,puncte,BLKHARRIS,0,&winConst2);
				ScaledWindowEx(chebysehv,puncte,BLKHARRIS,0,&winConst3);
			}
			DeleteGraphPlot(panel,FREQ_PANEL_GRAPH_2,-1,VAL_IMMEDIATE_DRAW);
			PlotY(panel,FREQ_PANEL_GRAPH_2,equirpl,puncte,VAL_DOUBLE,VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,VAL_CONNECTED_POINTS,VAL_BLUE);
			
			DeleteGraphPlot(panel,FREQ_PANEL_CHEBYSHEV,-1,VAL_IMMEDIATE_DRAW);
			PlotY(panel,FREQ_PANEL_CHEBYSHEV, chebysehv,puncte,VAL_DOUBLE,VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,1,VAL_BLUE);
			
			break;
	}
	
	return 0;
}

int CVICALLBACK P2_SPECTRU (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{

	switch (event)
	{
		case EVENT_COMMIT:
			
			int puncte=0;
			GetCtrlVal(panel,FREQ_PANEL_RING,&puncte);
			//pentru spectru 1
			double autoo[puncte/2];
			double converted[puncte/2];
			char unit[32]="V";
			double df=0.0;
			double fp=0.0;
			double pp=0.0;
			//spectru 1
			AutoPowerSpectrum(waveData,puncte,1./puncte,autoo,&df);
			PowerFrequencyEstimate(autoo,puncte/2,-1.0,winConst1,df,7,&fp,&pp);
			SpectrumUnitConversion(autoo,puncte/2,SPECTRUM_POWER,SCALING_MODE_LINEAR,DISPLAY_UNIT_VRMS,df,winConst1,converted,unit);
			SetCtrlVal(panel,FREQ_PANEL_FRECVENTA_PEAK,fp);
			SetCtrlVal(panel,FREQ_PANEL_POWER_PEAK,pp);
			DeleteGraphPlot(panel,FREQ_PANEL_SPECTRU_1,-1,VAL_IMMEDIATE_DRAW);
			PlotWaveform(panel,FREQ_PANEL_SPECTRU_1,converted,puncte/2,VAL_DOUBLE,1.0,0.0,0.0,df,VAL_FAT_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,VAL_CONNECTED_POINTS,VAL_BLUE);	
			
			//spectru 2 pt EquiRPL
			double autoe[puncte/2];
			double convertede[puncte/2];
			double dfe=0.0;
			double fpe=0.0;
			double ppe=0.0;
			AutoPowerSpectrum(equirpl,puncte,1./puncte,autoe,&dfe);
			PowerFrequencyEstimate(autoe,puncte/2,-1.0,winConst2,dfe,7,&fpe,&ppe);
			SpectrumUnitConversion(autoe,puncte/2,SPECTRUM_POWER,SCALING_MODE_LINEAR,DISPLAY_UNIT_VRMS,dfe,winConst2,convertede,unit);
			SetCtrlVal(panel,FREQ_PANEL_FRECVENTA_PEAK_2,fpe);
			SetCtrlVal(panel,FREQ_PANEL_POWER_PEAK_2,ppe);
			DeleteGraphPlot(panel,FREQ_PANEL_SPECTRU_2,-1,VAL_IMMEDIATE_DRAW);
			PlotWaveform(panel,FREQ_PANEL_SPECTRU_2,convertede,puncte/2,VAL_DOUBLE,1.0,0.0,0.0,dfe,VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,VAL_CONNECTED_POINTS,VAL_BLUE);	
			
			//spectru 3 pt chebyshev
			double autoc[puncte/2];
			double convertedc[puncte/2];
			double dfc=0.0;
			double fpc=0.0;
			double ppc=0.0;
			AutoPowerSpectrum(chebysehv,puncte,1./puncte,autoc,&dfc);
			PowerFrequencyEstimate(autoc,puncte/2,-1.0,winConst3,dfc,7,&fpc,&ppc);
			SpectrumUnitConversion(autoc,puncte/2,SPECTRUM_POWER,SCALING_MODE_LINEAR,DISPLAY_UNIT_VRMS,dfc,winConst3,convertedc,unit);
			SetCtrlVal(panel,FREQ_PANEL_FRECVENTA_PEAK_3,fpc);
			SetCtrlVal(panel,FREQ_PANEL_POWER_PEAK_3,ppc);
			DeleteGraphPlot(panel,FREQ_PANEL_SPECTRU_3,-1,VAL_IMMEDIATE_DRAW);
			PlotWaveform(panel,FREQ_PANEL_SPECTRU_3,convertedc,puncte/2,VAL_DOUBLE,1.0,0.0,0.0,dfc,VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,VAL_CONNECTED_POINTS,VAL_BLUE);	
			
			break;
	}
	return 0;
}

int CVICALLBACK OnTimer (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
				P2_SPECTRU(freqHandle,FREQ_PANEL_BUTON_SPECTRU,EVENT_COMMIT,0,0,0);
			break;
	}
	return 0;
}


int CVICALLBACK PORNESTE_TIMER (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	int val;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,FREQ_PANEL_ON_TIMER,&val);
			if(val){
				SetCtrlAttribute(freqHandle,FREQ_PANEL_TIMER,ATTR_ENABLED,1);
			}
			else{
				SetCtrlAttribute(freqHandle,FREQ_PANEL_TIMER,ATTR_ENABLED,0);
			}
			break;
	}
	return 0;
}

int CVICALLBACK FAC_SS (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	int l,z,a;
	int h,m,s;
	char buffer[256];
	int imgh;
	
	char buffer2[256];
	int imgh2;
	
	char buffer3[256];
	int imgh3;
	switch (event)
	{
		case EVENT_COMMIT:
				GetSystemDate(&l,&z, &a);
				GetSystemTime(&h,&m,&s);
				sprintf(buffer,"D:\\facultate\\Anul III\\Sem I\\APD-P\\etapa1_moodle\\poze\\%d-%d %d-%d-%d %d-%d.jpg",111,z,l,a,h,m,s); 
				GetCtrlDisplayBitmap(panel,FREQ_PANEL_SPECTRU_1,1,&imgh);
				SaveBitmapToJPEGFile(imgh,buffer,JPEG_DCTFAST,100);
				DiscardBitmap(imgh);
				
				GetSystemDate(&l,&z, &a);
				GetSystemTime(&h,&m,&s);
				sprintf(buffer2,"D:\\facultate\\Anul III\\Sem I\\APD-P\\etapa1_moodle\\poze\\%d-%d %d-%d-%d %d-%d.jpg",222,z,l,a,h,m,s); 
				GetCtrlDisplayBitmap(panel,FREQ_PANEL_SPECTRU_2,1,&imgh2);
				SaveBitmapToJPEGFile(imgh2,buffer2,JPEG_DCTFAST,100);
				DiscardBitmap(imgh2);
				
				GetSystemDate(&l,&z, &a);
				GetSystemTime(&h,&m,&s);
				sprintf(buffer3,"D:\\facultate\\Anul III\\Sem I\\APD-P\\etapa1_moodle\\poze\\%d-%d %d-%d-%d %d-%d.jpg",333,z,l,a,h,m,s); 
				GetCtrlDisplayBitmap(panel,FREQ_PANEL_SPECTRU_3,1,&imgh3);
				SaveBitmapToJPEGFile(imgh3,buffer3,JPEG_DCTFAST,100);
				DiscardBitmap(imgh3);
				
			break;
	}
	return 0;
}

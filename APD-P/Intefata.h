/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  FREQ_PANEL                       1       /* callback function: On_Freq */
#define  FREQ_PANEL_BINARYSWITCH          2       /* control type: binary, callback function: Switch_Panels */
#define  FREQ_PANEL_GRAPH_1               3       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_SPECTRU_3             4       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_SPECTRU_2             5       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_GRAPH_2               6       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_CHEBYSHEV             7       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_POWER_PEAK_3          8       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_FRECVENTA_PEAK_3      9       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_FRECVENTA_PEAK_2      10      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_FRECVENTA_PEAK        11      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_POWER_PEAK_2          12      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_SS                    13      /* control type: command, callback function: FAC_SS */
#define  FREQ_PANEL_ON_TIMER              14      /* control type: command, callback function: PORNESTE_TIMER */
#define  FREQ_PANEL_COMMANDBUTTON         15      /* control type: command, callback function: Panel2_Load */
#define  FREQ_PANEL_POWER_PEAK            16      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_RING                  17      /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_RING_2                18      /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_BUTON_SPECTRU         19      /* control type: command, callback function: P2_SPECTRU */
#define  FREQ_PANEL_COMMANDBUTTON_2       20      /* control type: command, callback function: Panel2_Filtrare */
#define  FREQ_PANEL_SPECTRU_1             21      /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_TIMER                 22      /* control type: timer, callback function: OnTimer */

#define  PANEL                            2       /* callback function: OnPanel */
#define  PANEL_LoadButton                 2       /* control type: command, callback function: OnLoadButtonCB */
#define  PANEL_GRAPH_RAW_DATA             3       /* control type: graph, callback function: (none) */
#define  PANEL_START                      4       /* control type: numeric, callback function: (none) */
#define  PANEL_STOP                       5       /* control type: numeric, callback function: (none) */
#define  PANEL_IDC_MAX                    6       /* control type: numeric, callback function: (none) */
#define  PANEL_IDC_MIN                    7       /* control type: numeric, callback function: (none) */
#define  PANEL_GRAPH_FILTERED_DATA        8       /* control type: graph, callback function: (none) */
#define  PANEL_NUMERIC                    9       /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_2                  10      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_3                  11      /* control type: numeric, callback function: (none) */
#define  PANEL_MIN_INDEX                  12      /* control type: numeric, callback function: (none) */
#define  PANEL_MAX_INDEX                  13      /* control type: numeric, callback function: (none) */
#define  PANEL_DISPERSIA                  14      /* control type: numeric, callback function: (none) */
#define  PANEL_GRAPH                      15      /* control type: graph, callback function: (none) */
#define  PANEL_FILTER_BUTTON              16      /* control type: ring, callback function: (none) */
#define  PANEL_COMMANDBUTTON_PREV         17      /* control type: command, callback function: ON_PREV */
#define  PANEL_COMMANDBUTTON_NEXT         18      /* control type: command, callback function: ON_NEXT */
#define  PANEL_COMMANDBUTTON              19      /* control type: command, callback function: On_Filtrare */
#define  PANEL_ALPHA                      20      /* control type: numeric, callback function: (none) */
#define  PANEL_INTERVAL                   21      /* control type: command, callback function: OnInterval */
#define  PANEL_RING                       22      /* control type: ring, callback function: (none) */
#define  PANEL_BINARYSWITCH               23      /* control type: binary, callback function: Switch_Panels */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK FAC_SS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Filtrare(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Freq(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ON_NEXT(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ON_PREV(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnInterval(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK P2_SPECTRU(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Panel2_Filtrare(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Panel2_Load(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PORNESTE_TIMER(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Switch_Panels(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
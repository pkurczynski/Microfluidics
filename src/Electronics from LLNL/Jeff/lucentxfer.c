#include <utility.h>
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* FILE:   lucent.c                                                        */
/*                                                                            */
/* PURPOSE:                                                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Include files                                                              */
/*----------------------------------------------------------------------------*/
#include <ansi_c.h>
#include <cvirte.h> 
#include <userint.h>
#include "lucentxferUir.h"
#include "toolbox.h"
//#include "lucentVMEmethodsxfer.h"
//#include "lucentVMEmethods.h"

/*----------------------------------------------------------------------------*/
/* Defines                                                                    */
/*----------------------------------------------------------------------------*/
#define X_DATA_POINTS 32
#define Y_DATA_POINTS 32
#define NUM_COLORS	40
#define MAX_VOLTS	150.0
#define FUNC_1  1
#define FUNC_2  2
#define FUNC_3  3
#define FUNC_4  4


extern int VMECtrl(void);
extern void bt_cleanup(void);
extern int DACReset(void);
//extern int SetDAC(long crate,long mod,long chip,long dac,bt_data16_t dacdata,int delay);
extern int SetDAC(long crate,long mod,long chip,long dac,unsigned short dacdata,int delay);           
extern int msdelay(long int tcnt);
/*----------------------------------------------------------------------------*/
/* Internal function prototypes                                               */
/*----------------------------------------------------------------------------*/
void InitTableCells    (int panel, int control);
void InitColorMap      (int panel, int control, ColorMapEntry colorMap[NUM_COLORS]);
void DrawIntensityPlot (int panel, int control, double* waveform,
                        ColorMapEntry colorMap[NUM_COLORS]);
void GenDataToTable    (int panel, int control, double* waveform, int function);
void ColorTableCells   (int panel, int control, ColorMapEntry colorMap[NUM_COLORS],
                        Point* pcell, int usecolorMap);
int SetAllDACS(long mod);
int SetDacMap(long crate,long mod,long chip,long dac,unsigned short dacdata,int delay, int actuator);

/*----------------------------------------------------------------------------*/
/* Module-globals                                                             */
/*----------------------------------------------------------------------------*/
int           g_useColorMap = 1;
double*       g_waveform;  
ColorMapEntry g_colorMap[NUM_COLORS];

int flag = 0; 
int chiparray[1024];
int dacchannel[1024];
int dacdataarray[1024];
 short int modarray[1026];
int map;
FILE *addrptr;

/*----------------------------------------------------------------------------*/
/* This is the application's entry-point.                                     */
/*----------------------------------------------------------------------------*/
int main (int argc, char *argv[])
{
    int hmainPanel;

    if (InitCVIRTE (0, argv, 0) == 0)
        return -1;  
    if ((hmainPanel = LoadPanel (0, "lucentxferUir.uir", MAINPNL)) < 0)
        return -1;
    DisplayPanel (hmainPanel);
    
    /* Allocate memory for our waveform */
    if (!(g_waveform = malloc (X_DATA_POINTS * Y_DATA_POINTS * sizeof(double))))
        goto Error;
    memset (g_waveform, 0, X_DATA_POINTS * Y_DATA_POINTS * sizeof(double));
    
    /* Initialize and display the color map */
    InitColorMap (hmainPanel, MAINPNL_COLORMAP, g_colorMap);
    
    /* Initialize the table and fill it with some data */
    InitTableCells (hmainPanel, MAINPNL_DATA);
    GenDataToTable (hmainPanel, MAINPNL_DATA, g_waveform, FUNC_4);
    ColorTableCells (hmainPanel, MAINPNL_DATA, g_colorMap, (Point*)0,
                     g_useColorMap);
    
    /* Now show the intensity plot */
    DrawIntensityPlot (hmainPanel, MAINPNL_GRAPH, g_waveform, g_colorMap);
    
    /* Display the panel and run the UI */
    RunUserInterface ();
    
    /* Free resources and return */
Error:
    if (g_waveform)
        free (g_waveform);
    DiscardPanel (hmainPanel);
    CloseCVIRTE ();
    return 0;
}

/*----------------------------------------------------------------------------*/
/* Respond to the panel close event to quit runnning the UI.                  */
/*----------------------------------------------------------------------------*/
int CVICALLBACK MainPanelCB (int panel, int event, void *callbackData,
                             int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE)
        QuitUserInterface (0);
    return 0;
}

/*----------------------------------------------------------------------------*/
/* Respond to a new value selection on the color numeric's popup to change a  */
/* color map enytry and update our display.                                   */
/*----------------------------------------------------------------------------*/
int CVICALLBACK ColorChangeCB (int panel, int control, int event,
                               void *callbackData, int eventData1,
                               int eventData2)
{
    int   color;
    int   top;
    int   left;
    Point activeCell;
    
    if (event == EVENT_COMMIT)
        {
        GetCtrlVal (panel, control, &color);
        GetCtrlAttribute (panel, control, ATTR_TOP, &top);
        GetCtrlAttribute (panel, control, ATTR_LEFT, &left);
        GetTableCellFromPoint (panel, MAINPNL_COLORMAP, MakePoint (left, top),
                               &activeCell);
        
        /* Update the color map and it's display */
        SetTableCellAttribute (panel, MAINPNL_COLORMAP, activeCell,
                               ATTR_TEXT_BGCOLOR, color);
        g_colorMap[NUM_COLORS - activeCell.y].color = color;        
        SetCtrlAttribute (panel, control, ATTR_VISIBLE, 0);
        
        /* Update the plot and table with our new color */
        DrawIntensityPlot (panel, MAINPNL_GRAPH, g_waveform, g_colorMap);
        if (g_useColorMap)
            ColorTableCells (panel, MAINPNL_DATA, g_colorMap, (Point*)0,
                             g_useColorMap);
        SetActiveCtrl (panel, MAINPNL_GENERATE);
        return 0;
        }
    return 0;
}   

/*----------------------------------------------------------------------------*/
/* Respond to clicks on the colormap table to let the user select a new color */
/* for that value.  We'll launch the color numeric's popup to allow           */
/* selection.                                                                 */        
/*----------------------------------------------------------------------------*/
int CVICALLBACK ColorMapCB (int panel, int control, int event,
                            void *callbackData, int eventData1,
                            int eventData2)
{
    int   color;
    Point activeCell;
    Rect  activeCellRect;
    
    if ((event == EVENT_LEFT_CLICK) || (event == EVENT_RIGHT_CLICK))
        {
        GetTableCellFromPoint (panel, control,
                               MakePoint (eventData2, eventData1), &activeCell);
        
        /* Make sure the user clicked on a cell */
        if ((activeCell.x <= 0) || (activeCell.y <= 0))
            return 0;
        GetTableCellAttribute (panel, control, activeCell, ATTR_TEXT_BGCOLOR,
                               &color);
        SetCtrlVal (panel, MAINPNL_COLORNUM, color);
        GetTableCellRangeRect (panel, control,
                               VAL_TABLE_SINGLE_CELL_RANGE(activeCell.y,
                                                           activeCell.x),
                               &activeCellRect);
        SetCtrlAttribute (panel, MAINPNL_COLORNUM, ATTR_TOP,
                          activeCellRect.top);
        SetCtrlAttribute (panel, MAINPNL_COLORNUM, ATTR_HEIGHT,
                          activeCellRect.height);
        SetCtrlAttribute (panel, MAINPNL_COLORNUM, ATTR_VISIBLE, 1);
        SetActiveCtrl (panel, MAINPNL_COLORNUM);
        FakeKeystroke (32);
        return 1;
        }
    return 0;
}

/*----------------------------------------------------------------------------*/
/* Respond to data value changes to refresh the displays.                     */
/*----------------------------------------------------------------------------*/
int CVICALLBACK DataTableCB (int panel, int control, int event,
                             void *callbackData, int eventData1,
                             int eventData2)
{
    Point affectedCell;
    long crate,mod,chip,dac;
	int delay, actuator;
	unsigned short dacdata;
	
	crate = 0;
	mod = 3;
	chip = 0;
	dac = 0;
	dacdata = 0;	   //8192 = 13 bit dac
	delay = 1;
    
    switch (event)
        {
        case EVENT_VAL_CHANGED:
            
            /* If eventData1 and eventData2 are non-zero, only one cell value */
            /* was affected and we do not need to redraw the entire table.    */
            if (eventData1 && eventData2)
                {
                actuator = (eventData1 - 1) * X_DATA_POINTS + (eventData2 - 1);
                GetTableCellVal (panel, control, 
                                 MakePoint (eventData2, eventData1), 
                                 &g_waveform[(eventData1 - 1)
                                             * X_DATA_POINTS
                                             + (eventData2 - 1)]);
                if (g_useColorMap)
                    {
                    affectedCell = MakePoint(eventData2, eventData1);
                    ColorTableCells (panel, MAINPNL_DATA, g_colorMap,
                                     &affectedCell, g_useColorMap);
                    } 
                //printf("\nx=%d  y=%d   %lf", eventData2, eventData1, g_waveform[(eventData1 - 1)
                //                             * X_DATA_POINTS
                //                             + (eventData2 - 1)]);
               if (map == 0){
                
                if (actuator < 288)
                	{
                	mod  = 0;
                	chip = actuator / 8;
                	dac  = actuator % 8;
                	}
                else if (actuator < 576)
                	{
                	mod  = 1;
                	chip = (actuator - 288) / 8;
                	dac  = (actuator - 288) % 8;
                	}
                else if (actuator < 864)
                	{
                	mod  = 2;
                	chip = (actuator - 576) / 8;
                	dac  =  (actuator - 576) % 8;
                	}
                else if (actuator < 1152)
                	{
                	mod  = 3;
                	chip = (actuator - 864) / 8;
                	dac  = (actuator - 864) % 8;
                	}
                   }
                   else if (map == 1){
                  	mod = modarray[actuator];
                	chip = chiparray[actuator];
                	dac = dacchannel[actuator];
                	}
                	
                dacdata = (8191 / MAX_VOLTS) * g_waveform[actuator];
                //dacdata = 8191;
                if (SetDAC(crate, mod, chip, dac, dacdata, delay) != 0)
						{
							printf("\n************ ERROR Writing DAC ****************\n");
						}
				SetCtrlAttribute (panel, MAINPNL_MODULE, ATTR_CTRL_VAL, mod);
				SetCtrlAttribute (panel, MAINPNL_CHIP, ATTR_CTRL_VAL, chip);
				SetCtrlAttribute (panel, MAINPNL_DAC, ATTR_CTRL_VAL, dac);
                SetCtrlAttribute (panel, MAINPNL_ACTUATOR, ATTR_CTRL_VAL, actuator);
                SetCtrlAttribute (panel, MAINPNL_VALUE, ATTR_CTRL_VAL, g_waveform[actuator]);
                }                    
            else
                {
                GetTableCellRangeVals (panel, control, VAL_TABLE_ENTIRE_RANGE,
                                       g_waveform, VAL_ROW_MAJOR);
                if (g_useColorMap)
                    ColorTableCells (panel, MAINPNL_DATA, g_colorMap, 
                                     (Point*)0, g_useColorMap);
                }
            DrawIntensityPlot (panel, MAINPNL_GRAPH, g_waveform, g_colorMap);
            break;
        }
    return 0;
}

/*----------------------------------------------------------------------------*/
/* Respond to the Generate Data button to generate and send some data to the  */
/* grid table display.                                                        */
/*----------------------------------------------------------------------------*/
int CVICALLBACK GenerateDataCB (int panel, int control, int event,
                                void *callbackData, int eventData1,
                                int eventData2)
{
    int function;
    
    if (event == EVENT_COMMIT)
        {
        GetCtrlVal (panel, MAINPNL_FUNC, &function);
        GenDataToTable (panel, MAINPNL_DATA, g_waveform, function);
        DrawIntensityPlot (panel, MAINPNL_GRAPH, g_waveform, g_colorMap);
        if (g_useColorMap)
            ColorTableCells (panel, MAINPNL_DATA, g_colorMap, (Point*)0,
                             g_useColorMap);
        }
    return 0;
}

/*----------------------------------------------------------------------------*/
/* Do the intensity plot for the passed data using the passed color map.      */
/*----------------------------------------------------------------------------*/
void DrawIntensityPlot (int panel, int control, double* waveform,
                        ColorMapEntry colorMap[NUM_COLORS])
{
    DeleteGraphPlot (panel, control, -1, VAL_DELAYED_DRAW);
    PlotIntensity (panel, control, waveform, X_DATA_POINTS,
                   Y_DATA_POINTS, VAL_DOUBLE, colorMap, VAL_WHITE, NUM_COLORS, 1, 1);
}

/*----------------------------------------------------------------------------*/
/* Initialize the table to have X_DATA_POINTS * Y_DATA_POINTS cells of        */
/* specified width.                                                           */
/*----------------------------------------------------------------------------*/
void InitTableCells (int panel, int control)
{
    int c;
    
    InsertTableRows (panel, control, 1, Y_DATA_POINTS,
                     VAL_USE_MASTER_CELL_TYPE);
    InsertTableColumns (panel, control, 1, X_DATA_POINTS,
                        VAL_USE_MASTER_CELL_TYPE);
    for (c = 1; c <= X_DATA_POINTS; c++)
        SetTableColumnAttribute (panel, control, c, ATTR_COLUMN_WIDTH, 38);
}

/*----------------------------------------------------------------------------*/
/* Initialize the color-map with NUM_COLORS entries equally distributed over  */
/* the range MIN_DATA_VAL to MAX_DATA_VAL from blue to red, and display the   */
/* values appropriately in the table.  Our table will be made up of string    */
/* controls, and we'll change their background to indicate the color.         */
/*----------------------------------------------------------------------------*/
void InitColorMap (int panel, int control, ColorMapEntry colorMap[NUM_COLORS])
{
    int              i;
    char             rowLabel[10];
    
    HideBuiltInCtrlMenuItem (panel, control, VAL_GOTO);
    HideBuiltInCtrlMenuItem (panel, control, VAL_SORT);
    HideBuiltInCtrlMenuItem (panel, control, VAL_SEARCH);
    
    /* Create rows for each color */
    InsertTableRows (panel, control, -1, NUM_COLORS, VAL_USE_MASTER_CELL_TYPE);
    
    /* Iterate through each of the new rows... */
    for (i = 0; i < NUM_COLORS; i++)
        {
        
        /* Set the colormap data */
        //colorMap[i].dataValue.valDouble = -1 + 2 * (i / (float)19.0);
        //colorMap[i].color = MakeColor (i * 255 / (float)19.0, 0,
        //                               255 - (i / (float)19.0 * 255));
                                       
        colorMap[i].dataValue.valDouble = i * (MAX_VOLTS / (float)NUM_COLORS);
        colorMap[i].color = MakeColor (i  * (255 / (float)NUM_COLORS), 0,
                                       255 - (i * (255 / (float)NUM_COLORS)));
        
        /* Set the row to have the correct color and label */
        SetTableCellAttribute (panel, control, MakePoint (1, NUM_COLORS - i),
                               ATTR_TEXT_BGCOLOR, colorMap[i].color);
        sprintf (rowLabel, "%1.2f", colorMap[i].dataValue.valDouble);
        SetTableRowAttribute (panel, control, NUM_COLORS - i, ATTR_USE_LABEL_TEXT,
                              1);
        SetTableRowAttribute (panel, control, NUM_COLORS - i, ATTR_LABEL_TEXT,
                              rowLabel);
        SetTableRowAttribute (panel, control, NUM_COLORS - i, ATTR_ROW_HEIGHT,
                              15);
        }
}

/*----------------------------------------------------------------------------*/
/* Fill up the passed array with data, and display the data in the table.     */
/*----------------------------------------------------------------------------*/
void GenDataToTable (int panel, int control, double* pwaveform, int func)
{
    int x;
    int y;
    
    /* Generate all data to the waveform array */
    for (x = 0; x < X_DATA_POINTS; x++)
        {
        for (y = 0; y < Y_DATA_POINTS; y++)
            {
            switch (func)
                {
                case FUNC_1:
                    pwaveform[x + y * X_DATA_POINTS] = sin (2 * PI * ((x / (float)X_DATA_POINTS)
                                                            * (x / (float)X_DATA_POINTS)
                                                               + (y / (float)Y_DATA_POINTS)
                                                            * (y / (float)Y_DATA_POINTS)));
                    break;
                case FUNC_2:
                    pwaveform[x + y * X_DATA_POINTS] = 0.5 * sin (PI * 2 * x / X_DATA_POINTS) 
                                                       + 0.5 * cos (PI * 2 * y / Y_DATA_POINTS);
                    break;
                case FUNC_3:
                    pwaveform[x + y * X_DATA_POINTS] = exp(-x * (y / (float)Y_DATA_POINTS))
                                                       * sin (2 * PI * ((x / (float)X_DATA_POINTS)
                                                       * (x / (float)X_DATA_POINTS)
                                                       + (y / (float)Y_DATA_POINTS)
                                                       * (y / (float)Y_DATA_POINTS)));
                     break;
                case FUNC_4:
                	pwaveform[x + y * X_DATA_POINTS] = 0.0;
                    break;
                }
            }
        }   
    
    /* Send the waveform data to the table */
    SetTableCellRangeVals (panel, control, VAL_TABLE_ENTIRE_RANGE, pwaveform,
                           VAL_ROW_MAJOR);
}

/*----------------------------------------------------------------------------*/
/* Apply the color map to the table data cells.  If pcell is not 0, then only */
/* color that cell -- otherwise color the entire table.                       */
/*----------------------------------------------------------------------------*/
void ColorTableCells (int panel, int control, ColorMapEntry colorMap[NUM_COLORS], 
                      Point* pcell, int useColorMap)
{
    int row = 1;
    int col;
    int startCol = 1;
    int i;
    int endRow = Y_DATA_POINTS;
    int endCol = X_DATA_POINTS;
    int colorIndex;
    int newColor;
    double offset;
    double val;
    
    if (pcell)
        {
        row = pcell->y;
        startCol = pcell->x;
        endRow = pcell->y;
        endCol = pcell->x;
        }
    while (row <= endRow)
        {
        col = startCol;
        while (col <= endCol)
            {
            if (useColorMap)
                {
                GetTableCellVal (panel, control, MakePoint (col, row), &val);
                i = 0;
                while ((val > colorMap[i].dataValue.valDouble) && (i <= 19))
                    i++;
                if (val == colorMap[i].dataValue.valDouble)
                    newColor = colorMap[i].color;
                else
                    {
                    if (i == 19)
                        newColor = colorMap[i].color;
                    else
                        {
                        int    red1;
                        int    blue1;
                        int    green1;
                        int    red2;
                        int    blue2;
                        int    green2;
                        double offset;
                    
                        red1 = (colorMap[i].color & 0x00FF0000) >> 16;
                        green1 = (colorMap[i].color & 0x0000FF00) >> 8;
                        blue1 = (colorMap[i].color & 0x000000FF);
                        red2 = (colorMap[i + 1].color & 0x00FF0000) >> 16;
                        green2 = (colorMap[i + 1].color & 0x0000FF00) >> 8;
                        blue2 = (colorMap[i + 1].color & 0x000000FF);
                        offset = fabs ((val - colorMap[i].dataValue.valDouble)
                                       / (colorMap[i + 1].dataValue.valDouble
                                          - colorMap[i].dataValue.valDouble));                                      
                        newColor = MakeColor(red1 + offset * (red2 - red1),
                                             green1
                                             + offset * (green2 - green1),
                                             blue1 + offset * (blue2 - blue1)); 
                        }
                    }
                }
            else
                newColor = VAL_BLACK;
            SetTableCellAttribute (panel, control, MakePoint (col, row),
                                   ATTR_TEXT_COLOR, newColor);
            col++;
            }
        row++;
        }
}

/*----------------------------------------------------------------------------*/
/* Respond to set the various table display options.                          */
/*----------------------------------------------------------------------------*/
int CVICALLBACK TableOptionsCB (int panel, int control, int event,
                                void *callbackData, int eventData1,
                                int eventData2)
{
    int val;
    
    if (event == EVENT_COMMIT)
        {
        GetCtrlVal (panel, control, &val);
        switch (control)
            {
            case MAINPNL_APPLY_COLOR:
                g_useColorMap = val;
                ColorTableCells (panel, MAINPNL_DATA, g_colorMap, (Point*)0, 
                                 g_useColorMap);
                break;
            case MAINPNL_GRID_VISIBLE:
                SetTableCellRangeAttribute (panel, MAINPNL_DATA,
                                            VAL_TABLE_ENTIRE_RANGE,
                                            ATTR_HORIZONTAL_GRID_VISIBLE, val);
                SetTableCellRangeAttribute (panel, MAINPNL_DATA,
                                            VAL_TABLE_ENTIRE_RANGE,
                                            ATTR_VERTICAL_GRID_VISIBLE, val);
                break;
            case MAINPNL_SHOW_LABELS:
                SetCtrlAttribute (panel, MAINPNL_DATA, ATTR_ROW_LABELS_VISIBLE,
                                  val);
                SetCtrlAttribute (panel, MAINPNL_DATA,
                                  ATTR_COLUMN_LABELS_VISIBLE, val);
                break;
            }
        }
    return 0;
}

/*----------------------------------------------------------------------------*/
/* Shut down.                                                                 */
/*----------------------------------------------------------------------------*/
int CVICALLBACK QuitCallback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    switch (event)
        {
        case EVENT_COMMIT:
        	bt_cleanup(); 
            QuitUserInterface (0);
            break;
        }
    return 0;
}



int CVICALLBACK initVME (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
				//printf("\nVMECtrl**********\n");
				VMECtrl();

				//printf("\nDACReset**********\n");
				DACReset();

				//printf("\nbt_cleanup**********\n");
				//bt_cleanup();
				SetCtrlAttribute (panel, control, ATTR_DIMMED, 1);
			break;
		}
	return 0;
}

int CVICALLBACK setDAC (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	long crate,mod,chip,dac;
	int delay, i;
	unsigned short dacdata;
	
	crate = 0;
	mod = 0;
	chip = 0;
	dac = 0;
	dacdata = 8191;
	delay = 0;
	
	switch (event)
		{
		case EVENT_COMMIT:
/* =============================== SetDAC ====================================
    SetDAC: Set a single channel
    Args:
        mod: board number 0-31
        chip: DAC number 0-7
        dac: Channel number 0-7
        data: unsigned int: value to set dac
    Return:
        0: success
        1: error

        THis routine is used in the Maintenance Window
============================================================================ */

/********************************************/
		for(mod=0; mod<4; mod++) 
			{
			for (chip=0;chip<36;++chip)
			//for (chip=0;chip<10;++chip) 
			
                {
                    for (dac=0;dac<8;++dac)
                    {
						if (SetDAC(crate, mod, chip, dac, dacdata, delay) != 0)
						{
							printf("\n************ ERROR Writing DAC ****************\n");
						}
					}
				}
			}
			break;
		}
	return 0;
}

int CVICALLBACK setDACLow (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	long crate,mod,chip,dac;
	int delay, i;
	unsigned short dacdata;
	
	crate = 0;
	mod = 1;
	chip = 0;
	dac = 0;
	dacdata = 0;
	delay = 0;
	
	switch (event)
		{
		case EVENT_COMMIT:
/* =============================== SetDAC ====================================
    SetDAC: Set a single channel
    Args:
        mod: board number 0-31
        chip: DAC number 0-7
        dac: Channel number 0-7
        data: unsigned int: value to set dac
    Return:
        0: success
        1: error

        THis routine is used in the Maintenance Window
============================================================================ */

/********************************************/
		for(mod=0; mod<4; mod++) 
			{
			for (chip=0;chip<36;++chip)
			//for (chip=0;chip<10;++chip) 
                {
                    for (dac=0;dac<8;++dac)
                    {
						if (SetDAC(crate, mod, chip, dac, dacdata, delay) != 0)
						{
							printf("\n************ ERROR Writing DAC ****************\n");
						}
					}
				}
			}
			break;		 
		}
	return 0;
}

int CVICALLBACK xfer_callback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{

	long mods;
	
	switch (event)
		{
		case EVENT_COMMIT:
		
		for(mods=0; mods<4; mods++){
        	SetAllDACS(mods);
			}
			break;
		}
	return 0;
}

int CVICALLBACK dacdata_callback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
		//read the indicator for the dac value desired
{
	unsigned short val;
	long mods;
	
	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel, MAINPNL_NUMERIC, &val);
			//printf("\nval = %d", (int)val);
			break;
		}
	return 0;
}


int CVICALLBACK setval_callback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{

	long crate,mod,chip,dac;
	int delay, i;
	unsigned short dacdata,val;
	
	crate = 0;
	mod = 1;
	chip = 0;
	dac = 0;
	dacdata = 0;
	delay = 0;
	 
	switch (event)
		{
		case EVENT_COMMIT:
			 GetCtrlVal (panel, MAINPNL_NUMERIC, &val);
			//printf("\nval = %d", (int)val);
			
			dacdata = val;
			
			/* =============================== SetDAC ====================================
    SetDAC: Set a single channel
    Args:
        mod: board number 0-31
        chip: DAC number 0-7
        dac: Channel number 0-7
        data: unsigned int: value to set dac
    Return:
        0: success
        1: error

        THis routine is used in the Maintenance Window
============================================================================ */

/********************************************/
		for(mod=0; mod<4; mod++) 
			{
			for (chip=0;chip<36;++chip)
			//for (chip=0;chip<10;++chip) 
                {
                    for (dac=0;dac<8;++dac)
                    {
						if (SetDAC(crate, mod, chip, dac, dacdata, delay) != 0)
						{
							printf("\n************ ERROR Writing DAC ****************\n");
						}
					}
				}
			}

			break;
		}
	return 0;
}

int CVICALLBACK HighLow (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	long crate,mod,chip,dac;
	int delay, i;
	unsigned int count;
	unsigned short dacdata;
	double del;
	
	del = 0.00001;
	
	crate = 0;
	mod = 1;
	chip = 0;
	dac = 0;
	dacdata = 8191;
	delay = 0;
	count = 0;
	
	switch (event)
		{
		case EVENT_COMMIT: 
			while(flag == 0) {
				dacdata = 8191;   
				for(mod=0; mod<4; mod++) {
					for (chip=0;chip<36;++chip) {
                	    for (dac=0;dac<8;++dac) {
                	    	Delay (del);  
							if (SetDAC(crate, mod, chip, dac, dacdata, delay)) {
								printf("\n************ ERROR Writing DAC ****************\n");
								}
							}
						}
					}
				dacdata = 0;  
				for(mod=0; mod<4; mod++) {
					for (chip=0;chip<36;++chip) {
                	    for (dac=0;dac<8;++dac) {
                	    	Delay (del);
							if (SetDAC(crate, mod, chip, dac, dacdata, delay)) {
								printf("\n************ ERROR Writing DAC ****************\n");
								}
							}
						}
					}
				ProcessSystemEvents ();
				count++;
				SetCtrlVal (panel, MAINPNL_COUNT, count);
				}

			break;
		}
	return 0;
}

int CVICALLBACK stopHL (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			flag = 1;
			break;
		}
	return 0;
}

int CVICALLBACK genchip (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char in_modarray[1026];
	char junk;
	int	addrarray[1024];
	int crate = 0;
	int dacarray[1024];

	int i,j,k;
	
	unsigned short dacdata = 0;	   //8192 = 13 bit dac
	int delay = 1;
	
	FILE *dacptr;
	FILE *modptr;
	
	FILE *chipptr;
	FILE *chanptr;
	FILE *modout;

	switch (event)
		{
		case EVENT_COMMIT:

		if( (dacptr = fopen("dacfile.txt", "r")) ==NULL)
			 printf("error opening dacfile \n");
			 
		if( (modptr = fopen("modfile.txt", "r")) ==NULL)
			 printf("error opening modfile \n");
			 
		if( (addrptr = fopen("addrfile.txt", "r")) ==NULL)
			 printf("error opening addrfile \n");
			 
		
		
		for (i=0; i<1024; i++)
			{
			 
			 
			if( (fscanf( dacptr, "%4d", &dacarray[i] ))==NULL) {
				printf("Error reading file dacfile \n");
				}
				
			if( (fscanf( modptr, "%c", &in_modarray[i])) == EOF) {
				printf("error reading modfile \n");
				}
			if( (fscanf( modptr, "%c", &junk)) == EOF) {
				printf("error reading modfile \n");
				}
				//printf("\n%d:%c",i,in_modarray[i]);
				switch(in_modarray[i])
				{ 
				case    'A' : modarray[i] = 0;
				break;
			
				case    'B' : modarray[i] = 1;
				break;
			
				case	'C' : modarray[i] = 2;
				break;
				
				case	'D' : modarray[i] = 3;
				break;
			
				default: 
					printf("\nERROR\n");
				break;
				
				
			
				}
				
				//printf(" %4d \n", dacarray[i]);
				//printf(" %d", modarray[i]);
				

			 }
			for(j = 0; j<1024; j++)
				{
				chiparray[j] = (int)(dacarray[j] / 8);	   // get chip # and dac channel
                dacchannel[j]  = dacarray[j] % 8;
                //printf("%d\n" ,chiparray[j]);
                //printf("%d\n" ,dacchannel[j]);
			    }		
			fclose(dacptr);
			fclose(modptr);
			
			//////////////////create address, chip, mod, and dac file
			
			if( (chipptr = fopen("chipfile.txt", "w")) ==NULL)
			 printf("error opening chipfile \n");
			 
			 if( (chanptr = fopen("chanfile.txt", "w")) ==NULL)
			 printf("error opening chanfile \n");
			 
			 if( (addrptr = fopen("addrfile.txt", "w")) ==NULL)
			 printf("error opening addrfile \n");
			 
			 if( (modout = fopen("modfile_out.txt", "w")) ==NULL)
			 		printf("error opening modfile_out \n");
			 
			for(k = 0; k<1024; k++)
			{
			 fprintf( chipptr, "%d\n", chiparray[k]);
			 fprintf( chanptr, "%d\n", dacchannel[k]);
			 fprintf(modout, "%d\n", modarray[k]);
			 //SetDACADDR(crate, modarray[k], chiparray[k], dacchannel[k], dacdata, delay);
			 //SetDAC(crate, modarray[k], chiparray[k], dacchannel[k], dacdata, delay);  
			 
			}
			
			fclose(chipptr);
			fclose(addrptr);
			fclose(chanptr);
			break;
		}
	return 0;
}

int CVICALLBACK mapactuators (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int  j, i, k;
	int dacarray[1024];
	char in_modarray[1024];
	char junk;
	
	FILE *chipptr;
	FILE *chanptr;
	FILE *modout;
	FILE *mapptr;
	
	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &map);
			//printf("\n %d", map);
			if (map == 1){
			
				if( (chipptr = fopen("chipfile.txt", "r")) ==NULL)
			 	printf("error opening chipfile \n");
			 
			 	if( (chanptr = fopen("chanfile.txt", "r")) ==NULL)
			 	printf("error opening chanfile \n");
			 	
			 	
			 	if( (modout = fopen("modfile_out.txt", "r")) ==NULL)
			 		printf("error opening modfile_out \n");
			 		
			    if( (mapptr = fopen("mapfile.txt", "w")) ==NULL)
			 		printf("error opening mapfile \n");
			 	
			 	
			 		for (i=0; i<1024; i++){
			 			if( (fscanf( chipptr, "%4d", &chiparray[i] ))==NULL) 
						printf("Error reading file chipfile \n");
						
						if( (fscanf( chanptr, "%4d", &dacchannel[i] ))==NULL) 
						printf("Error reading file chanfile \n");
						 
						if( (fscanf( modout, "%d", (int*)&modarray[i] ))==NULL) 
						printf("Error reading file chanfile \n");
					}
			 	
			 	for(k = 0; k<1024; k++) 
			 	{	fprintf(mapptr, "\n %d \t %d\t %d", modarray[k], chiparray[k],dacchannel[k]);
			 		//printf("\n %d", dacchannel[k]);
			 		
			 		//fprintf( chipptr, "%d\n", chiparray[k]);
			 		//fprintf( chanptr, "%d\n", dacchannel[k]);
			 	}
			 	fclose(chipptr);
				fclose(chanptr);
				fclose(modout);
				fclose(mapptr);
			 }
					  
			break;
		}
	return 0;
}

int CVICALLBACK checker (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	long crate,mod,chip,dac;
	int delay, i, test, CheckerOn, actuator, x, alt, l, j, k;
	int row, col;
	
	unsigned short dacdata;
	unsigned short dacdataarray[32][32];
	crate = 0;
	mod = 0;
	chip = 0;
	dac = 0;
	dacdata = 0;
	delay = 0;
	
	switch (event)
		{
		case EVENT_COMMIT:
		GetCtrlVal (panel, control, &CheckerOn);
		//if ((map == 1) && (CheckerOn == 1)){
		  if (CheckerOn == 1){
		  if (map !=1 ) 
		  MessagePopup ("You Dummy", "Turn on mapping first");
		 alt = 4195;
		 x=0;
		for(l=0; l<32; l= l+1){
		  for(j=0; j<32; j= j+1){
		   	 	dacdataarray[j][l] = alt;
		   	 	
		   	if ((x & 0x1f) == 0x1f)
		   		alt = alt;
		   	else if (alt == 4195)
		   			alt = 0;
		   	else if (alt == 0)
		   			alt = 4195;
		   			
		     x= x+1;
		       }
		   	 }
		   actuator=0;
		   	 	for(col=0; col<32; col++){
		   	 		for (row=0; row<32; row++){
		   	 		dacdata = dacdataarray[col][row];
		   	 		
		   	 		if (SetDacMap(crate, mod, chip, dac, dacdata, delay, actuator) !=0)
						printf("\n************ ERROR Writing DAC ****************\n");
						
						actuator++;
		   	 		   }
		   	 		   
		   	 		   
		   	 		    //if(col==l) msdelay(val);
		   	 		  }

			  }
			  
			  else {
					
					k = 0;
		 			i = 0;
					l = 0;
		 			actuator=0;
		   				while (l <32){
		   					for(k=0; k<32; k++){	   //init 2 dim dataarray to 0
		   	 					for(i=0; i<32; i++)
		   	 					dacdataarray[i][k] = 0;
		   	 						
	   	 			
		   	 						
		   	 		}
		   	 		actuator=0;
		   	 	for(col=0; col<32; col++){
		   	 		for (row=0; row<32; row++){
		   	 		dacdata = dacdataarray[col][row];
		   	 		
		   	 		if (SetDacMap(crate, mod, chip, dac, dacdata, delay, actuator) !=0)
						printf("\n************ ERROR Writing DAC ****************\n");
						
						actuator++;
		   	 		   }
		   	 		    //if(col==l) msdelay(val);
		   	 		    

						}
						l++;
		   	 		}
		   	 
 				   }
 			
			break;
		}
	return 0;
}

int CVICALLBACK walkingCol (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{

int actuator, walking, delay, i,j,k,l;
int row, col;
long crate,mod,chip,dac;
unsigned short dacdata,val;
unsigned short dacdataarray[32][32];
delay = 0;
crate = 0;
	switch (event)
		{
		case EVENT_COMMIT:
		GetCtrlVal (panel, MAINPNL_NUMERIC_2, &val);
		GetCtrlVal (panel, control, &walking);
		   if (walking == 1){
		   if (map !=1 ){ 
		  MessagePopup ("You Dummy", "Turn on mapping first");
		  walking = 0;
		  SetCtrlVal (panel, control, walking);
		  break;
		  }
/***********************************		   
		   
		   	for(k=0; k<31; k++){	   //init 2 dim dataarray to 0
		   	 for(i=0; i<1024; i++)
		   	 	dacdataarray[i][k] = 0;
		   	 	}
**********************************/		   	 	
		 k = 0;
		 i = 0;
		 l = 0;
		 actuator=0;
		   while (l <32){
		   	for(k=0; k<32; k++){	   //init 2 dim dataarray to 0
		   	 for(i=0; i<32; i++)
		   	 	dacdataarray[i][k] = 0;
		   	 	}
		   	 //for(l=0; l<32; l++){ 	
		   	 for(j=0; j<32; j= j+1)
		   	 	dacdataarray[j][l] = 8191;
		   	 	
		   	 //while(actuator <1024){
		   	 	actuator=0;
		   	 	for(col=0; col<32; col++){
		   	 		for (row=0; row<32; row++){
		   	 		dacdata = dacdataarray[col][row];
		   	 		
		   	 		if (SetDacMap(crate, mod, chip, dac, dacdata, delay, actuator) !=0)
						printf("\n************ ERROR Writing DAC ****************\n");
						
						actuator++;
		   	 		   }
		   	 		    //if(col==l) msdelay(val);
		   	 		  }
		   	 		  msdelay(val);
		   	 		  col=0;
		   	 		  row=0;
		   	 		//}
		   	 	
		   	   //for (actuator=0; actuator<1024; actuator++){
		   	   		//dacdata = dacdataarray[actuator][l];
					//if (SetDacMap(crate, mod, chip, dac, dacdata, delay, actuator) !=0)
						//rintf("\n************ ERROR Writing DAC ****************\n");
				//}
					l++;
			   
			    }
			    
			   }
			   
			else
			DACReset();
			
			break;
		}
	return 0;
}

int CVICALLBACK walkingrow (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
int actuator, walking, delay, i,j,k,l;
int row, col;
long crate,mod,chip,dac;
unsigned short dacdata,val;
unsigned short dacdataarray[32][32];
delay = 0;
crate = 0;

	switch (event)
		{
		case EVENT_COMMIT:
		GetCtrlVal (panel, MAINPNL_NUMERIC_2, &val);
		GetCtrlVal (panel, control, &walking);
		   if (walking == 1){
		   	if (map !=1 ){ 
		  		MessagePopup ("You Dummy", "Turn on mapping first");
		  		walking = 0;
		  		SetCtrlVal (panel, control, walking);
		  		break;
		  	}
/***********************************		   
		   
		   	for(k=0; k<31; k++){	   //init 2 dim dataarray to 0
		   	 for(i=0; i<1024; i++)
		   	 	dacdataarray[i][k] = 0;
		   	 	}
**********************************/		   	 	
		 k = 0;
		 i = 0;
		 l = 0;
		 j = 0;
		actuator=0; 
		 while (j <32){
		   	for(k=0; k<32; k++){	   //init 2 dim dataarray to 0
		   	 for(i=0; i<32; i++)
		   	 	dacdataarray[i][k] = 0;
		   	 	}
		   	 	
		   	 for(l=0; l<32; l++)
		   	 	dacdataarray[j][l] = 8191;
/*********************		   	 	
		   	   for (actuator=0; actuator<32; actuator++){
		   	   		dacdata = dacdataarray[actuator][j];
*********************/
				//while(actuator <1024){
				actuator=0;
				
				 for(col=0; col<32; col++){
		   	 		for (row=0; row<32; row++){
		   	 		dacdata = dacdataarray[col][row];
		   	 		
					if (SetDacMap(crate, mod, chip, dac, dacdata, delay, actuator) !=0)
						printf("\n************ ERROR Writing DAC ****************\n");
					
					 actuator++;
					}
				     if(col==j) msdelay(val);
				   }
				    col=0;
		   	 		row=0;
		 	  		j++;
		 		 }
		 		 
		 
		 /*************************
		 
		 
		 
		   while (j <32){
		   	for(k=0; k<32; k++){	   //init 2 dim dataarray to 0
		   	 for(i=0; i<1024; i++)
		   	 	dacdataarray[i][k] = 0;
		   	 	}
		   	 	
		   	 for(l=0; l<32; l++)
		   	 	dacdataarray[j][l] = 8191;
		   	 	
		   	   for (actuator=0; actuator<1024; actuator++){
		   	   		dacdata = dacdataarray[actuator][j];
					if (SetDacMap(crate, mod, chip, dac, dacdata, delay, actuator) !=0)
						printf("\n************ ERROR Writing DAC ****************\n");
				}
					j++;
		********************/	   
			  
			    //}
			  }
			 
			else
			DACReset();
			
		

			break;
		}
	return 0;
}

int CVICALLBACK walking_data (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	unsigned short val;
	
	switch (event)
		{
		case EVENT_COMMIT:
		

			GetCtrlVal (panel, MAINPNL_NUMERIC_2, &val);
			printf("\nval = %d", (int)val);

			break;
		}
	return 0;
}

/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.28                          *
*        Compiled Jan 30 2015, 16:41:06                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
#include "includes.h"
#include "app.h"
#include "DIALOG.h"
#include "string.h"
#include "./Bsp/sersor_i2c/sersor_i2c.h"
#include "./Bsp/mpu6050/mpu6050.h"
#include "./Bsp/exti/bsp_exti.h"
// USER END


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/*--------------------  (ʹ��U2C.exeС��������)  ------------------------------------*/
#define UTF8_MPU6050       "\xe9\x87\x8d\xe5\x8a\x9b\xe5\x8a\xa0\xe9\x80\x9f\xe5\xba\xa6\xe9\x99\x80\xe8\x9e\xba\xe4\xbb\xaa"//�������ٶ�������
#define UTF8_YAW           "\xe8\x88\xaa\xe5\x90\x91\xe8\xa7\x92"//�����
#define UTF8_PITCH         "\xe4\xbf\xaf\xe4\xbb\xb0\xe8\xa7\x92"//������
#define UTF8_ROLL          "\xe6\xbb\x9a\xe8\xbd\xac\xe8\xa7\x92"//�����
/*-----------------------------------------------------------------------------------*/

  
// USER START (Optionally insert additional defines)
#define DATACOLOR        0x00FF00FF 
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
static MPU6050_DATA_T mpu6050_data={0};
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateCompass[] = {
  { FRAMEWIN_CreateIndirect, "Compass",               0,    0,   0,   800, 480, 0, 0x0, 0 },
  { EDIT_CreateIndirect,     "GYR0_X",     GUI_ID_EDIT0,    65,  30,  60,  20,  0, 0x8, 0 },
  { EDIT_CreateIndirect,     "GYR0_Y",     GUI_ID_EDIT1,    65,  60,  60,  20,  0, 0x8, 0 },
  { EDIT_CreateIndirect,     "GYR0_Z",     GUI_ID_EDIT2,    65,  90,  60,  20,  0, 0x8, 0 },
  { EDIT_CreateIndirect,     "Accel_X",    GUI_ID_EDIT3,    65,  120, 60,  20,  0, 0x8, 0 },
  { EDIT_CreateIndirect,     "Accel_Y",    GUI_ID_EDIT4,    65,  150, 60,  20,  0, 0x8, 0 },
  { EDIT_CreateIndirect,     "Accel_Z",    GUI_ID_EDIT5,    65,  180, 60,  20,  0, 0x8, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
static void DrawMeter(uint16_t x0,uint16_t y0,uint8_t r,GUI_COLOR CircleColor,GUI_COLOR NumColor,GUI_COLOR CenterColor,float data)
{
  uint8_t i;  
  int x_num,y_num;
  double temp;
  char ac[10];
  
  GUI_SetColor(CircleColor);
  GUI_SetPenSize( 10 );
  GUI_DrawCircle( x0, y0,r);
  
  GUI_SetColor(NumColor);
  for(i=0;i<36;++i)
  {
    float a = (90+i*10)*3.1415926/180;
    int x = -(r-5)*cos(a)+x0;
    int y = -(r-5)*sin(a)+y0;
    if (i%3 == 0)
      GUI_SetPenSize( 6 );
    else
      GUI_SetPenSize( 3 );
    
    GUI_DrawPoint(x,y);
    if(i%3==0)
    {
       signed char num;       
       x = -(r-20)*cos(a)+x0;
       y = -(r-15)*sin(a)+y0;
       if(i>18)
         num=i-36;
       else
         num=i;
       sprintf(ac, "%d", 10*num);      
       GUI_SetFont(GUI_FONT_10_ASCII);           
       GUI_SetTextAlign(GUI_TA_VCENTER);
       GUI_DispStringHCenterAt(ac,x,y);
    }    
  }
  GUI_SetColor(CenterColor);  
  GUI_SetPenSize( 12 );
  GUI_DrawPoint(x0,y0);
  
  GUI_SetPenSize( 2 );
  temp= (double)(90+data)*3.1415926/180;
  x_num= -(r-20)*cos(temp)+x0;
  y_num= -(r-20)*sin(temp)+y0;
  GUI_DrawLine(x0,y0,x_num,y_num);
  
  GUI_SetFont(GUI_FONT_13_ASCII);
  sprintf(ac,"%0.1f",data);
  GUI_DispStringHCenterAt(ac,x0,r/3+y0);
}
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialogCompass(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  // USER START (Optionally insert additional variables)
  // USER END
  switch (pMsg->MsgId) {
  case WM_DELETE:
		// USER START (Optionally insert additional code for further widget initialization)
		OS_INFO("CompassDLG delete\n");
		Flag_ICON4 = 0;
		UserApp_Flag = 0;
    tpad_flag=0;  
    EXTI_MPU_STOP();  
		// USER END
	  break;
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Compass'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetText(hItem,UTF8_MPU6050);
    FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
    //
    // Initialization of 'GYR0_X'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT0);
    EDIT_SetFont(hItem, GUI_FONT_20B_ASCII);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    EDIT_SetTextColor(hItem,EDIT_CI_ENABLED,DATACOLOR);
    EDIT_SetText(hItem, "0000");
    //
    // Initialization of 'GYR0_Y'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT1);
    EDIT_SetFont(hItem, GUI_FONT_20B_ASCII);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    EDIT_SetTextColor(hItem,EDIT_CI_ENABLED,DATACOLOR);   
    EDIT_SetText(hItem, "0000");
    //
    // Initialization of 'GYR0_Z'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT2);
    EDIT_SetFont(hItem, GUI_FONT_20B_ASCII);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    EDIT_SetTextColor(hItem,EDIT_CI_ENABLED,DATACOLOR);   
    EDIT_SetText(hItem, "0000");
    //
    // Initialization of 'Accel_X'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT3);
    EDIT_SetFont(hItem, GUI_FONT_20B_ASCII);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    EDIT_SetTextColor(hItem,EDIT_CI_ENABLED,DATACOLOR);   
    EDIT_SetText(hItem, "0000");
    //
    // Initialization of 'Accel_Y'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT4);
    EDIT_SetFont(hItem, GUI_FONT_20B_ASCII);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    EDIT_SetTextColor(hItem,EDIT_CI_ENABLED,DATACOLOR);   
    EDIT_SetText(hItem, "0000");
    //
    // Initialization of 'Accel_Z'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT5);
    EDIT_SetFont(hItem, GUI_FONT_20B_ASCII);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    EDIT_SetTextColor(hItem,EDIT_CI_ENABLED,DATACOLOR);   
    EDIT_SetText(hItem, "0000");
    // USER START (Optionally insert additional code for further widget initialization)
    
    // USER END
    break;
  // USER START (Optionally insert additional message handling)
  case WM_PAINT:
    WM_SetFocus(pMsg->hWin);
    GUI_SetColor(GUI_LIGHTGREEN); 
    GUI_SetFont(&XBF_XINSONGTI19);  
    GUI_DispStringHCenterAt(UTF8_YAW,360,65);
    GUI_DispStringHCenterAt(UTF8_PITCH,260,240);
    GUI_DispStringHCenterAt(UTF8_ROLL,460,240);  
  
    GUI_SetFont(GUI_FONT_16_ASCII);    
    GUI_DispStringAt("GYR0_X:", 10,30);
    GUI_DispStringAt("GYR0_Y:", 10,60);
    GUI_DispStringAt("GYR0_Z:", 10,90); 
    
    GUI_DispStringAt("Accel_X:", 10,120);
    GUI_DispStringAt("Accel_Y:", 10,150);
    GUI_DispStringAt("Accel_Z:", 10,180); 
  
    DrawMeter(360,105,90,GUI_BLUE,GUI_LIGHTBLUE,GUI_YELLOW,mpu6050_data.MPU6050_Yaw);
    DrawMeter(260,280,90,GUI_BLUE,GUI_LIGHTBLUE,GUI_YELLOW,mpu6050_data.MPU6050_Pitch);
    DrawMeter(460,280,90,GUI_BLUE,GUI_LIGHTBLUE,GUI_YELLOW,mpu6050_data.MPU6050_Roll);
    break;
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateCompass
*/
void FUN_ICON4Clicked(void)  
{
  uint8_t timecount=0;
  WM_HWIN hWin;
  char tempstr[20];
  
  OS_INFO("CompassDLG create\n");
  
  hWin = GUI_CreateDialogBox(_aDialogCreateCompass, GUI_COUNTOF(_aDialogCreateCompass), _cbDialogCompass, HDTWIN, 0, 0);
  GUI_Delay(100);

  EXTI_MPU_Config();
  mpu6050_init();
  
  while(Flag_ICON4)
  {
    if(timecount>=10)
    {
      GUI_RECT Rect={145,10,555,375};
      MPU6050_DataUpdate(&mpu6050_data);               
      sprintf(tempstr,"%d",mpu6050_data.MPU6050_Gyro_Raw[0]);
      EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0),tempstr);
      sprintf(tempstr,"%d",mpu6050_data.MPU6050_Gyro_Raw[1]);
      EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1),tempstr);
      sprintf(tempstr,"%d",mpu6050_data.MPU6050_Gyro_Raw[2]);
      EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2),tempstr);
      
      sprintf(tempstr,"%d",mpu6050_data.MPU6050_Accel_Raw[0]);
      EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT3),tempstr);
      sprintf(tempstr,"%d",mpu6050_data.MPU6050_Accel_Raw[1]);
      EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT4),tempstr);
      sprintf(tempstr,"%d",mpu6050_data.MPU6050_Accel_Raw[2]);
      EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT5),tempstr);         
            
      WM_InvalidateRect(hWin,&Rect);       
      timecount=0;
    }    
    timecount++;    
    if(tpad_flag)WM_DeleteWindow(hWin);
    GUI_Delay(10);
  }
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
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
#include "./Bsp/ov2640/bsp_ov2640.h"
// USER END

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
OV2640_IDTypeDef OV2640_Camera_ID;
// USER END


/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END


/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
void cbCameraWin(WM_MESSAGE * pMsg)
{
	switch (pMsg->MsgId) 
	{
	  case WM_DELETE:
			OS_INFO("CameraDLG delete\n");
			Flag_ICON7 = 0;
		  UserApp_Flag = 0;
      tpad_flag=0;
			break;
		case WM_PAINT:
      #if   LCD_NUM_LAYERS == 1 
		    GUI_SetBkColor(APPBKCOLOR);
      #elif LCD_NUM_LAYERS == 2    
        GUI_SetBkColor(GUI_TRANSPARENT);        
      #endif
			GUI_Clear();  
			break;
	}
}
/*********************************************************************
*
*       CreateCamera
*/
void FUN_ICON7Clicked(void)
{
  OS_ERR  err;
  WM_HWIN hWin;
  OS_INFO("CameraDLG create\n");
  
  hWin = WM_CreateWindowAsChild(0,0,800,480,HDTWIN,WM_CF_SHOW,cbCameraWin,0);
  GUI_Delay(100);
  
  /* Initializes the DCMI interface (I2C and GPIO) used to configure the camera */
  OV2640_HW_Init();  
  
  /* Read the OV9655/OV2640 Manufacturer identifier */
  OV2640_ReadID(&OV2640_Camera_ID);
  if(OV2640_Camera_ID.PIDH  == 0x26)
  {
    bsp_result &=~BSP_CAMERA;
  }
  else
  {
    bsp_result |= BSP_CAMERA;
  }
  
  if(bsp_result&BSP_CAMERA)
  {    
    GUI_Delay(100);    
    ErrorDialog(hWin,"Camera Error","camera drvice cannot work!");
    while(1)
    {
      if(tpad_flag)WM_DeleteWindow(hWin);
       if(!Flag_ICON7)return;
       GUI_Delay(10);
    }
  }
  LCD_LayerInit(0);
  OV2640_QVGAConfig(); 
  OV2640_Init();   
  DCMI_Cmd(ENABLE); 
  DCMI_CaptureCmd(ENABLE); 
  OSSchedLock(&err);
  while(Flag_ICON7)
  {     
    if(TPAD_Scan(0))//GetPinStateOfKey2()==1)
    {
      LCD_BL_OFF();   
      OV2640_Stop();
      LCD_LayerInit(1); 
      OSSchedUnlock(&err);
      BEEP_ON;              
      WM_DeleteWindow(hWin);      
    }
  }
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/

#include    "Type.h"
#include    "GxImage.h"
#include    "ldws_int.h"
#include    "ldws_lib.h"
#include    "IPL_Utility.h"
#include    "IPL_Cmd.h"

#define THIS_DBGLVL         0 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          LDWSlib
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define THIRDPARTY_A_LDWS_BUF_SIZE (1280*720*5)// working buffer size

UINT32              gldwsDectionFlag = FALSE;
UINT32              gldwsInitialFlag = OFF;
//UINT32              sdCurInitialFlag = OFF;
static UINT32       gldwsBufAddr = 0;
UINT32              gldwsSrcFrameAddr;
UINT32              gldwsFrameWidth = 0, gldwsFrameHeight = 0, gldwsFrameLineOfs=0;
LDWS_CBMSG_FP       gLdwsCbFP = NULL;
BOOL                gLdwsLock = FALSE;
UINT32              gLDWSRate = 5;
UINT32              gLDWScount = 0;



UINT32 LDWS_CalcBuffSize(void)
{
    UINT32 BuffSize;

    DBG_IND("\r\n LDWS:: LDWS_CalcBuffSize \r\n" );
    BuffSize = THIRDPARTY_A_LDWS_BUF_SIZE;//third party required

    return BuffSize;
}

INT32 LDWS_Init(MEM_RANGE *buf, UINT32 ProcessRate)
{
    //IMG_BUF   in_img;

    if (!buf)
    {
        DBG_ERR("buf is NULL\r\n");
        return LDWS_STA_ERROR;
    }
    if (!buf->Addr)
    {
        DBG_ERR("bufAddr is NULL\r\n");
        return LDWS_STA_ERROR;
    }
    if (buf->Size < LDWS_CalcBuffSize())
    {
        DBG_ERR("bufSize %d < need %d\r\n",buf->Size,LDWS_CalcBuffSize());
        return LDWS_STA_ERROR;
    }
    //LDWS initial
    gldwsBufAddr = buf->Addr;
    gLDWSRate = ProcessRate;

    DBG_IND("LDWS:: buf Addr =0x%x size=%d, ProcessRate =%d\r\n", buf->Addr,buf->Size,ProcessRate);

    return LDWS_STA_OK;

}

INT32 LDWS_UnInit(void)
{
    DBG_ERR(" \r\n");
    LDWS_SetInitFlag(OFF);
    return LDWS_STA_OK;
}


INT32 LDWS_Process(void)
{
    INT32 Status;

    gLDWScount++;

    if (!gLdwsLock)
    {
        if((gLDWScount%gLDWSRate)==0)
        {

            if (LDWS_CheckInitFlag() == OFF)
            {
               DBG_ERR("\r\n LDWS is not initial yet \r\n");
               return LDWS_STA_ERROR;
            }
            else
            {

               Status=LDWS_Detection();//The main detection function


               if((gLDWScount%30)==0)//debug message
                {
                    DBG_IND("\r\n LDWS Detection Status=%d \r\n",Status );//debug message
               }
               return Status;


            }
        }
    }
    return LDWS_STA_OK;
}

//IPL_YCC_IMG_INFO IPL_YCC_IMG_INFO1;
IPL_IME_BUF_ADDR IPL_IME_CurInfo1;
LDWS_INFO gLdws_infor1;

INT32 LDWS_Detection(void)//The main LDWS Detection code
{

    IPL_IME_CurInfo1.Id = IPL_ID_1;

//Get source frame data from ime output
    IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&IPL_IME_CurInfo1);
    gldwsSrcFrameAddr=IPL_IME_CurInfo1.ImeP1.PixelAddr[1];//Image source frame buffer Y address
    gldwsFrameWidth=IPL_IME_CurInfo1.ImeP1.Ch[1].Width;//Image source width
    gldwsFrameHeight=IPL_IME_CurInfo1.ImeP1.Ch[1].Height;//Image source height
    gldwsFrameLineOfs=IPL_IME_CurInfo1.ImeP1.Ch[1].LineOfs;//Image source line offset

    if((gLDWScount%30)==0)//debug message
    {
        DBG_IND("\r\n LDWS:: Frame Addr =0x %x  \r\n",gldwsSrcFrameAddr );
        DBG_IND("\r\n LDWS:: Frame Buffer %d x %d \r\n",gldwsFrameWidth,gldwsFrameHeight );
    }

//////////////////////////////////////////////////////////////////////////
//Put the main detection code here~






/////////////////////////////////////////////////////////////////////////


//Output the detection result~
    gLdws_infor1.ldws_status=0;
    gLdws_infor1.ldws_warning=0;// 1 for test
    gLdws_infor1.Line1_m=0;
    gLdws_infor1.Line1_b=0;
    gLdws_infor1.Line2_m=0;
    gLdws_infor1.Line2_b=0;

    if (gLdws_infor1.ldws_warning == TRUE)
    {
     if(gLdwsCbFP)
       {
        DBG_IND("\r\n LDWS:: gLdwsCbFP callback function   \r\n" );
        gLdwsCbFP(LDWS_CBMSG_WARNNING, (void *)&gLdws_infor1);
       }
     else
        {
          DBG_IND("\r\n LDWS:: gLdwsCbFP is not registered...   \r\n" );

        }
     }
    return LDWS_STA_OK;
}

UINT32 LDWS_CheckInitFlag(void)
{
    return gldwsInitialFlag;
}

void LDWS_SetInitFlag(UINT32 Flag)
{
    gldwsInitialFlag = Flag;
}

/**
    get detection information
    @param lock  TRUE:LDWS OK  FALSE:LDWS Fail
*/
LDWS_INFO LDWS_GetResult(void)
{
    return gLdws_infor1;
}

/**
     Set working buffer of LDWS.

     @param[in] BufAddr: The working buffer starting address.
*/

void LDWS_RegisterCB(LDWS_CBMSG_FP CB)
{
    gLdwsCbFP = CB;
}

void LDWS_Lock(BOOL bLock)
{
    gLdwsLock = bLock;
}

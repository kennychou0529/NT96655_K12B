#include "UIAppMovie.h"
#include "NVTUserCommand.h"
#include "uimode.h"
#include "ae_task.h"
#include "awb_task.h"
#include "SysCfg.h"
#include "MediaRecAPI.h"
// Audio NR
#include "AudFilterAPI.h"
#include "UIAppMovie_AudNR.h"
#include "Lens.h"
#include "ldws_nvt_lib.h"

#define __MODULE__          UIMovieCB
//#define __DBGLVL__ 0        //OFF mode, show nothing
//#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

#define AUDNR_DELAYFRAME  2

// Lens State
static LENS_CB_MESSAGE gUIMovieLensState = 0;

UINT32 gAudNrIrisDelayFrame = AUDNR_DELAYFRAME;
UINT32 gAudNrAFDelayFrame = AUDNR_DELAYFRAME;
BOOL   gbAudNrZoomOperating = FALSE;

extern BOOL UIFlowWndMovie_IsGsensorTrig(void);
extern void UIFlowWndMovie_SetGsensorTrigFlag(BOOL En);

//-------------------------------------------------------------------------------------------------
//
// Callback function for audio filtering & Noise Reduction
//
//-------------------------------------------------------------------------------------------------
void UIMovie_AudFilterProc(void)
{
    // Audio NR
    UINT32              uiNrStopFreq, uiNrReplaceFreq, uiNrDecreaseLvl, uiNrMixBackgrd;
    //INT32               iNrBandNum;
    AUDNR_NOISE_SPEC    *pNoiseSpec = NULL;

    //
    //  Default Config
    //
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_SAMPLE_RATE,            32000);
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_CHANNEL_NO,             2);

    //
    // AudNR Background Suppress Enabled
    //
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_BACKGROUND_ENABLE,       FALSE);
    //AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_BACKGROUND_DECREASE_LVL, AUDNR_COEF(4.0));
    //AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_BACKGROUND_LOWER_BOUND,  AUDNR_COEF(0.1));

    //
    // Aud NR Enabled
    //
    switch (gUIMovieLensState)
    {
        case LENS_CB_ZOOM_START: // Zoom
        {
            //DBG_IND("Lens state is Zoom Moving!\r\n");
            uiNrStopFreq    = gAudNrZoomStopFreq;
            uiNrReplaceFreq = gAudNrZoomReplaceFreq;
            uiNrDecreaseLvl = gAudNrZoomDecreaseLvl;
            uiNrMixBackgrd  = gAudNrZoomMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Zoom;
            //iNrBandNum      = gAudNrZoomBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrZoomBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrZoomNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrZoomNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrZoomNoiseSpec;

            gAudNrAFDelayFrame = gAudNrIrisDelayFrame = AUDNR_DELAYFRAME;
            gbAudNrZoomOperating = TRUE;
            AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                         TRUE);
        }
        break;

        case LENS_CB_FOCUS_START: // Focus
        {
            //DBG_IND("Lens state is Focus Moving!\r\n");
            if (gbAudNrZoomOperating) return; // Zoom operation is enabling, skip AF event
            uiNrStopFreq    = gAudNrFocusStopFreq;
            uiNrReplaceFreq = gAudNrFocusReplaceFreq;
            uiNrDecreaseLvl = gAudNrFocusDecreaseLvl;
            uiNrMixBackgrd  = gAudNrFocusMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Focus;
            //iNrBandNum      = gAudNrFocusBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrFocusBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrFocusNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrFocusNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrFocusNoiseSpec;

            gAudNrAFDelayFrame = 0; // Reset delay frame count
            AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                         TRUE);
        }
        break;

        case LENS_CB_APERTURE_START: // Iris
        {
            // Support CAF case
            if((gAudNrAFDelayFrame < AUDNR_DELAYFRAME) ||(gbAudNrZoomOperating)) // Zoom operation and Auto Focus is enabling, skip AE event
                return;

            //DBG_IND("Lens state is Iris Moving!\r\n");
            uiNrStopFreq    = gAudNrIrisStopFreq;
            uiNrReplaceFreq = gAudNrIrisReplaceFreq;
            uiNrDecreaseLvl = gAudNrIrisDecreaseLvl;
            uiNrMixBackgrd  = gAudNrIrisMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Iris;
            //iNrBandNum      = gAudNrIrisBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrIrisBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrIrisNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrIrisNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrIrisNoiseSpec;

            gAudNrIrisDelayFrame = 0; // Reset delay frame count
            AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                         TRUE);
        }
        break;


        default:
        case LENS_CB_ZOOM_END:
        {
            //DBG_IND("Lens state is ZOOM end!\r\n");
            uiNrStopFreq    = gAudNrZoomStopFreq;
            uiNrReplaceFreq = gAudNrZoomReplaceFreq;
            uiNrDecreaseLvl = gAudNrZoomDecreaseLvl;
            uiNrMixBackgrd  = gAudNrZoomMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Zoom;
            //iNrBandNum      = gAudNrZoomBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrZoomBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrZoomNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrZoomNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrZoomNoiseSpec;

            gbAudNrZoomOperating = FALSE;
            AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                         FALSE);
        }
        break;

        case LENS_CB_FOCUS_END:
        {
            //DBG_IND("Lens state is Idle!\r\n");
            if (gbAudNrZoomOperating) return; // Zoom operation is enabling, skip AF event

            uiNrStopFreq    = gAudNrFocusStopFreq;
            uiNrReplaceFreq = gAudNrFocusReplaceFreq;
            uiNrDecreaseLvl = gAudNrFocusDecreaseLvl;
            uiNrMixBackgrd  = gAudNrFocusMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Focus;
            //iNrBandNum      = gAudNrFocusBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrFocusBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrFocusNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrFocusNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrFocusNoiseSpec;

            if(gAudNrAFDelayFrame >= AUDNR_DELAYFRAME)
            {
                //DBG_IND("IRIS Lens state is Idle!\r\n");
                AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                     FALSE);
            }
            else
            {
                //DBG_IND("IRIS Lens state is not Idle!\r\n");
                gAudNrAFDelayFrame++;
                AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                     TRUE);
            }
        }
        break;

        case LENS_CB_APERTURE_END:
        {
            // Support CAF case
            if((gAudNrAFDelayFrame < AUDNR_DELAYFRAME) ||(gbAudNrZoomOperating)) // Zoom operation and Auto Focus is enabling, skip AE event
                return;

            uiNrStopFreq    = gAudNrIrisStopFreq;
            uiNrReplaceFreq = gAudNrIrisReplaceFreq;
            uiNrDecreaseLvl = gAudNrIrisDecreaseLvl;
            uiNrMixBackgrd  = gAudNrIrisMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Iris;
            //iNrBandNum      = gAudNrIrisBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrIrisBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrIrisNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrIrisNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrIrisNoiseSpec;

            if(gAudNrIrisDelayFrame >= AUDNR_DELAYFRAME)
            {
                //DBG_IND("IRIS Lens state is Idle!\r\n");
                AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                     FALSE);
            }
            else
            {
                //DBG_IND("IRIS Lens state is not Idle!\r\n");
                gAudNrIrisDelayFrame++;
                AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                     TRUE);
            }
        }
        break;
    }

    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_STOP_FREQUENCY,         uiNrStopFreq);
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_REPLACE_FREQUENCY,   uiNrReplaceFreq);
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_MIX_BACKGROUND,       uiNrMixBackgrd);
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_DECREASE_LVL,        uiNrDecreaseLvl);

    // Noise Pattern Spec.
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_SPEC,             (UINT32)pNoiseSpec);

}

//-------------------------------------------------------------------------------------------------
//
// API function for Lens state updating
//
//-------------------------------------------------------------------------------------------------
void UIMovie_UpdateLensState(UINT32 LensState)
{
    gUIMovieLensState = LensState;
}

//-------------------------------------------------------------------------------------------------
//
// Callback function for the events which should be processed immediately (cannot use post event)
//
//-------------------------------------------------------------------------------------------------
void UIMovie_3DNRSetting(void)
{
    UINT32  ui3DNRLevel;

    if (UI_GetData(FL_MovieMCTFIndex) == MOVIE_MCTF_OFF)
    {
        ui3DNRLevel = H264_3DNR_DISABLE;
    }
    else
    {
        ui3DNRLevel = UIMovie_3DNRAdjust();
      }

    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_H2643DNRLEVEL, ui3DNRLevel, 0, 0);
}

void UIMovie_ImmProc(UINT32 type, UINT32 p1, UINT32 p2, UINT32 p3)
{
    switch (type)
    {
    case MEDIAREC_IMMPROC_CUTFILE_END:
        #if (UI_STYLE==UI_STYLE_DRIVER)
        // update DCF Folder/File name
        FlowDCF_UpdateName();
	if(UIFlowWndMovie_IsGsensorTrig()==TRUE)
	{
		UIFlowWndMovie_SetGsensorTrigFlag(FALSE);
		FlowMovie_IconHideLock(); //eric 
	}
        #endif

        // [TO DO]: log GPS data
        DBG_IND("EVENT %d: Cut file end!\r\n", type);
        break;

    case MEDIAREC_IMMPROC_DELETE_FILE:
        // [TO DO]: delete related files, such as GPS data file
        DBG_IND("EVENT %d: Delete file!\r\n", type);
        break;

    // special dynamic encoding parameters setting, such as H.264 3DNR
    case MEDIAREC_IMMPROC_ENC_SETTING:
        UIMovie_3DNRSetting();
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------------------------
//
// Callback function for AF events
//
//-------------------------------------------------------------------------------------------------
void UIMovie_AFCB(AF_CBMSG uiMsgID, void *Data)
{
    UINT32 uiData;

    switch (uiMsgID)
    {
        case AF_CBMSG_AF_START:
            uiData = *(UINT32 *)Data;
            DBG_IND("===AF Start! - %d===\r\n", uiData);
            break;
        case AF_CBMSG_AF_DONE:
            uiData = *(UINT32 *)Data;
            DBG_IND("===AF Done! - %d===\r\n", uiData);
            AE_Lock(AE_ID_1,FALSE,FALSE);
            AWB_Lock(AWB_ID_1,FALSE,FALSE);
            Ux_PostEvent(NVTEVT_ALGMSG_FOCUSEND,1, NVTEVT_ALGMSG_FOCUSEND);
            break;
        default:
            break;
    }
}

//-------------------------------------------------------------------------------------------------
//
// Callback function for LDWS events
//
//-------------------------------------------------------------------------------------------------
#include "GxSystem.h"
#include "GxFlash.h"
#include "GxDC.h"
#include "Uiresource.h"
#include "uiDisplay.h"
#include "UIFlow.h"

#define LDWS_ROI_WIDTH 720
#define LDWS_ROI_HEIGHT 480

#define OSD_RATIO_16_9  1
#define OSD_RATIO_4_3   2
#define OSD_RATIO       OSD_RATIO_4_3

#if ((OSD_RATIO == OSD_RATIO_4_3) && (DISPLAY_169HD == DISABLE))
#define LDWS_OSD_ROIX(ROIX, ROI_SX, InWidth, InHeight) ((ROIX+ROI_SX)/(InWidth/OSD_W))
#define LDWS_OSD_ROIY(ROIY, ROI_SY, InWidth, InHeight) ((ROIY+ROI_SY)/(InWidth/OSD_W) + 30)

#define LDWS_OSD_VPX(VPx,ROI_SX, InWidth, InHeight) (VPx/(InWidth/OSD_W))
#define LDWS_OSD_VPY(VPy,ROI_SY, InWidth, InHeight) (VPy/(InWidth/OSD_W) + 30)

#define LDWS_OSD_LANEX(VideoX, ROI_SX, InWidth, InHeight) ((VideoX+ROI_SX)/(InWidth/OSD_W))
#define LDWS_OSD_LANEY(VideoY, ROI_SY, InWidth, InHeight) ((VideoY+ROI_SY)/(InWidth/OSD_W) + 30)
#elif ((OSD_RATIO == OSD_RATIO_4_3) && (DISPLAY_169HD == ENABLE))
#define LDWS_OSD_ROIX(ROIX, ROI_SX, InWidth, InHeight) ((ROIX+ROI_SX)*OSD_W/InWidth)
#define LDWS_OSD_ROIY(ROIY, ROI_SY, InWidth, InHeight) ((ROIY+ROI_SY)*OSD_H/InHeight)

#define LDWS_OSD_VPX(VPx,ROI_SX, InWidth, InHeight) (VPx*OSD_W/InWidth)
#define LDWS_OSD_VPY(VPy,ROI_SY, InWidth, InHeight) (VPy*OSD_H/InHeight)

#define LDWS_OSD_LANEX(VideoX, ROI_SX, InWidth, InHeight) ((VideoX+ROI_SX)*OSD_W/InWidth)
#define LDWS_OSD_LANEY(VideoY, ROI_SY, InWidth, InHeight) ((VideoY+ROI_SY)*OSD_H/InHeight)
#elif ((OSD_RATIO == OSD_RATIO_16_9) && (DISPLAY_169HD == ENABLE))
#define LDWS_OSD_ROIX(ROIX, ROI_SX, InWidth, InHeight) ((ROIX+ROI_SX)/(InWidth/OSD_W))
#define LDWS_OSD_ROIY(ROIY, ROI_SY, InWidth, InHeight) ((ROIY+ROI_SY)/(InHeight/OSD_H))

#define LDWS_OSD_VPX(VPx,ROI_SX, InWidth, InHeight) (VPx/(InWidth/OSD_W))
#define LDWS_OSD_VPY(VPy,ROI_SY, InWidth, InHeight) (VPy/(InHeight/OSD_H))

#define LDWS_OSD_LANEX(VideoX, ROI_SX, InWidth, InHeight) ((VideoX+ROI_SX)/(InWidth/OSD_W))
#define LDWS_OSD_LANEY(VideoY, ROI_SY, InWidth, InHeight) ((VideoY+ROI_SY)/(InHeight/OSD_H))
#endif

UINT32 MovieLine[6][10][5];
UINT32 MovieLineIdx = 0;
LDWS_NVT_INFO g_LwdsInfo = {0};
void UIMovie_LDWSCB(LDWS_CBMSG uiMsgID, void *Data)
{
    DC**   pDCList;
    UINT32 Video_VPx1=0, Video_VPy1=0, Video_VPx2=0, Video_VPy2=0;
    UINT32 Video_RoiXYArr[4*2]={0}; // [X0, X1, X2, X3, Y0, Y1, Y2, Y3]
    UINT32 iML;
    INT32  MovieLineCnt = 0;
    LDWS_OSD_COORDINATE LdwsOsdCoord = {0};

    switch (uiMsgID)
    {
        case LDWS_NVT_CBMSG_WARNNING:

             if (FlowMovie_GetMovDataState() !=  MOV_ST_REC)
             {
                break;
             }

             g_LwdsInfo = *(LDWS_NVT_INFO*)Data;
#if 0
             debug_msg("g_LwdsInfo.LD_In_Width  = %d\r\n", g_LwdsInfo.LD_In_Width);
             debug_msg("g_LwdsInfo.LD_In_Height = %d\r\n", g_LwdsInfo.LD_In_Height);
             debug_msg("g_LwdsInfo.ROI_SX       = %d\r\n", g_LwdsInfo.ROI_SX);
             debug_msg("g_LwdsInfo.ROI_SY       = %d\r\n", g_LwdsInfo.ROI_SY);
             debug_msg("g_LwdsInfo.Lx1          = %d\r\n", g_LwdsInfo.Lx1);
             debug_msg("g_LwdsInfo.Ly1          = %d\r\n", g_LwdsInfo.Ly1);
             debug_msg("g_LwdsInfo.Lx2          = %d\r\n", g_LwdsInfo.Lx2);
             debug_msg("g_LwdsInfo.Ly2          = %d\r\n", g_LwdsInfo.Ly2);
             debug_msg("g_LwdsInfo.Rx1          = %d\r\n", g_LwdsInfo.Rx1);
             debug_msg("g_LwdsInfo.Ry1          = %d\r\n", g_LwdsInfo.Ry1);
             debug_msg("g_LwdsInfo.Rx2          = %d\r\n", g_LwdsInfo.Rx2);
             debug_msg("g_LwdsInfo.Ry2          = %d\r\n", g_LwdsInfo.Ry2);
             debug_msg("Fail mode               = %d\r\n", g_LwdsInfo.Failure);
             debug_msg("Departure Dir           = %d\r\n", g_LwdsInfo.Departure_dir);
#endif
            /*--- draw on OSD ---*/
            //draw ROI
            Video_RoiXYArr[0] = g_LwdsInfo.ROI_SX; // X1
            Video_RoiXYArr[4] = g_LwdsInfo.ROI_SY; // Y1
            Video_RoiXYArr[1] = g_LwdsInfo.ROI_SX + g_LwdsInfo.ROI_WIDTH; // X2
            Video_RoiXYArr[5] = g_LwdsInfo.ROI_SY; // Y2
            Video_RoiXYArr[2] = g_LwdsInfo.ROI_SX; // X3
            Video_RoiXYArr[6] = g_LwdsInfo.ROI_SY + g_LwdsInfo.ROI_HEIGHT; // Y3
            Video_RoiXYArr[3] = g_LwdsInfo.ROI_SX + g_LwdsInfo.ROI_WIDTH; //X4
            Video_RoiXYArr[7] = g_LwdsInfo.ROI_SY + g_LwdsInfo.ROI_HEIGHT; // Y4
            LdwsOsdCoord.ROIx1 = LDWS_OSD_ROIX(Video_RoiXYArr[0]-g_LwdsInfo.ROI_SX, g_LwdsInfo.ROI_SX, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            LdwsOsdCoord.ROIy1 = LDWS_OSD_ROIY(Video_RoiXYArr[4]-g_LwdsInfo.ROI_SY, g_LwdsInfo.ROI_SY, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            LdwsOsdCoord.ROIx2 = LDWS_OSD_ROIX(Video_RoiXYArr[1]-g_LwdsInfo.ROI_SX, g_LwdsInfo.ROI_SX, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            LdwsOsdCoord.ROIy2 = LDWS_OSD_ROIY(Video_RoiXYArr[5]-g_LwdsInfo.ROI_SY, g_LwdsInfo.ROI_SY, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            LdwsOsdCoord.ROIx3 = LDWS_OSD_ROIX(Video_RoiXYArr[2]-g_LwdsInfo.ROI_SX, g_LwdsInfo.ROI_SX, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            LdwsOsdCoord.ROIy3 = LDWS_OSD_ROIY(Video_RoiXYArr[6]-g_LwdsInfo.ROI_SY, g_LwdsInfo.ROI_SY, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            LdwsOsdCoord.ROIx4 = LDWS_OSD_ROIX(Video_RoiXYArr[3]-g_LwdsInfo.ROI_SX, g_LwdsInfo.ROI_SX, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            LdwsOsdCoord.ROIy4 = LDWS_OSD_ROIY(Video_RoiXYArr[7]-g_LwdsInfo.ROI_SY, g_LwdsInfo.ROI_SY, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);

            //vanishing point
            Video_VPx1 = (g_LwdsInfo.LD_In_Width - 48)/2;
            Video_VPy1 = g_LwdsInfo.LD_In_Height/2;
            Video_VPx2 = (g_LwdsInfo.LD_In_Width + 48)/2;
            Video_VPy2 = g_LwdsInfo.LD_In_Height/2;
            LdwsOsdCoord.VPx1 = LDWS_OSD_VPX(Video_VPx1, g_LwdsInfo.ROI_SX, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            LdwsOsdCoord.VPy1 = LDWS_OSD_VPY(Video_VPy1, g_LwdsInfo.ROI_SY, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            LdwsOsdCoord.VPx2 = LDWS_OSD_VPX(Video_VPx2, g_LwdsInfo.ROI_SX, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            LdwsOsdCoord.VPy2 = LDWS_OSD_VPY(Video_VPy2, g_LwdsInfo.ROI_SY, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);

            // lane
            LdwsOsdCoord.LaneLx1 = LDWS_OSD_LANEX(g_LwdsInfo.Lx1, g_LwdsInfo.ROI_SX, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            LdwsOsdCoord.LaneLy1 = LDWS_OSD_LANEY(g_LwdsInfo.Ly1, g_LwdsInfo.ROI_SY, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            LdwsOsdCoord.LaneLx2 = LDWS_OSD_LANEX(g_LwdsInfo.Lx2, g_LwdsInfo.ROI_SX, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            LdwsOsdCoord.LaneLy2 = LDWS_OSD_LANEY(g_LwdsInfo.Ly2, g_LwdsInfo.ROI_SY, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            LdwsOsdCoord.LaneRx1 = LDWS_OSD_LANEX(g_LwdsInfo.Rx1, g_LwdsInfo.ROI_SX, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            LdwsOsdCoord.LaneRy1 = LDWS_OSD_LANEY(g_LwdsInfo.Ry1, g_LwdsInfo.ROI_SY, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            LdwsOsdCoord.LaneRx2 = LDWS_OSD_LANEX(g_LwdsInfo.Rx2, g_LwdsInfo.ROI_SX, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            LdwsOsdCoord.LaneRy2 = LDWS_OSD_LANEY(g_LwdsInfo.Ry2, g_LwdsInfo.ROI_SY, g_LwdsInfo.LD_In_Width, g_LwdsInfo.LD_In_Height);
            FlowMovie_LwdsProcess(LdwsOsdCoord);
#if 1
            // draw on video
            //=================================================================
            iML = MovieLineIdx % 6;
            MovieLineCnt = 0;
            memset(MovieLine[iML], 0, sizeof(UINT32)*10*5);

            //draw ROI
            MovieLine[iML][MovieLineCnt][0] = Video_RoiXYArr[0];
            MovieLine[iML][MovieLineCnt][1] = Video_RoiXYArr[4];
            MovieLine[iML][MovieLineCnt][2] = Video_RoiXYArr[1];
            MovieLine[iML][MovieLineCnt][3] = Video_RoiXYArr[5];
            if (g_LwdsInfo.Failure == LDWS_FAILURE_TRUE)
            {
                MovieLine[iML][MovieLineCnt][4] = 1; // red
            }else{
                MovieLine[iML][MovieLineCnt][4] = 2; // green
            }
            MovieLineCnt++;

            MovieLine[iML][MovieLineCnt][0] = Video_RoiXYArr[0];
            MovieLine[iML][MovieLineCnt][1] = Video_RoiXYArr[4];
            MovieLine[iML][MovieLineCnt][2] = Video_RoiXYArr[2];
            MovieLine[iML][MovieLineCnt][3] = Video_RoiXYArr[6];
            if (g_LwdsInfo.Failure == LDWS_FAILURE_TRUE)
            {
                MovieLine[iML][MovieLineCnt][4] = 1;
            }else{
                MovieLine[iML][MovieLineCnt][4] = 2;
            }
            MovieLineCnt++;

            MovieLine[iML][MovieLineCnt][0] = Video_RoiXYArr[1];
            MovieLine[iML][MovieLineCnt][1] = Video_RoiXYArr[5];
            MovieLine[iML][MovieLineCnt][2] = Video_RoiXYArr[3];
            MovieLine[iML][MovieLineCnt][3] = Video_RoiXYArr[7];
            if (g_LwdsInfo.Failure == LDWS_FAILURE_TRUE)
            {
                MovieLine[iML][MovieLineCnt][4] = 1;
            }else{
                MovieLine[iML][MovieLineCnt][4] = 2;
            }
            MovieLineCnt++;

            MovieLine[iML][MovieLineCnt][0] = Video_RoiXYArr[2];
            MovieLine[iML][MovieLineCnt][1] = Video_RoiXYArr[6];
            MovieLine[iML][MovieLineCnt][2] = Video_RoiXYArr[3];
            MovieLine[iML][MovieLineCnt][3] = Video_RoiXYArr[7];
            if (g_LwdsInfo.Failure == LDWS_FAILURE_TRUE)
            {
                MovieLine[iML][MovieLineCnt][4] = 1;
            }else{
                MovieLine[iML][MovieLineCnt][4] = 4;
            }
            MovieLineCnt++;

            // draw vanishing point
            MovieLine[iML][MovieLineCnt][0] = Video_VPx1;
            MovieLine[iML][MovieLineCnt][1] = Video_VPy1;
            MovieLine[iML][MovieLineCnt][2] = Video_VPx2;
            MovieLine[iML][MovieLineCnt][3] = Video_VPy2;
            MovieLine[iML][MovieLineCnt][4] = 2;
            MovieLineCnt++;

            // draw lane
            MovieLine[iML][MovieLineCnt][0] = (UINT32)(g_LwdsInfo.Lx1 + g_LwdsInfo.ROI_SX);
            MovieLine[iML][MovieLineCnt][1] = (UINT32)(g_LwdsInfo.Ly1 + g_LwdsInfo.ROI_SY);
            MovieLine[iML][MovieLineCnt][2] = (UINT32)(g_LwdsInfo.Lx2 + g_LwdsInfo.ROI_SX);
            MovieLine[iML][MovieLineCnt][3] = (UINT32)(g_LwdsInfo.Ly2 + g_LwdsInfo.ROI_SY);
            if (g_LwdsInfo.Departure_dir == LDWS_DEPARTURE_LEFT)
            {
                MovieLine[iML][MovieLineCnt][4] = 1; // red
            }else{
                MovieLine[iML][MovieLineCnt][4] = 2; // green
            }

            //DBG_DUMP("Left lane info %d %d %d %d\r\n",MovieLine[iML][MovieLineCnt][0],MovieLine[iML][MovieLineCnt][1],MovieLine[iML][MovieLineCnt][2],MovieLine[iML][MovieLineCnt][3]);
            MovieLineCnt++;

            MovieLine[iML][MovieLineCnt][0] = (UINT32)(g_LwdsInfo.Rx1 + g_LwdsInfo.ROI_SX);
            MovieLine[iML][MovieLineCnt][1] = (UINT32)(g_LwdsInfo.Ry1 + g_LwdsInfo.ROI_SY);
            MovieLine[iML][MovieLineCnt][2] = (UINT32)(g_LwdsInfo.Rx2 + g_LwdsInfo.ROI_SX);
            MovieLine[iML][MovieLineCnt][3] = (UINT32)(g_LwdsInfo.Ry2 + g_LwdsInfo.ROI_SY);
            if (g_LwdsInfo.Departure_dir == LDWS_DEPARTURE_RIGHT)
            {
                MovieLine[iML][MovieLineCnt][4] = 1; // red
            }else{
                MovieLine[iML][MovieLineCnt][4] = 2; // green
            }
            //DBG_DUMP("right lane info %d %d %d %d\r\n",MovieLine[iML][MovieLineCnt][0],MovieLine[iML][MovieLineCnt][1],MovieLine[iML][MovieLineCnt][2],MovieLine[iML][MovieLineCnt][3]);

            MovieLineCnt++;

            //=================================================================
#endif
            break;
        default:
            break;
    }



}

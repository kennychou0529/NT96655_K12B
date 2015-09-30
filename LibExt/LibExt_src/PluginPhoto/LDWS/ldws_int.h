#ifndef _LDWSINT_H
#define _LDWSINT_H


UINT32 LDWS_CheckInitFlag(void);
void LDWS_SetInitFlag(UINT32 Flag);
//INT32 ldws_Init(UINT32 BufAddr, UINT32 ImgWidth, UINT32 ImgHeight, UINT32 ImgLineOffset, UINT32 ImgFmt);
INT32 LDWS_Detection(void);

#endif

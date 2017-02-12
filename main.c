/******************************************************************************
  File Name     : main.c
  Version       : Initial Draft
  Author        : robben
  Created       : 2017/01/05
  Description   :
  History       :
  1.Date        : 2017/01/05
    Author      : robben
    Modification: Created file
******************************************************************************/
/******************************* Include Files *******************************/
#include <stdio.h>
#include <stdlib.h>                                                           
/****************************** Macro Definition *****************************/
#define DEBUG_INFO

#define MJ_S32 int
#define MJ_U32 unsigned int
#define MJ_U16 unsigned short
#define MJ_U8  unsigned char

#define PRINT_BANNER()  printf("|                                            |\n");\
                printf("|                                            |\n");\
                printf("|                                            |\n");

/************************ Static Structure Definition ************************/
typedef enum {
    MARKER_SOF0 = 0xC0,
    MARKER_DHT  = 0XC4,
    MARKER_SOI  = 0XD8,
    MARKER_EOI,
    MARKER_DQT  = 0xDB,
    MARKER_APP0 = 0XE0,
}MJ_MARKER_E;

typedef struct {
    MJ_U16 u32FramHdrLen; 
    MJ_U8  u32SamplePrecision; 
    MJ_U16 u32PicWidth; 
    MJ_U16 u32PicHeight; 
    MJ_U8  Nf; 
    MJ_U16 Ci; 
    MJ_U8  Hi;    
    MJ_U8  Vi; 
    MJ_U8 Tqi; 
}PIC_HEADER_S;
/***************************** Global Definition *****************************/
/***************************** Static Definition *****************************/


/*********************************** Code ************************************/
int parseFrameHeader(MJ_U8 *pData, MJ_S32 length, PIC_HEADER_S* pstPicHeader)
{
    int i = 0;

    if(NULL==pData|| 0==length)
        return -1;

    pstPicHeader->u32SamplePrecision = pData[i++];
    pstPicHeader->u32PicWidth = pData[i++]<<8|pData[i++];
    pstPicHeader->u32PicHeight = pData[i++]<<8|pData[i++];
    pstPicHeader->Nf = pData[i++];
#ifdef DEBUG_INFO
    printf("frame size is [%dx%d]\n", pstPicHeader->u32PicWidth, pstPicHeader->u32PicHeight);
#endif
    return 0;
}



int parseHeader(MJ_U8 *pData, MJ_S32 length, PIC_HEADER_S* pstPicHeader){
    int i = 0;
    MJ_U32 u32readptr = 0;
    MJ_U32 s32SegIndicator = 0;
    MJ_U32 s32SegLength = 0;
    MJ_U8  *pSegData = NULL;

    if(0XFF!=pData[u32readptr]||MARKER_SOI!=pData[u32readptr+1]){
        printf("This is not a jpeg format!!!\n");
        return -1;
    }
    else{
        printf("This is a jpeg format!!!\n");
        u32readptr = u32readptr+2;

    }

    while(u32readptr<length) {
        if (0XFF == pData[u32readptr++]) {
            s32SegIndicator = pData[u32readptr++];
            s32SegLength = pData[u32readptr + 1] | pData[u32readptr] << 8;
            pSegData = &pData[u32readptr + 2];

            switch (s32SegIndicator) {
                case MARKER_SOF0:
                    printf("MARKER_SOF0\n");
                    parseFrameHeader(pSegData, s32SegLength-3, pstPicHeader);
                    break;
                case MARKER_SOI:
                    printf("MARKER_EOI\n");
                    break;
                case MARKER_EOI:
                    printf("MARKER_EOI\n");
                    break;
                case MARKER_DQT:
                    printf("MARKER_DQT\n");
                    break;
                case MARKER_APP0:
                    printf("MARKER_DQT\n");
                    break;
                case MARKER_DHT:
                    printf("MARKER_DQT\n");
                    break;
                default:
                    break;
            }
        }
    }

    return 0;
}                                                                             

int main(int argc, char **argv) {
    FILE *fp = NULL;                                                          
    int readcnt = 0;
    MJ_U8 *pData = NULL;
    int fileLen = 0;    
    PIC_HEADER_S stPicHeader;

    printf("start Jpeg decode...\n");                                         
    fp = fopen("./1.jpg", "r");                                               
    if (NULL == fp){                                                          
            perror("open file error:");                                           
            return -1;                                                            
    }                                                                         

    fseek(fp, 0, SEEK_END);
    fileLen = ftell(fp);
    fseek(fp, 0, SEEK_SET);
#ifdef DEBUG_INFO
    printf("file len is %d\n", fileLen);                                      
#endif                                                                        

    pData = malloc(fileLen*sizeof(char));                                     
    if (NULL==pData){                                                         
            return -1;                                                            
    }
    fread(pData, fileLen, 1, fp);

    parseHeader(pData, fileLen, &stPicHeader);

    //DecoderJpeg(pData);
    fclose(fp);
    return 0;
}


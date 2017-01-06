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

#define LB_U32 unsigned int
#define LB_U16 unsigned short
#define LB_U8  unsigned char
/************************ Static Structure Definition ************************/
typedef struct {
    LB_U16 u32FramHdrLen; 
    LB_U8  u32SamplePrecision; 
    LB_U16 u32PicWidth; 
    LB_U16 u32PicHeight; 
    LB_U8  Nf; 
    LB_U16 Ci; 
    LB_U8  Hi;    
    LB_U8  Vi; 
    LB_U8 Tqi; 
}PIC_HEADER_S;
/***************************** Global Definition *****************************/
/***************************** Static Definition *****************************/
/*********************************** Code ************************************/
int parseHeader(char *pData, int length, PIC_HEADER_S* pstPicHeader){
    int i = 0;

    printf("\n\n\n|--------------------------------------------|\n");
    for(i=0; i<length-1; i++){
        if (0xff==(pData[i]&0xff)){
            if (0xd8==(pData[i+1]&0xff)){
                printf("|                                            |\n");
                printf("|                                            |\n");
                printf("|                                            |\n");
                printf("|---         Start of image               ---|\n");
            }
            
            if (0xda==(pData[i+1]&0xff)){
                printf("|                                            |\n");
                printf("|                                            |\n");
                printf("|                                            |\n");
                printf("|---         Start of scan                ---|\n");
            }
        
            if (0xd9==(pData[i+1]&0xff)){    
                printf("|                                            |\n");
                printf("|                                            |\n");
                printf("|                                            |\n");
                printf("|---          End of image                ---|\n");

            }
            
            if (0xc0==(pData[i+1]&0xff)){
                printf("|                                            |\n");
                printf("|                                            |\n");
                printf("|                                            |\n");
                printf("|---          Baseline DCT                ---|\n");
                //fhdrLen = (pData[i+2]<<8)|(pData[i+3]);  
                pstPicHeader->u32FramHdrLen      = ((pData[i+2]&0xff)<<8)|((pData[i+3]&0xff));        
                pstPicHeader->u32SamplePrecision = (pData[i+4]&0xff);
                pstPicHeader->u32PicHeight       = ((pData[i+5]&0xff)<<8)|((pData[i+6]&0xff));
                pstPicHeader->u32PicWidth        = ((pData[i+7]&0xff)<<8)|((pData[i+8]&0xff));
                pstPicHeader->Nf                 = (pData[i+9]&0xff);                
                pstPicHeader->Ci                 = (pData[i+10]&0xff);               
                pstPicHeader->Hi                 = (pData[i+11]&0xf0);                
                pstPicHeader->Vi                 = (pData[i+11]&0x0f);                
                pstPicHeader->Tqi                = (pData[i+12]&0xff);
            }
        
            if (0xdb==(pData[i+1]&0xff)){  
                printf("|                                            |\n");
                printf("|                                            |\n");
                printf("|                                            |\n");
                printf("|---     Define quantization table(s)     ---|\n");
                //parseQuaTable();
            }
            
            if (0xc4==(pData[i+1]&0xff)){  
                printf("|                                            |\n");
                printf("|                                            |\n");
                printf("|                                            |\n");
                printf("|---        Define huffman table(s)       ---|\n");
                //parseHuffmanTable();
            }
            
            if (0xdd==(pData[i+1]&0xff)){  
                printf("|                                            |\n");
                printf("|                                            |\n");
                printf("|                                            |\n");
                printf("|---       Define restart interval        ---|\n");
            }
        
#if 0
            if (0xc1==(pData[i+1]&0xff)){
                printf("Extended sequential DCT\n");
            }
            if (0xc2==(pData[i+1]&0xff)){
                printf("Progressive DCT\n");

            }
            if (0xc3==(pData[i+1]&0xff)){
                    printf("Lossless (sequential)\n");
            }

#endif    
        }
    }    
    printf("|____________________________________________|\n");
}                                                                             

int main() {                                                                  
    FILE *fp = NULL;                                                          
    int readcnt = 0;                                                          
    char *pData = NULL;                                                       
    int fileLen = 0;    
    PIC_HEADER_S stPicHeader;

    printf("start Jpeg decode...\n");                                         
    fp = fopen("./1.jpg", "r");                                               
    if (NULL == fp){                                                          
            perror("open file error:");                                           
            return -1;                                                            
    }                                                                         

    fseek(fp, 0, SEEK_END);//get the file length                              
    fileLen = ftell(fp);                                                      
#ifdef DEBUG_INFO                                                             
    printf("file len is %d\n", fileLen);                                      
#endif                                                                        
    fseek(fp, 0, SEEK_SET);//get the file length                              
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


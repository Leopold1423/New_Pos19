#include "flash.h"
#include "usart.h"
#include "math.h"
#include "can_func.h"
#include "calculate.h"
#include "main.h"
#include "cmd.h"
#include "can_func.h"
#include "encoder.h"

//如果不成功，就有�?能是FLASH的大小和地址设置�?题�?

#define FLASH_Start 0x080A0000   

//#define FLASH_Start 0x08000000 //  注意每�?�都要查一下手册的flash起�?�地址


//对FLASH写入数据�?
//1.解锁FLASH
//2.擦除FLASH
//3.写入数据到FLASH
//4.锁住FLASH
//FLASH读取数据：直接�?�取相应的FLASH地址即可

float flash_data[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//烧进flash�?�?14�?数据


//将参数写进flash�?
void write_prams()
{  
  uint32_t SectorError;
  uint32_t temp;
  int i;
  FLASH_EraseInitTypeDef EraseInitStruct;
  
  HAL_FLASH_Unlock();//解锁flash
  
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;//选择页擦除还�?块擦除，这里�?页擦�?
  EraseInitStruct.Sector=STMFLASH_GetFlashSector(FLASH_Start);
  EraseInitStruct.NbSectors=1;   //擦除的页�?
  EraseInitStruct.VoltageRange=FLASH_VOLTAGE_RANGE_1;
  
  if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)//调用擦除函数
  { 
    uprintf("erase flash fail!\r\n");
    HAL_FLASH_Lock();
    return ;
  }
  int pram_num=sizeof(flash_data)/sizeof(flash_data[0]);
  
  for(i=0;i<pram_num;++i){
    temp=*((uint32_t *)(flash_data+i));//将flash_data[i]对应的float类型�?4字节数据以无符号整型读出�?
    //flash_data是flash_data[0]地址,flash_data+i是flash_data[i]地址,然后将flash_data[i]对应的float型转化为无�?�号整型,再取这个指针指向的地址里的�?
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,FLASH_Start+i*4,temp);  //对flash烧写
    //uprintf("write Pram[%d]Ok!\r\n",i);
  }
  HAL_FLASH_Lock();//锁住flash
  uprintf("Write OK!\r\n");
}

void load_prams()
{
  int i;
  int pram_num=sizeof(flash_data)/sizeof(flash_data[0]);
  
  for(i=0;i<pram_num;++i){
    flash_data[i]=*((float *)(FLASH_Start+i*4));
    uprintf("flash_data[%d]=%lf\r\n",i,flash_data[i]);
    if(isnan(flash_data[i])) flash_data[i] = 0;
  }	
  uprintf("\r\n");
  kx_center=flash_data[0];
  ky_center=flash_data[1];
  X_diameter=flash_data[2];
  y_diameter=flash_data[3];
  time=(int)flash_data[4];
  CANSEND_ID=(uint16_t)flash_data[5];
  encoder.encoderX_dir=(int)flash_data[6];
  encoder.encoderY_dir=(int)flash_data[7];
//  canmtrid = (int)flash_data[17];
}

//获取某个地址所在的flash扇区
//addr:flash地址
//返回�?:0~11,即addr所在的扇区
uint8_t STMFLASH_GetFlashSector(uint32_t addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_SECTOR_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_SECTOR_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_SECTOR_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_SECTOR_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_SECTOR_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_SECTOR_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_SECTOR_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_SECTOR_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_SECTOR_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_SECTOR_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_SECTOR_10;   
	return FLASH_SECTOR_11;	
}

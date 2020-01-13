/*******************************************************************************
 * Copyright:		BUPT
 * File Name:		can_utils.c
 * Description:		CAN 工具函数
 * Author:			ZeroVoid
 * Version:			0.1
 * Data:			2019/09/23 Mon 13:59
 *******************************************************************************/
// TODO: ZeroVoid	due:9/26	错�??处理
// TODO: ZeroVoid	due:10/2	动态配�?
// TODO: ZeroVoid	due:10/7	优化多中�?管理

#include "can_utils.h"
#include "hash.h"
#include "cmd.h"
#include "can_func.h"
#include <stdlib.h>

CAN_HandleTypeDef HCAN;

int can_exc_callback_flag = 0;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint32_t TxMailbox;
can_msg can_rx_data;
can_msg can_tx_data;
static can_msg rx_buffer = {0};
static uint32_t rx_id = 0;

static void CAN_config(CAN_HandleTypeDef *hcan);
static HashTable can_callback_table = NULL;

static unsigned int hash_id(const void* id);
static int id_cmp(const void*, const void*);



void can_init(CAN_HandleTypeDef *hcan) {
    HCAN = *hcan;
    CAN_config(&HCAN);
    if (can_callback_table == NULL) {
        can_callback_table = HashTable_create(id_cmp, hash_id, NULL);
    }
    can_func_init();
}

/**
 * @brief	添加CAN回调函数
 * @param	id          触发回调的can id 
 * @param   callback    回调函数指针 data: can接收到数�?联合�?
 * @return	None
 */
void can_callback_add(const uint32_t id, void (*callback)(can_msg *data)) {
    uint32_t *can_id = (uint32_t*) malloc(sizeof(uint32_t));
    *can_id = id;
    HashTable_insert(can_callback_table, can_id, (void*)callback);
}

void can_exc_callback(void) {
    void (*callback_func)(can_msg *) = (void(*)(can_msg*))HashTable_get(can_callback_table, &rx_id);
    if (callback_func) {
        callback_func(&rx_buffer);
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, can_rx_data.ui8);
    rx_id = RxHeader.StdId;
    rx_buffer.df = can_rx_data.df;
    can_exc_callback_flag = 1;    
}

void can_send_test(void) {
    can_tx_data.in[0] = 0x1;
    can_tx_data.in[1] = 0xAD;
    TxHeader.StdId = 1;
    uprintf("send data\r\n");
    HAL_CAN_AddTxMessage(&HCAN, &TxHeader, can_tx_data.ui8, &TxMailbox);
}

/* @Brief can 发送数�?
 * @param	id: 发送数据id
 *          msg: can数据封�?�结构体
 *          len: 数据长度
 * @return	0: 正常发�?
 *          1: 发送失�?
 */
int can_send_msg(uint16_t std_id, can_msg *msg,uint32_t len) {
  TxHeader.StdId = std_id;
  TxHeader.DLC = len;

  //uprintf("%d %d %d\r\n", std_id, msg->in[0], msg->in[1]);  

  if (HAL_CAN_AddTxMessage(&HCAN, &TxHeader, msg->ui8, &TxMailbox) != HAL_OK) {
    uprintf("Error: CAN can't send msg.\r\n");
    return 1;
  }
  return 0;
}

void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan) {
    return ;
}

void CAN_config(CAN_HandleTypeDef *hcan) {
    CAN_FilterTypeDef sFilterConfig;

    /* Configure the CAN Filter 
     bxCAN提供28�?位�?�可�?/�?配置的标识�?�过滤器�?
     通过设置CAN_FMR的FBMx�? 设置过滤器类�? 0: mask mode; 1: list mode
  */
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    /* CAN_FILTERSCALE_32BIT
     FilterIdHight = StdId << 5 
     FilterIdHight = (ExtId << 3)>>16 & 0xFFFF
     FIlterIdLow   = ((uint16_t)(ExtId <<3)) | CAN_ID_EXT;*/
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK) {
        /* Filter configuration Error */
        Error_Handler();
    }

    /* Start the CAN peripheral */
    if (HAL_CAN_Start(hcan) != HAL_OK) {
        /* Start Error */
        Error_Handler();
    }

    /* Activate CAN RX notification */
    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        /* Notification Error */
        Error_Handler();
    }

    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_FULL) != HAL_OK) {
        /* Notification Error */
        Error_Handler();
    }

    /* Configure Transmission provess */
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.DLC = 8;
    TxHeader.TransmitGlobalTime = DISABLE;
}

static unsigned int hash_id(const void* id) {
    return *((unsigned int*)id);
}

static int id_cmp(const void* a, const void* b) {
    return ( *((unsigned int*)a) == *((unsigned int*)b) )? 0:1;
}

/*
 * @Author: Retuze
 * @Date: 2023-02-20 22:01:33
 * @LastEditors: Dorothy-GM 2678796474@qq.com
 * @LastEditTime: 2023-02-23 17:02:24
 * @FilePath: \STM32F103C8T6\tmp\fifo\fifo.h
 * @Description:
 *
 * Copyright (c) 2023 by Dorothy-GM 2678796474@qq.com, All Rights Reserved.
 */

#ifndef _FIFO_H_
#define _FIFO_H_

#include "main.h"

//! FIFO Memory Model (Single Byte Mode)
typedef struct
{
    char *p_start_addr; //!< FIFO Memory Pool Start Address
    char *p_end_addr;   //!< FIFO Memory Pool End Address
    int free_num;       //!< The remain capacity of FIFO
    int used_num;       //!< The number of elements in FIFO
    char *p_read_addr;  //!< FIFO Data Read Index Pointer
    char *p_write_addr; //!< FIFO Data Write Index Pointer
} fifo_s_t;

//******************************************************************************************
//
//! \brief  Initialize an static FIFO struct(in single mode).
//!
//! \param  [in] p_fifo is the pointer of valid FIFO instance.
//! \param  [in] p_base_addr is the base address of pre-allocate memory, such as array.
//! \param  [in] uint_cnt is count of fifo elements.
//! \retval 0 if initialize successfully, otherwise return -1.
//
//******************************************************************************************
int fifo_s_init(fifo_s_t *p_fifo, void *p_base_addr, int uint_cnt);

//******************************************************************************************
//
//! \brief  Put an element into FIFO(in single mode).
//!
//! \param  [in]  p_fifo is the pointer of valid FIFO.
//! \param  [in]  element is the data element you want to put
//!
//! \retval 0 if operate successfully, otherwise return -1.
//
//******************************************************************************************
int fifo_s_put(fifo_s_t *p_fifo, char element);

int fifo_s_puts(fifo_s_t *p_fifo, char *p_source, int len);

//******************************************************************************************
//
//! \brief  Get an element from FIFO(in single mode).
//!
//! \param  [in]  p_fifo is the pointer of valid FIFO.
//!
//! \retval the data element of FIFO.
//
//******************************************************************************************
char fifo_s_get(fifo_s_t *p_fifo);
int fifo_s_gets(fifo_s_t *p_fifo, char *p_dest, int len);



//******************************************************************************************
//
//! \brief  FIFO is empty (in single mode)?
//!
//! \param  [in] p_fifo is the pointer of valid FIFO.
//!
//! \retval - None-zero(true) if empty.
//!         - Zero(false) if not empty.
//
//******************************************************************************************
char fifo_s_isempty(fifo_s_t *p_fifo);

//******************************************************************************************
//
//! \brief  FIFO is full (in single mode)?
//!
//! \param  [in] p_fifo is the pointer of valid FIFO.
//!
//! \retval - None-zero(true) if full.
//!         - Zero(false) if not full.
//
//******************************************************************************************
char fifo_s_isfull(fifo_s_t *p_fifo);

//******************************************************************************************
//
//! \brief  Get FIFO the number of elements(in single mode)?
//!
//! \param  [in] p_fifo is the pointer of valid FIFO.
//!
//! \retval The number of elements in FIFO.
//
//******************************************************************************************
int fifo_s_used(fifo_s_t *p_fifo);

//******************************************************************************************
//
//! \brief  Get FIFO the number of elements(in single mode)?
//!
//! \param  [in] p_fifo is the pointer of valid FIFO.
//!
//! \retval The number of elements in FIFO.
//
//******************************************************************************************
int fifo_s_free(fifo_s_t *p_fifo);

//******************************************************************************************
//
//! \brief  Flush the content of FIFO.
//!
//! \param  [in] p_fifo is the pointer of valid FIFO.
//!
//! \retval 0 if success, -1 if failure.
//
//******************************************************************************************
void fifo_s_flush(fifo_s_t *p_fifo);
int fifo_s_discard(fifo_s_t *p_fifo, int len);


#endif // _FIFO_H_

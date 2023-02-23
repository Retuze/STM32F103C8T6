/*
 * @Author: Retuze
 * @Date: 2023-02-20 22:01:26
 * @LastEditors: Dorothy-GM 2678796474@qq.com
 * @LastEditTime: 2023-02-23 17:03:15
 * @FilePath: \STM32F103C8T6\tmp\fifo\fifo.c
 * @Description:
 * Copyright (c) 2023 by Dorothy-GM 2678796474@qq.com, All Rights Reserved.
 */


#include "fifo.h"
#include "main.h"
//******************************************************************************************
//!                     ASSERT MACRO
//******************************************************************************************
#ifndef ASSERT

#ifdef FIFO_NDEBUG
#define ASSERT(x)
#else
#define ASSERT(x)                                           \
    do                                                      \
    {                                                       \
        if (!(x))                                           \
            printf("[assert]: %s, %d", __FILE__, __LINE__); \
        while (!(x))                                        \
            ;                                               \
    } while (0)
#endif

#endif // ASSERT



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
int fifo_s_init(fifo_s_t *p_fifo, void *p_base_addr, int uint_cnt)
{
    //! Check input parameters.
    ASSERT(p_fifo);
    ASSERT(p_base_addr);
    ASSERT(uint_cnt);

    //! Initialize FIFO Control Block.
    p_fifo->p_start_addr = (char *)p_base_addr;
    p_fifo->p_end_addr = (char *)p_base_addr + uint_cnt - 1;
    p_fifo->free_num = uint_cnt;
    p_fifo->used_num = 0;
    p_fifo->p_read_addr = (char *)p_base_addr;
    p_fifo->p_write_addr = (char *)p_base_addr;

    return (0);
}

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
int fifo_s_put(fifo_s_t *p_fifo, char element)
{
    //! Check input parameters.
    ASSERT(p_fifo);

    if (0 == p_fifo->free_num)
    {
        //! Error, FIFO is full!
        return (-1);
    }


    if (p_fifo->p_write_addr > p_fifo->p_end_addr)
    {
        p_fifo->p_write_addr = p_fifo->p_start_addr;
    }

    *(p_fifo->p_write_addr) = element;
    p_fifo->p_write_addr++;
    p_fifo->free_num--;
    p_fifo->used_num++;

    return (0);
}

//******************************************************************************************
//
//! \brief  Put some elements into FIFO(in single mode).
//!
//! \param  [in]  p_fifo is the pointer of valid FIFO.
//! \param  [in]  p_source is the data element you want to put
//! \param  [in]  the number of elements
//! \retval the number of really write data, otherwise return -1.
//
//******************************************************************************************
int fifo_s_puts(fifo_s_t *p_fifo, char *p_source, int len)
{
    int retval;
    int len_to_end;
    int len_from_start;

    ASSERT(p_fifo);

    if (NULL == p_source)
    {
        return -1;
    }

    if (0 == p_fifo->free_num)
    {
        return 0;
    }


    if (p_fifo->p_write_addr > p_fifo->p_end_addr)
    {
        p_fifo->p_write_addr = p_fifo->p_start_addr;
    }

    len = (len < p_fifo->free_num) ? len : p_fifo->free_num;
    len_to_end = p_fifo->p_end_addr - p_fifo->p_write_addr + 1;

    if (len_to_end >= len) //no rollback
    {
        len_to_end = len;
        memcpy(p_fifo->p_write_addr, p_source, len_to_end);
        p_fifo->p_write_addr += len_to_end;
    }
    else //rollback
    {
        len_from_start = len - len_to_end;
        memcpy(p_fifo->p_write_addr, p_source, len_to_end);
        memcpy(p_fifo->p_start_addr, p_source + len_to_end, len_from_start);
        p_fifo->p_write_addr = p_fifo->p_start_addr + len_from_start;
    }

    p_fifo->free_num -= len;
    p_fifo->used_num += len;
    retval = len;


    return retval;
}


//******************************************************************************************
//
//! \brief  Get an element from FIFO(in single mode).
//!
//! \param  [in]  p_fifo is the pointer of valid FIFO.
//!
//! \retval the data element of FIFO.
//
//******************************************************************************************
char fifo_s_get(fifo_s_t *p_fifo)
{
    char retval = 0;

    //! Check input parameters.
    ASSERT(p_fifo);

    //TODO:
    if (0 == p_fifo->used_num)
    {
        return 0;
    }


    if (p_fifo->p_read_addr > p_fifo->p_end_addr)
    {
        p_fifo->p_read_addr = p_fifo->p_start_addr;
    }

    retval = *p_fifo->p_read_addr;
    // Update information
    p_fifo->p_read_addr++;
    p_fifo->free_num++;
    p_fifo->used_num--;


    return (retval);
}

//******************************************************************************************
//
//! \brief  Get some element from FIFO(in single mode).
//!
//! \param  [in]  p_fifo is the pointer of valid FIFO.
//!
//! \retval the number of really read data.
//
//******************************************************************************************
int fifo_s_gets(fifo_s_t *p_fifo, char *p_dest, int len)
{
    int retval;
    int len_to_end;
    int len_from_start;

    ASSERT(p_fifo);

    if (NULL == p_dest)
    {
        return -1;
    }

    if (0 == p_fifo->used_num)
    {
        return 0;
    }

    if (p_fifo->p_read_addr > p_fifo->p_end_addr)
    {
        p_fifo->p_read_addr = p_fifo->p_start_addr;
    }

    len = (len < p_fifo->used_num) ? len : p_fifo->used_num;
    len_to_end = p_fifo->p_end_addr - p_fifo->p_read_addr + 1;

    if (len_to_end >= len) //no rollback
    {
        len_to_end = len;
        memcpy(p_dest, p_fifo->p_read_addr, len_to_end);
        p_fifo->p_read_addr += len_to_end;
    }
    else //rollback
    {
        len_from_start = len - len_to_end;
        memcpy(p_dest, p_fifo->p_read_addr, len_to_end);
        memcpy(p_dest + len_to_end, p_fifo->p_start_addr, len_from_start);
        p_fifo->p_read_addr = p_fifo->p_start_addr + len_from_start;
    }

    p_fifo->free_num += len;
    p_fifo->used_num -= len;
    retval = len;


    return retval;
}


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
char fifo_s_isempty(fifo_s_t *p_fifo)
{
    //! Check input parameter.
    ASSERT(p_fifo);
    return (p_fifo->used_num ? 0 : 1);
}

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
char fifo_s_isfull(fifo_s_t *p_fifo)
{
    //! Check input parameter.
    ASSERT(p_fifo);
    return (p_fifo->free_num ? 0 : 1);
}

//******************************************************************************************
//
//! \brief  Get FIFO the number of elements(in single mode)?
//!
//! \param  [in] p_fifo is the pointer of valid FIFO.
//!
//! \retval The number of elements in FIFO.
//
//******************************************************************************************
int fifo_s_used(fifo_s_t *p_fifo)
{
    //! Check input parameter.
    ASSERT(p_fifo);
    return p_fifo->used_num;
}

//******************************************************************************************
//
//! \brief  Get FIFO the number of elements(in single mode)?
//!
//! \param  [in] p_fifo is the pointer of valid FIFO.
//!
//! \retval The number of elements in FIFO.
//
//******************************************************************************************
int fifo_s_free(fifo_s_t *p_fifo)
{
    //! Check input parameter.
    ASSERT(p_fifo);
    return p_fifo->free_num;
}

//******************************************************************************************
//
//! \brief  Flush the content of FIFO.
//!
//! \param  [in] p_fifo is the pointer of valid FIFO.
//!
//! \retval 0 if success, -1 if failure.
//
//******************************************************************************************
void fifo_s_flush(fifo_s_t *p_fifo)
{
    //! Check input parameters.
    ASSERT(p_fifo);
    //! Initialize FIFO Control Block.

    p_fifo->free_num = p_fifo->p_end_addr - p_fifo->p_start_addr + 1;
    p_fifo->used_num = 0;
    p_fifo->p_read_addr = p_fifo->p_start_addr;
    p_fifo->p_write_addr = p_fifo->p_start_addr;

}



/**
 * @description:
 * @param {fifo_s_t} *p_fifo
 * @param {int} len
 * @return {*}
 */
int fifo_s_discard(fifo_s_t *p_fifo, int len)
{
    //! Check input parameters.
    char *tmp_index;
    ASSERT(p_fifo);


    if (len > p_fifo->used_num)
    {
        len = p_fifo->used_num;
    }

    tmp_index = len + p_fifo->p_read_addr;
    if (tmp_index > p_fifo->p_end_addr)
    {
        tmp_index = tmp_index - p_fifo->p_end_addr + p_fifo->p_start_addr - 1;
    }
    p_fifo->p_read_addr = tmp_index;
    p_fifo->free_num += len;
    p_fifo->used_num -= len;

    return len;
}



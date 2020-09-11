/*
 * Copyright (C) 2015 MediaTek Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef __HAL_BTIF_PUB_H_
#define __HAL_BTIF_PUB_H_

#include "plat_common.h"

/*Enum Defination*/
/*BTIF Mode Enum */
enum _ENUM_BTIF_MODE_ {
	BTIF_MODE_PIO = 0,
	BTIF_MODE_DMA = BTIF_MODE_PIO + 1,
	BTIF_MODE_MAX,
};

/*****************************************************************************
 * FUNCTION
 *  hal_btif_info_get
 * DESCRIPTION
 *  get btif's information included base address , irq related information
 * PARAMETERS
 * RETURNS
 *  BTIF's information
 *****************************************************************************/
struct _MTK_BTIF_INFO_STR_ *hal_btif_info_get(void);

#if 0				/*included in hal_btif_info_get */
/*****************************************************************************
 * FUNCTION
 *  hal_btif_get_irq
 * DESCRIPTION
 *  get BTIF module's IRQ information
 * PARAMETERS
 * RETURNS
 *  pointer to BTIF's irq structure
 *****************************************************************************/
struct _MTK_BTIF_IRQ_STR_ *hal_btif_get_irq(void);
#endif

#if !defined(CONFIG_MTK_CLKMGR)
/*****************************************************************************
 * FUNCTION
 *  hal_btif_clk_get_and_prepare
 * DESCRIPTION
 *  get clock from device tree and prepare for enable/disable control
 * PARAMETERS
 *  pdev  device pointer
 * RETURNS
 *  0 means success, negative means fail
 *****************************************************************************/
int hal_btif_clk_get_and_prepare(struct platform_device *pdev);
/*****************************************************************************
 * FUNCTION
 *  hal_btif_clk_unprepare
 * DESCRIPTION
 *  unprepare btif clock and apdma clock
 * PARAMETERS
 *  none
 * RETURNS
 *  0 means success, negative means fail
 *****************************************************************************/
int hal_btif_clk_unprepare(void);
#endif
/*****************************************************************************
 * FUNCTION
 *  hal_btif_clk_ctrl
 * DESCRIPTION
 *  control clock output enable/disable of BTIF module
 * PARAMETERS
 *  p_base   [IN]        BTIF module's base address
 * RETURNS
 *  0 means success, negative means fail
 *****************************************************************************/
int hal_btif_clk_ctrl(struct _MTK_BTIF_INFO_STR_ *p_btif,
		      enum _ENUM_CLOCK_CTRL_ flag);

/*****************************************************************************
 * FUNCTION
 *  hal_btif_hw_init
 * DESCRIPTION
 *  BTIF module init, after this step, BTIF should enable to do tx/rx with PIO
 *  mode
 * PARAMETERS
 *  p_base   [IN]        BTIF module's base address
 * RETURNS
 *  0 means success, negative means fail
 *****************************************************************************/
int hal_btif_hw_init(struct _MTK_BTIF_INFO_STR_ *p_btif);

/*****************************************************************************
 * FUNCTION
 *  hal_btif_rx_cb_reg
 * DESCRIPTION
 *  BTIF rx callback register API
 * PARAMETERS
 *  p_btif_info   [IN]        pointer to BTIF's information
 *  rx_cb         [IN]        rx callback function
 * RETURNS
 *  0 means success, negative means fail
 *****************************************************************************/
int hal_btif_rx_cb_reg(struct _MTK_BTIF_INFO_STR_ *p_btif_info,
		       btif_rx_buf_write rx_cb);

/*****************************************************************************
 * FUNCTION
 *  hal_btif_loopback_ctrl
 * DESCRIPTION
 *  BTIF Tx/Rx loopback mode set, this operation can only be done
 *  after set BTIF to normal mode
 * PARAMETERS
 * RETURNS
 *  0 means success, negative means fail
 *****************************************************************************/
int hal_btif_loopback_ctrl(struct _MTK_BTIF_INFO_STR_ *p_btif, bool en);

/*****************************************************************************
 * FUNCTION
 *  hal_btif_rx_handler
 * DESCRIPTION
 *  lower level interrupt handler
 * PARAMETERS
 *  p_base   [IN]        BTIF module's base address
 *  p_buf    [IN/OUT]    pointer to rx data buffer
 *  max_len  [IN]        max length of rx buffer
 * RETURNS
 *  0 means success; negative means fail; positive means rx data length
 *****************************************************************************/
int hal_btif_irq_handler(struct _MTK_BTIF_INFO_STR_ *p_btif,
			 unsigned char *p_buf, const unsigned int max_len);

/*****************************************************************************
 * FUNCTION
 *  hal_btif_tx_mode_ctrl
 * DESCRIPTION
 *  set BTIF tx to corresponding mode (PIO/DMA)
 * PARAMETERS
 *  p_base   [IN]        BTIF module's base address
 *  mode     [IN]        rx mode <PIO/DMA>
 * RETURNS
 *  0 means success, negative means fail
 *****************************************************************************/
int hal_btif_tx_mode_ctrl(struct _MTK_BTIF_INFO_STR_ *p_btif,
			  enum _ENUM_BTIF_MODE_ mode);

/*****************************************************************************
 * FUNCTION
 *  hal_btif_rx_mode_ctrl
 * DESCRIPTION
 *  set BTIF rx to corresponding mode (PIO/DMA)
 * PARAMETERS
 *  p_base   [IN]        BTIF module's base address
 *  mode     [IN]        rx mode <PIO/DMA>
 * RETURNS
 *  0 means success, negative means fail
 *****************************************************************************/
int hal_btif_rx_mode_ctrl(struct _MTK_BTIF_INFO_STR_ *p_btif,
			  enum _ENUM_BTIF_MODE_ mode);

/*****************************************************************************
 * FUNCTION
 *  hal_btif_send_data
 * DESCRIPTION
 *  send data through btif in FIFO mode
 * PARAMETERS
 *  p_base   [IN]        BTIF module's base address
 *  p_buf    [IN]        pointer to rx data buffer
 *  max_len  [IN]        tx buffer length
 * RETURNS
 *  positive means number of data sent;
 *  0 means no data put to FIFO;
 *  negative means error happens
 *****************************************************************************/
int hal_btif_send_data(struct _MTK_BTIF_INFO_STR_ *p_btif,
		       const unsigned char *p_buf, const unsigned int buf_len);

/*****************************************************************************
 * FUNCTION
 *  hal_btif_raise_wak_sig
 * DESCRIPTION
 *  raise wakeup signal to counterpart
 * PARAMETERS
 *  p_base   [IN]        BTIF module's base address
 * RETURNS
 *  0 means success, negative means fail
 *****************************************************************************/
int hal_btif_raise_wak_sig(struct _MTK_BTIF_INFO_STR_ *p_btif);

/*****************************************************************************
 * FUNCTION
 *  hal_btif_dump_reg
 * DESCRIPTION
 *  dump BTIF module's information when needed
 * PARAMETERS
 *  p_base   [IN]        BTIF module's base address
 *  flag     [IN]        register id flag
 * RETURNS
 *  0 means success, negative means fail
 *****************************************************************************/
int hal_btif_dump_reg(struct _MTK_BTIF_INFO_STR_ *p_btif,
		      enum _ENUM_BTIF_REG_ID_ flag);

/*****************************************************************************
 * FUNCTION
 *  hal_btif_is_tx_complete
 * DESCRIPTION
 *  get tx complete flag
 * PARAMETERS
 *  p_base   [IN]        BTIF module's base address
 * RETURNS
 *  true means tx complete, false means tx in process
 *****************************************************************************/
bool hal_btif_is_tx_complete(struct _MTK_BTIF_INFO_STR_ *p_btif);

/*****************************************************************************
 * FUNCTION
 *  hal_btif_is_tx_allow
 * DESCRIPTION
 *  whether tx is allowed
 * PARAMETERS
 *  p_base   [IN]        BTIF module's base address
 * RETURNS
 *  true if tx operation is allowed; false if tx is not allowed
 *****************************************************************************/
bool hal_btif_is_tx_allow(struct _MTK_BTIF_INFO_STR_ *p_btif);

int hal_btif_pm_ops(struct _MTK_BTIF_INFO_STR_ *p_btif,
		    enum _MTK_BTIF_PM_OPID_ opid);

void mtk_btif_read_cpu_sw_rst_debug_plat(void);

#ifdef DUMP_BGF_REG
void hal_btif_dump_bgf_reg_init(void);
void hal_btif_dump_bgf_reg_deinit(void);
#endif

#endif /*__HAL_BTIF_PUB_H_*/
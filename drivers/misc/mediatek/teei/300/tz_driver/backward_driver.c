/*
 * Copyright (c) 2015-2019, MICROTRUST Incorporated
 * Copyright (C) 2020 XiaoMi, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/semaphore.h>
#include <linux/types.h>
#include <linux/cpu.h>
#include "nt_smc_call.h"
#include "backward_driver.h"
#include "teei_id.h"
#include "sched_status.h"
#include "utdriver_macro.h"
#include "teei_log.h"
#include "teei_common.h"
#include "switch_queue.h"
#include "teei_client_main.h"
#include <linux/time.h>
#include <teei_secure_api.h>
#include <notify_queue.h>

#define IMSG_TAG "[tz_driver]"
#include <imsg_log.h>

#define		GET_UPTIME	(1)
#define		GET_SYSTIME	(0)

struct bdrv_call_struct {
	int bdrv_call_type;
	struct service_handler *handler;
	int retVal;
};

static long register_shared_param_buf(struct service_handler *handler);
unsigned char *daulOS_VFS_share_mem;
unsigned char *vfs_flush_address;
struct service_handler reetime;
struct service_handler vfs_handler;

static long register_shared_param_buf(struct service_handler *handler)
{
	long retVal = 0;

	if (handler->size > VDRV_MAX_SIZE) {
		IMSG_ERROR("[%s][%d]: The vDrv buffer is too large.\n",
					__FILE__, __LINE__);
		return -EINVAL;
	}

#ifdef UT_DMA_ZONE
	handler->param_buf = (void *)__get_free_pages(GFP_KERNEL | GFP_DMA,
				get_order(ROUND_UP(handler->size, SZ_4K)));
#else
	handler->param_buf = (void *)__get_free_pages(GFP_KERNEL,
				get_order(ROUND_UP(handler->size, SZ_4K)));
#endif
	if ((unsigned char *)(handler->param_buf) == NULL) {
		IMSG_ERROR("[%s][%d]: kmalloc vdrv_buffer failed.\n",
							__FILE__, __LINE__);
		return -ENOMEM;
	}

	retVal = add_nq_entry(TEEI_CREAT_BDRV, handler->sysno,
				(unsigned long long)(&boot_sema),
				virt_to_phys((void *)(handler->param_buf)),
				handler->size, 0);

	if (retVal != 0) {
		IMSG_ERROR("TEEI: Failed to add one nq to n_t_buffer\n");
		goto free_memory;
	}


	retVal = add_work_entry(INVOKE_NQ_CALL, 0);
	if (retVal != 0) {
		IMSG_ERROR("TEEI: Failed to add_work_entry[%s]\n", __func__);
		goto free_memory;
	}

	down(&boot_sema);

	return 0;

free_memory:
	/* Release the resource and return. */
	free_pages((unsigned long)(handler->param_buf),
				get_order(ROUND_UP(handler->size, SZ_4K)));
	handler->param_buf = NULL;

	return retVal;
}

/******************************TIME**************************************/
static long reetime_init(struct service_handler *handler)
{
	return 0;
}

static void reetime_deinit(struct service_handler *handler)
{
}

static int reetime_handle(struct NQ_entry *entry)
{
	struct timespec tp;
	struct timeval tv;
	int tv_sec;
	int tv_usec;
	unsigned long long block_p = 0;
	unsigned long long time_type = 0;
	int retVal = 0;

	time_type = entry->param[0];
	block_p = entry->block_p;

	if (time_type == GET_UPTIME) {
		get_monotonic_boottime(&tp);
		tv_sec = tp.tv_sec;
		tv_usec = tp.tv_nsec/1000;
	} else {
		do_gettimeofday(&tv);
		tv_sec = tv.tv_sec;
		tv_usec = tv.tv_usec;
	}

	retVal = add_bdrv_nq_entry(TEEI_BDRV_CALL, reetime.sysno,
				block_p, time_type,
				tv_sec, tv_usec);
	if (retVal != 0)
		IMSG_ERROR("TEEI: Failed to add_nq_entry[%s]\n", __func__);

	retVal = add_work_entry(INVOKE_NQ_CALL, 0);
	if (retVal != 0)
		IMSG_ERROR("TEEI: Failed to add_work_entry[%s]\n", __func__);

	return retVal;
}

/********************************************************************
 *                      VFS functions                               *
 ********************************************************************/



int vfs_thread_function(unsigned long virt_addr,
			unsigned long para_vaddr, unsigned long buff_vaddr)
{
	int retVal = 0;

	daulOS_VFS_share_mem = (unsigned char *)virt_addr;

	retVal = notify_vfs_handle();
	if (retVal != 0) {
		IMSG_ERROR("[%s][%d] Can NOT notify the tz_vfs node!\n",
					__func__, __LINE__);
		return retVal;
	}

	retVal = wait_for_vfs_done();
	if (retVal != 0) {
		IMSG_ERROR("[%s][%d] Failed to waiting for the tz_vfs node!\n",
					__func__, __LINE__);
		return retVal;
	}

	return 0;
}

static long vfs_init(struct service_handler *handler) /*! init service */
{
	long retVal = 0;

	retVal = register_shared_param_buf(handler);
	vfs_flush_address = handler->param_buf;

	return retVal;
}

static void vfs_deinit(struct service_handler *handler) /*! stop service  */
{
}

static int vfs_handle(struct NQ_entry *entry)
{
	unsigned long long block_p = 0;
	int retVal = 0;

	block_p = entry->block_p;

	vfs_thread_function((unsigned long)(vfs_handler.param_buf), 0, 0);

	retVal = add_bdrv_nq_entry(TEEI_BDRV_CALL, vfs_handler.sysno,
						block_p, 0, 0, 0);
	if (retVal != 0)
		IMSG_ERROR("TEEI: Failed to add_nq_entry[%s]\n", __func__);

	retVal = add_work_entry(INVOKE_NQ_CALL, 0);
	if (retVal != 0)
		IMSG_ERROR("TEEI: Failed to add_work_entry[%s]\n", __func__);

	return retVal;
}

int init_all_service_handlers(void)
{
	long retVal = 0;

	reetime.init = reetime_init;
	reetime.deinit = reetime_deinit;
	reetime.handle = reetime_handle;
	reetime.size = 0x1000;
	reetime.sysno  = 7;

	vfs_handler.init = vfs_init;
	vfs_handler.deinit = vfs_deinit;
	vfs_handler.handle = vfs_handle;
	vfs_handler.size = 0x80000;
	vfs_handler.sysno = 8;

	retVal = reetime.init(&reetime);
	if (retVal < 0) {
		IMSG_ERROR("[%s][%d] init reetime service failed!\n",
					__func__, __LINE__);
		return retVal;
	}

	retVal = vfs_handler.init(&vfs_handler);
	if (retVal < 0) {
		IMSG_ERROR("[%s][%d] init vfs service failed!\n",
					__func__, __LINE__);
		return retVal;
	}

	return 0;
}

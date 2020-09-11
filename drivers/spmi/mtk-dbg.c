// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2019 MediaTek Inc.
 * Author: Argus Lin <argus.lin@mediatek.com>
 */
#define DEBUG

#include <linux/clk.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/regmap.h>
#include <linux/reset.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/sched/clock.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/debugfs.h>
#include <spmi_sw.h>
#include <mt-plat/aee.h>

enum pmif_dbg_regs {
	PMIF_INIT_DONE,
	PMIF_INF_BUSY_STA,
	PMIF_OTHER_BUSY_STA_0,
	PMIF_OTHER_BUSY_STA_1,
	PMIF_IRQ_EVENT_EN_0,
	PMIF_IRQ_FLAG_0,
	PMIF_IRQ_CLR_0,
	PMIF_IRQ_EVENT_EN_1,
	PMIF_IRQ_FLAG_1,
	PMIF_IRQ_CLR_1,
	PMIF_IRQ_EVENT_EN_2,
	PMIF_IRQ_FLAG_2,
	PMIF_IRQ_CLR_2,
	PMIF_IRQ_EVENT_EN_3,
	PMIF_IRQ_FLAG_3,
	PMIF_IRQ_CLR_3,
	PMIF_IRQ_EVENT_EN_4,
	PMIF_IRQ_FLAG_4,
	PMIF_IRQ_CLR_4,
	PMIF_WDT_EVENT_EN_0,
	PMIF_WDT_FLAG_0,
	PMIF_WDT_EVENT_EN_1,
	PMIF_WDT_FLAG_1,
	PMIF_MONITOR_CTRL,
	PMIF_MONITOR_TARGET_CHAN_0,
	PMIF_MONITOR_TARGET_CHAN_1,
	PMIF_MONITOR_TARGET_CHAN_2,
	PMIF_MONITOR_TARGET_CHAN_3,
	PMIF_MONITOR_TARGET_CHAN_4,
	PMIF_MONITOR_TARGET_CHAN_5,
	PMIF_MONITOR_TARGET_CHAN_6,
	PMIF_MONITOR_TARGET_CHAN_7,
	PMIF_MONITOR_TARGET_WRITE,
	PMIF_MONITOR_TARGET_SLVID_0,
	PMIF_MONITOR_TARGET_SLVID_1,
	PMIF_MONITOR_TARGET_ADDR_0,
	PMIF_MONITOR_TARGET_ADDR_1,
	PMIF_MONITOR_TARGET_ADDR_2,
	PMIF_MONITOR_TARGET_ADDR_3,
	PMIF_MONITOR_TARGET_ADDR_4,
	PMIF_MONITOR_TARGET_ADDR_5,
	PMIF_MONITOR_TARGET_ADDR_6,
	PMIF_MONITOR_TARGET_ADDR_7,
	PMIF_MONITOR_TARGET_WDATA_0,
	PMIF_MONITOR_TARGET_WDATA_1,
	PMIF_MONITOR_TARGET_WDATA_2,
	PMIF_MONITOR_TARGET_WDATA_3,
	PMIF_MONITOR_TARGET_WDATA_4,
	PMIF_MONITOR_TARGET_WDATA_5,
	PMIF_MONITOR_TARGET_WDATA_6,
	PMIF_MONITOR_TARGET_WDATA_7,
	PMIF_MONITOR_STA,
	PMIF_MONITOR_RECORD_0_0,
	PMIF_MONITOR_RECORD_0_1,
	PMIF_MONITOR_RECORD_0_2,
	PMIF_MONITOR_RECORD_0_3,
	PMIF_MONITOR_RECORD_0_4,
	PMIF_MONITOR_RECORD_1_0,
	PMIF_MONITOR_RECORD_1_1,
	PMIF_MONITOR_RECORD_1_2,
	PMIF_MONITOR_RECORD_1_3,
	PMIF_MONITOR_RECORD_1_4,
	PMIF_MONITOR_RECORD_2_0,
	PMIF_MONITOR_RECORD_2_1,
	PMIF_MONITOR_RECORD_2_2,
	PMIF_MONITOR_RECORD_2_3,
	PMIF_MONITOR_RECORD_2_4,
	PMIF_MONITOR_RECORD_3_0,
	PMIF_MONITOR_RECORD_3_1,
	PMIF_MONITOR_RECORD_3_2,
	PMIF_MONITOR_RECORD_3_3,
	PMIF_MONITOR_RECORD_3_4,
	PMIF_MONITOR_RECORD_4_0,
	PMIF_MONITOR_RECORD_4_1,
	PMIF_MONITOR_RECORD_4_2,
	PMIF_MONITOR_RECORD_4_3,
	PMIF_MONITOR_RECORD_4_4,
	PMIF_MONITOR_RECORD_5_0,
	PMIF_MONITOR_RECORD_5_1,
	PMIF_MONITOR_RECORD_5_2,
	PMIF_MONITOR_RECORD_5_3,
	PMIF_MONITOR_RECORD_5_4,
	PMIF_MONITOR_RECORD_6_0,
	PMIF_MONITOR_RECORD_6_1,
	PMIF_MONITOR_RECORD_6_2,
	PMIF_MONITOR_RECORD_6_3,
	PMIF_MONITOR_RECORD_6_4,
	PMIF_MONITOR_RECORD_7_0,
	PMIF_MONITOR_RECORD_7_1,
	PMIF_MONITOR_RECORD_7_2,
	PMIF_MONITOR_RECORD_7_3,
	PMIF_MONITOR_RECORD_7_4,
	PMIF_MONITOR_RECORD_8_0,
	PMIF_MONITOR_RECORD_8_1,
	PMIF_MONITOR_RECORD_8_2,
	PMIF_MONITOR_RECORD_8_3,
	PMIF_MONITOR_RECORD_8_4,
	PMIF_MONITOR_RECORD_9_0,
	PMIF_MONITOR_RECORD_9_1,
	PMIF_MONITOR_RECORD_9_2,
	PMIF_MONITOR_RECORD_9_3,
	PMIF_MONITOR_RECORD_9_4,
	PMIF_MONITOR_RECORD_10_0,
	PMIF_MONITOR_RECORD_10_1,
	PMIF_MONITOR_RECORD_10_2,
	PMIF_MONITOR_RECORD_10_3,
	PMIF_MONITOR_RECORD_10_4,
	PMIF_MONITOR_RECORD_11_0,
	PMIF_MONITOR_RECORD_11_1,
	PMIF_MONITOR_RECORD_11_2,
	PMIF_MONITOR_RECORD_11_3,
	PMIF_MONITOR_RECORD_11_4,
	PMIF_MONITOR_RECORD_12_0,
	PMIF_MONITOR_RECORD_12_1,
	PMIF_MONITOR_RECORD_12_2,
	PMIF_MONITOR_RECORD_12_3,
	PMIF_MONITOR_RECORD_12_4,
	PMIF_MONITOR_RECORD_13_0,
	PMIF_MONITOR_RECORD_13_1,
	PMIF_MONITOR_RECORD_13_2,
	PMIF_MONITOR_RECORD_13_3,
	PMIF_MONITOR_RECORD_13_4,
	PMIF_MONITOR_RECORD_14_0,
	PMIF_MONITOR_RECORD_14_1,
	PMIF_MONITOR_RECORD_14_2,
	PMIF_MONITOR_RECORD_14_3,
	PMIF_MONITOR_RECORD_14_4,
	PMIF_MONITOR_RECORD_15_0,
	PMIF_MONITOR_RECORD_15_1,
	PMIF_MONITOR_RECORD_15_2,
	PMIF_MONITOR_RECORD_15_3,
	PMIF_MONITOR_RECORD_15_4,
	PMIF_MONITOR_RECORD_16_0,
	PMIF_MONITOR_RECORD_16_1,
	PMIF_MONITOR_RECORD_16_2,
	PMIF_MONITOR_RECORD_16_3,
	PMIF_MONITOR_RECORD_16_4,
	PMIF_MONITOR_RECORD_17_0,
	PMIF_MONITOR_RECORD_17_1,
	PMIF_MONITOR_RECORD_17_2,
	PMIF_MONITOR_RECORD_17_3,
	PMIF_MONITOR_RECORD_17_4,
	PMIF_MONITOR_RECORD_18_0,
	PMIF_MONITOR_RECORD_18_1,
	PMIF_MONITOR_RECORD_18_2,
	PMIF_MONITOR_RECORD_18_3,
	PMIF_MONITOR_RECORD_18_4,
	PMIF_MONITOR_RECORD_19_0,
	PMIF_MONITOR_RECORD_19_1,
	PMIF_MONITOR_RECORD_19_2,
	PMIF_MONITOR_RECORD_19_3,
	PMIF_MONITOR_RECORD_19_4,
	PMIF_MONITOR_RECORD_20_0,
	PMIF_MONITOR_RECORD_20_1,
	PMIF_MONITOR_RECORD_20_2,
	PMIF_MONITOR_RECORD_20_3,
	PMIF_MONITOR_RECORD_20_4,
	PMIF_MONITOR_RECORD_21_0,
	PMIF_MONITOR_RECORD_21_1,
	PMIF_MONITOR_RECORD_21_2,
	PMIF_MONITOR_RECORD_21_3,
	PMIF_MONITOR_RECORD_21_4,
	PMIF_MONITOR_RECORD_22_0,
	PMIF_MONITOR_RECORD_22_1,
	PMIF_MONITOR_RECORD_22_2,
	PMIF_MONITOR_RECORD_22_3,
	PMIF_MONITOR_RECORD_22_4,
	PMIF_MONITOR_RECORD_23_0,
	PMIF_MONITOR_RECORD_23_1,
	PMIF_MONITOR_RECORD_23_2,
	PMIF_MONITOR_RECORD_23_3,
	PMIF_MONITOR_RECORD_23_4,
	PMIF_MONITOR_RECORD_24_0,
	PMIF_MONITOR_RECORD_24_1,
	PMIF_MONITOR_RECORD_24_2,
	PMIF_MONITOR_RECORD_24_3,
	PMIF_MONITOR_RECORD_24_4,
	PMIF_MONITOR_RECORD_25_0,
	PMIF_MONITOR_RECORD_25_1,
	PMIF_MONITOR_RECORD_25_2,
	PMIF_MONITOR_RECORD_25_3,
	PMIF_MONITOR_RECORD_25_4,
	PMIF_MONITOR_RECORD_26_0,
	PMIF_MONITOR_RECORD_26_1,
	PMIF_MONITOR_RECORD_26_2,
	PMIF_MONITOR_RECORD_26_3,
	PMIF_MONITOR_RECORD_26_4,
	PMIF_MONITOR_RECORD_27_0,
	PMIF_MONITOR_RECORD_27_1,
	PMIF_MONITOR_RECORD_27_2,
	PMIF_MONITOR_RECORD_27_3,
	PMIF_MONITOR_RECORD_27_4,
	PMIF_MONITOR_RECORD_28_0,
	PMIF_MONITOR_RECORD_28_1,
	PMIF_MONITOR_RECORD_28_2,
	PMIF_MONITOR_RECORD_28_3,
	PMIF_MONITOR_RECORD_28_4,
	PMIF_MONITOR_RECORD_29_0,
	PMIF_MONITOR_RECORD_29_1,
	PMIF_MONITOR_RECORD_29_2,
	PMIF_MONITOR_RECORD_29_3,
	PMIF_MONITOR_RECORD_29_4,
	PMIF_MONITOR_RECORD_30_0,
	PMIF_MONITOR_RECORD_30_1,
	PMIF_MONITOR_RECORD_30_2,
	PMIF_MONITOR_RECORD_30_3,
	PMIF_MONITOR_RECORD_30_4,
	PMIF_MONITOR_RECORD_31_0,
	PMIF_MONITOR_RECORD_31_1,
	PMIF_MONITOR_RECORD_31_2,
	PMIF_MONITOR_RECORD_31_3,
	PMIF_MONITOR_RECORD_31_4,
	PMIF_DEBUG_CTRL,
	PMIF_RESERVED_0,
	PMIF_SWINF_0_ACC,
	PMIF_SWINF_0_WDATA_31_0,
	PMIF_SWINF_0_WDATA_63_32,
	PMIF_SWINF_0_RDATA_31_0,
	PMIF_SWINF_0_RDATA_63_32,
	PMIF_SWINF_0_VLD_CLR,
	PMIF_SWINF_0_STA,
	PMIF_SWINF_1_ACC,
	PMIF_SWINF_1_WDATA_31_0,
	PMIF_SWINF_1_WDATA_63_32,
	PMIF_SWINF_1_RDATA_31_0,
	PMIF_SWINF_1_RDATA_63_32,
	PMIF_SWINF_1_VLD_CLR,
	PMIF_SWINF_1_STA,
	PMIF_SWINF_2_ACC,
	PMIF_SWINF_2_WDATA_31_0,
	PMIF_SWINF_2_WDATA_63_32,
	PMIF_SWINF_2_RDATA_31_0,
	PMIF_SWINF_2_RDATA_63_32,
	PMIF_SWINF_2_VLD_CLR,
	PMIF_SWINF_2_STA,
	PMIF_SWINF_3_ACC,
	PMIF_SWINF_3_WDATA_31_0,
	PMIF_SWINF_3_WDATA_63_32,
	PMIF_SWINF_3_RDATA_31_0,
	PMIF_SWINF_3_RDATA_63_32,
	PMIF_SWINF_3_VLD_CLR,
	PMIF_SWINF_3_STA,

	PMIC_ACC_VIO_INFO_0,
	PMIC_ACC_VIO_INFO_1,
	PMIC_ACC_VIO_INFO_2,
	PMIC_ACC_VIO_INFO_3,
	PMIC_ACC_VIO_INFO_4,
	PMIC_ACC_VIO_INFO_5,
	PMIC_ACC_SCP_VIO_INFO_0,
	PMIC_ACC_SCP_VIO_INFO_1,
	PMIC_ACC_SCP_VIO_INFO_2,
	PMIC_ACC_SCP_VIO_INFO_3,
	PMIC_ACC_SCP_VIO_INFO_4,
	PMIC_ACC_SCP_VIO_INFO_5,
	PMIF_ACC_VIO_INFO_0,
	PMIF_ACC_VIO_INFO_1,
	PMIF_ACC_VIO_INFO_2,
};

static const u32 mt6xxx_pmif_dbg_regs[] = {
	[PMIF_INIT_DONE] =			0x0000,
	[PMIF_INF_BUSY_STA] =			0x0018,
	[PMIF_OTHER_BUSY_STA_0] =		0x001C,
	[PMIF_OTHER_BUSY_STA_1] =		0x0020,
	[PMIF_IRQ_EVENT_EN_0] =                 0x0418,
	[PMIF_IRQ_FLAG_0] =                     0x0420,
	[PMIF_IRQ_CLR_0] =                      0x0424,
	[PMIF_IRQ_EVENT_EN_1] =                 0x0428,
	[PMIF_IRQ_FLAG_1] =                     0x0430,
	[PMIF_IRQ_CLR_1] =                      0x0434,
	[PMIF_IRQ_EVENT_EN_2] =                 0x0438,
	[PMIF_IRQ_FLAG_2] =                     0x0440,
	[PMIF_IRQ_CLR_2] =                      0x0444,
	[PMIF_IRQ_EVENT_EN_3] =                 0x0448,
	[PMIF_IRQ_FLAG_3] =                     0x0450,
	[PMIF_IRQ_CLR_3] =                      0x0454,
	[PMIF_IRQ_EVENT_EN_4] =                 0x0458,
	[PMIF_IRQ_FLAG_4] =                     0x0460,
	[PMIF_IRQ_CLR_4] =                      0x0464,
	[PMIF_WDT_EVENT_EN_0] =			0x046C,
	[PMIF_WDT_FLAG_0] =			0x0470,
	[PMIF_WDT_EVENT_EN_1] =			0x0474,
	[PMIF_WDT_FLAG_1] =			0x0478,
	[PMIF_MONITOR_CTRL] =			0x047C,
	[PMIF_MONITOR_TARGET_CHAN_0] =		0x0480,
	[PMIF_MONITOR_TARGET_CHAN_1] =		0x0484,
	[PMIF_MONITOR_TARGET_CHAN_2] =		0x0488,
	[PMIF_MONITOR_TARGET_CHAN_3] =		0x048C,
	[PMIF_MONITOR_TARGET_CHAN_4] =		0x0490,
	[PMIF_MONITOR_TARGET_CHAN_5] =		0x0494,
	[PMIF_MONITOR_TARGET_CHAN_6] =		0x0498,
	[PMIF_MONITOR_TARGET_CHAN_7] =		0x049C,
	[PMIF_MONITOR_TARGET_WRITE] =		0x04A0,
	[PMIF_MONITOR_TARGET_SLVID_0] =		0x04A4,
	[PMIF_MONITOR_TARGET_SLVID_1] =		0x04A8,
	[PMIF_MONITOR_TARGET_ADDR_0] =		0x04AC,
	[PMIF_MONITOR_TARGET_ADDR_1] =		0x04B0,
	[PMIF_MONITOR_TARGET_ADDR_2] =		0x04B4,
	[PMIF_MONITOR_TARGET_ADDR_3] =		0x04B8,
	[PMIF_MONITOR_TARGET_ADDR_4] =		0x04BC,
	[PMIF_MONITOR_TARGET_ADDR_5] =		0x04C0,
	[PMIF_MONITOR_TARGET_ADDR_6] =		0x04C4,
	[PMIF_MONITOR_TARGET_ADDR_7] =		0x04C8,
	[PMIF_MONITOR_TARGET_WDATA_0] =		0x04CC,
	[PMIF_MONITOR_TARGET_WDATA_1] =		0x04D0,
	[PMIF_MONITOR_TARGET_WDATA_2] =		0x04D4,
	[PMIF_MONITOR_TARGET_WDATA_3] =		0x04D8,
	[PMIF_MONITOR_TARGET_WDATA_4] =		0x04DC,
	[PMIF_MONITOR_TARGET_WDATA_5] =		0x04E0,
	[PMIF_MONITOR_TARGET_WDATA_6] =		0x04E4,
	[PMIF_MONITOR_TARGET_WDATA_7] =		0x04E8,
	[PMIF_MONITOR_STA] =			0x04EC,
	[PMIF_MONITOR_RECORD_0_0] =		0x04F0,
	[PMIF_MONITOR_RECORD_0_1] =		0x04F4,
	[PMIF_MONITOR_RECORD_0_2] =		0x04F8,
	[PMIF_MONITOR_RECORD_0_3] =		0x04FC,
	[PMIF_MONITOR_RECORD_0_4] =		0x0500,
	[PMIF_MONITOR_RECORD_1_0] =		0x0504,
	[PMIF_MONITOR_RECORD_1_1] =		0x0508,
	[PMIF_MONITOR_RECORD_1_2] =		0x050C,
	[PMIF_MONITOR_RECORD_1_3] =		0x0510,
	[PMIF_MONITOR_RECORD_1_4] =		0x0514,
	[PMIF_MONITOR_RECORD_2_0] =		0x0518,
	[PMIF_MONITOR_RECORD_2_1] =		0x051C,
	[PMIF_MONITOR_RECORD_2_2] =		0x0520,
	[PMIF_MONITOR_RECORD_2_3] =		0x0524,
	[PMIF_MONITOR_RECORD_2_4] =		0x0528,
	[PMIF_MONITOR_RECORD_3_0] =		0x052C,
	[PMIF_MONITOR_RECORD_3_1] =		0x0530,
	[PMIF_MONITOR_RECORD_3_2] =		0x0534,
	[PMIF_MONITOR_RECORD_3_3] =		0x0538,
	[PMIF_MONITOR_RECORD_3_4] =		0x053C,
	[PMIF_MONITOR_RECORD_4_0] =		0x0540,
	[PMIF_MONITOR_RECORD_4_1] =		0x0544,
	[PMIF_MONITOR_RECORD_4_2] =		0x0548,
	[PMIF_MONITOR_RECORD_4_3] =		0x054C,
	[PMIF_MONITOR_RECORD_4_4] =		0x0550,
	[PMIF_MONITOR_RECORD_5_0] =		0x0554,
	[PMIF_MONITOR_RECORD_5_1] =		0x0558,
	[PMIF_MONITOR_RECORD_5_2] =		0x055C,
	[PMIF_MONITOR_RECORD_5_3] =		0x0560,
	[PMIF_MONITOR_RECORD_5_4] =		0x0564,
	[PMIF_MONITOR_RECORD_6_0] =		0x0568,
	[PMIF_MONITOR_RECORD_6_1] =		0x056C,
	[PMIF_MONITOR_RECORD_6_2] =		0x0570,
	[PMIF_MONITOR_RECORD_6_3] =		0x0574,
	[PMIF_MONITOR_RECORD_6_4] =		0x0578,
	[PMIF_MONITOR_RECORD_7_0] =		0x057C,
	[PMIF_MONITOR_RECORD_7_1] =		0x0580,
	[PMIF_MONITOR_RECORD_7_2] =		0x0584,
	[PMIF_MONITOR_RECORD_7_3] =		0x0588,
	[PMIF_MONITOR_RECORD_7_4] =		0x058C,
	[PMIF_MONITOR_RECORD_8_0] =		0x0590,
	[PMIF_MONITOR_RECORD_8_1] =		0x0594,
	[PMIF_MONITOR_RECORD_8_2] =		0x0598,
	[PMIF_MONITOR_RECORD_8_3] =		0x059C,
	[PMIF_MONITOR_RECORD_8_4] =		0x05A0,
	[PMIF_MONITOR_RECORD_9_0] =		0x05A4,
	[PMIF_MONITOR_RECORD_9_1] =		0x05A8,
	[PMIF_MONITOR_RECORD_9_2] =		0x05AC,
	[PMIF_MONITOR_RECORD_9_3] =		0x05B0,
	[PMIF_MONITOR_RECORD_9_4] =		0x05B4,
	[PMIF_MONITOR_RECORD_10_0] =		0x05B8,
	[PMIF_MONITOR_RECORD_10_1] =		0x05BC,
	[PMIF_MONITOR_RECORD_10_2] =		0x05C0,
	[PMIF_MONITOR_RECORD_10_3] =		0x05C4,
	[PMIF_MONITOR_RECORD_10_4] =		0x05C8,
	[PMIF_MONITOR_RECORD_11_0] =		0x05CC,
	[PMIF_MONITOR_RECORD_11_1] =		0x05D0,
	[PMIF_MONITOR_RECORD_11_2] =		0x05D4,
	[PMIF_MONITOR_RECORD_11_3] =		0x05D8,
	[PMIF_MONITOR_RECORD_11_4] =		0x05DC,
	[PMIF_MONITOR_RECORD_12_0] =		0x05E0,
	[PMIF_MONITOR_RECORD_12_1] =		0x05E4,
	[PMIF_MONITOR_RECORD_12_2] =		0x05E8,
	[PMIF_MONITOR_RECORD_12_3] =		0x05EC,
	[PMIF_MONITOR_RECORD_12_4] =		0x05F0,
	[PMIF_MONITOR_RECORD_13_0] =		0x05F4,
	[PMIF_MONITOR_RECORD_13_1] =		0x05F8,
	[PMIF_MONITOR_RECORD_13_2] =		0x05FC,
	[PMIF_MONITOR_RECORD_13_3] =		0x0600,
	[PMIF_MONITOR_RECORD_13_4] =		0x0604,
	[PMIF_MONITOR_RECORD_14_0] =		0x0608,
	[PMIF_MONITOR_RECORD_14_1] =		0x060C,
	[PMIF_MONITOR_RECORD_14_2] =		0x0610,
	[PMIF_MONITOR_RECORD_14_3] =		0x0614,
	[PMIF_MONITOR_RECORD_14_4] =		0x0618,
	[PMIF_MONITOR_RECORD_15_0] =		0x061C,
	[PMIF_MONITOR_RECORD_15_1] =		0x0620,
	[PMIF_MONITOR_RECORD_15_2] =		0x0624,
	[PMIF_MONITOR_RECORD_15_3] =		0x0628,
	[PMIF_MONITOR_RECORD_15_4] =		0x062C,
	[PMIF_MONITOR_RECORD_16_0] =		0x0630,
	[PMIF_MONITOR_RECORD_16_1] =		0x0634,
	[PMIF_MONITOR_RECORD_16_2] =		0x0638,
	[PMIF_MONITOR_RECORD_16_3] =		0x063C,
	[PMIF_MONITOR_RECORD_16_4] =		0x0640,
	[PMIF_MONITOR_RECORD_17_0] =		0x0644,
	[PMIF_MONITOR_RECORD_17_1] =		0x0648,
	[PMIF_MONITOR_RECORD_17_2] =		0x064C,
	[PMIF_MONITOR_RECORD_17_3] =		0x0650,
	[PMIF_MONITOR_RECORD_17_4] =		0x0654,
	[PMIF_MONITOR_RECORD_18_0] =		0x0658,
	[PMIF_MONITOR_RECORD_18_1] =		0x065C,
	[PMIF_MONITOR_RECORD_18_2] =		0x0660,
	[PMIF_MONITOR_RECORD_18_3] =		0x0664,
	[PMIF_MONITOR_RECORD_18_4] =		0x0668,
	[PMIF_MONITOR_RECORD_19_0] =		0x066C,
	[PMIF_MONITOR_RECORD_19_1] =		0x0670,
	[PMIF_MONITOR_RECORD_19_2] =		0x0674,
	[PMIF_MONITOR_RECORD_19_3] =		0x0678,
	[PMIF_MONITOR_RECORD_19_4] =		0x067C,
	[PMIF_MONITOR_RECORD_20_0] =		0x0680,
	[PMIF_MONITOR_RECORD_20_1] =		0x0684,
	[PMIF_MONITOR_RECORD_20_2] =		0x0688,
	[PMIF_MONITOR_RECORD_20_3] =		0x068C,
	[PMIF_MONITOR_RECORD_20_4] =		0x0690,
	[PMIF_MONITOR_RECORD_21_0] =		0x0694,
	[PMIF_MONITOR_RECORD_21_1] =		0x0698,
	[PMIF_MONITOR_RECORD_21_2] =		0x069C,
	[PMIF_MONITOR_RECORD_21_3] =		0x06A0,
	[PMIF_MONITOR_RECORD_21_4] =		0x06A4,
	[PMIF_MONITOR_RECORD_22_0] =		0x06A8,
	[PMIF_MONITOR_RECORD_22_1] =		0x06AC,
	[PMIF_MONITOR_RECORD_22_2] =		0x06B0,
	[PMIF_MONITOR_RECORD_22_3] =		0x06B4,
	[PMIF_MONITOR_RECORD_22_4] =		0x06B8,
	[PMIF_MONITOR_RECORD_23_0] =		0x06BC,
	[PMIF_MONITOR_RECORD_23_1] =		0x06C0,
	[PMIF_MONITOR_RECORD_23_2] =		0x06C4,
	[PMIF_MONITOR_RECORD_23_3] =		0x06C8,
	[PMIF_MONITOR_RECORD_23_4] =		0x06CC,
	[PMIF_MONITOR_RECORD_24_0] =		0x06D0,
	[PMIF_MONITOR_RECORD_24_1] =		0x06D4,
	[PMIF_MONITOR_RECORD_24_2] =		0x06D8,
	[PMIF_MONITOR_RECORD_24_3] =		0x06DC,
	[PMIF_MONITOR_RECORD_24_4] =		0x06E0,
	[PMIF_MONITOR_RECORD_25_0] =		0x06E4,
	[PMIF_MONITOR_RECORD_25_1] =		0x06E8,
	[PMIF_MONITOR_RECORD_25_2] =		0x06EC,
	[PMIF_MONITOR_RECORD_25_3] =		0x06F0,
	[PMIF_MONITOR_RECORD_25_4] =		0x06F4,
	[PMIF_MONITOR_RECORD_26_0] =		0x06F8,
	[PMIF_MONITOR_RECORD_26_1] =		0x06FC,
	[PMIF_MONITOR_RECORD_26_2] =		0x0700,
	[PMIF_MONITOR_RECORD_26_3] =		0x0704,
	[PMIF_MONITOR_RECORD_26_4] =		0x0708,
	[PMIF_MONITOR_RECORD_27_0] =		0x070C,
	[PMIF_MONITOR_RECORD_27_1] =		0x0710,
	[PMIF_MONITOR_RECORD_27_2] =		0x0714,
	[PMIF_MONITOR_RECORD_27_3] =		0x0718,
	[PMIF_MONITOR_RECORD_27_4] =		0x071C,
	[PMIF_MONITOR_RECORD_28_0] =		0x0720,
	[PMIF_MONITOR_RECORD_28_1] =		0x0724,
	[PMIF_MONITOR_RECORD_28_2] =		0x0728,
	[PMIF_MONITOR_RECORD_28_3] =		0x072C,
	[PMIF_MONITOR_RECORD_28_4] =		0x0730,
	[PMIF_MONITOR_RECORD_29_0] =		0x0734,
	[PMIF_MONITOR_RECORD_29_1] =		0x0738,
	[PMIF_MONITOR_RECORD_29_2] =		0x073C,
	[PMIF_MONITOR_RECORD_29_3] =		0x0740,
	[PMIF_MONITOR_RECORD_29_4] =		0x0744,
	[PMIF_MONITOR_RECORD_30_0] =		0x0748,
	[PMIF_MONITOR_RECORD_30_1] =		0x074C,
	[PMIF_MONITOR_RECORD_30_2] =		0x0750,
	[PMIF_MONITOR_RECORD_30_3] =		0x0754,
	[PMIF_MONITOR_RECORD_30_4] =		0x0758,
	[PMIF_MONITOR_RECORD_31_0] =		0x075C,
	[PMIF_MONITOR_RECORD_31_1] =		0x0760,
	[PMIF_MONITOR_RECORD_31_2] =		0x0764,
	[PMIF_MONITOR_RECORD_31_3] =		0x0768,
	[PMIF_MONITOR_RECORD_31_4] =		0x076C,
	[PMIF_DEBUG_CTRL] =			0x0770,
	[PMIF_RESERVED_0] =			0x0778,
	[PMIF_SWINF_0_ACC] =			0x0C00,
	[PMIF_SWINF_0_WDATA_31_0] =		0x0C04,
	[PMIF_SWINF_0_WDATA_63_32] =		0x0C08,
	[PMIF_SWINF_0_RDATA_31_0] =		0x0C14,
	[PMIF_SWINF_0_RDATA_63_32] =		0x0C18,
	[PMIF_SWINF_0_VLD_CLR] =		0x0C24,
	[PMIF_SWINF_0_STA] =			0x0C28,
	[PMIF_SWINF_1_ACC] =			0x0C40,
	[PMIF_SWINF_1_WDATA_31_0] =		0x0C44,
	[PMIF_SWINF_1_WDATA_63_32] =		0x0C48,
	[PMIF_SWINF_1_RDATA_31_0] =		0x0C54,
	[PMIF_SWINF_1_RDATA_63_32] =		0x0C58,
	[PMIF_SWINF_1_VLD_CLR] =		0x0C64,
	[PMIF_SWINF_1_STA] =			0x0C68,
	[PMIF_SWINF_2_ACC] =			0x0C80,
	[PMIF_SWINF_2_WDATA_31_0] =		0x0C84,
	[PMIF_SWINF_2_WDATA_63_32] =		0x0C88,
	[PMIF_SWINF_2_RDATA_31_0] =		0x0C94,
	[PMIF_SWINF_2_RDATA_63_32] =		0x0C98,
	[PMIF_SWINF_2_VLD_CLR] =		0x0CA4,
	[PMIF_SWINF_2_STA] =			0x0CA8,
	[PMIF_SWINF_3_ACC] =			0x0CC0,
	[PMIF_SWINF_3_WDATA_31_0] =		0x0CC4,
	[PMIF_SWINF_3_WDATA_63_32] =		0x0CC8,
	[PMIF_SWINF_3_RDATA_31_0] =		0x0CD4,
	[PMIF_SWINF_3_RDATA_63_32] =		0x0CD8,
	[PMIF_SWINF_3_VLD_CLR] =		0x0CE4,
	[PMIF_SWINF_3_STA] =			0x0CE8,
	[PMIC_ACC_VIO_INFO_0] =			0x0F50,
	[PMIC_ACC_VIO_INFO_1] =			0x0F54,
	[PMIC_ACC_VIO_INFO_2] =			0x0F58,
	[PMIC_ACC_VIO_INFO_3] =			0x0F5C,
	[PMIC_ACC_VIO_INFO_4] =			0x0F60,
	[PMIC_ACC_VIO_INFO_5] =			0x0F64,
	[PMIC_ACC_SCP_VIO_INFO_0] =		0x0F68,
	[PMIC_ACC_SCP_VIO_INFO_1] =		0x0F6C,
	[PMIC_ACC_SCP_VIO_INFO_2] =		0x0F70,
	[PMIC_ACC_SCP_VIO_INFO_3] =		0x0F74,
	[PMIC_ACC_SCP_VIO_INFO_4] =		0x0F78,
	[PMIC_ACC_SCP_VIO_INFO_5] =		0x0F7C,
	[PMIF_ACC_VIO_INFO_0] =			0x0F80,
	[PMIF_ACC_VIO_INFO_1] =			0x0F84,
	[PMIF_ACC_VIO_INFO_2] =			0x0F88,
};

static const u32 mt6885_pmif_dbg_regs[] = {
	[PMIF_INIT_DONE] =			0x0000,
	[PMIF_INF_BUSY_STA] =			0x0018,
	[PMIF_OTHER_BUSY_STA_0] =		0x001C,
	[PMIF_OTHER_BUSY_STA_1] =		0x0020,
	[PMIF_IRQ_EVENT_EN_0] =                 0x0418,
	[PMIF_IRQ_FLAG_0] =                     0x0420,
	[PMIF_IRQ_CLR_0] =                      0x0424,
	[PMIF_IRQ_EVENT_EN_1] =                 0x0428,
	[PMIF_IRQ_FLAG_1] =                     0x0430,
	[PMIF_IRQ_CLR_1] =                      0x0434,
	[PMIF_IRQ_EVENT_EN_2] =                 0x0438,
	[PMIF_IRQ_FLAG_2] =                     0x0440,
	[PMIF_IRQ_CLR_2] =                      0x0444,
	[PMIF_IRQ_EVENT_EN_3] =                 0x0448,
	[PMIF_IRQ_FLAG_3] =                     0x0450,
	[PMIF_IRQ_CLR_3] =                      0x0454,
	[PMIF_IRQ_EVENT_EN_4] =                 0x0458,
	[PMIF_IRQ_FLAG_4] =                     0x0460,
	[PMIF_IRQ_CLR_4] =                      0x0464,
	[PMIF_WDT_EVENT_EN_0] =			0x046C,
	[PMIF_WDT_FLAG_0] =			0x0470,
	[PMIF_WDT_EVENT_EN_1] =			0x0474,
	[PMIF_WDT_FLAG_1] =			0x0478,
	[PMIF_MONITOR_CTRL] =			0x047C,
	[PMIF_MONITOR_TARGET_CHAN_0] =		0x0480,
	[PMIF_MONITOR_TARGET_CHAN_1] =		0x0484,
	[PMIF_MONITOR_TARGET_CHAN_2] =		0x0488,
	[PMIF_MONITOR_TARGET_CHAN_3] =		0x048C,
	[PMIF_MONITOR_TARGET_CHAN_4] =		0x0490,
	[PMIF_MONITOR_TARGET_CHAN_5] =		0x0494,
	[PMIF_MONITOR_TARGET_CHAN_6] =		0x0498,
	[PMIF_MONITOR_TARGET_CHAN_7] =		0x049C,
	[PMIF_MONITOR_TARGET_WRITE] =		0x04A0,
	[PMIF_MONITOR_TARGET_ADDR_0] =		0x04A4,
	[PMIF_MONITOR_TARGET_ADDR_1] =		0x04A8,
	[PMIF_MONITOR_TARGET_ADDR_2] =		0x04AC,
	[PMIF_MONITOR_TARGET_ADDR_3] =		0x04B0,
	[PMIF_MONITOR_TARGET_ADDR_4] =		0x04B4,
	[PMIF_MONITOR_TARGET_ADDR_5] =		0x04B8,
	[PMIF_MONITOR_TARGET_ADDR_6] =		0x04BC,
	[PMIF_MONITOR_TARGET_ADDR_7] =		0x04C0,
	[PMIF_MONITOR_TARGET_WDATA_0] =		0x04C4,
	[PMIF_MONITOR_TARGET_WDATA_1] =		0x04C8,
	[PMIF_MONITOR_TARGET_WDATA_2] =		0x04CC,
	[PMIF_MONITOR_TARGET_WDATA_3] =		0x04D0,
	[PMIF_MONITOR_TARGET_WDATA_4] =		0x04D4,
	[PMIF_MONITOR_TARGET_WDATA_5] =		0x04D8,
	[PMIF_MONITOR_TARGET_WDATA_6] =		0x04DC,
	[PMIF_MONITOR_TARGET_WDATA_7] =		0x04E0,
	[PMIF_MONITOR_STA] =			0x04E4,
	[PMIF_MONITOR_RECORD_0_0] =		0x04E8,
	[PMIF_MONITOR_RECORD_0_1] =		0x04EC,
	[PMIF_MONITOR_RECORD_0_2] =		0x04F0,
	[PMIF_MONITOR_RECORD_0_3] =		0x04F4,
	[PMIF_MONITOR_RECORD_0_4] =		0x04F8,
	[PMIF_MONITOR_RECORD_1_0] =		0x04FC,
	[PMIF_MONITOR_RECORD_1_1] =		0x0500,
	[PMIF_MONITOR_RECORD_1_2] =		0x0504,
	[PMIF_MONITOR_RECORD_1_3] =		0x0508,
	[PMIF_MONITOR_RECORD_1_4] =		0x050C,
	[PMIF_MONITOR_RECORD_2_0] =		0x0510,
	[PMIF_MONITOR_RECORD_2_1] =		0x0514,
	[PMIF_MONITOR_RECORD_2_2] =		0x0518,
	[PMIF_MONITOR_RECORD_2_3] =		0x051C,
	[PMIF_MONITOR_RECORD_2_4] =		0x0520,
	[PMIF_MONITOR_RECORD_3_0] =		0x0524,
	[PMIF_MONITOR_RECORD_3_1] =		0x0528,
	[PMIF_MONITOR_RECORD_3_2] =		0x052C,
	[PMIF_MONITOR_RECORD_3_3] =		0x0530,
	[PMIF_MONITOR_RECORD_3_4] =		0x0534,
	[PMIF_MONITOR_RECORD_4_0] =		0x0538,
	[PMIF_MONITOR_RECORD_4_1] =		0x053C,
	[PMIF_MONITOR_RECORD_4_2] =		0x0540,
	[PMIF_MONITOR_RECORD_4_3] =		0x0544,
	[PMIF_MONITOR_RECORD_4_4] =		0x0548,
	[PMIF_MONITOR_RECORD_5_0] =		0x054C,
	[PMIF_MONITOR_RECORD_5_1] =		0x0550,
	[PMIF_MONITOR_RECORD_5_2] =		0x0554,
	[PMIF_MONITOR_RECORD_5_3] =		0x0558,
	[PMIF_MONITOR_RECORD_5_4] =		0x055C,
	[PMIF_MONITOR_RECORD_6_0] =		0x0560,
	[PMIF_MONITOR_RECORD_6_1] =		0x0564,
	[PMIF_MONITOR_RECORD_6_2] =		0x0568,
	[PMIF_MONITOR_RECORD_6_3] =		0x056C,
	[PMIF_MONITOR_RECORD_6_4] =		0x0570,
	[PMIF_MONITOR_RECORD_7_0] =		0x0574,
	[PMIF_MONITOR_RECORD_7_1] =		0x0578,
	[PMIF_MONITOR_RECORD_7_2] =		0x057C,
	[PMIF_MONITOR_RECORD_7_3] =		0x0580,
	[PMIF_MONITOR_RECORD_7_4] =		0x0584,
	[PMIF_MONITOR_RECORD_8_0] =		0x0588,
	[PMIF_MONITOR_RECORD_8_1] =		0x058C,
	[PMIF_MONITOR_RECORD_8_2] =		0x0590,
	[PMIF_MONITOR_RECORD_8_3] =		0x0594,
	[PMIF_MONITOR_RECORD_8_4] =		0x0598,
	[PMIF_MONITOR_RECORD_9_0] =		0x059C,
	[PMIF_MONITOR_RECORD_9_1] =		0x05A0,
	[PMIF_MONITOR_RECORD_9_2] =		0x05A4,
	[PMIF_MONITOR_RECORD_9_3] =		0x05A8,
	[PMIF_MONITOR_RECORD_9_4] =		0x05AC,
	[PMIF_MONITOR_RECORD_10_0] =		0x05B0,
	[PMIF_MONITOR_RECORD_10_1] =		0x05B4,
	[PMIF_MONITOR_RECORD_10_2] =		0x05B8,
	[PMIF_MONITOR_RECORD_10_3] =		0x05BC,
	[PMIF_MONITOR_RECORD_10_4] =		0x05C0,
	[PMIF_MONITOR_RECORD_11_0] =		0x05C4,
	[PMIF_MONITOR_RECORD_11_1] =		0x05C8,
	[PMIF_MONITOR_RECORD_11_2] =		0x05CC,
	[PMIF_MONITOR_RECORD_11_3] =		0x05D0,
	[PMIF_MONITOR_RECORD_11_4] =		0x05D4,
	[PMIF_MONITOR_RECORD_12_0] =		0x05D8,
	[PMIF_MONITOR_RECORD_12_1] =		0x05DC,
	[PMIF_MONITOR_RECORD_12_2] =		0x05E0,
	[PMIF_MONITOR_RECORD_12_3] =		0x05E4,
	[PMIF_MONITOR_RECORD_12_4] =		0x05E8,
	[PMIF_MONITOR_RECORD_13_0] =		0x05EC,
	[PMIF_MONITOR_RECORD_13_1] =		0x05F0,
	[PMIF_MONITOR_RECORD_13_2] =		0x05F4,
	[PMIF_MONITOR_RECORD_13_3] =		0x05F8,
	[PMIF_MONITOR_RECORD_13_4] =		0x05FC,
	[PMIF_MONITOR_RECORD_14_0] =		0x0600,
	[PMIF_MONITOR_RECORD_14_1] =		0x0604,
	[PMIF_MONITOR_RECORD_14_2] =		0x0608,
	[PMIF_MONITOR_RECORD_14_3] =		0x060C,
	[PMIF_MONITOR_RECORD_14_4] =		0x0610,
	[PMIF_MONITOR_RECORD_15_0] =		0x0614,
	[PMIF_MONITOR_RECORD_15_1] =		0x0618,
	[PMIF_MONITOR_RECORD_15_2] =		0x061C,
	[PMIF_MONITOR_RECORD_15_3] =		0x0620,
	[PMIF_MONITOR_RECORD_15_4] =		0x0624,
	[PMIF_MONITOR_RECORD_16_0] =		0x0628,
	[PMIF_MONITOR_RECORD_16_1] =		0x062C,
	[PMIF_MONITOR_RECORD_16_2] =		0x0630,
	[PMIF_MONITOR_RECORD_16_3] =		0x0634,
	[PMIF_MONITOR_RECORD_16_4] =		0x0638,
	[PMIF_MONITOR_RECORD_17_0] =		0x063C,
	[PMIF_MONITOR_RECORD_17_1] =		0x0640,
	[PMIF_MONITOR_RECORD_17_2] =		0x0644,
	[PMIF_MONITOR_RECORD_17_3] =		0x0648,
	[PMIF_MONITOR_RECORD_17_4] =		0x064C,
	[PMIF_MONITOR_RECORD_18_0] =		0x0650,
	[PMIF_MONITOR_RECORD_18_1] =		0x0654,
	[PMIF_MONITOR_RECORD_18_2] =		0x0658,
	[PMIF_MONITOR_RECORD_18_3] =		0x065C,
	[PMIF_MONITOR_RECORD_18_4] =		0x0660,
	[PMIF_MONITOR_RECORD_19_0] =		0x0664,
	[PMIF_MONITOR_RECORD_19_1] =		0x0668,
	[PMIF_MONITOR_RECORD_19_2] =		0x066C,
	[PMIF_MONITOR_RECORD_19_3] =		0x0670,
	[PMIF_MONITOR_RECORD_19_4] =		0x0674,
	[PMIF_MONITOR_RECORD_20_0] =		0x0678,
	[PMIF_MONITOR_RECORD_20_1] =		0x067C,
	[PMIF_MONITOR_RECORD_20_2] =		0x0680,
	[PMIF_MONITOR_RECORD_20_3] =		0x0684,
	[PMIF_MONITOR_RECORD_20_4] =		0x0688,
	[PMIF_MONITOR_RECORD_21_0] =		0x068C,
	[PMIF_MONITOR_RECORD_21_1] =		0x0690,
	[PMIF_MONITOR_RECORD_21_2] =		0x0694,
	[PMIF_MONITOR_RECORD_21_3] =		0x0698,
	[PMIF_MONITOR_RECORD_21_4] =		0x069C,
	[PMIF_MONITOR_RECORD_22_0] =		0x06A0,
	[PMIF_MONITOR_RECORD_22_1] =		0x06A4,
	[PMIF_MONITOR_RECORD_22_2] =		0x06A8,
	[PMIF_MONITOR_RECORD_22_3] =		0x06AC,
	[PMIF_MONITOR_RECORD_22_4] =		0x06B0,
	[PMIF_MONITOR_RECORD_23_0] =		0x06B4,
	[PMIF_MONITOR_RECORD_23_1] =		0x06B8,
	[PMIF_MONITOR_RECORD_23_2] =		0x06BC,
	[PMIF_MONITOR_RECORD_23_3] =		0x06C0,
	[PMIF_MONITOR_RECORD_23_4] =		0x06C4,
	[PMIF_MONITOR_RECORD_24_0] =		0x06C8,
	[PMIF_MONITOR_RECORD_24_1] =		0x06CC,
	[PMIF_MONITOR_RECORD_24_2] =		0x06D0,
	[PMIF_MONITOR_RECORD_24_3] =		0x06D4,
	[PMIF_MONITOR_RECORD_24_4] =		0x06D8,
	[PMIF_MONITOR_RECORD_25_0] =		0x06DC,
	[PMIF_MONITOR_RECORD_25_1] =		0x06E0,
	[PMIF_MONITOR_RECORD_25_2] =		0x06E4,
	[PMIF_MONITOR_RECORD_25_3] =		0x06E8,
	[PMIF_MONITOR_RECORD_25_4] =		0x06EC,
	[PMIF_MONITOR_RECORD_26_0] =		0x06F0,
	[PMIF_MONITOR_RECORD_26_1] =		0x06F4,
	[PMIF_MONITOR_RECORD_26_2] =		0x06F8,
	[PMIF_MONITOR_RECORD_26_3] =		0x06FC,
	[PMIF_MONITOR_RECORD_26_4] =		0x0700,
	[PMIF_MONITOR_RECORD_27_0] =		0x0704,
	[PMIF_MONITOR_RECORD_27_1] =		0x0708,
	[PMIF_MONITOR_RECORD_27_2] =		0x070C,
	[PMIF_MONITOR_RECORD_27_3] =		0x0710,
	[PMIF_MONITOR_RECORD_27_4] =		0x0714,
	[PMIF_MONITOR_RECORD_28_0] =		0x0718,
	[PMIF_MONITOR_RECORD_28_1] =		0x071C,
	[PMIF_MONITOR_RECORD_28_2] =		0x0720,
	[PMIF_MONITOR_RECORD_28_3] =		0x0724,
	[PMIF_MONITOR_RECORD_28_4] =		0x0728,
	[PMIF_MONITOR_RECORD_29_0] =		0x072C,
	[PMIF_MONITOR_RECORD_29_1] =		0x0730,
	[PMIF_MONITOR_RECORD_29_2] =		0x0734,
	[PMIF_MONITOR_RECORD_29_3] =		0x0738,
	[PMIF_MONITOR_RECORD_29_4] =		0x073C,
	[PMIF_MONITOR_RECORD_30_0] =		0x0740,
	[PMIF_MONITOR_RECORD_30_1] =		0x0744,
	[PMIF_MONITOR_RECORD_30_2] =		0x0748,
	[PMIF_MONITOR_RECORD_30_3] =		0x074C,
	[PMIF_MONITOR_RECORD_30_4] =		0x0750,
	[PMIF_MONITOR_RECORD_31_0] =		0x0754,
	[PMIF_MONITOR_RECORD_31_1] =		0x0758,
	[PMIF_MONITOR_RECORD_31_2] =		0x075C,
	[PMIF_MONITOR_RECORD_31_3] =		0x0760,
	[PMIF_MONITOR_RECORD_31_4] =		0x0764,
	[PMIF_DEBUG_CTRL] =			0x0768,
	[PMIF_RESERVED_0] =			0x0770,
	[PMIF_SWINF_0_ACC] =			0x0C00,
	[PMIF_SWINF_0_WDATA_31_0] =		0x0C04,
	[PMIF_SWINF_0_WDATA_63_32] =		0x0C08,
	[PMIF_SWINF_0_RDATA_31_0] =		0x0C14,
	[PMIF_SWINF_0_RDATA_63_32] =		0x0C18,
	[PMIF_SWINF_0_VLD_CLR] =		0x0C24,
	[PMIF_SWINF_0_STA] =			0x0C28,
	[PMIF_SWINF_1_ACC] =			0x0C40,
	[PMIF_SWINF_1_WDATA_31_0] =		0x0C44,
	[PMIF_SWINF_1_WDATA_63_32] =		0x0C48,
	[PMIF_SWINF_1_RDATA_31_0] =		0x0C54,
	[PMIF_SWINF_1_RDATA_63_32] =		0x0C58,
	[PMIF_SWINF_1_VLD_CLR] =		0x0C64,
	[PMIF_SWINF_1_STA] =			0x0C68,
	[PMIF_SWINF_2_ACC] =			0x0C80,
	[PMIF_SWINF_2_WDATA_31_0] =		0x0C84,
	[PMIF_SWINF_2_WDATA_63_32] =		0x0C88,
	[PMIF_SWINF_2_RDATA_31_0] =		0x0C94,
	[PMIF_SWINF_2_RDATA_63_32] =		0x0C98,
	[PMIF_SWINF_2_VLD_CLR] =		0x0CA4,
	[PMIF_SWINF_2_STA] =			0x0CA8,
	[PMIF_SWINF_3_ACC] =			0x0CC0,
	[PMIF_SWINF_3_WDATA_31_0] =		0x0CC4,
	[PMIF_SWINF_3_WDATA_63_32] =		0x0CC8,
	[PMIF_SWINF_3_RDATA_31_0] =		0x0CD4,
	[PMIF_SWINF_3_RDATA_63_32] =		0x0CD8,
	[PMIF_SWINF_3_VLD_CLR] =		0x0CE4,
	[PMIF_SWINF_3_STA] =			0x0CE8,
	[PMIC_ACC_VIO_INFO_0] =			0x0F50,
	[PMIC_ACC_VIO_INFO_1] =			0x0F54,
	[PMIC_ACC_VIO_INFO_2] =			0x0F58,
	[PMIC_ACC_VIO_INFO_3] =			0x0F5C,
	[PMIC_ACC_VIO_INFO_4] =			0x0F60,
	[PMIC_ACC_VIO_INFO_5] =			0x0F64,
	[PMIC_ACC_SCP_VIO_INFO_0] =		0x0F68,
	[PMIC_ACC_SCP_VIO_INFO_1] =		0x0F6C,
	[PMIC_ACC_SCP_VIO_INFO_2] =		0x0F70,
	[PMIC_ACC_SCP_VIO_INFO_3] =		0x0F74,
	[PMIC_ACC_SCP_VIO_INFO_4] =		0x0F78,
	[PMIC_ACC_SCP_VIO_INFO_5] =		0x0F7C,
	[PMIF_ACC_VIO_INFO_0] =			0x0F80,
	[PMIF_ACC_VIO_INFO_1] =			0x0F84,
	[PMIF_ACC_VIO_INFO_2] =			0x0F88,
};

static char d_log_buf[1280];
static struct spmi_controller *dbg_ctrl;
static int is_drv_attr;
static char *wp;

/*
 * Function : mtk_spmi_readl_d()
 * Description : mtk spmi controller read api
 * Parameter :
 * Return :
 */
static u32 mtk_spmi_readl_d(struct pmif *arb, enum spmi_regs reg)
{
	return readl(arb->spmimst_base + arb->spmimst_regs[reg]);
}
#if 0
/*
 * Function : mtk_spmi_writel_d()
 * Description : mtk spmi controller write api
 * Parameter :
 * Return :
 */
static void mtk_spmi_writel_d(struct pmif *arb, u32 val,
		enum spmi_regs reg)
{
	writel(val, arb->spmimst_base + arb->spmimst_regs[reg]);
}
#endif
/* spmi & pmif debug mechanism */
void spmi_dump_wdt_reg(void)
{
	struct pmif *arb = spmi_controller_get_drvdata(dbg_ctrl);
	unsigned int offset, tmp_dat;
	unsigned int start, end, log_size = 0;

	start = arb->dbgregs[PMIF_WDT_EVENT_EN_0];
	end = arb->dbgregs[PMIF_WDT_FLAG_1];

	log_size += sprintf(wp, "");
	for (offset = start; offset <= end; offset += 4) {
		tmp_dat = readl(arb->base + offset);
		log_size += sprintf(wp + log_size, "(0x%x)=0x%x ",
				    offset, tmp_dat);
	}
	log_size += sprintf(wp + log_size, "\n");
	pr_info("[PMIF] %s", wp);
}

void spmi_dump_pmif_acc_vio_reg(void)
{
	struct pmif *arb = spmi_controller_get_drvdata(dbg_ctrl);
	unsigned int offset, tmp_dat;
	unsigned int start, end, log_size = 0;

	start = arb->dbgregs[PMIF_ACC_VIO_INFO_0];
	end = arb->dbgregs[PMIF_ACC_VIO_INFO_2];

	log_size += sprintf(wp, "");
	for (offset = start; offset <= end; offset += 4) {
		tmp_dat = readl(arb->base + offset);
		log_size += sprintf(wp + log_size, "(0x%x)=0x%x ",
				    offset, tmp_dat);
	}
	log_size += sprintf(wp + log_size, "\n");
	pr_info("[PMIF] %s", wp);
}

void spmi_dump_pmic_acc_vio_reg(void)
{
	struct pmif *arb = spmi_controller_get_drvdata(dbg_ctrl);
	unsigned int offset, tmp_dat;
	unsigned int start, end, log_size = 0;

	start = arb->dbgregs[PMIC_ACC_VIO_INFO_0];
	end = arb->dbgregs[PMIC_ACC_VIO_INFO_5];

	log_size += sprintf(wp, "");
	for (offset = start; offset <= end; offset += 4) {
		tmp_dat = readl(arb->base + offset);
		log_size += sprintf(wp + log_size, "(0x%x)=0x%x ",
				    offset, tmp_dat);
	}
	log_size += sprintf(wp + log_size, "\n");
	pr_info("[PMIF] %s", wp);
}

static char *get_pmif_busy_reg_dump(void)
{
	struct pmif *arb = spmi_controller_get_drvdata(dbg_ctrl);
	unsigned int offset, tmp_dat;
	unsigned int start, end, log_size = 0;

	start = arb->dbgregs[PMIF_INF_BUSY_STA];
	end = arb->dbgregs[PMIF_OTHER_BUSY_STA_1];

	log_size += sprintf(wp, "");
	for (offset = start; offset <= end; offset += 4) {
		tmp_dat = readl(arb->base + offset);
		log_size += sprintf(wp + log_size, "(0x%x)=0x%x ",
				    offset, tmp_dat);
	}
	log_size += sprintf(wp + log_size, "\n");
	return wp;
}

static char *get_pmif_swinf_reg_dump(void)
{
	struct pmif *arb = spmi_controller_get_drvdata(dbg_ctrl);
	unsigned int swinf = 0, step, offset, tmp_dat, log_size = 0;
	unsigned int cmd, is_write, slvid, bytecnt, addr;
	unsigned int wd_31_0, rd_31_0;
	unsigned int err, sbusy, done, qfillcnt, qfreecnt, qempty, qfull;
	unsigned int req, fsm, en;

	step = arb->dbgregs[PMIF_SWINF_1_ACC] - arb->dbgregs[PMIF_SWINF_0_ACC];
	log_size += sprintf(wp, "");
	for (swinf = 0; swinf < 4; swinf++) {
		offset = arb->dbgregs[PMIF_SWINF_0_ACC] + swinf * step;
		tmp_dat = readl(arb->base + offset);
		cmd = (tmp_dat & (0x3 << 30)) >> 30;
		is_write = (tmp_dat & (0x1 << 29)) >> 29;
		slvid = (tmp_dat & (0xf << 24)) >> 24;
		bytecnt = (tmp_dat & (0xf << 16)) >> 16;
		addr = (tmp_dat & (0xffff << 0)) >> 0;

		offset = arb->dbgregs[PMIF_SWINF_0_WDATA_31_0] + swinf * step;
		wd_31_0 = readl(arb->base + offset);

		offset = arb->dbgregs[PMIF_SWINF_0_RDATA_31_0] + swinf * step;
		rd_31_0 = readl(arb->base + offset);

		offset = arb->dbgregs[PMIF_SWINF_0_STA] + swinf * step;
		tmp_dat = readl(arb->base + offset);
		err = (tmp_dat & (0x1 << 18)) >> 18;
		sbusy = (tmp_dat & (0x1 << 17)) >> 17;
		done = (tmp_dat & (0x1 << 15)) >> 15;
		qfillcnt = (tmp_dat & (0xf << 11)) >> 11;
		qfreecnt = (tmp_dat & (0xf << 7)) >> 7;
		qempty = (tmp_dat & (0x1 << 6)) >> 6;
		qfull = (tmp_dat & (0x1 << 5)) >> 5;
		req = (tmp_dat & (0x1 << 4)) >> 4;
		fsm = (tmp_dat & (0x7 << 1)) >> 1;
		en = (tmp_dat & (0x1 << 0)) >> 0;

		log_size += sprintf(
			wp + log_size,
			"[swinf:%d, cmd:0x%x, is_write:%d, slvid:%d ",
			swinf, cmd, is_write, slvid);
		if (is_write) {
			log_size += sprintf(
				wp + log_size,
				"bytecnt:%d (write addr 0x%x=0x%x)]\n",
				bytecnt, addr, wd_31_0);
		} else {
			log_size += sprintf(
				wp + log_size,
				"bytecnt:%d (read addr 0x%x=0x%x)]\n",
				bytecnt, addr, rd_31_0);
		}
		log_size += sprintf(
			wp + log_size,
			"[err:%d, sbusy:%d, done:%d, qfillcnt:%d ",
			err, sbusy, done, qfillcnt);
		log_size += sprintf(
			wp + log_size,
			"qfreecnt:%d, qempty:%d, qfull:%d, req:%d ",
			qfreecnt, qempty, qfull, req);
		log_size += sprintf(wp + log_size, "fsm:%d, en:%d]\n", fsm, en);
	}
	log_size += sprintf(wp + log_size, "\n");
	return wp;
}

static char *get_spmimst_all_reg_dump(void)
{
	struct pmif *arb = spmi_controller_get_drvdata(dbg_ctrl);
	unsigned int offset, tmp_dat;
	unsigned int start, end, log_size = 0;
	int i = 0;

	start = arb->spmimst_regs[SPMI_OP_ST_CTRL];
	end = arb->spmimst_regs[SPMI_REC4];

	log_size += sprintf(wp, "\n[SPMI] ");
	for (offset = start; offset <= end; offset += 4) {
		tmp_dat = readl(arb->spmimst_base + offset);
		log_size += sprintf(wp + log_size,
				    "(0x%x)=0x%x ", offset, tmp_dat);
		i++;
		if (i % 8 == 0)
			log_size += sprintf(wp + log_size, "\n[SPMI] ");
	}
#if SPMI_RCS_SUPPORT
	offset = arb->spmimst_regs[SPMI_DEC_DBG];
	tmp_dat = readl(arb->spmimst_base + offset);
	log_size += sprintf(wp + log_size, "(0x%x)=0x%x ", offset, tmp_dat);
#endif
	offset = arb->spmimst_regs[SPMI_MST_DBG];
	tmp_dat = readl(arb->spmimst_base + offset);
	log_size += sprintf(wp + log_size, "(0x%x)=0x%x\n", offset, tmp_dat);
	return wp;
}

void spmi_dump_pmif_busy_reg(void)
{
	pr_info("[PMIF] %s", get_pmif_busy_reg_dump());
}

static void spmi_dump_pmif_busy_reg_d(struct seq_file *m)
{
	seq_puts(m, get_pmif_busy_reg_dump());
}

void spmi_dump_pmif_swinf_reg(void)
{
	pr_info("[PMIF]\n%s", get_pmif_swinf_reg_dump());
}

static void spmi_dump_pmif_swinf_reg_d(struct seq_file *m)
{
	seq_puts(m, get_pmif_swinf_reg_dump());
}

void spmi_dump_pmif_all_reg(void)
{
	struct pmif *arb = spmi_controller_get_drvdata(dbg_ctrl);
	unsigned int offset, tmp_dat, log_size = 0;
	unsigned int start, end;
	int i = 0;

	start = arb->dbgregs[PMIF_INIT_DONE];
	end = arb->dbgregs[PMIF_RESERVED_0];

	log_size += sprintf(wp, "");
	for (offset = start; offset <= end; offset += 4) {
		tmp_dat = readl(arb->base + offset);
		log_size += sprintf(wp + log_size, "(0x%x)=0x%x ",
				    offset, tmp_dat);
		i++;
		if (i % 64 == 0) {
			pr_info("\n[PMIF] %s", wp);
			log_size = 0;
		} else if (i % 8 == 0)
			log_size += sprintf(wp + log_size, "\n[PMIF] ");
	}
	pr_info("\n[PMIF] %s", wp);
}

static void spmi_dump_pmif_all_reg_d(struct seq_file *m)
{
	struct pmif *arb = spmi_controller_get_drvdata(dbg_ctrl);
	unsigned int offset, tmp_dat, log_size = 0;
	unsigned int start, end;
	int i = 0;

	start = arb->dbgregs[PMIF_INIT_DONE];
	end = arb->dbgregs[PMIF_RESERVED_0];

	for (offset = start; offset <= end; offset += 4) {
		tmp_dat = readl(arb->base + offset);
		seq_printf(m, "(0x%x)=0x%x ", offset, tmp_dat);
	}
	seq_puts(m, "\n");
}

void spmi_dump_pmif_record_reg(void)
{
	struct pmif *arb = spmi_controller_get_drvdata(dbg_ctrl);
	unsigned int i = 0, step, offset, tmp_dat;
	unsigned int chan, cmd, is_write, slvid, bytecnt, addr;
	unsigned int wd_31_0, log_size = 0;

	step = arb->dbgregs[PMIF_MONITOR_RECORD_1_0] -
		arb->dbgregs[PMIF_MONITOR_RECORD_0_0];

	log_size += sprintf(wp, "");
	for (i = 0; i < 32; i++) {
		offset = arb->dbgregs[PMIF_MONITOR_RECORD_0_0] + i * step;
		tmp_dat = readl(arb->base + offset);
		chan = (tmp_dat & (0xf8 << 27)) >> 27;
		cmd = (tmp_dat & (0x3 << 25)) >> 25;
		is_write = (tmp_dat & (0x1 << 24)) >> 24;
		slvid = (tmp_dat & (0xf << 20)) >> 20;
		bytecnt = (tmp_dat & (0xf << 16)) >> 16;
		addr = (tmp_dat & (0xffff << 0)) >> 0;

		offset = arb->dbgregs[PMIF_MONITOR_RECORD_0_1] + i * step;
		wd_31_0 = readl(arb->base + offset);

		log_size += sprintf(wp + log_size,
			"[PMIF] (%d)[swinf:%d, cmd:0x%x, rw:0x%x, slvid:%d, ",
			i, chan, cmd, is_write, slvid);
		log_size += sprintf(wp + log_size,
			"bytecnt:%d, (addr 0x%x=0x%x)]\n",
			bytecnt, addr, wd_31_0);
		if ((i + 1) % 8 == 0) {
			pr_info("\n%s", wp);
			log_size = 0;
		}
	}
	/* logging mode no need to clear record */
}

static void spmi_dump_pmif_record_reg_d(struct seq_file *m)
{
	struct pmif *arb = spmi_controller_get_drvdata(dbg_ctrl);
	unsigned int i = 0, step, offset, tmp_dat;
	unsigned int chan, cmd, is_write, slvid, bytecnt, addr;
	unsigned int wd_31_0;

	step = arb->dbgregs[PMIF_MONITOR_RECORD_1_0] -
		arb->dbgregs[PMIF_MONITOR_RECORD_0_0];

	for (i = 0; i < 32; i++) {
		offset = arb->dbgregs[PMIF_MONITOR_RECORD_0_0] + i * step;
		tmp_dat = readl(arb->base + offset);
		chan = (tmp_dat & (0xf8 << 27)) >> 27;
		cmd = (tmp_dat & (0x3 << 25)) >> 25;
		is_write = (tmp_dat & (0x1 << 24)) >> 24;
		slvid = (tmp_dat & (0xf << 20)) >> 20;
		bytecnt = (tmp_dat & (0xf << 16)) >> 16;
		addr = (tmp_dat & (0xffff << 0)) >> 0;

		offset = arb->dbgregs[PMIF_MONITOR_RECORD_0_1] + i * step;
		wd_31_0 = readl(arb->base + offset);

		seq_printf(m, "(%d)[swinf:%d, cmd:0x%x, rw:0x%x, slvid:%d, ",
			   i, chan, cmd, is_write, slvid);
		seq_printf(m, "bytecnt:%d, (addr 0x%x=0x%x)]\n",
			   bytecnt, addr, wd_31_0);
	}
	/* logging mode no need to clear record */
}

void spmi_dump_spmimst_all_reg(void)
{
	pr_info("%s", get_spmimst_all_reg_dump());
}

static void spmi_dump_spmimst_all_reg_d(struct seq_file *m)
{
	seq_puts(m, get_spmimst_all_reg_dump());
}

/*
 * PMIF dump busy register log
 */
static int proc_dump_pmif_busy_reg_show(struct seq_file *m, void *v)
{
	seq_puts(m, "********** PMIF dump busy register**********\n");
	spmi_dump_pmif_busy_reg();
	spmi_dump_pmif_busy_reg_d(m);

	return 0;
}

static int proc_dump_pmif_busy_reg_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_dump_pmif_busy_reg_show, NULL);
}

static const struct file_operations dump_pmif_busy_reg_proc_fops = {
	.open = proc_dump_pmif_busy_reg_open,
	.read = seq_read,
};

/*
 * PMIF dump swinf log
 */

static int proc_dump_pmif_swinf_show(struct seq_file *m, void *v)
{
	seq_puts(m, "********** PMIF dump swinf register**********\n");
	spmi_dump_pmif_swinf_reg();
	spmi_dump_pmif_swinf_reg_d(m);

	return 0;
}

static int proc_dump_pmif_swinf_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_dump_pmif_swinf_show, NULL);
}

static const struct file_operations dump_pmif_swinf_proc_fops = {
	.open = proc_dump_pmif_swinf_open,
	.read = seq_read,
};

/*
 * PMIF dump all register log
 */
static int proc_dump_pmif_all_reg_show(struct seq_file *m, void *v)
{
	seq_puts(m, "********** PMIF dump all register**********\n");
	spmi_dump_pmif_all_reg();
	spmi_dump_pmif_all_reg_d(m);

	return 0;
}

static int proc_dump_pmif_all_reg_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_dump_pmif_all_reg_show, NULL);
}

static const struct file_operations dump_pmif_all_reg_proc_fops = {
	.open = proc_dump_pmif_all_reg_open,
	.read = seq_read,
};

/*
 * PMIF dump record register log
 */
static int proc_dump_pmif_record_reg_show(struct seq_file *m, void *v)
{
	seq_puts(m, "********** PMIF dump record register**********\n");
	seq_puts(m, "*swinf:4=MD,swinf:5:GZ,swinf:6:AP,swinf:7:RSV\n");
	spmi_dump_pmif_record_reg();
	spmi_dump_pmif_record_reg_d(m);

	return 0;
}

static int proc_dump_pmif_record_reg_open(struct inode *inode,
					  struct file *file)
{
	return single_open(file, proc_dump_pmif_record_reg_show, NULL);
}

static const struct file_operations dump_pmif_record_reg_proc_fops = {
	.open = proc_dump_pmif_record_reg_open,
	.read = seq_read,
};

/*
 * SPMIMST dump all register log
 */
static int proc_dump_spmimst_all_reg_show(struct seq_file *m, void *v)
{
	seq_puts(m, "********** SPMIMST dump all register**********\n");
	spmi_dump_spmimst_all_reg();
	spmi_dump_spmimst_all_reg_d(m);

	return 0;
}

static int proc_dump_spmimst_all_reg_open(struct inode *inode,
					struct file *file)
{
	return single_open(file, proc_dump_spmimst_all_reg_show, NULL);
}

static const struct file_operations dump_spmimst_all_reg_proc_fops = {
	.open = proc_dump_spmimst_all_reg_open,
	.read = seq_read,
};

static u32 gpmif_of;
static u32 gpmif_val;
static ssize_t pmif_access_show(struct device_driver *ddri, char *buf)
{
	if (buf == NULL) {
		pr_notice("[%s] *buf is NULL!\n",  __func__);
		return -EINVAL;
	}
	sprintf(buf, "[%s] [0x%x]=0x%x\n", __func__, gpmif_of, gpmif_val);

	return strlen(buf);
}
static ssize_t pmif_access_store(struct device_driver *ddri,
	const char *buf, size_t count)
{
	struct pmif *arb = spmi_controller_get_drvdata(dbg_ctrl);
	int ret = 0;
	u32 offset = 0;
	u32 value = 0;

	pr_info("[%s]\n", __func__);
	if (buf != NULL && count != 0) {
		pr_info("[%s] size is %d, buf is %s\n",
			__func__, (int)count, buf);

		if (strlen(buf) < 3) {
			pr_notice("%s() Invalid input!!\n", __func__);
			return -EINVAL;
		}

		ret = sscanf(buf, "0x%x 0x%x", &offset, &value);
		if (ret < 0)
			return ret;

		if (value) {
			if (offset > arb->dbgregs[PMIF_SWINF_3_STA]) {
				pr_notice("%s() Illegal offset[0x%x]!!\n",
					__func__, offset);
			} else {
				pr_info("%s() set offset[0x%x]=0x%x\n",
					__func__, arb->base + offset, value);
				writel(value, arb->base + offset);
			}
		}

		gpmif_of = offset;
		gpmif_val = readl(arb->base + offset);
	}
	return count;
}
static u32 gspmi_of;
static u32 gspmi_val;
static ssize_t spmi_access_show(struct device_driver *ddri, char *buf)
{
	if (buf == NULL) {
		pr_notice("[%s] *buf is NULL!\n",  __func__);
		return -EINVAL;
	}
	sprintf(buf, "[%s] [0x%x]=0x%x\n", __func__, gspmi_of, gspmi_val);

	return strlen(buf);
}
static ssize_t spmi_access_store(struct device_driver *ddri,
	const char *buf, size_t count)
{
	struct pmif *arb = spmi_controller_get_drvdata(dbg_ctrl);
	int ret = 0;
	u32 offset = 0;
	u32 value = 0;

	pr_info("[%s]\n", __func__);
	if (buf != NULL && count != 0) {
		pr_info("[%s] size is %d, buf is %s\n",
			__func__, (int)count, buf);

		if (strlen(buf) < 3) {
			pr_notice("%s() Invalid input!!\n", __func__);
			return -EINVAL;
		}

		ret = sscanf(buf, "0x%x 0x%x", &offset, &value);
		if (ret < 0)
			return ret;

		if (value) {
			if (offset > arb->spmimst_regs[SPMI_MST_DBG]) {
				pr_notice("%s() Illegal offset[0x%x]!!\n",
					__func__, offset);
			} else {
				pr_info("%s() set offset[0x%x]=0x%x\n",
					__func__, arb->spmimst_base + offset,
					value);
				writel(value, arb->spmimst_base + offset);
			}
		}

		gspmi_of = offset;
		gspmi_val = readl(arb->spmimst_base + offset);
	}
	return count;
}
static DRIVER_ATTR_RW(pmif_access);
static DRIVER_ATTR_RW(spmi_access);

static struct driver_attribute *spmi_pmif_attr_list[] = {
	&driver_attr_pmif_access,
	&driver_attr_spmi_access,
};

int spmi_pmif_create_attr(struct device_driver *driver)
{
	int idx, err;
	int num = ARRAY_SIZE(spmi_pmif_attr_list);
	struct dentry *mtk_spmi_dir;

	mtk_spmi_dir = debugfs_create_dir("mtk_spmi", NULL);
	if (!mtk_spmi_dir) {
		pr_notice("fail to mkdir /sys/kernel/debug/mtk_spmi\n");
		return -ENOMEM;
	}

	/*--/sys/kernel/debug/mtk_spmi--*/
	debugfs_create_file("dump_pmif_busy_reg", 0644,
				mtk_spmi_dir, NULL,
				&dump_pmif_busy_reg_proc_fops);
	debugfs_create_file("dump_pmif_swinf", 0644,
				mtk_spmi_dir, NULL,
				&dump_pmif_swinf_proc_fops);
	debugfs_create_file("dump_pmif_all_reg", 0644,
				mtk_spmi_dir, NULL,
				&dump_pmif_all_reg_proc_fops);
	debugfs_create_file("dump_pmif_record_reg", 0644,
				mtk_spmi_dir, NULL,
				&dump_pmif_record_reg_proc_fops);
	debugfs_create_file("dump_spmimst_all_reg", 0644,
				mtk_spmi_dir, NULL,
				&dump_spmimst_all_reg_proc_fops);

	if (driver == NULL)
		return -EINVAL;

	/*--/sys/devices/platform/10027000.spmi/driver--*/
	for (idx = 0; idx < num; idx++) {
		err = driver_create_file(driver, spmi_pmif_attr_list[idx]);
		if (err) {
			pr_notice("%s() driver_create_file %s err:%d\n",
			__func__, spmi_pmif_attr_list[idx]->attr.name, err);
			break;
		}
	}
	return err;
}

int spmi_pmif_dbg_init(struct spmi_controller *ctrl)
{
	struct pmif *arb = spmi_controller_get_drvdata(ctrl);

	dbg_ctrl = ctrl;
	wp = d_log_buf;
	if (of_device_is_compatible(ctrl->dev.parent->of_node,
				    "mediatek,mt6885-pmif"))
		arb->dbgregs = mt6885_pmif_dbg_regs;
	else
		arb->dbgregs = mt6xxx_pmif_dbg_regs;

	return 0;
}

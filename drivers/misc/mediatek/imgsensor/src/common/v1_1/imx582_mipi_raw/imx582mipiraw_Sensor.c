/*
 * Copyright (C) 2016 MediaTek Inc.
 * Copyright (C) 2020 XiaoMi, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 */

/*****************************************************************************
 *
 * Filename:
 * ---------
 *	 IMX582mipi_Sensor.c
 *
 * Project:
 * --------
 *	 ALPS
 *
 * Description:
 * ------------
 *	 Source code of Sensor driver
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/atomic.h>
#include <linux/types.h>

#include "kd_camera_typedef.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"

#include "imx582mipiraw_Sensor.h"

#undef VENDOR_EDIT

/***************Modify Following Strings for Debug**********************/
#define PFX "IMX582_camera_sensor"
#define LOG_1 LOG_INF("IMX582,MIPI 4LANE\n")
/****************************   Modify end	**************************/

#define LOG_INF(format, args...) pr_debug(PFX "[%s] " format, __func__, ##args)


#define CUS1_12M_60_FPS 1/* 0:CUS1_12M_100_FPS, 1:CUS1_12M_60_FPS */

#ifdef VENDOR_EDIT
#define MODULE_ID_OFFSET 0x0000
#endif

static DEFINE_SPINLOCK(imgsensor_drv_lock);

static struct imgsensor_info_struct imgsensor_info = {
	.sensor_id = IMX582_SENSOR_ID,

	.checksum_value = 0x8ac2d94a,

	.pre = {
		.pclk = 863200000,
		.linelength = 7872,
		.framelength = 3654,
		.startx = 0,
		.starty = 0,
		.grabwindow_width = 3000,
		.grabwindow_height = 3000,
		.mipi_data_lp2hs_settle_dc = 85,
		/* following for GetDefaultFramerateByScenario() */
		.mipi_pixel_rate = 823200000,
		.max_framerate = 300, /* 30fps */
	},

	.cap = { /* reg_E Full size @30fps*/
		.pclk = 863200000,
		.linelength = 7872,
		.framelength = 3654,
		.startx = 0,
		.starty = 0,
		.grabwindow_width = 4000,
		.grabwindow_height = 3000,
		.mipi_data_lp2hs_settle_dc = 85,
		.mipi_pixel_rate = 823200000,
		.max_framerate = 300,
	},

	.normal_video = {
		.pclk = 863200000,
		.linelength = 7872,
		.framelength = 3654,
		.startx = 0,
		.starty = 0,
		.grabwindow_width = 4000,
		.grabwindow_height = 2256,
		.mipi_data_lp2hs_settle_dc = 85,
		.mipi_pixel_rate = 823200000,
		.max_framerate = 300,
	},

	.hs_video = { /* reg_G 1920x1080 @120fps (binning)*/
		.pclk = 863200000,
		.linelength = 2912,
		.framelength = 2470,
		.startx = 0,
		.starty = 0,
		.grabwindow_width = 2000,
		.grabwindow_height = 1500,
		.mipi_data_lp2hs_settle_dc = 85,
		.mipi_pixel_rate = 739200000,
		.max_framerate = 1200,
	},

	.slim_video = { /* reg_F 2000x1500 @30fps (binning) */
		.pclk = 1728000000,
		.linelength = 5376,
		.framelength = 10714,
		.startx = 0,
		.starty = 0,
		.grabwindow_width = 2000,
		.grabwindow_height = 1500,
		.mipi_data_lp2hs_settle_dc = 85,
		.mipi_pixel_rate = 814630000,
		.max_framerate = 300,
	},

#if CUS1_12M_60_FPS
	.custom1 = { /*reg_B 4000x3000 @60fps (binning)*/
		.pclk = 1003200000,
		.linelength = 5376,
		.framelength = 3110,
		.startx = 0,
		.starty = 0,
		.grabwindow_width = 4000,
		.grabwindow_height = 3000,
		.mipi_data_lp2hs_settle_dc = 85,
		.mipi_pixel_rate = 846510000,
		.max_framerate = 600, /*60fps*/
	},
#else /*CUS1_12M_100_FPS*/
	.custom1 = { /*reg_B 4000x3000 @100fps (binning)*/
		.pclk = 1702400000,
		.linelength = 5376,
		.framelength = 3166,
		.startx = 0,
		.starty = 0,
		.grabwindow_width = 4000,
		.grabwindow_height = 3000,
		.mipi_data_lp2hs_settle_dc = 85,
		.mipi_pixel_rate = 1492110000,
		.max_framerate = 1000, /*100fps*/
	},
#endif

	.custom2 = { /* reg_E 1280x720 @ 240fps (binning)*/
		.pclk = 1728000000,
		.linelength = 5376,
		.framelength = 1338,
		.startx = 0,
		.starty = 0,
		.grabwindow_width = 1280,
		.grabwindow_height = 720,
		.mipi_pixel_rate = 563660000,
		.mipi_data_lp2hs_settle_dc = 85,
		.max_framerate = 2400,
	},

	.margin = 48,		/* sensor framelength & shutter margin */
	.min_shutter = 6,	/* min shutter */
	.min_gain = 64, /*1x gain*/
	.max_gain = 4096, /*64x gain*/
	.min_gain_iso = 100,
	.gain_step = 16,
	.gain_type = 0,
	.max_frame_length = 0xffff,
	.ae_shut_delay_frame = 0,
	.ae_sensor_gain_delay_frame = 0,
	.ae_ispGain_delay_frame = 2,	/* isp gain delay frame for AE cycle */
	.ihdr_support = 0,	/* 1, support; 0,not support */
	.ihdr_le_firstline = 0,	/* 1,le first ; 0, se first */
	.sensor_mode_num = 6,	/* support sensor mode num */

	.cap_delay_frame = 2,	/* enter capture delay frame num */
	.pre_delay_frame = 2,	/* enter preview delay frame num */
	.video_delay_frame = 2,	/* enter video delay frame num */
	.hs_video_delay_frame = 2,
	.slim_video_delay_frame = 2,	/* enter slim video delay frame num */
	.custom1_delay_frame = 2,	/* enter custom1 delay frame num */
	.custom2_delay_frame = 2,	/* enter custom2 delay frame num */

	.isp_driving_current = ISP_DRIVING_4MA,
	.sensor_interface_type = SENSOR_INTERFACE_TYPE_MIPI,
	/* .mipi_sensor_type = MIPI_OPHY_NCSI2, */
	/* 0,MIPI_OPHY_NCSI2;  1,MIPI_OPHY_CSI2 */
	.mipi_sensor_type = MIPI_OPHY_NCSI2,
	.mipi_settle_delay_mode = 0,
	.sensor_output_dataformat = SENSOR_OUTPUT_FORMAT_RAW_4CELL_HW_BAYER_R,
	.mclk = 26, /* mclk value, suggest 24 or 26 for 24Mhz or 26Mhz */
	/*.mipi_lane_num = SENSOR_MIPI_4_LANE,*/
	.mipi_lane_num = SENSOR_MIPI_4_LANE,
	.i2c_addr_table = {0x34, 0x20, 0xff},
	/* record sensor support all write id addr,
	 * only supprt 4 must end with 0xff
	 */
	.i2c_speed = 1000, /* i2c read/write speed */
};

static struct imgsensor_struct imgsensor = {
	.mirror = IMAGE_NORMAL,	/* mirrorflip information */
	.sensor_mode = IMGSENSOR_MODE_INIT,
	/* IMGSENSOR_MODE enum value,record current sensor mode,such as:
	 * INIT, Preview, Capture, Video,High Speed Video, Slim Video
	 */
	.shutter = 0x3D0,	/* current shutter */
	.gain = 0x100,		/* current gain */
	.dummy_pixel = 0,	/* current dummypixel */
	.dummy_line = 0,	/* current dummyline */
	.current_fps = 300,
	.autoflicker_en = KAL_FALSE,
	.test_pattern = KAL_FALSE,
	.current_scenario_id = MSDK_SCENARIO_ID_CAMERA_PREVIEW,
	.ihdr_mode = 0, /* sensor need support LE, SE with HDR feature */
	.i2c_write_id = 0x34, /* record current sensor's i2c write id */
};


/* Sensor output window information */
static struct SENSOR_WINSIZE_INFO_STRUCT imgsensor_winsize_info[7] = {
	{8000, 6000, 0000, 0000, 8000, 6000, 4000, 3000,
	  500, 0000, 3000, 3000,    0,    0, 3000, 3000}, /* Preview */
	{8000, 6000, 0000, 0000, 8000, 6000, 4000, 3000,
	 0000, 0000, 4000, 3000,    0,    0, 4000, 3000}, /* capture */
	{8000, 6000, 0000, 0000, 8000, 6000, 4000, 3000,
	 0000,  372, 4000, 2256,    0,    0, 4000, 2256}, /* video */
	{8000, 6000, 0000, 0000, 8000, 6000, 4000, 3000,
	 0000, 0000, 2000, 1500,    0,    0, 2000, 1500}, /* high speed video */
	{8000, 6000, 0000, 0000, 8000, 6000, 2000, 1500,
	 0000, 0000, 2000, 1500,    0,    0, 2000, 1500}, /*  slim */
#if CUS1_12M_60_FPS
	{8000, 6000, 0000, 0000, 8000, 6000, 4000, 3000,
	 0000, 0000, 4000, 3000,    0,    0, 4000, 3000}, /*  custom1 */
#else /*CUS1_12M_100_FPS*/
	{8000, 6000, 0000, 0000, 8000, 6000, 4000, 3000,
	 0000, 0000, 4000, 3000,    0,    0, 4000, 3000}, /*  custom1 */
#endif
	{8000, 6000, 0000, 1560, 8000, 2880, 2000,  720,
	  360, 0000, 1280,  720,    0,    0, 1280,  720}, /* custom2 */
};
 /*VC1 for HDR(DT=0X35), VC2 for PDAF(DT=0X36), unit : 10bit */
static struct SENSOR_VC_INFO_STRUCT SENSOR_VC_INFO[3] = {
	/* Preview mode setting */
	{0x03, 0x0a, 0x00, 0x08, 0x40, 0x00,
	 0x00, 0x2b, 0x0BB8, 0x0BB8, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x34, 0x04D8, 0x05D0, 0x00, 0x00, 0x0000, 0x0000},
	/* Capture mode setting */
	{0x03, 0x0a, 0x00, 0x08, 0x40, 0x00,
	 0x00, 0x2b, 0x0FA0, 0x0BB8, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x34, 0x026C, 0x0BA0, 0x00, 0x00, 0x0000, 0x0000},
	/* Video mode setting */
	{0x02, 0x0a, 0x00, 0x08, 0x40, 0x00,
	 0x00, 0x2b, 0x0FA0, 0x08D0, 0x00, 0x00, 0x0000, 0x0000,
	 0x00, 0x34, 0x04b0, 0x0430, 0x00, 0x00, 0x0000, 0x0000}
};


/* If mirror flip */
static struct SET_PD_BLOCK_INFO_T imgsensor_pd_info = {
	.i4OffsetX = 17,
	.i4OffsetY = 12,
	.i4PitchX = 8,
	.i4PitchY = 16,
	.i4PairNum = 8,
	.i4SubBlkW = 8,
	.i4SubBlkH = 2,
	.i4PosL = {{20, 13}, {18, 15}, {22, 17}, {24, 19},
		   {20, 21}, {18, 23}, {22, 25}, {24, 27} },
	.i4PosR = {{19, 13}, {17, 15}, {21, 17}, {23, 19},
		   {19, 21}, {17, 23}, {21, 25}, {23, 27} },
	.iMirrorFlip = 3,
	.i4BlockNumX = 496,
	.i4BlockNumY = 186,
	.i4Crop = {{0, 0}, {0, 0}, {0, 372}, {0, 0}, {0, 0},
		   {0, 0}, {0, 0}, {0,   0}, {0, 0}, {0, 0} }
};

static struct SET_PD_BLOCK_INFO_T imgsensor_pd_info_video = {
	.i4OffsetX = 17,
	.i4OffsetY = 12,
	.i4PitchX = 8,
	.i4PitchY = 16,
	.i4PairNum = 8,
	.i4SubBlkW = 8,
	.i4SubBlkH = 2,
	.i4PosL = {{20, 13}, {18, 15}, {22, 17}, {24, 19},
		   {20, 21}, {18, 23}, {22, 25}, {24, 27} },
	.i4PosR = {{19, 13}, {17, 15}, {21, 17}, {23, 19},
		   {19, 21}, {17, 23}, {21, 25}, {23, 27} },
	.iMirrorFlip = 3,
	.i4BlockNumX = 480,
	.i4BlockNumY = 134,
	.i4Crop = {{0, 0}, {0, 0}, {80, 420}, {0, 0}, {0, 0},
		   {0, 0}, {0, 0}, {0,   0}, {0, 0}, {0, 0} }
};

static kal_uint16 read_cmos_sensor(kal_uint32 addr)
{
	kal_uint16 get_byte = 0;
	char pusendcmd[2] = {(char)(addr >> 8), (char)(addr & 0xFF)};

	iReadRegI2C(pusendcmd, 2, (u8 *)&get_byte, 2, imgsensor.i2c_write_id);
	return ((get_byte<<8)&0xff00) | ((get_byte>>8)&0x00ff);
}

static kal_uint16 read_cmos_sensor_8(kal_uint16 addr)
{
	kal_uint16 get_byte = 0;
	char pusendcmd[2] = {(char)(addr >> 8), (char)(addr & 0xFF) };

	iReadRegI2C(pusendcmd, 2, (u8 *)&get_byte, 1, imgsensor.i2c_write_id);
	return get_byte;
}

static void write_cmos_sensor_8(kal_uint16 addr, kal_uint8 para)
{
	char pusendcmd[3] = {(char)(addr >> 8), (char)(addr & 0xFF),
			(char)(para & 0xFF)};

	iWriteRegI2C(pusendcmd, 3, imgsensor.i2c_write_id);
}

static void imx582_get_pdaf_reg_setting(MUINT32 regNum, kal_uint16 *regDa)
{
	int i, idx;

	for (i = 0; i < regNum; i++) {
		idx = 2 * i;
		regDa[idx + 1] = read_cmos_sensor_8(regDa[idx]);
		LOG_INF("%x %x", regDa[idx], regDa[idx+1]);
	}
}
static void imx582_set_pdaf_reg_setting(MUINT32 regNum, kal_uint16 *regDa)
{
	int i, idx;

	for (i = 0; i < regNum; i++) {
		idx = 2 * i;
		write_cmos_sensor_8(regDa[idx], regDa[idx + 1]);
		LOG_INF("%x %x", regDa[idx], regDa[idx+1]);
	}
}

static void set_dummy(void)
{
	LOG_INF("dummyline = %d, dummypixels = %d\n",
		imgsensor.dummy_line, imgsensor.dummy_pixel);
	/* return;*/ /* for test */
	write_cmos_sensor_8(0x0104, 0x01);

	write_cmos_sensor_8(0x0340, imgsensor.frame_length >> 8);
	write_cmos_sensor_8(0x0341, imgsensor.frame_length & 0xFF);
	write_cmos_sensor_8(0x0342, imgsensor.line_length >> 8);
	write_cmos_sensor_8(0x0343, imgsensor.line_length & 0xFF);

	write_cmos_sensor_8(0x0104, 0x00);

}	/*	set_dummy  */

static void set_mirror_flip(kal_uint8 image_mirror)
{
	kal_uint8 itemp;

	LOG_INF("image_mirror = %d\n", image_mirror);
	itemp = read_cmos_sensor_8(0x0101);
	itemp &= ~0x03;

	switch (image_mirror) {

	case IMAGE_NORMAL:
	write_cmos_sensor_8(0x0101, itemp);
	break;

	case IMAGE_V_MIRROR:
	write_cmos_sensor_8(0x0101, itemp | 0x02);
	break;

	case IMAGE_H_MIRROR:
	write_cmos_sensor_8(0x0101, itemp | 0x01);
	break;

	case IMAGE_HV_MIRROR:
	write_cmos_sensor_8(0x0101, itemp | 0x03);
	break;
	}
}

static void set_max_framerate(UINT16 framerate, kal_bool min_framelength_en)
{
	/*kal_int16 dummy_line;*/
	kal_uint32 frame_length = imgsensor.frame_length;

	LOG_INF("framerate = %d, min framelength should enable %d\n", framerate,
		min_framelength_en);

	frame_length = imgsensor.pclk / framerate * 10 / imgsensor.line_length;
	spin_lock(&imgsensor_drv_lock);
	if (frame_length >= imgsensor.min_frame_length)
		imgsensor.frame_length = frame_length;
	else
		imgsensor.frame_length = imgsensor.min_frame_length;

	imgsensor.dummy_line =
			imgsensor.frame_length - imgsensor.min_frame_length;

	if (imgsensor.frame_length > imgsensor_info.max_frame_length) {
		imgsensor.frame_length = imgsensor_info.max_frame_length;
		imgsensor.dummy_line =
			imgsensor.frame_length - imgsensor.min_frame_length;
	}
	if (min_framelength_en)
		imgsensor.min_frame_length = imgsensor.frame_length;
	spin_unlock(&imgsensor_drv_lock);
	set_dummy();
}	/*	set_max_framerate  */

static void write_shutter(kal_uint32 shutter)
{
	kal_uint16 realtime_fps = 0;
	#ifdef VENDOR_EDIT
	/*Yijun.Tan@camera.driver,20180116,add for slow shutter */
	int longexposure_times = 0;
	static int long_exposure_status;
	#endif

	spin_lock(&imgsensor_drv_lock);
	if (shutter > imgsensor.min_frame_length - imgsensor_info.margin)
		imgsensor.frame_length = shutter + imgsensor_info.margin;
	else
		imgsensor.frame_length = imgsensor.min_frame_length;
	if (imgsensor.frame_length > imgsensor_info.max_frame_length)
		imgsensor.frame_length = imgsensor_info.max_frame_length;
	spin_unlock(&imgsensor_drv_lock);
	if (shutter < imgsensor_info.min_shutter)
		shutter = imgsensor_info.min_shutter;

	if (imgsensor.autoflicker_en) {
		realtime_fps = imgsensor.pclk / imgsensor.line_length * 10
				/ imgsensor.frame_length;
		if (realtime_fps >= 297 && realtime_fps <= 305)
			set_max_framerate(296, 0);
		else if (realtime_fps >= 147 && realtime_fps <= 150)
			set_max_framerate(146, 0);
		else {
		/* Extend frame length*/
		write_cmos_sensor_8(0x0104, 0x01);
		write_cmos_sensor_8(0x0340, imgsensor.frame_length >> 8);
		write_cmos_sensor_8(0x0341, imgsensor.frame_length & 0xFF);
		write_cmos_sensor_8(0x0104, 0x00);
		}
	} else {
		/* Extend frame length*/
		write_cmos_sensor_8(0x0104, 0x01);
		write_cmos_sensor_8(0x0340, imgsensor.frame_length >> 8);
		write_cmos_sensor_8(0x0341, imgsensor.frame_length & 0xFF);
		write_cmos_sensor_8(0x0104, 0x00);

		LOG_INF("(else)imgsensor.frame_length = %d\n",
			imgsensor.frame_length);

	}
	#ifdef VENDOR_EDIT
	while (shutter >= 65535) {
		shutter = shutter / 2;
		longexposure_times += 1;
	}

	if (longexposure_times > 0) {
		LOG_INF("enter long exposure mode, time is %d",
			longexposure_times);
		long_exposure_status = 1;
		imgsensor.frame_length = shutter + 32;
		write_cmos_sensor_8(0x0104, 0x01);
		write_cmos_sensor_8(0x3100, longexposure_times & 0x07);
		write_cmos_sensor_8(0x0340, imgsensor.frame_length >> 8);
		write_cmos_sensor_8(0x0341, imgsensor.frame_length & 0xFF);
		write_cmos_sensor_8(0x0104, 0x00);
	} else if (long_exposure_status == 1) {
		long_exposure_status = 0;
		write_cmos_sensor_8(0x0104, 0x01);
		write_cmos_sensor_8(0x3100, 0x00);
		write_cmos_sensor_8(0x0340, imgsensor.frame_length >> 8);
		write_cmos_sensor_8(0x0341, imgsensor.frame_length & 0xFF);
		write_cmos_sensor_8(0x0104, 0x00);

		LOG_INF("exit long exposure mode");
	}
	#endif
	/* Update Shutter */
	write_cmos_sensor_8(0x0104, 0x01);
	write_cmos_sensor_8(0x0202, (shutter >> 8) & 0xFF);
	write_cmos_sensor_8(0x0203, shutter  & 0xFF);
	write_cmos_sensor_8(0x0104, 0x00);
	LOG_INF("shutter =%d, framelength =%d\n",
		shutter, imgsensor.frame_length);

}	/*	write_shutter  */

/*************************************************************************
 * FUNCTION
 *	set_shutter
 *
 * DESCRIPTION
 *	This function set e-shutter of sensor to change exposure time.
 *
 * PARAMETERS
 *	iShutter : exposured lines
 *
 * RETURNS
 *	None
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
static void set_shutter(kal_uint32 shutter)
{
	unsigned long flags;

	spin_lock_irqsave(&imgsensor_drv_lock, flags);
	imgsensor.shutter = shutter;
	spin_unlock_irqrestore(&imgsensor_drv_lock, flags);

	write_shutter(shutter);
} /* set_shutter */


/*************************************************************************
 * FUNCTION
 *	set_shutter_frame_length
 *
 * DESCRIPTION
 *	for frame & 3A sync
 *
 *************************************************************************/
static void set_shutter_frame_length(kal_uint16 shutter,
				     kal_uint16 frame_length)
{
	unsigned long flags;
	kal_uint16 realtime_fps = 0;
	kal_int32 dummy_line = 0;

	spin_lock_irqsave(&imgsensor_drv_lock, flags);
	imgsensor.shutter = shutter;
	spin_unlock_irqrestore(&imgsensor_drv_lock, flags);

	/*0x3500, 0x3501, 0x3502 will increase VBLANK to
	 *get exposure larger than frame exposure
	 *AE doesn't update sensor gain at capture mode,
	 *thus extra exposure lines must be updated here.
	 */

	/* OV Recommend Solution */
	/*if shutter bigger than frame_length,
	 *should extend frame length first
	 */
	spin_lock(&imgsensor_drv_lock);
	/* Change frame time */
	dummy_line = frame_length - imgsensor.frame_length;
	imgsensor.frame_length = imgsensor.frame_length + dummy_line;
	imgsensor.min_frame_length = imgsensor.frame_length;

	if (shutter > imgsensor.min_frame_length - imgsensor_info.margin)
		imgsensor.frame_length = shutter + imgsensor_info.margin;
	else
		imgsensor.frame_length = imgsensor.min_frame_length;
	if (imgsensor.frame_length > imgsensor_info.max_frame_length)
		imgsensor.frame_length = imgsensor_info.max_frame_length;
	spin_unlock(&imgsensor_drv_lock);
	shutter = (shutter < imgsensor_info.min_shutter)
			? imgsensor_info.min_shutter : shutter;
	shutter =
	(shutter > (imgsensor_info.max_frame_length - imgsensor_info.margin))
		? (imgsensor_info.max_frame_length - imgsensor_info.margin)
		: shutter;

	if (imgsensor.autoflicker_en) {
		realtime_fps = imgsensor.pclk / imgsensor.line_length * 10 /
				imgsensor.frame_length;
		if (realtime_fps >= 297 && realtime_fps <= 305)
			set_max_framerate(296, 0);
		else if (realtime_fps >= 147 && realtime_fps <= 150)
			set_max_framerate(146, 0);
		else {
			/* Extend frame length */
			write_cmos_sensor_8(0x0104, 0x01);
			write_cmos_sensor_8(0x0340,
					imgsensor.frame_length >> 8);
			write_cmos_sensor_8(0x0341,
					imgsensor.frame_length & 0xFF);
			write_cmos_sensor_8(0x0104, 0x00);
		}
	} else {
		/* Extend frame length */
		write_cmos_sensor_8(0x0104, 0x01);
		write_cmos_sensor_8(0x0340, imgsensor.frame_length >> 8);
		write_cmos_sensor_8(0x0341, imgsensor.frame_length & 0xFF);
		write_cmos_sensor_8(0x0104, 0x00);
	}

	/* Update Shutter */
	write_cmos_sensor_8(0x0104, 0x01);
	write_cmos_sensor_8(0x0350, 0x00); /* Disable auto extend */
	write_cmos_sensor_8(0x0202, (shutter >> 8) & 0xFF);
	write_cmos_sensor_8(0x0203, shutter  & 0xFF);
	write_cmos_sensor_8(0x0104, 0x00);
	LOG_INF(
		"Exit! shutter =%d, framelength =%d/%d, dummy_line=%d, auto_extend=%d\n",
		shutter, imgsensor.frame_length, frame_length,
		dummy_line, read_cmos_sensor_8(0x0350));

}	/* set_shutter_frame_length */

static kal_uint16 gain2reg(const kal_uint16 gain)
{
	 kal_uint16 reg_gain = 0x0;

	reg_gain = 1024 - (1024*64)/gain;
	return (kal_uint16) reg_gain;
}

/*************************************************************************
 * FUNCTION
 *	set_gain
 *
 * DESCRIPTION
 *	This function is to set global gain to sensor.
 *
 * PARAMETERS
 *	iGain : sensor global gain(base: 0x40)
 *
 * RETURNS
 *	the actually gain set to sensor.
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
static kal_uint16 set_gain(kal_uint16 gain)
{
	kal_uint16 reg_gain, max_gain = imgsensor_info.max_gain;

	if (gain < imgsensor_info.min_gain || gain > imgsensor_info.max_gain) {
		LOG_INF("Error gain setting");

		if (gain < imgsensor_info.min_gain)
			gain = imgsensor_info.min_gain;
		else if (gain > imgsensor_info.max_gain)
			gain = imgsensor_info.max_gain;
	}

	reg_gain = gain2reg(gain);
	spin_lock(&imgsensor_drv_lock);
	imgsensor.gain = reg_gain;
	spin_unlock(&imgsensor_drv_lock);
	LOG_INF("gain = %d, reg_gain = 0x%x, max_gain: 0x%x\n",
		gain, reg_gain, max_gain);

	write_cmos_sensor_8(0x0104, 0x01);
	write_cmos_sensor_8(0x0204, (reg_gain>>8) & 0xFF);
	write_cmos_sensor_8(0x0205, reg_gain & 0xFF);
	write_cmos_sensor_8(0x0104, 0x00);

	return gain;
} /* set_gain */

/*************************************************************************
 * FUNCTION
 *	night_mode
 *
 * DESCRIPTION
 *	This function night mode of sensor.
 *
 * PARAMETERS
 *	bEnable: KAL_TRUE -> enable night mode, otherwise, disable night mode
 *
 * RETURNS
 *	None
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
static kal_uint32 streaming_control(kal_bool enable)
{
	LOG_INF("streaming_enable(0=Sw Standby,1=streaming): %d\n",
		enable);
	if (enable)
		write_cmos_sensor_8(0x0100, 0X01);
	else
		write_cmos_sensor_8(0x0100, 0x00);
	return ERROR_NONE;
}


#define MULTI_WRITE 0

#if MULTI_WRITE
#define I2C_BUFFER_LEN 765 /* trans# max is 255, each 3 bytes */
#else
#define I2C_BUFFER_LEN 3
#endif
static kal_uint16 imx582_table_write_cmos_sensor(kal_uint16 *para,
						 kal_uint32 len)
{
	char puSendCmd[I2C_BUFFER_LEN];
	kal_uint32 tosend, IDX;
	kal_uint16 addr = 0, addr_last = 0, data;

	tosend = 0;
	IDX = 0;

	while (len > IDX) {
		addr = para[IDX];

		{
			puSendCmd[tosend++] = (char)(addr >> 8);
			puSendCmd[tosend++] = (char)(addr & 0xFF);
			data = para[IDX + 1];
			puSendCmd[tosend++] = (char)(data & 0xFF);
			IDX += 2;
			addr_last = addr;

		}
#if MULTI_WRITE
		/* Write when remain buffer size is less than 3 bytes
		 * or reach end of data
		 */
		if ((I2C_BUFFER_LEN - tosend) < 3
			|| IDX == len || addr != addr_last) {
			iBurstWriteReg_multi(puSendCmd,
						tosend,
						imgsensor.i2c_write_id,
						3,
						imgsensor_info.i2c_speed);
			tosend = 0;
		}
#else
		iWriteRegI2C(puSendCmd, 3, imgsensor.i2c_write_id);
		tosend = 0;

#endif
	}
	return 0;
}

static kal_uint16 imx582_init_setting[] = {
	/* External Clock Setting */
	0x0136,	0x1A,
	0x0137,	0x00,

	/* Register version */
	0x3C7E,	0x02,
	0x3C7F,	0x04,

	/* Global Setting */
	0x3C00,	0x10,
	0x3C01,	0x10,
	0x3C02,	0x10,
	0x3C03,	0x10,
	0x3C04,	0x10,
	0x3C05,	0x01,
	0x3C06,	0x00,
	0x3C07,	0x00,
	0x3C08,	0x03,
	0x3C09,	0xFF,
	0x3C0A,	0x01,
	0x3C0B,	0x00,
	0x3C0C,	0x00,
	0x3C0D,	0x03,
	0x3C0E,	0xFF,
	0x3C0F,	0x20,
	0x3F89,	0x01,
	0x5265,	0x13,
	0x5268,	0x05,
	0x526A,	0x27,
	0x526B,	0x23,
	0x5727,	0x77,
	0x5729,	0x05,
	0x572B,	0x0A,
	0x5737,	0x88,
	0x5739,	0x0F,
	0x573B,	0x0A,
	0x574B,	0x4D,
	0x574F,	0x0A,
	0x5757,	0xA8,
	0x5759,	0x14,
	0x575B,	0x0A,
	0x5765,	0x8C,
	0x5768,	0x00,
	0x5769,	0xE7,
	0x577D,	0x5A,
	0x577F,	0x00,
	0x5781,	0x0A,
	0x5783,	0x95,
	0x5787,	0xC2,
	0x5789,	0x06,
	0x578B,	0x0A,
	0x579B,	0x8A,
	0x579D,	0x00,
	0x579F,	0x0A,
	0x57A5,	0x95,
	0x57A9,	0x99,
	0x57AB,	0x01,
	0x57AF,	0xC9,
	0x57B3,	0x95,
	0x57BF,	0x00,
	0x57C1,	0x0A,
	0x57C9,	0x00,
	0x57CB,	0x0A,
	0x5801,	0x00,
	0x5803,	0x0A,
	0x5811,	0x00,
	0x5813,	0x0A,
	0x5B79,	0xB5,
	0x5B7B,	0x51,
	0x5C57,	0x54,
	0x5C59,	0xD9,
	0x5C5B,	0x90,
	0x5C5D,	0x18,
	0x5C5F,	0x2A,
	0x5C61,	0xAF,
	0x5C62,	0x01,
	0x5C63,	0xBA,
	0x5C65,	0x42,
	0x5C67,	0xC8,
	0x5C69,	0x4D,
	0x5C6B,	0xBC,
	0x5C6D,	0x44,
	0x5C6E,	0x01,
	0x5C6F,	0xE5,
	0x5C73,	0x34,
	0x5C74,	0x02,
	0x5C75,	0xBC,
	0x5C7F,	0x40,
	0x5C81,	0xA7,
	0x5C82,	0x01,
	0x5C83,	0xD8,
	0x5C85,	0xA6,
	0x5C8B,	0x6F,
	0x5C8C,	0x01,
	0x5C8D,	0xF7,
	0x5E2C,	0x03,
	0x5E2D,	0x03,
	0x6024,	0x05,
	0x6027,	0x05,
	0x6030,	0x05,
	0x6033,	0x05,
	0x603C,	0x05,
	0x603F,	0x05,
	0x6048,	0x05,
	0x604B,	0x05,
	0x606A,	0x08,
	0x606F,	0x08,
	0x6071,	0x08,
	0x6079,	0x06,
	0x607A,	0x07,
	0x607B,	0x07,
	0x607C,	0x06,
	0x607D,	0x0D,
	0x607E,	0x06,
	0x607F,	0x06,
	0x6080,	0x06,
	0x6081,	0x0E,
	0x6082,	0x06,
	0x6083,	0x06,
	0x6084,	0x0E,
	0x6085,	0x0B,
	0x6086,	0x0E,
	0x6087,	0x0E,
	0x6088,	0x0E,
	0x6089,	0x0B,
	0x608B,	0x0E,
	0x608C,	0x0E,
	0x6091,	0x06,
	0x6095,	0x06,
	0x6098,	0x06,
	0x6099,	0x06,
	0x609A,	0x06,
	0x609B,	0x06,
	0x609C,	0x06,
	0x609D,	0x06,
	0x609E,	0x06,
	0x609F,	0x06,
	0x60A0,	0x06,
	0x60B7,	0x32,
	0x60C1,	0x32,
	0x60C5,	0x32,
	0x60D5,	0x99,
	0x60DF,	0x99,
	0x60E3,	0x99,
	0x60F3,	0x99,
	0x60F7,	0xAD,
	0x60FB,	0xCE,
	0x60FD,	0x99,
	0x60FF,	0xCE,
	0x6103,	0xCE,
	0x6106,	0x16,
	0x6107,	0x15,
	0x6108,	0x0E,
	0x610A,	0x17,
	0x610B,	0x16,
	0x610C,	0x0F,
	0x610E,	0x18,
	0x610F,	0x16,
	0x6110,	0x0F,
	0x6112,	0x18,
	0x6113,	0x17,
	0x6114,	0x10,
	0x6116,	0x18,
	0x6117,	0x18,
	0x6118,	0x11,
	0x6147,	0x0D,
	0x6148,	0x0D,
	0x6149,	0x0D,
	0x614A,	0x0D,
	0x614B,	0x0D,
	0x614C,	0x0D,
	0x614D,	0x02,
	0x614E,	0x0D,
	0x614F,	0x0D,
	0x6150,	0x02,
	0x6151,	0x0D,
	0x6152,	0x02,
	0x6153,	0x02,
	0x6154,	0x02,
	0x6155,	0x02,
	0x6157,	0x21,
	0x6159,	0x0C,
	0x615B,	0x0C,
	0x615D,	0x0A,
	0x6161,	0x22,
	0x6163,	0x20,
	0x6165,	0x1F,
	0x6167,	0x1A,
	0x616B,	0x22,
	0x616D,	0x21,
	0x616F,	0x20,
	0x6171,	0x1B,
	0x6175,	0x23,
	0x6177,	0x22,
	0x6179,	0x21,
	0x617B,	0x1E,
	0x617F,	0x23,
	0x6181,	0x23,
	0x6183,	0x23,
	0x6185,	0x1E,
	0x6188,	0xB4,
	0x6189,	0x7E,
	0x618A,	0x96,
	0x618B,	0x50,
	0x618D,	0xD2,
	0x618E,	0xC8,
	0x618F,	0xB4,
	0x6190,	0x78,
	0x6192,	0xE6,
	0x6193,	0xCC,
	0x6194,	0xB4,
	0x6195,	0x82,
	0x6197,	0xE6,
	0x6198,	0xD2,
	0x6199,	0xD2,
	0x619A,	0xAA,
	0x619C,	0xE6,
	0x619D,	0xE8,
	0x619E,	0xDC,
	0x619F,	0xCD,
	0x61A1,	0x09,
	0x61A2,	0x0A,
	0x61A3,	0x0B,
	0x61A4,	0x0A,
	0x61A6,	0x08,
	0x61A7,	0x0A,
	0x61A8,	0x0B,
	0x61A9,	0x0A,
	0x61AB,	0x08,
	0x61AC,	0x0A,
	0x61AD,	0x0B,
	0x61AE,	0x0A,
	0x61B0,	0x08,
	0x61B1,	0x08,
	0x61B2,	0x0A,
	0x61B3,	0x09,
	0x61B5,	0x08,
	0x61B6,	0x08,
	0x61B7,	0x09,
	0x61B8,	0x09,
	0x61C4,	0x0F,
	0x61C5,	0x05,
	0x61C9,	0x14,
	0x61CA,	0x05,
	0x61CC,	0x0A,
	0x61CE,	0x14,
	0x61CF,	0x14,
	0x61D1,	0x23,
	0x621E,	0x50,
	0x621F,	0x3C,
	0x6220,	0x30,
	0x6221,	0x30,
	0x6223,	0x6E,
	0x6224,	0x5E,
	0x6225,	0x5C,
	0x6226,	0x5C,
	0x6229,	0x64,
	0x622A,	0x66,
	0x622B,	0x66,
	0x6E1D,	0x00,
	0x6E25,	0x00,
	0x6E38,	0x03,
	0x6E3B,	0x01,
	0x9004,	0x2C,
	0x9200,	0xF4,
	0x9201,	0xA7,
	0x9202,	0xF4,
	0x9203,	0xAA,
	0x9204,	0xF4,
	0x9205,	0xAD,
	0x9206,	0xF4,
	0x9207,	0xB0,
	0x9208,	0xF4,
	0x9209,	0xB3,
	0x920A,	0xB7,
	0x920B,	0x34,
	0x920C,	0xB7,
	0x920D,	0x36,
	0x920E,	0xB7,
	0x920F,	0x37,
	0x9210,	0xB7,
	0x9211,	0x38,
	0x9212,	0xB7,
	0x9213,	0x39,
	0x9214,	0xB7,
	0x9215,	0x3A,
	0x9216,	0xB7,
	0x9217,	0x3C,
	0x9218,	0xB7,
	0x9219,	0x3D,
	0x921A,	0xB7,
	0x921B,	0x3E,
	0x921C,	0xB7,
	0x921D,	0x3F,
	0x921E,	0x85,
	0x921F,	0x77,
	0x9226,	0x42,
	0x9227,	0x52,
	0x9228,	0x60,
	0x9229,	0xB9,
	0x922A,	0x60,
	0x922B,	0xBF,
	0x922C,	0x60,
	0x922D,	0xC5,
	0x922E,	0x60,
	0x922F,	0xCB,
	0x9230,	0x60,
	0x9231,	0xD1,
	0x9232,	0x60,
	0x9233,	0xD7,
	0x9234,	0x60,
	0x9235,	0xDD,
	0x9236,	0x60,
	0x9237,	0xE3,
	0x9238,	0x60,
	0x9239,	0xE9,
	0x923A,	0x60,
	0x923B,	0xEF,
	0x923C,	0x60,
	0x923D,	0xF5,
	0x923E,	0x60,
	0x923F,	0xF9,
	0x9240,	0x60,
	0x9241,	0xFD,
	0x9242,	0x61,
	0x9243,	0x01,
	0x9244,	0x61,
	0x9245,	0x05,
	0x924A,	0x61,
	0x924B,	0x6B,
	0x924C,	0x61,
	0x924D,	0x7F,
	0x924E,	0x61,
	0x924F,	0x92,
	0x9250,	0x61,
	0x9251,	0x9C,
	0x9252,	0x61,
	0x9253,	0xAB,
	0x9254,	0x61,
	0x9255,	0xC4,
	0x9256,	0x61,
	0x9257,	0xCE,
	0x9810,	0x14,
	0x9814,	0x14,
	0xA112,	0x68,
	0xA113,	0x56,
	0xA114,	0x2B,
	0xA115,	0x55,
	0xA116,	0x55,
	0xA117,	0x16,
	0xA119,	0x51,
	0xA11A,	0x34,
	0xA139,	0x4F,
	0xA13A,	0x48,
	0xA13B,	0x45,
	0xA13C,	0x02,
	0xA13F,	0x23,
	0xA140,	0x16,
	0xA141,	0x12,
	0xA142,	0x02,
	0xA596,	0x06,
	0xA597,	0x13,
	0xA598,	0x13,
	0xA779,	0x20,
	0xAC12,	0x01,
	0xAC13,	0x26,
	0xAC14,	0x01,
	0xAC15,	0x26,
	0xAC17,	0xC4,
	0xAF05,	0x48,
	0xB069,	0x02,
	0xC448,	0x01,
	0xC44B,	0x07,
	0xC44C,	0x0E,
	0xC44D,	0x50,
	0xC451,	0x00,
	0xC452,	0x01,
	0xC455,	0x00,
	0xE286,	0x31,
	0xE2A6,	0x32,
	0xE2C6,	0x33,
	0xEA4B,	0x00,
	0xEA4C,	0x00,
	0xEA4D,	0x00,
	0xEA4E,	0x00,
	0xF001,	0x10,
	0xF00D,	0x40,
	0xF031,	0x10,
	0xF03D,	0x40,
	0xF44B,	0x80,
	0xF44D,	0x06,
	0xF44E,	0x80,
	0xF450,	0x06,
	0xF451,	0x80,
	0xF453,	0x06,
	0xF454,	0x80,
	0xF456,	0x06,
	0xF457,	0x80,
	0xF459,	0x06,
	0xF478,	0x20,
	0xF47B,	0x20,
	0xF47E,	0x20,
	0xF481,	0x20,
	0xF484,	0x20,
	0x3E14,	0x01,
	0xE013,	0x01,
	0xE186,	0x2B,

	/* Image Quality adjustment setting */
	0x88D6,	0x60,
	0x9852,	0x00,
	0xAE09,	0xFF,
	0xAE0A,	0xFF,
	0xAE12,	0x58,
	0xAE13,	0x58,
	0xAE15,	0x10,
	0xAE16,	0x10,
	0xB071,	0x00,
};

static kal_uint16 imx582_preview_setting[] = {
	/* MIPI output setting */
	0x0112,	0x0A,
	0x0113,	0x0A,
	0x0114,	0x03,

	/* Line Length PCK Setting */
	0x0342,	0x1E,
	0x0343,	0xC0,

	/* Frame Length Lines Setting */
	0x0340,	0x0E,
	0x0341,	0x46,

	/* ROI Setting */
	0x0344,	0x00,
	0x0345,	0x00,
	0x0346,	0x00,
	0x0347,	0x00,
	0x0348,	0x1F,
	0x0349,	0x3F,
	0x034A,	0x17,
	0x034B,	0x6F,

	/* Mode Setting */
	0x0900,	0x01,
	0x0901,	0x22,
	0x0902,	0x08,
	0x3246,	0x81,
	0x3247,	0x81,

	/* Digital Crop & Scaling */
	0x0401,	0x00,
	0x0404,	0x00,
	0x0405,	0x10,
	0x0408,	0x01,
	0x0409,	0xF4,
	0x040A,	0x00,
	0x040B,	0x00,
	0x040C,	0x0B,
	0x040D,	0xB8,
	0x040E,	0x0B,
	0x040F,	0xB8,

	/* Output Size Setting */
	0x034C,	0x0B,
	0x034D,	0xB8,
	0x034E,	0x0B,
	0x034F,	0xB8,

	/* Clock Setting */
	0x0301,	0x05,
	0x0303,	0x02,
	0x0305,	0x04,
	0x0306,	0x01,
	0x0307,	0x4C,
	0x030B,	0x01,
	0x030D,	0x0D,
	0x030E,	0x04,
	0x030F,	0x05,
	0x0310,	0x01,

	/* Other Setting */
	0x3620,	0x00,
	0x3621,	0x00,
	0x380C,	0x80,
	0x3C13,	0x00,
	0x3C14,	0x28,
	0x3C15,	0x28,
	0x3C16,	0x32,
	0x3C17,	0x46,
	0x3C18,	0x67,
	0x3C19,	0x8F,
	0x3C1A,	0x8F,
	0x3C1B,	0x99,
	0x3C1C,	0xAD,
	0x3C1D,	0xCE,
	0x3C1E,	0x8F,
	0x3C1F,	0x8F,
	0x3C20,	0x99,
	0x3C21,	0xAD,
	0x3C22,	0xCE,
	0x3C25,	0x22,
	0x3C26,	0x23,
	0x3C27,	0xE6,
	0x3C28,	0xE6,
	0x3C29,	0x08,
	0x3C2A,	0x0F,
	0x3C2B,	0x14,
	0x3F0C,	0x01,
	0x3F14,	0x00,
	0x3F80,	0x00,
	0x3F81,	0x60,
	0x3F82,	0x03,
	0x3F83,	0x91,
	0x3F8C,	0x07,
	0x3F8D,	0xD0,
	0x3FF4,	0x05,
	0x3FF5,	0xAE,
	0x3FFC,	0x02,
	0x3FFD,	0x58,

	/* Integration Setting */
	0x0202,	0x0E,
	0x0203,	0x16,
	0x0224,	0x01,
	0x0225,	0xF4,
	0x3FE0,	0x01,
	0x3FE1,	0xF4,

	/* Gain Setting */
	0x0204,	0x00,
	0x0205,	0x70,
	0x0216,	0x00,
	0x0217,	0x70,
	0x0218,	0x01,
	0x0219,	0x00,
	0x020E,	0x01,
	0x020F,	0x00,
	0x0210,	0x01,
	0x0211,	0x00,
	0x0212,	0x01,
	0x0213,	0x00,
	0x0214,	0x01,
	0x0215,	0x00,
	0x3FE2,	0x00,
	0x3FE3,	0x70,
	0x3FE4,	0x01,
	0x3FE5,	0x00,
};

static kal_uint16 imx582_capture_setting[] = {
	/* MIPI output setting */
	0x0112,	0x0A,
	0x0113,	0x0A,
	0x0114,	0x03,

	/* Line Length PCK Setting */
	0x0342,	0x1E,
	0x0343,	0xC0,

	/* Frame Length Lines Setting */
	0x0340,	0x0E,
	0x0341,	0x46,

	/* ROI Setting */
	0x0344,	0x00,
	0x0345,	0x00,
	0x0346,	0x00,
	0x0347,	0x00,
	0x0348,	0x1F,
	0x0349,	0x3F,
	0x034A,	0x17,
	0x034B,	0x6F,


	/* Mode Setting */
	0x0900,	0x01,
	0x0901,	0x22,
	0x0902,	0x08,
	0x3246,	0x81,
	0x3247,	0x81,

	/* Digital Crop & Scaling */
	0x0401,	0x00,
	0x0404,	0x00,
	0x0405,	0x10,
	0x0408,	0x00,
	0x0409,	0x00,
	0x040A,	0x00,
	0x040B,	0x00,
	0x040C,	0x0F,
	0x040D,	0xA0,
	0x040E,	0x0B,
	0x040F,	0xB8,

	/* Output Size Setting */
	0x034C,	0x0F,
	0x034D,	0xA0,
	0x034E,	0x0B,
	0x034F,	0xB8,

	/* Clock Setting */
	0x0301,	0x05,
	0x0303,	0x02,
	0x0305,	0x04,
	0x0306,	0x01,
	0x0307,	0x4C,
	0x030B,	0x01,
	0x030D,	0x0D,
	0x030E,	0x04,
	0x030F,	0x05,
	0x0310,	0x01,

	/* Other Setting */
	0x3620,	0x00,
	0x3621,	0x00,
	0x380C,	0x80,
	0x3C13,	0x00,
	0x3C14,	0x28,
	0x3C15,	0x28,
	0x3C16,	0x32,
	0x3C17,	0x46,
	0x3C18,	0x67,
	0x3C19,	0x8F,
	0x3C1A,	0x8F,
	0x3C1B,	0x99,
	0x3C1C,	0xAD,
	0x3C1D,	0xCE,
	0x3C1E,	0x8F,
	0x3C1F,	0x8F,
	0x3C20,	0x99,
	0x3C21,	0xAD,
	0x3C22,	0xCE,
	0x3C25,	0x22,
	0x3C26,	0x23,
	0x3C27,	0xE6,
	0x3C28,	0xE6,
	0x3C29,	0x08,
	0x3C2A,	0x0F,
	0x3C2B,	0x14,
	0x3F0C,	0x01,
	0x3F14,	0x00,
	0x3F80,	0x00,
	0x3F81,	0x60,
	0x3F82,	0x03,
	0x3F83,	0x91,
	0x3F8C,	0x07,
	0x3F8D,	0xD0,
	0x3FF4,	0x05,
	0x3FF5,	0xAE,
	0x3FFC,	0x02,
	0x3FFD,	0x58,

	/* Integration Setting */
	0x0202,	0x0E,
	0x0203,	0x16,
	0x0224,	0x01,
	0x0225,	0xF4,
	0x3FE0,	0x01,
	0x3FE1,	0xF4,

	/* Gain Setting */
	0x0204,	0x00,
	0x0205,	0x70,
	0x0216,	0x00,
	0x0217,	0x70,
	0x0218,	0x01,
	0x0219,	0x00,
	0x020E,	0x01,
	0x020F,	0x00,
	0x0210,	0x01,
	0x0211,	0x00,
	0x0212,	0x01,
	0x0213,	0x00,
	0x0214,	0x01,
	0x0215,	0x00,
	0x3FE2,	0x00,
	0x3FE3,	0x70,
	0x3FE4,	0x01,
	0x3FE5,	0x00,
};

static kal_uint16 imx582_normal_video_setting[] = {
	/* MIPI output setting */
	0x0112,	0x0A,
	0x0113,	0x0A,
	0x0114,	0x03,

	/* Line Length PCK Setting */
	0x0342,	0x1E,
	0x0343,	0xC0,

	/* Frame Length Lines Setting */
	0x0340,	0x0E,
	0x0341,	0x46,

	/* ROI Setting */
	0x0344,	0x00,
	0x0345,	0x00,
	0x0346,	0x00,
	0x0347,	0x00,
	0x0348,	0x1F,
	0x0349,	0x3F,
	0x034A,	0x17,
	0x034B,	0x6F,

	/* Mode Setting */
	0x0900,	0x01,
	0x0901,	0x22,
	0x0902,	0x08,
	0x3246,	0x81,
	0x3247,	0x81,

	/* Digital Crop & Scaling */
	0x0401,	0x00,
	0x0404,	0x00,
	0x0405,	0x10,
	0x0408,	0x00,
	0x0409,	0x00,
	0x040A,	0x01,
	0x040B,	0x74,
	0x040C,	0x0F,
	0x040D,	0xA0,
	0x040E,	0x08,
	0x040F,	0xD0,

	/* Output Size Setting */
	0x034C,	0x0F,
	0x034D,	0xA0,
	0x034E,	0x08,
	0x034F,	0xD0,

	/* Clock Setting */
	0x0301,	0x05,
	0x0303,	0x02,
	0x0305,	0x04,
	0x0306,	0x01,
	0x0307,	0x4C,
	0x030B,	0x01,
	0x030D,	0x0D,
	0x030E,	0x04,
	0x030F,	0x05,
	0x0310,	0x01,

	/* Other Setting */
	0x3620,	0x00,
	0x3621,	0x00,
	0x380C,	0x80,
	0x3C13,	0x00,
	0x3C14,	0x28,
	0x3C15,	0x28,
	0x3C16,	0x32,
	0x3C17,	0x46,
	0x3C18,	0x67,
	0x3C19,	0x8F,
	0x3C1A,	0x8F,
	0x3C1B,	0x99,
	0x3C1C,	0xAD,
	0x3C1D,	0xCE,
	0x3C1E,	0x8F,
	0x3C1F,	0x8F,
	0x3C20,	0x99,
	0x3C21,	0xAD,
	0x3C22,	0xCE,
	0x3C25,	0x22,
	0x3C26,	0x23,
	0x3C27,	0xE6,
	0x3C28,	0xE6,
	0x3C29,	0x08,
	0x3C2A,	0x0F,
	0x3C2B,	0x14,
	0x3F0C,	0x01,
	0x3F14,	0x00,
	0x3F80,	0x00,
	0x3F81,	0x60,
	0x3F82,	0x03,
	0x3F83,	0x91,
	0x3F8C,	0x07,
	0x3F8D,	0xD0,
	0x3FF4,	0x05,
	0x3FF5,	0xAE,
	0x3FFC,	0x02,
	0x3FFD,	0x58,

	/* Integration Setting */
	0x0202,	0x0E,
	0x0203,	0x16,
	0x0224,	0x01,
	0x0225,	0xF4,
	0x3FE0,	0x01,
	0x3FE1,	0xF4,

	 /*Gain Setting */
	0x0204,	0x00,
	0x0205,	0x70,
	0x0216,	0x00,
	0x0217,	0x70,
	0x0218,	0x01,
	0x0219,	0x00,
	0x020E,	0x01,
	0x020F,	0x00,
	0x0210,	0x01,
	0x0211,	0x00,
	0x0212,	0x01,
	0x0213,	0x00,
	0x0214,	0x01,
	0x0215,	0x00,
	0x3FE2,	0x00,
	0x3FE3,	0x70,
	0x3FE4,	0x01,
	0x3FE5,	0x00,
};

static kal_uint16 imx582_hs_video_setting[] = {
	/* MIPI output setting */
	0x0112,	0x0A,
	0x0113,	0x0A,
	0x0114,	0x03,

	/* Line Length PCK Setting */
	0x0342,	0x0B,
	0x0343,	0x60,

	/* Frame Length Lines Setting */
	0x0340,	0x09,
	0x0341,	0xA6,

	/* ROI Setting */
	0x0344,	0x00,
	0x0345,	0x00,
	0x0346,	0x00,
	0x0347,	0x00,
	0x0348,	0x1F,
	0x0349,	0x3F,
	0x034A,	0x17,
	0x034B,	0x6F,

	/* Mode Setting */
	0x0900,	0x01,
	0x0901,	0x44,
	0x0902,	0x08,
	0x3246,	0x89,
	0x3247,	0x89,

	/* Digital Crop & Scaling */
	0x0401,	0x00,
	0x0404,	0x00,
	0x0405,	0x10,
	0x0408,	0x00,
	0x0409,	0x00,
	0x040A,	0x00,
	0x040B,	0x00,
	0x040C,	0x07,
	0x040D,	0xD0,
	0x040E,	0x05,
	0x040F,	0xDC,

	/* Output Size Setting */
	0x034C,	0x07,
	0x034D,	0xD0,
	0x034E,	0x05,
	0x034F,	0xDC,

	/* Clock Setting */
	0x0301,	0x05,
	0x0303,	0x02,
	0x0305,	0x04,
	0x0306,	0x01,
	0x0307,	0x4C,
	0x030B,	0x01,
	0x030D,	0x0D,
	0x030E,	0x03,
	0x030F,	0x9C,
	0x0310,	0x01,

	/* Other Setting */
	0x3620,	0x00,
	0x3621,	0x00,
	0x380C,	0x80,
	0x3C13,	0x00,
	0x3C14,	0x28,
	0x3C15,	0x28,
	0x3C16,	0x32,
	0x3C17,	0x46,
	0x3C18,	0x67,
	0x3C19,	0x8F,
	0x3C1A,	0x8F,
	0x3C1B,	0x99,
	0x3C1C,	0xAD,
	0x3C1D,	0xCE,
	0x3C1E,	0x8F,
	0x3C1F,	0x8F,
	0x3C20,	0x99,
	0x3C21,	0xAD,
	0x3C22,	0xCE,
	0x3C25,	0x22,
	0x3C26,	0x23,
	0x3C27,	0xE6,
	0x3C28,	0xE6,
	0x3C29,	0x08,
	0x3C2A,	0x0F,
	0x3C2B,	0x14,
	0x3F0C,	0x00,
	0x3F14,	0x00,
	0x3F80,	0x00,
	0x3F81,	0x00,
	0x3F82,	0x00,
	0x3F83,	0x00,
	0x3F8C,	0x00,
	0x3F8D,	0x00,
	0x3FF4,	0x00,
	0x3FF5,	0x4C,
	0x3FFC,	0x00,
	0x3FFD,	0x00,

	/* Integration Setting */
	0x0202,	0x09,
	0x0203,	0x76,
	0x0224,	0x01,
	0x0225,	0xF4,
	0x3FE0,	0x01,
	0x3FE1,	0xF4,

	/* Gain Setting */
	0x0204,	0x00,
	0x0205,	0x70,
	0x0216,	0x00,
	0x0217,	0x70,
	0x0218,	0x01,
	0x0219,	0x00,
	0x020E,	0x01,
	0x020F,	0x00,
	0x0210,	0x01,
	0x0211,	0x00,
	0x0212,	0x01,
	0x0213,	0x00,
	0x0214,	0x01,
	0x0215,	0x00,
	0x3FE2,	0x00,
	0x3FE3,	0x70,
	0x3FE4,	0x01,
	0x3FE5,	0x00,
};

static kal_uint16 imx582_slim_video_setting[] = {
	/*MIPI output setting*/
	0x0112, 0x0A,
	0x0113, 0x0A,
	0x0114, 0x02,
	/*Line Length PCK Setting*/
	0x0342, 0x15,
	0x0343, 0x00,
	/*Frame Length Lines Setting*/
	0x0340, 0x29,
	0x0341, 0xDA,
	/*ROI Setting*/
	0x0344, 0x00,
	0x0345, 0x00,
	0x0346, 0x00,
	0x0347, 0x00,
	0x0348, 0x1F,
	0x0349, 0x3F,
	0x034A, 0x17,
	0x034B, 0x6F,
	/*Mode Setting*/
	0x0220, 0x62,
	0x0222, 0x01,
	0x0900, 0x01,
	0x0901, 0x44,
	0x0902, 0x09,
	0x3140, 0x00,
	0x3246, 0x89,
	0x3247, 0x89,
	0x3F15, 0x00,
	/*Digital Crop & Scaling*/
	0x0401, 0x00,
	0x0404, 0x00,
	0x0405, 0x10,
	0x0408, 0x00,
	0x0409, 0x00,
	0x040A, 0x00,
	0x040B, 0x00,
	0x040C, 0x07,
	0x040D, 0xD0,
	0x040E, 0x05,
	0x040F, 0xDC,
	/*Output Size Setting*/
	0x034C, 0x07,
	0x034D, 0xD0,
	0x034E, 0x05,
	0x034F, 0xDC,
	/*Clock Setting*/
	0x0301, 0x05,
	0x0303, 0x02,
	0x0305, 0x04,
	0x0306, 0x01,
	0x0307, 0x68,
	0x030B, 0x01,
	0x030D, 0x04,
	0x030E, 0x00,
	0x030F, 0xC6,
	0x0310, 0x01,
	/*Other Setting*/
	0x3620, 0x00,
	0x3621, 0x00,
	0x3F0C, 0x00,
	0x3F14, 0x00,
	0x3F80, 0x00,
	0x3F81, 0x00,
	0x3FFC, 0x00,
	0x3FFD, 0x00,
	/*Integration Setting*/
	0x0202, 0x29,
	0x0203, 0xAA,
	0x0224, 0x01,
	0x0225, 0xF4,
	0x3FE0, 0x01,
	0x3FE1, 0xF4,
	/*Gain Setting*/
	0x0204, 0x00,
	0x0205, 0x00,
	0x0216, 0x00,
	0x0217, 0x00,
	0x0218, 0x01,
	0x0219, 0x00,
	0x020E, 0x01,
	0x020F, 0x00,
	0x0210, 0x01,
	0x0211, 0x00,
	0x0212, 0x01,
	0x0213, 0x00,
	0x0214, 0x01,
	0x0215, 0x00,
	0x3FE2, 0x00,
	0x3FE3, 0x00,
	0x3FE4, 0x01,
	0x3FE5, 0x00,
};

#if CUS1_12M_60_FPS
static kal_uint16 imx582_custom1_setting[] = {
	/*MIPI output setting*/
	0x0112, 0x0A,
	0x0113, 0x0A,
	0x0114, 0x02,
	/*Line Length PCK Setting*/
	0x0342, 0x15,
	0x0343, 0x00,
	/*Frame Length Lines Setting*/
	0x0340, 0x0C,
	0x0341, 0x26,
	/*ROI Setting*/
	0x0344, 0x00,
	0x0345, 0x00,
	0x0346, 0x00,
	0x0347, 0x00,
	0x0348, 0x1F,
	0x0349, 0x3F,
	0x034A, 0x17,
	0x034B, 0x6F,
	/*Mode Setting*/
	0x0220, 0x62,
	0x0222, 0x01,
	0x0900, 0x01,
	0x0901, 0x22,
	0x0902, 0x09,
	0x3140, 0x00,
	0x3246, 0x81,
	0x3247, 0x81,
	0x3F15, 0x00,
	/*Digital Crop & Scaling*/
	0x0401, 0x00,
	0x0404, 0x00,
	0x0405, 0x10,
	0x0408, 0x00,
	0x0409, 0x00,
	0x040A, 0x00,
	0x040B, 0x00,
	0x040C, 0x0F,
	0x040D, 0xA0,
	0x040E, 0x0B,
	0x040F, 0xB8,
	/*Output Size Setting*/
	0x034C, 0x0F,
	0x034D, 0xA0,
	0x034E, 0x0B,
	0x034F, 0xB8,
	/*Clock Setting*/
	0x0301, 0x05,
	0x0303, 0x02,
	0x0305, 0x04,
	0x0306, 0x00,
	0x0307, 0xD1,
	0x030B, 0x02,
	0x030D, 0x08,
	0x030E, 0x03,
	0x030F, 0x37,
	0x0310, 0x01,
	/*Other Setting*/
	0x3620, 0x00,
	0x3621, 0x00,
	0x3F0C, 0x00,
	0x3F14, 0x00,
	0x3F80, 0x00,
	0x3F81, 0x00,
	0x3FFC, 0x00,
	0x3FFD, 0x00,
	/*Integration Setting*/
	0x0202, 0x0B,
	0x0203, 0xF6,
	0x0224, 0x01,
	0x0225, 0xF4,
	0x3FE0, 0x01,
	0x3FE1, 0xF4,
	/*Gain Setting*/
	0x0204, 0x00,
	0x0205, 0x00,
	0x0216, 0x00,
	0x0217, 0x00,
	0x0218, 0x01,
	0x0219, 0x00,
	0x020E, 0x01,
	0x020F, 0x00,
	0x0210, 0x01,
	0x0211, 0x00,
	0x0212, 0x01,
	0x0213, 0x00,
	0x0214, 0x01,
	0x0215, 0x00,
	0x3FE2, 0x00,
	0x3FE3, 0x00,
	0x3FE4, 0x01,
	0x3FE5, 0x00,
};

#else /* CUS1_12M_100_FPS */

static kal_uint16 imx582_custom1_setting[] = {
	/*MIPI output setting*/
	0x0112, 0x0A,
	0x0113, 0x0A,
	0x0114, 0x02,
	/*Line Length PCK Setting*/
	0x0342, 0x15,
	0x0343, 0x00,
	/*Frame Length Lines Setting*/
	0x0340, 0x0C,
	0x0341, 0x5E,
	/*ROI Setting*/
	0x0344, 0x00,
	0x0345, 0x00,
	0x0346, 0x00,
	0x0347, 0x00,
	0x0348, 0x1F,
	0x0349, 0x3F,
	0x034A, 0x17,
	0x034B, 0x6F,
	/*Mode Setting*/
	0x0220, 0x62,
	0x0222, 0x01,
	0x0900, 0x01,
	0x0901, 0x22,
	0x0902, 0x09,
	0x3140, 0x00,
	0x3246, 0x81,
	0x3247, 0x81,
	0x3F15, 0x00,
	/*Digital Crop & Scaling*/
	0x0401, 0x00,
	0x0404, 0x00,
	0x0405, 0x10,
	0x0408, 0x00,
	0x0409, 0x00,
	0x040A, 0x00,
	0x040B, 0x00,
	0x040C, 0x0F,
	0x040D, 0xA0,
	0x040E, 0x0B,
	0x040F, 0xB8,
	/*Output Size Setting*/
	0x034C, 0x0F,
	0x034D, 0xA0,
	0x034E, 0x0B,
	0x034F, 0xB8,
	/*Clock Setting*/
	0x0301, 0x05,
	0x0303, 0x02,
	0x0305, 0x03,
	0x0306, 0x01,
	0x0307, 0x0A,
	0x030B, 0x01,
	0x030D, 0x03,
	0x030E, 0x01,
	0x030F, 0x10,
	0x0310, 0x01,
	/*Other Setting*/
	0x3620, 0x00,
	0x3621, 0x00,
	0x3F0C, 0x00,
	0x3F14, 0x00,
	0x3F80, 0x00,
	0x3F81, 0x00,
	0x3FFC, 0x00,
	0x3FFD, 0x00,
	/*Integration Setting*/
	0x0202, 0x0C,
	0x0203, 0x2E,
	0x0224, 0x01,
	0x0225, 0xF4,
	0x3FE0, 0x01,
	0x3FE1, 0xF4,
	/*Gain Setting*/
	0x0204, 0x00,
	0x0205, 0x00,
	0x0216, 0x00,
	0x0217, 0x00,
	0x0218, 0x01,
	0x0219, 0x00,
	0x020E, 0x01,
	0x020F, 0x00,
	0x0210, 0x01,
	0x0211, 0x00,
	0x0212, 0x01,
	0x0213, 0x00,
	0x0214, 0x01,
	0x0215, 0x00,
	0x3FE2, 0x00,
	0x3FE3, 0x00,
	0x3FE4, 0x01,
	0x3FE5, 0x00,
};
#endif

static kal_uint16 imx582_custom2_setting[] = {
	/*MIPI output setting*/
	0x0112, 0x0A,
	0x0113, 0x0A,
	0x0114, 0x02,
	/*Line Length PCK Setting*/
	0x0342, 0x15,
	0x0343, 0x00,
	/*Frame Length Lines Setting*/
	0x0340, 0x05,
	0x0341, 0x3A,
	/*ROI Setting*/
	0x0344, 0x00,
	0x0345, 0x00,
	0x0346, 0x06,
	0x0347, 0x18,
	0x0348, 0x1F,
	0x0349, 0x3F,
	0x034A, 0x11,
	0x034B, 0x57,
	/*Mode Setting*/
	0x0220, 0x62,
	0x0222, 0x01,
	0x0900, 0x01,
	0x0901, 0x44,
	0x0902, 0x09,
	0x3140, 0x00,
	0x3246, 0x89,
	0x3247, 0x89,
	0x3F15, 0x00,
	/*Digital Crop & Scaling*/
	0x0401, 0x00,
	0x0404, 0x00,
	0x0405, 0x10,
	0x0408, 0x01,
	0x0409, 0x68,
	0x040A, 0x00,
	0x040B, 0x00,
	0x040C, 0x05,
	0x040D, 0x00,
	0x040E, 0x02,
	0x040F, 0xD0,
	/*Output Size Setting*/
	0x034C, 0x05,
	0x034D, 0x00,
	0x034E, 0x02,
	0x034F, 0xD0,
	/*Clock Setting*/
	0x0301, 0x05,
	0x0303, 0x02,
	0x0305, 0x04,
	0x0306, 0x01,
	0x0307, 0x68,
	0x030B, 0x02,
	0x030D, 0x04,
	0x030E, 0x01,
	0x030F, 0x12,
	0x0310, 0x01,
	/*Other Setting*/
	0x3620, 0x00,
	0x3621, 0x00,
	0x3F0C, 0x00,
	0x3F14, 0x00,
	0x3F80, 0x00,
	0x3F81, 0x00,
	0x3FFC, 0x00,
	0x3FFD, 0x00,
	/*Integration Setting*/
	0x0202, 0x05,
	0x0203, 0x0A,
	0x0224, 0x01,
	0x0225, 0xF4,
	0x3FE0, 0x01,
	0x3FE1, 0xF4,
	/*Gain Setting*/
	0x0204, 0x00,
	0x0205, 0x00,
	0x0216, 0x00,
	0x0217, 0x00,
	0x0218, 0x01,
	0x0219, 0x00,
	0x020E, 0x01,
	0x020F, 0x00,
	0x0210, 0x01,
	0x0211, 0x00,
	0x0212, 0x01,
	0x0213, 0x00,
	0x0214, 0x01,
	0x0215, 0x00,
	0x3FE2, 0x00,
	0x3FE3, 0x00,
	0x3FE4, 0x01,
	0x3FE5, 0x00,
};


static void sensor_init(void)
{
	LOG_INF("E\n");

	imx582_table_write_cmos_sensor(imx582_init_setting,
		sizeof(imx582_init_setting)/sizeof(kal_uint16));


	set_mirror_flip(imgsensor.mirror);

	LOG_INF("X");
}	/*	  sensor_init  */

static void preview_setting(void)
{
	LOG_INF("E\n");

	imx582_table_write_cmos_sensor(imx582_preview_setting,
		sizeof(imx582_preview_setting)/sizeof(kal_uint16));
	LOG_INF("X");
} /* preview_setting */


static void capture_setting(kal_uint16 currefps)
{
	LOG_INF("%s 30 fps E! currefps:%d\n", __func__, currefps);

	imx582_table_write_cmos_sensor(imx582_capture_setting,
		sizeof(imx582_capture_setting)/sizeof(kal_uint16));

	LOG_INF("%s 30 fpsX\n", __func__);
}

static void normal_video_setting(kal_uint16 currefps)
{
	LOG_INF("%s E! currefps:%d\n", __func__, currefps);

	LOG_INF("imx582 VIDEO_30_FPS\n");
	imx582_table_write_cmos_sensor(imx582_normal_video_setting,
		sizeof(imx582_normal_video_setting)/sizeof(kal_uint16));

	LOG_INF("X\n");
}

static void hs_video_setting(void)
{
	LOG_INF("%s E! currefps 120\n", __func__);


	imx582_table_write_cmos_sensor(imx582_hs_video_setting,
		sizeof(imx582_hs_video_setting)/sizeof(kal_uint16));

	LOG_INF("X\n");
}

static void slim_video_setting(void)
{
	LOG_INF("%s E\n", __func__);

	LOG_INF("%s 30 fps E! currefps\n", __func__);


	imx582_table_write_cmos_sensor(imx582_slim_video_setting,
		sizeof(imx582_slim_video_setting)/sizeof(kal_uint16));

	LOG_INF("X\n");
}


static void custom1_setting(void)
{
	LOG_INF("%s E! currefps %d\n",
				__func__, imgsensor.current_fps);
	/*************MIPI output setting************/

	imx582_table_write_cmos_sensor(imx582_custom1_setting,
		sizeof(imx582_custom1_setting)/sizeof(kal_uint16));

	LOG_INF("X");
}


static void custom2_setting(void)
{
	LOG_INF("%s 1280x720@240fps E! currefps\n", __func__);
	/*************MIPI output setting************/

	imx582_table_write_cmos_sensor(imx582_custom2_setting,
		sizeof(imx582_custom2_setting)/sizeof(kal_uint16));
	LOG_INF("X");
}

/*************************************************************************
 * FUNCTION
 *	get_imgsensor_id
 *
 * DESCRIPTION
 *	This function get the sensor ID
 *
 * PARAMETERS
 *	*sensorID : return the sensor ID
 *
 * RETURNS
 *	None
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
static kal_uint32 get_imgsensor_id(UINT32 *sensor_id)
{
	kal_uint8 i = 0;
	kal_uint8 retry = 2;
	/*sensor have two i2c address 0x34 & 0x20,
	 *we should detect the module used i2c address
	 */
	while (imgsensor_info.i2c_addr_table[i] != 0xff) {
		spin_lock(&imgsensor_drv_lock);
		imgsensor.i2c_write_id = imgsensor_info.i2c_addr_table[i];
		spin_unlock(&imgsensor_drv_lock);
		do {
			*sensor_id = ((read_cmos_sensor_8(0x0016) << 8)
					| read_cmos_sensor_8(0x0017));
			LOG_INF(
				"read_0x0000=0x%x, 0x0001=0x%x,0x0000_0001=0x%x\n",
				read_cmos_sensor_8(0x0016),
				read_cmos_sensor_8(0x0017),
				read_cmos_sensor(0x0000));
			if (*sensor_id == imgsensor_info.sensor_id) {
				LOG_INF("i2c write id: 0x%x, sensor id: 0x%x\n",
					imgsensor.i2c_write_id, *sensor_id);

				return ERROR_NONE;
			}

			LOG_INF("Read sensor id fail, id: 0x%x\n",
				imgsensor.i2c_write_id);
			retry--;
		} while (retry > 0);
		i++;
		retry = 2;
	}
	if (*sensor_id != imgsensor_info.sensor_id) {
		/*if Sensor ID is not correct,
		 *Must set *sensor_id to 0xFFFFFFFF
		 */
		*sensor_id = 0xFFFFFFFF;
		return ERROR_SENSOR_CONNECT_FAIL;
	}
	return ERROR_NONE;
}


/*************************************************************************
 * FUNCTION
 *	open
 *
 * DESCRIPTION
 *	This function initialize the registers of CMOS sensor
 *
 * PARAMETERS
 *	None
 *
 * RETURNS
 *	None
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
static kal_uint32 open(void)
{
	kal_uint8 i = 0;
	kal_uint8 retry = 2;
	kal_uint16 sensor_id = 0;


	/*sensor have two i2c address 0x6c 0x6d & 0x21 0x20,
	 *we should detect the module used i2c address
	 */
	while (imgsensor_info.i2c_addr_table[i] != 0xff) {
		spin_lock(&imgsensor_drv_lock);
		imgsensor.i2c_write_id = imgsensor_info.i2c_addr_table[i];
		spin_unlock(&imgsensor_drv_lock);
		do {
			sensor_id = ((read_cmos_sensor_8(0x0016) << 8)
					| read_cmos_sensor_8(0x0017));
			if (sensor_id == imgsensor_info.sensor_id) {
				LOG_INF("i2c write id: 0x%x, sensor id: 0x%x\n",
					imgsensor.i2c_write_id, sensor_id);
				break;
			}
			LOG_INF("Read sensor id fail, id: 0x%x\n",
				imgsensor.i2c_write_id);
			retry--;
		} while (retry > 0);
		i++;
		if (sensor_id == imgsensor_info.sensor_id)
			break;
		retry = 2;
	}
	if (imgsensor_info.sensor_id != sensor_id)
		return ERROR_SENSOR_CONNECT_FAIL;

	/* initail sequence write in  */
	sensor_init();

	spin_lock(&imgsensor_drv_lock);

	imgsensor.autoflicker_en = KAL_FALSE;
	imgsensor.sensor_mode = IMGSENSOR_MODE_INIT;
	imgsensor.shutter = 0x3D0;
	imgsensor.gain = 0x100;
	imgsensor.pclk = imgsensor_info.pre.pclk;
	imgsensor.frame_length = imgsensor_info.pre.framelength;
	imgsensor.line_length = imgsensor_info.pre.linelength;
	imgsensor.min_frame_length = imgsensor_info.pre.framelength;
	imgsensor.dummy_pixel = 0;
	imgsensor.dummy_line = 0;
	imgsensor.ihdr_mode = 0;
	imgsensor.test_pattern = KAL_FALSE;
	imgsensor.current_fps = imgsensor_info.pre.max_framerate;
	spin_unlock(&imgsensor_drv_lock);

	return ERROR_NONE;
} /* open */

/*************************************************************************
 * FUNCTION
 *	close
 *
 * DESCRIPTION
 *
 *
 * PARAMETERS
 *	None
 *
 * RETURNS
 *	None
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
static kal_uint32 close(void)
{
	LOG_INF("E\n");

	/*No Need to implement this function*/

	return ERROR_NONE;
} /* close */


/*************************************************************************
 * FUNCTION
 * preview
 *
 * DESCRIPTION
 *	This function start the sensor preview.
 *
 * PARAMETERS
 *	*image_window : address pointer of pixel numbers in one period of HSYNC
 *  *sensor_config_data : address pointer of line numbers in one period of VSYNC
 *
 * RETURNS
 *	None
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
static kal_uint32 preview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
			  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("%s E\n", __func__);

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_PREVIEW;
	imgsensor.pclk = imgsensor_info.pre.pclk;
	imgsensor.line_length = imgsensor_info.pre.linelength;
	imgsensor.frame_length = imgsensor_info.pre.framelength;
	imgsensor.min_frame_length = imgsensor_info.pre.framelength;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);

	preview_setting();

	return ERROR_NONE;
} /* preview */

/*************************************************************************
 * FUNCTION
 *	capture
 *
 * DESCRIPTION
 *	This function setup the CMOS sensor in capture MY_OUTPUT mode
 *
 * PARAMETERS
 *
 * RETURNS
 *	None
 *
 * GLOBALS AFFECTED
 *
 *************************************************************************/
static kal_uint32 capture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
			  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");
	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_CAPTURE;
#if 0
	if (imgsensor.current_fps == imgsensor_info.cap1.max_framerate) {
	/* PIP capture: 24fps for less than 13M, 20fps for 16M,15fps for 20M */
		imgsensor.pclk = imgsensor_info.cap1.pclk;
		imgsensor.line_length = imgsensor_info.cap1.linelength;
		imgsensor.frame_length = imgsensor_info.cap1.framelength;
		imgsensor.min_frame_length = imgsensor_info.cap1.framelength;
		imgsensor.autoflicker_en = KAL_FALSE;
	} else if (imgsensor.current_fps == imgsensor_info.cap2.max_framerate) {
		if (imgsensor.current_fps != imgsensor_info.cap.max_framerate)
			LOG_INF(
			"Warning: current_fps %d fps is not support, so use cap1's setting: %d fps!\n",
		imgsensor.current_fps, imgsensor_info.cap1.max_framerate/10);
		imgsensor.pclk = imgsensor_info.cap2.pclk;
		imgsensor.line_length = imgsensor_info.cap2.linelength;
		imgsensor.frame_length = imgsensor_info.cap2.framelength;
		imgsensor.min_frame_length = imgsensor_info.cap2.framelength;
		imgsensor.autoflicker_en = KAL_FALSE;
	} else {
		if (imgsensor.current_fps != imgsensor_info.cap.max_framerate)
			LOG_INF(
			"Warning: current_fps %d fps is not support, so use cap's setting: %d fps!\n",
			imgsensor.current_fps,
			imgsensor_info.cap.max_framerate / 10);
		imgsensor.pclk = imgsensor_info.cap.pclk;
		imgsensor.line_length = imgsensor_info.cap.linelength;
		imgsensor.frame_length = imgsensor_info.cap.framelength;
		imgsensor.min_frame_length = imgsensor_info.cap.framelength;
		imgsensor.autoflicker_en = KAL_FALSE;
	}
#else
	if (imgsensor.current_fps != imgsensor_info.cap.max_framerate)
		LOG_INF(
			"Warning: current_fps %d fps is not support, so use cap's setting: %d fps!\n",
			imgsensor.current_fps,
			imgsensor_info.cap.max_framerate / 10);
	imgsensor.pclk = imgsensor_info.cap.pclk;
	imgsensor.line_length = imgsensor_info.cap.linelength;
	imgsensor.frame_length = imgsensor_info.cap.framelength;
	imgsensor.min_frame_length = imgsensor_info.cap.framelength;
	imgsensor.autoflicker_en = KAL_FALSE;
#endif

	spin_unlock(&imgsensor_drv_lock);

	capture_setting(imgsensor.current_fps);


	/* set_mirror_flip(imgsensor.mirror); */

	return ERROR_NONE;
}	/* capture() */
static kal_uint32 normal_video(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
				MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_VIDEO;
	imgsensor.pclk = imgsensor_info.normal_video.pclk;
	imgsensor.line_length = imgsensor_info.normal_video.linelength;
	imgsensor.frame_length = imgsensor_info.normal_video.framelength;
	imgsensor.min_frame_length = imgsensor_info.normal_video.framelength;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	normal_video_setting(imgsensor.current_fps);
	/*set_mirror_flip(imgsensor.mirror);*/

	return ERROR_NONE;
}	/*	normal_video   */

static kal_uint32 hs_video(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
				MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_HIGH_SPEED_VIDEO;
	imgsensor.pclk = imgsensor_info.hs_video.pclk;
	/*imgsensor.video_mode = KAL_TRUE;*/
	imgsensor.line_length = imgsensor_info.hs_video.linelength;
	imgsensor.frame_length = imgsensor_info.hs_video.framelength;
	imgsensor.min_frame_length = imgsensor_info.hs_video.framelength;
	imgsensor.dummy_line = 0;
	imgsensor.dummy_pixel = 0;
	/*imgsensor.current_fps = 300;*/
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	hs_video_setting();
	/*set_mirror_flip(imgsensor.mirror);*/

	return ERROR_NONE;
}	/*	hs_video   */

static kal_uint32 slim_video(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
				MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_SLIM_VIDEO;
	imgsensor.pclk = imgsensor_info.slim_video.pclk;
	/*imgsensor.video_mode = KAL_TRUE;*/
	imgsensor.line_length = imgsensor_info.slim_video.linelength;
	imgsensor.frame_length = imgsensor_info.slim_video.framelength;
	imgsensor.min_frame_length = imgsensor_info.slim_video.framelength;
	imgsensor.dummy_line = 0;
	imgsensor.dummy_pixel = 0;
	/*imgsensor.current_fps = 300;*/
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	slim_video_setting();
	/*set_mirror_flip(imgsensor.mirror);*/

	return ERROR_NONE;
}	/* slim_video */


static kal_uint32 custom1(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
			  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("%s.\n", __func__);

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_CUSTOM1;
	imgsensor.pclk = imgsensor_info.custom1.pclk;
	imgsensor.line_length = imgsensor_info.custom1.linelength;
	imgsensor.frame_length = imgsensor_info.custom1.framelength;
	imgsensor.min_frame_length = imgsensor_info.custom1.framelength;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	custom1_setting();


	return ERROR_NONE;
}	/* custom1 */

static kal_uint32 custom2(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
			  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("%s.\n", __func__);

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_CUSTOM2;
	imgsensor.pclk = imgsensor_info.custom2.pclk;
	imgsensor.line_length = imgsensor_info.custom2.linelength;
	imgsensor.frame_length = imgsensor_info.custom2.framelength;
	imgsensor.min_frame_length = imgsensor_info.custom2.framelength;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	custom2_setting();


	return ERROR_NONE;
}	/* custom2 */

static kal_uint32
get_resolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *sensor_resolution)
{
	LOG_INF("E\n");
	sensor_resolution->SensorFullWidth =
		imgsensor_info.cap.grabwindow_width;
	sensor_resolution->SensorFullHeight =
		imgsensor_info.cap.grabwindow_height;

	sensor_resolution->SensorPreviewWidth =
		imgsensor_info.pre.grabwindow_width;
	sensor_resolution->SensorPreviewHeight =
		imgsensor_info.pre.grabwindow_height;

	sensor_resolution->SensorVideoWidth =
		imgsensor_info.normal_video.grabwindow_width;
	sensor_resolution->SensorVideoHeight =
		imgsensor_info.normal_video.grabwindow_height;

	sensor_resolution->SensorHighSpeedVideoWidth =
		imgsensor_info.hs_video.grabwindow_width;
	sensor_resolution->SensorHighSpeedVideoHeight =
		imgsensor_info.hs_video.grabwindow_height;

	sensor_resolution->SensorSlimVideoWidth =
		imgsensor_info.slim_video.grabwindow_width;
	sensor_resolution->SensorSlimVideoHeight =
		imgsensor_info.slim_video.grabwindow_height;

	sensor_resolution->SensorCustom1Width =
		imgsensor_info.custom1.grabwindow_width;
	sensor_resolution->SensorCustom1Height =
		imgsensor_info.custom1.grabwindow_height;

	sensor_resolution->SensorCustom2Width =
		imgsensor_info.custom2.grabwindow_width;
	sensor_resolution->SensorCustom2Height =
		imgsensor_info.custom2.grabwindow_height;

	return ERROR_NONE;
} /* get_resolution */

static kal_uint32 get_info(enum MSDK_SCENARIO_ID_ENUM scenario_id,
			   MSDK_SENSOR_INFO_STRUCT *sensor_info,
			   MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("scenario_id = %d\n", scenario_id);

	sensor_info->SensorClockPolarity = SENSOR_CLOCK_POLARITY_LOW;
	sensor_info->SensorClockFallingPolarity = SENSOR_CLOCK_POLARITY_LOW;
	sensor_info->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
	sensor_info->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
	sensor_info->SensorInterruptDelayLines = 4; /* not use */
	sensor_info->SensorResetActiveHigh = FALSE; /* not use */
	sensor_info->SensorResetDelayCount = 5; /* not use */

	sensor_info->SensroInterfaceType = imgsensor_info.sensor_interface_type;
	sensor_info->MIPIsensorType = imgsensor_info.mipi_sensor_type;
	sensor_info->SettleDelayMode = imgsensor_info.mipi_settle_delay_mode;
	sensor_info->SensorOutputDataFormat =
		imgsensor_info.sensor_output_dataformat;

	sensor_info->CaptureDelayFrame = imgsensor_info.cap_delay_frame;
	sensor_info->PreviewDelayFrame = imgsensor_info.pre_delay_frame;
	sensor_info->VideoDelayFrame = imgsensor_info.video_delay_frame;
	sensor_info->HighSpeedVideoDelayFrame =
		imgsensor_info.hs_video_delay_frame;
	sensor_info->SlimVideoDelayFrame =
		imgsensor_info.slim_video_delay_frame;
	sensor_info->Custom1DelayFrame = imgsensor_info.custom1_delay_frame;
	sensor_info->Custom2DelayFrame = imgsensor_info.custom2_delay_frame;

	sensor_info->SensorMasterClockSwitch = 0; /* not use */
	sensor_info->SensorDrivingCurrent = imgsensor_info.isp_driving_current;

	sensor_info->AEShutDelayFrame = imgsensor_info.ae_shut_delay_frame;
	sensor_info->AESensorGainDelayFrame =
		imgsensor_info.ae_sensor_gain_delay_frame;
	sensor_info->AEISPGainDelayFrame =
		imgsensor_info.ae_ispGain_delay_frame;
	sensor_info->IHDR_Support = imgsensor_info.ihdr_support;
	sensor_info->IHDR_LE_FirstLine = imgsensor_info.ihdr_le_firstline;
	sensor_info->SensorModeNum = imgsensor_info.sensor_mode_num;
	sensor_info->PDAF_Support = 0;
	sensor_info->SensorMIPILaneNumber = imgsensor_info.mipi_lane_num;
	sensor_info->SensorClockFreq = imgsensor_info.mclk;
	sensor_info->SensorClockDividCount = 3; /* not use */
	sensor_info->SensorClockRisingCount = 0;
	sensor_info->SensorClockFallingCount = 2; /* not use */
	sensor_info->SensorPixelClockCount = 3; /* not use */
	sensor_info->SensorDataLatchCount = 2; /* not use */

	sensor_info->MIPIDataLowPwr2HighSpeedTermDelayCount = 0;
	sensor_info->MIPICLKLowPwr2HighSpeedTermDelayCount = 0;
	sensor_info->SensorWidthSampling = 0; /* 0 is default 1x */
	sensor_info->SensorHightSampling = 0; /* 0 is default 1x */
	sensor_info->SensorPacketECCOrder = 1;

	switch (scenario_id) {
	case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		sensor_info->SensorGrabStartX = imgsensor_info.pre.startx;
		sensor_info->SensorGrabStartY = imgsensor_info.pre.starty;

		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.pre.mipi_data_lp2hs_settle_dc;

		break;
	case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		sensor_info->SensorGrabStartX = imgsensor_info.cap.startx;
		sensor_info->SensorGrabStartY = imgsensor_info.cap.starty;

		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.cap.mipi_data_lp2hs_settle_dc;

		break;
	case MSDK_SCENARIO_ID_VIDEO_PREVIEW:

		sensor_info->SensorGrabStartX =
			imgsensor_info.normal_video.startx;
		sensor_info->SensorGrabStartY =
			imgsensor_info.normal_video.starty;

		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.normal_video.mipi_data_lp2hs_settle_dc;

		break;
	case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
		sensor_info->SensorGrabStartX = imgsensor_info.hs_video.startx;
		sensor_info->SensorGrabStartY = imgsensor_info.hs_video.starty;

		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.hs_video.mipi_data_lp2hs_settle_dc;

		break;
	case MSDK_SCENARIO_ID_SLIM_VIDEO:
		sensor_info->SensorGrabStartX =
			imgsensor_info.slim_video.startx;
		sensor_info->SensorGrabStartY =
			imgsensor_info.slim_video.starty;

		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.slim_video.mipi_data_lp2hs_settle_dc;

		break;
	default:
		sensor_info->SensorGrabStartX = imgsensor_info.pre.startx;
		sensor_info->SensorGrabStartY = imgsensor_info.pre.starty;

		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.pre.mipi_data_lp2hs_settle_dc;
		break;

	case MSDK_SCENARIO_ID_CUSTOM1:
		sensor_info->SensorGrabStartX = imgsensor_info.custom1.startx;
		sensor_info->SensorGrabStartY = imgsensor_info.custom1.starty;

		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.custom1.mipi_data_lp2hs_settle_dc;
		break;

	case MSDK_SCENARIO_ID_CUSTOM2:
		sensor_info->SensorGrabStartX = imgsensor_info.custom2.startx;
		sensor_info->SensorGrabStartY = imgsensor_info.custom2.starty;

		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.custom2.mipi_data_lp2hs_settle_dc;
		break;
	}

	return ERROR_NONE;
}	/*	get_info  */


static kal_uint32 control(enum MSDK_SCENARIO_ID_ENUM scenario_id,
			MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
			MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("scenario_id = %d\n", scenario_id);
	spin_lock(&imgsensor_drv_lock);
	imgsensor.current_scenario_id = scenario_id;
	spin_unlock(&imgsensor_drv_lock);
	switch (scenario_id) {
	case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		preview(image_window, sensor_config_data);
		break;
	case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		capture(image_window, sensor_config_data);
		break;
	case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		normal_video(image_window, sensor_config_data);
		break;
	case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
		hs_video(image_window, sensor_config_data);
		break;
	case MSDK_SCENARIO_ID_SLIM_VIDEO:
		slim_video(image_window, sensor_config_data);
		break;
	case MSDK_SCENARIO_ID_CUSTOM1:
		custom1(image_window, sensor_config_data);
		break;
	case MSDK_SCENARIO_ID_CUSTOM2:
		custom2(image_window, sensor_config_data);
		break;
	default:
		LOG_INF("Error ScenarioId setting");
		preview(image_window, sensor_config_data);
		return ERROR_INVALID_SCENARIO_ID;
	}

	return ERROR_NONE;
}	/* control() */



static kal_uint32 set_video_mode(UINT16 framerate)
{
	LOG_INF("framerate = %d\n ", framerate);
	/* SetVideoMode Function should fix framerate */
	if (framerate == 0)
		/* Dynamic frame rate */
		return ERROR_NONE;
	spin_lock(&imgsensor_drv_lock);
	if ((framerate == 300) && (imgsensor.autoflicker_en == KAL_TRUE))
		imgsensor.current_fps = 296;
	else if ((framerate == 150) && (imgsensor.autoflicker_en == KAL_TRUE))
		imgsensor.current_fps = 146;
	else
		imgsensor.current_fps = framerate;
	spin_unlock(&imgsensor_drv_lock);
	set_max_framerate(imgsensor.current_fps, 1);

	return ERROR_NONE;
}

static kal_uint32 set_auto_flicker_mode(kal_bool enable, UINT16 framerate)
{
	LOG_INF("enable = %d, framerate = %d\n", enable, framerate);
	spin_lock(&imgsensor_drv_lock);
	if (enable) /*enable auto flicker*/
		imgsensor.autoflicker_en = KAL_TRUE;
	else /*Cancel Auto flick*/
		imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	return ERROR_NONE;
}


static kal_uint32 set_max_framerate_by_scenario(
		enum MSDK_SCENARIO_ID_ENUM scenario_id, MUINT32 framerate)
{
	kal_uint32 frame_length;

	LOG_INF("scenario_id = %d, framerate = %d\n", scenario_id, framerate);

	switch (scenario_id) {
	case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		frame_length = imgsensor_info.pre.pclk / framerate * 10
				/ imgsensor_info.pre.linelength;
		spin_lock(&imgsensor_drv_lock);
		imgsensor.dummy_line =
			(frame_length > imgsensor_info.pre.framelength)
		? (frame_length - imgsensor_info.pre.framelength) : 0;
		imgsensor.frame_length =
			imgsensor_info.pre.framelength
			+ imgsensor.dummy_line;
		imgsensor.min_frame_length = imgsensor.frame_length;
		spin_unlock(&imgsensor_drv_lock);
		if (imgsensor.frame_length > imgsensor.shutter)
			set_dummy();
		break;
	case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		if (framerate == 0)
			return ERROR_NONE;
		frame_length = imgsensor_info.normal_video.pclk /
				framerate * 10 /
				imgsensor_info.normal_video.linelength;
		spin_lock(&imgsensor_drv_lock);
		imgsensor.dummy_line =
			(frame_length > imgsensor_info.normal_video.framelength)
		? (frame_length - imgsensor_info.normal_video.framelength)
		: 0;
		imgsensor.frame_length =
			imgsensor_info.normal_video.framelength
			+ imgsensor.dummy_line;
		imgsensor.min_frame_length = imgsensor.frame_length;
		spin_unlock(&imgsensor_drv_lock);
		if (imgsensor.frame_length > imgsensor.shutter)
			set_dummy();
		break;
	case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
#if 0
	if (imgsensor.current_fps == imgsensor_info.cap1.max_framerate) {
		frame_length = imgsensor_info.cap1.pclk / framerate * 10
				/ imgsensor_info.cap1.linelength;
		spin_lock(&imgsensor_drv_lock);
			imgsensor.dummy_line =
			(frame_length > imgsensor_info.cap1.framelength)
			? (frame_length - imgsensor_info.cap1.framelength) : 0;
			imgsensor.frame_length =
				imgsensor_info.cap1.framelength
				+ imgsensor.dummy_line;
			imgsensor.min_frame_length = imgsensor.frame_length;
			spin_unlock(&imgsensor_drv_lock);
	} else if (imgsensor.current_fps == imgsensor_info.cap2.max_framerate) {
		frame_length = imgsensor_info.cap2.pclk / framerate * 10
					/ imgsensor_info.cap2.linelength;
		spin_lock(&imgsensor_drv_lock);
			imgsensor.dummy_line =
			  (frame_length > imgsensor_info.cap2.framelength)
			  ? (frame_length - imgsensor_info.cap2.framelength)
			  : 0;
			imgsensor.frame_length =
				imgsensor_info.cap2.framelength
				+ imgsensor.dummy_line;
			imgsensor.min_frame_length = imgsensor.frame_length;
			spin_unlock(&imgsensor_drv_lock);
	} else {
			if (imgsensor.current_fps
				!= imgsensor_info.cap.max_framerate)
			LOG_INF(
			"Warning: current_fps %d fps is not support, so use cap's setting: %d fps!\n"
			, framerate, imgsensor_info.cap.max_framerate/10);
		frame_length = imgsensor_info.cap.pclk / framerate * 10
				/ imgsensor_info.cap.linelength;
		spin_lock(&imgsensor_drv_lock);
			imgsensor.dummy_line =
			(frame_length > imgsensor_info.cap.framelength)
			  ? (frame_length - imgsensor_info.cap.framelength) : 0;
			imgsensor.frame_length =
				imgsensor_info.cap.framelength
				+ imgsensor.dummy_line;
			imgsensor.min_frame_length = imgsensor.frame_length;
			spin_unlock(&imgsensor_drv_lock);
	}
#else
	if (imgsensor.current_fps != imgsensor_info.cap.max_framerate)
		LOG_INF(
			"Warning: current_fps %d fps is not support, so use cap's setting: %d fps!\n"
			, framerate, imgsensor_info.cap.max_framerate/10);
		frame_length = imgsensor_info.cap.pclk / framerate * 10
				/ imgsensor_info.cap.linelength;
		spin_lock(&imgsensor_drv_lock);
			imgsensor.dummy_line =
			(frame_length > imgsensor_info.cap.framelength)
			  ? (frame_length - imgsensor_info.cap.framelength) : 0;
			imgsensor.frame_length =
				imgsensor_info.cap.framelength
				+ imgsensor.dummy_line;
			imgsensor.min_frame_length = imgsensor.frame_length;
			spin_unlock(&imgsensor_drv_lock);
#endif
		if (imgsensor.frame_length > imgsensor.shutter)
			set_dummy();
		break;
	case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
		frame_length = imgsensor_info.hs_video.pclk / framerate * 10
				/ imgsensor_info.hs_video.linelength;
		spin_lock(&imgsensor_drv_lock);
		imgsensor.dummy_line =
			(frame_length > imgsensor_info.hs_video.framelength)
			  ? (frame_length - imgsensor_info.hs_video.framelength)
			  : 0;
		imgsensor.frame_length =
			imgsensor_info.hs_video.framelength
				+ imgsensor.dummy_line;
		imgsensor.min_frame_length = imgsensor.frame_length;
		spin_unlock(&imgsensor_drv_lock);
		if (imgsensor.frame_length > imgsensor.shutter)
			set_dummy();
		break;
	case MSDK_SCENARIO_ID_SLIM_VIDEO:
		frame_length = imgsensor_info.slim_video.pclk / framerate * 10
			/ imgsensor_info.slim_video.linelength;
		spin_lock(&imgsensor_drv_lock);
		imgsensor.dummy_line =
			(frame_length > imgsensor_info.slim_video.framelength)
			? (frame_length - imgsensor_info.slim_video.framelength)
			: 0;
		imgsensor.frame_length =
			imgsensor_info.slim_video.framelength
			+ imgsensor.dummy_line;
		imgsensor.min_frame_length = imgsensor.frame_length;
		spin_unlock(&imgsensor_drv_lock);
		if (imgsensor.frame_length > imgsensor.shutter)
			set_dummy();
		break;
	case MSDK_SCENARIO_ID_CUSTOM1:
		frame_length = imgsensor_info.custom1.pclk / framerate * 10
				/ imgsensor_info.custom1.linelength;
		spin_lock(&imgsensor_drv_lock);
		imgsensor.dummy_line =
			(frame_length > imgsensor_info.custom1.framelength)
			? (frame_length - imgsensor_info.custom1.framelength)
			: 0;
		imgsensor.frame_length =
			imgsensor_info.custom1.framelength
			+ imgsensor.dummy_line;
		imgsensor.min_frame_length = imgsensor.frame_length;
		spin_unlock(&imgsensor_drv_lock);
		if (imgsensor.frame_length > imgsensor.shutter)
			set_dummy();
		break;
	case MSDK_SCENARIO_ID_CUSTOM2:
		frame_length = imgsensor_info.custom2.pclk / framerate * 10
				/ imgsensor_info.custom2.linelength;
		spin_lock(&imgsensor_drv_lock);
		imgsensor.dummy_line =
			(frame_length > imgsensor_info.custom2.framelength)
			? (frame_length - imgsensor_info.custom2.framelength)
			: 0;
		imgsensor.frame_length =
			imgsensor_info.custom2.framelength
			+ imgsensor.dummy_line;
		imgsensor.min_frame_length = imgsensor.frame_length;
		spin_unlock(&imgsensor_drv_lock);
		if (imgsensor.frame_length > imgsensor.shutter)
			set_dummy();
		break;
	default:  /*coding with  preview scenario by default*/
		frame_length = imgsensor_info.pre.pclk / framerate * 10
			/ imgsensor_info.pre.linelength;
		spin_lock(&imgsensor_drv_lock);
		imgsensor.dummy_line =
			(frame_length > imgsensor_info.pre.framelength)
			? (frame_length - imgsensor_info.pre.framelength) : 0;
		imgsensor.frame_length =
			imgsensor_info.pre.framelength + imgsensor.dummy_line;
		imgsensor.min_frame_length = imgsensor.frame_length;
		spin_unlock(&imgsensor_drv_lock);
		if (imgsensor.frame_length > imgsensor.shutter)
			set_dummy();
		LOG_INF("error scenario_id = %d, we use preview scenario\n",
			scenario_id);
		break;
	}
	return ERROR_NONE;
}


static kal_uint32 get_default_framerate_by_scenario(
		enum MSDK_SCENARIO_ID_ENUM scenario_id, MUINT32 *framerate)
{
	LOG_INF("scenario_id = %d\n", scenario_id);

	switch (scenario_id) {
	case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		*framerate = imgsensor_info.pre.max_framerate;
		break;
	case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		*framerate = imgsensor_info.normal_video.max_framerate;
		break;
	case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		*framerate = imgsensor_info.cap.max_framerate;
		break;
	case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
		*framerate = imgsensor_info.hs_video.max_framerate;
		break;
	case MSDK_SCENARIO_ID_SLIM_VIDEO:
		*framerate = imgsensor_info.slim_video.max_framerate;
		break;
	case MSDK_SCENARIO_ID_CUSTOM1:
		*framerate = imgsensor_info.custom1.max_framerate;
		break;
	case MSDK_SCENARIO_ID_CUSTOM2:
		*framerate = imgsensor_info.custom2.max_framerate;
		break;
	default:
		break;
	}

	return ERROR_NONE;
}

static kal_uint32 set_test_pattern_mode(kal_bool enable)
{
	LOG_INF("enable: %d\n", enable);

	if (enable)
		write_cmos_sensor_8(0x0601, 0x0002); /*100% Color bar*/
	else
		write_cmos_sensor_8(0x0601, 0x0000); /*No pattern*/

	spin_lock(&imgsensor_drv_lock);
	imgsensor.test_pattern = enable;
	spin_unlock(&imgsensor_drv_lock);
	return ERROR_NONE;
}
static kal_uint32 feature_control(MSDK_SENSOR_FEATURE_ENUM feature_id,
				 UINT8 *feature_para, UINT32 *feature_para_len)
{
	UINT16 *feature_return_para_16 = (UINT16 *) feature_para;
	UINT16 *feature_data_16 = (UINT16 *) feature_para;
	UINT32 *feature_return_para_32 = (UINT32 *) feature_para;
	UINT32 *feature_data_32 = (UINT32 *) feature_para;
	unsigned long long *feature_data = (unsigned long long *) feature_para;
	/* unsigned long long *feature_return_para
	 *  = (unsigned long long *) feature_para;
	 */
	struct SET_PD_BLOCK_INFO_T *PDAFinfo;
	struct SENSOR_WINSIZE_INFO_STRUCT *wininfo;
	struct SENSOR_VC_INFO_STRUCT *pvcinfo;
	/* SET_SENSOR_AWB_GAIN *pSetSensorAWB
	 *  = (SET_SENSOR_AWB_GAIN *)feature_para;
	 */
	MSDK_SENSOR_REG_INFO_STRUCT *sensor_reg_data
		= (MSDK_SENSOR_REG_INFO_STRUCT *) feature_para;

	/*LOG_INF("feature_id = %d\n", feature_id);*/
	switch (feature_id) {
	case SENSOR_FEATURE_GET_GAIN_RANGE_BY_SCENARIO:
		*(feature_data + 1) = imgsensor_info.min_gain;
		*(feature_data + 2) = imgsensor_info.max_gain;
		break;
	case SENSOR_FEATURE_GET_BASE_GAIN_ISO_AND_STEP:
		*(feature_data + 0) = imgsensor_info.min_gain_iso;
		*(feature_data + 1) = imgsensor_info.gain_step;
		*(feature_data + 2) = imgsensor_info.gain_type;
		break;
	case SENSOR_FEATURE_GET_MIN_SHUTTER_BY_SCENARIO:
		*(feature_data + 1) = imgsensor_info.min_shutter;
		break;
	case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ_BY_SCENARIO:
		switch (*feature_data) {
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
				= imgsensor_info.cap.pclk;
			break;
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
				= imgsensor_info.normal_video.pclk;
			break;
		case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
				= imgsensor_info.hs_video.pclk;
			break;
		case MSDK_SCENARIO_ID_CUSTOM1:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
				= imgsensor_info.custom1.pclk;
			break;
		case MSDK_SCENARIO_ID_CUSTOM2:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
				= imgsensor_info.custom2.pclk;
			break;
		case MSDK_SCENARIO_ID_SLIM_VIDEO:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
				= imgsensor_info.slim_video.pclk;
			break;
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		default:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
				= imgsensor_info.pre.pclk;
			break;
		}
		break;
	case SENSOR_FEATURE_GET_PERIOD_BY_SCENARIO:
		switch (*feature_data) {
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
			= (imgsensor_info.cap.framelength << 16)
				+ imgsensor_info.cap.linelength;
			break;
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
			= (imgsensor_info.normal_video.framelength << 16)
				+ imgsensor_info.normal_video.linelength;
			break;
		case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
			= (imgsensor_info.hs_video.framelength << 16)
				+ imgsensor_info.hs_video.linelength;
			break;
		case MSDK_SCENARIO_ID_SLIM_VIDEO:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
			= (imgsensor_info.slim_video.framelength << 16)
				+ imgsensor_info.slim_video.linelength;
			break;
		case MSDK_SCENARIO_ID_CUSTOM1:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
			= (imgsensor_info.custom1.framelength << 16)
				+ imgsensor_info.custom1.linelength;
			break;
		case MSDK_SCENARIO_ID_CUSTOM2:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
			= (imgsensor_info.custom2.framelength << 16)
				+ imgsensor_info.custom2.linelength;
			break;
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		default:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
			= (imgsensor_info.pre.framelength << 16)
				+ imgsensor_info.pre.linelength;
			break;
		}
		break;

	case SENSOR_FEATURE_GET_PERIOD:
		*feature_return_para_16++ = imgsensor.line_length;
		*feature_return_para_16 = imgsensor.frame_length;
		*feature_para_len = 4;
		break;
	case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
		*feature_return_para_32 = imgsensor.pclk;
		*feature_para_len = 4;
		break;
	case SENSOR_FEATURE_SET_ESHUTTER:
		 set_shutter(*feature_data);
		break;
	case SENSOR_FEATURE_SET_NIGHTMODE:
		 /* night_mode((BOOL) *feature_data); */
		break;
	#ifdef VENDOR_EDIT
	case SENSOR_FEATURE_CHECK_MODULE_ID:
		*feature_return_para_32 = imgsensor_info.module_id;
		break;
	#endif
	case SENSOR_FEATURE_SET_GAIN:
		set_gain((UINT16) *feature_data);
		break;
	case SENSOR_FEATURE_SET_FLASHLIGHT:
		break;
	case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
		break;
	case SENSOR_FEATURE_SET_REGISTER:
		write_cmos_sensor_8(sensor_reg_data->RegAddr,
				    sensor_reg_data->RegData);
		break;
	case SENSOR_FEATURE_GET_REGISTER:
		sensor_reg_data->RegData =
			read_cmos_sensor_8(sensor_reg_data->RegAddr);
		break;
	case SENSOR_FEATURE_GET_LENS_DRIVER_ID:
		/*get the lens driver ID from EEPROM
		 * or just return LENS_DRIVER_ID_DO_NOT_CARE
		 * if EEPROM does not exist in camera module.
		 */
		*feature_return_para_32 = LENS_DRIVER_ID_DO_NOT_CARE;
		*feature_para_len = 4;
		break;
	case SENSOR_FEATURE_SET_VIDEO_MODE:
		set_video_mode(*feature_data);
		break;
	case SENSOR_FEATURE_CHECK_SENSOR_ID:
		get_imgsensor_id(feature_return_para_32);
		break;
	case SENSOR_FEATURE_SET_AUTO_FLICKER_MODE:
		set_auto_flicker_mode((BOOL)*feature_data_16,
				      *(feature_data_16+1));
		break;
	case SENSOR_FEATURE_SET_MAX_FRAME_RATE_BY_SCENARIO:
		 set_max_framerate_by_scenario(
				(enum MSDK_SCENARIO_ID_ENUM)*feature_data,
				*(feature_data+1));
		break;
	case SENSOR_FEATURE_GET_DEFAULT_FRAME_RATE_BY_SCENARIO:
		 get_default_framerate_by_scenario(
				(enum MSDK_SCENARIO_ID_ENUM)*(feature_data),
				(MUINT32 *)(uintptr_t)(*(feature_data+1)));
		break;
	case SENSOR_FEATURE_GET_PDAF_DATA:
		LOG_INF("SENSOR_FEATURE_GET_PDAF_DATA\n");
		break;
	case SENSOR_FEATURE_SET_TEST_PATTERN:
		set_test_pattern_mode((BOOL)*feature_data);
		break;
	case SENSOR_FEATURE_GET_TEST_PATTERN_CHECKSUM_VALUE:
		/* for factory mode auto testing */
		*feature_return_para_32 = imgsensor_info.checksum_value;
		*feature_para_len = 4;
		break;
	case SENSOR_FEATURE_SET_FRAMERATE:
		LOG_INF("current fps :%d\n", (UINT32)*feature_data_32);
		spin_lock(&imgsensor_drv_lock);
		imgsensor.current_fps = *feature_data_32;
		spin_unlock(&imgsensor_drv_lock);
		break;
	case SENSOR_FEATURE_SET_HDR:
		LOG_INF("ihdr enable :%d\n", (BOOL)*feature_data_32);
		spin_lock(&imgsensor_drv_lock);
		imgsensor.ihdr_mode = *feature_data_32;
		spin_unlock(&imgsensor_drv_lock);
		break;
	case SENSOR_FEATURE_GET_CROP_INFO:
		LOG_INF("SENSOR_FEATURE_GET_CROP_INFO scenarioId:%d\n",
			(UINT32)*feature_data);
		wininfo =
	(struct SENSOR_WINSIZE_INFO_STRUCT *)(uintptr_t)(*(feature_data+1));

		switch (*feature_data_32) {
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
			memcpy((void *)wininfo,
				(void *)&imgsensor_winsize_info[1],
				sizeof(struct SENSOR_WINSIZE_INFO_STRUCT));
			break;
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			memcpy((void *)wininfo,
				(void *)&imgsensor_winsize_info[2],
				sizeof(struct SENSOR_WINSIZE_INFO_STRUCT));
			break;
		case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
			memcpy((void *)wininfo,
			(void *)&imgsensor_winsize_info[3],
			sizeof(struct SENSOR_WINSIZE_INFO_STRUCT));
			break;
		case MSDK_SCENARIO_ID_SLIM_VIDEO:
			memcpy((void *)wininfo,
			(void *)&imgsensor_winsize_info[4],
			sizeof(struct SENSOR_WINSIZE_INFO_STRUCT));
			break;
		case MSDK_SCENARIO_ID_CUSTOM1:
			memcpy((void *)wininfo,
			(void *)&imgsensor_winsize_info[5],
			sizeof(struct SENSOR_WINSIZE_INFO_STRUCT));
			break;
		case MSDK_SCENARIO_ID_CUSTOM2:
			memcpy((void *)wininfo,
			(void *)&imgsensor_winsize_info[6],
			sizeof(struct SENSOR_WINSIZE_INFO_STRUCT));
			break;
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		default:
			memcpy((void *)wininfo,
			(void *)&imgsensor_winsize_info[0],
			sizeof(struct SENSOR_WINSIZE_INFO_STRUCT));
			break;
		}
		break;
	case SENSOR_FEATURE_GET_PDAF_INFO:
		LOG_INF("SENSOR_FEATURE_GET_PDAF_INFO scenarioId:%d\n",
			(UINT16) *feature_data);
		PDAFinfo =
		  (struct SET_PD_BLOCK_INFO_T *)(uintptr_t)(*(feature_data+1));
		switch (*feature_data) {
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
			memcpy((void *)PDAFinfo, (void *)&imgsensor_pd_info,
				 sizeof(struct SET_PD_BLOCK_INFO_T));
			break;
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
			memcpy((void *)PDAFinfo,
				(void *)&imgsensor_pd_info,
				sizeof(struct SET_PD_BLOCK_INFO_T));
			break;
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			memcpy((void *)PDAFinfo,
				(void *)&imgsensor_pd_info_video,
				sizeof(struct SET_PD_BLOCK_INFO_T));
			break;
		case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
		case MSDK_SCENARIO_ID_SLIM_VIDEO:

		default:
			break;
		}
		break;
	case SENSOR_FEATURE_GET_PDAF_REG_SETTING:
		LOG_INF("SENSOR_FEATURE_GET_PDAF_REG_SETTING %d",
			(*feature_para_len));
		imx582_get_pdaf_reg_setting((*feature_para_len) / sizeof(UINT32)
					   , feature_data_16);
		break;
	case SENSOR_FEATURE_SET_PDAF_REG_SETTING:
		LOG_INF("SENSOR_FEATURE_SET_PDAF_REG_SETTING %d",
			(*feature_para_len));
		imx582_set_pdaf_reg_setting((*feature_para_len) / sizeof(UINT32)
					   , feature_data_16);
		break;
	case SENSOR_FEATURE_SET_PDAF:
		LOG_INF("PDAF mode :%d\n", *feature_data_16);
		imgsensor.pdaf_mode = *feature_data_16;
		break;
	case SENSOR_FEATURE_SET_IHDR_SHUTTER_GAIN:
		LOG_INF("SENSOR_SET_SENSOR_IHDR LE=%d, SE=%d, Gain=%d\n",
			(UINT16)*feature_data,
			(UINT16)*(feature_data+1),
			(UINT16)*(feature_data+2));
		break;
	case SENSOR_FEATURE_SET_SHUTTER_FRAME_TIME:
		set_shutter_frame_length((UINT16) (*feature_data),
					(UINT16) (*(feature_data + 1)));
		break;
	case SENSOR_FEATURE_SET_AWB_GAIN:
		break;
	case SENSOR_FEATURE_SET_HDR_SHUTTER:
		LOG_INF("SENSOR_FEATURE_SET_HDR_SHUTTER LE=%d, SE=%d\n",
			(UINT16)*feature_data, (UINT16)*(feature_data+1));
		#if 0
		ihdr_write_shutter((UINT16)*feature_data,
				   (UINT16)*(feature_data+1));
		#endif
		break;
	case SENSOR_FEATURE_SET_STREAMING_SUSPEND:
		LOG_INF("SENSOR_FEATURE_SET_STREAMING_SUSPEND\n");
		streaming_control(KAL_FALSE);
		break;
	case SENSOR_FEATURE_SET_STREAMING_RESUME:
		LOG_INF("SENSOR_FEATURE_SET_STREAMING_RESUME, shutter:%llu\n",
			*feature_data);
		if (*feature_data != 0)
			set_shutter(*feature_data);
		streaming_control(KAL_TRUE);
		break;
	case SENSOR_FEATURE_GET_BINNING_TYPE:
		switch (*(feature_data + 1)) {
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
			*feature_return_para_32 = 1; /*BINNING_NONE*/
			break;
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
		case MSDK_SCENARIO_ID_SLIM_VIDEO:
			*feature_return_para_32 = 4; /*BINNING_SUMMED*/
			break;
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		default:
			*feature_return_para_32 = 2; /*BINNING_AVERAGED*/
			break;
		}
		pr_debug("SENSOR_FEATURE_GET_BINNING_TYPE AE_binning_type:%d,\n",
			*feature_return_para_32);
		*feature_para_len = 4;

		break;
	case SENSOR_FEATURE_GET_MIPI_PIXEL_RATE:
	{
		switch (*feature_data) {
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
				= imgsensor_info.cap.mipi_pixel_rate;
			break;
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
				= imgsensor_info.normal_video.mipi_pixel_rate;
			break;
		case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
				= imgsensor_info.hs_video.mipi_pixel_rate;
			break;
		case MSDK_SCENARIO_ID_SLIM_VIDEO:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
				= imgsensor_info.slim_video.mipi_pixel_rate;
			break;
		case MSDK_SCENARIO_ID_CUSTOM1:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
				= imgsensor_info.custom1.mipi_pixel_rate;
			break;
		case MSDK_SCENARIO_ID_CUSTOM2:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
				= imgsensor_info.custom2.mipi_pixel_rate;
			break;
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		default:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1))
				= imgsensor_info.pre.mipi_pixel_rate;
			break;
		}
	}
break;

	case SENSOR_FEATURE_GET_VC_INFO:
		LOG_INF("SENSOR_FEATURE_GET_VC_INFO %d\n",
			(UINT16)*feature_data);
		pvcinfo =
		 (struct SENSOR_VC_INFO_STRUCT *)(uintptr_t)(*(feature_data+1));
		switch (*feature_data_32) {
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
			memcpy((void *)pvcinfo, (void *)&SENSOR_VC_INFO[1],
				sizeof(struct SENSOR_VC_INFO_STRUCT));
			break;
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			memcpy((void *)pvcinfo, (void *)&SENSOR_VC_INFO[2],
				sizeof(struct SENSOR_VC_INFO_STRUCT));
			break;
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
			memcpy((void *)pvcinfo, (void *)&SENSOR_VC_INFO[0],
				sizeof(struct SENSOR_VC_INFO_STRUCT));
			break;
		case MSDK_SCENARIO_ID_CUSTOM1:
			memcpy((void *)pvcinfo, (void *)&SENSOR_VC_INFO[1],
				sizeof(struct SENSOR_VC_INFO_STRUCT));
			break;
		case MSDK_SCENARIO_ID_CUSTOM2:
			memcpy((void *)pvcinfo, (void *)&SENSOR_VC_INFO[1],
				sizeof(struct SENSOR_VC_INFO_STRUCT));
			break;
		default:
			#if 0
			memcpy((void *)pvcinfo, (void *)&SENSOR_VC_INFO[0],
				sizeof(struct SENSOR_VC_INFO_STRUCT));
			#endif
			break;
		}
	default:
		break;
	}

	return ERROR_NONE;
} /* feature_control() */

static struct SENSOR_FUNCTION_STRUCT sensor_func = {
	open,
	get_info,
	get_resolution,
	feature_control,
	control,
	close
};

UINT32 IMX582_MIPI_RAW_SensorInit(struct SENSOR_FUNCTION_STRUCT **pfFunc)
{
	/* To Do : Check Sensor status here */
	if (pfFunc != NULL)
		*pfFunc = &sensor_func;
	return ERROR_NONE;
} /* IMX582_MIPI_RAW_SensorInit */

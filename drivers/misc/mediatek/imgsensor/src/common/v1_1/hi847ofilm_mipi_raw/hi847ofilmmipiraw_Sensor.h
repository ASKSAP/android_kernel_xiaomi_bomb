/*
 * Copyright (C) 2018 XiaoMi, Inc.
 * Copyright (C) 2020 XiaoMi, Inc.
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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *	 hi847ofilmmipi_Sensor.h
 *
 * Project:
 * --------
 *	 ALPS
 *
 * Description:
 * ------------
 *	 CMOS sensor header file
 *
 ****************************************************************************/
#ifndef _HI847OFILMMIPI_SENSOR_H
#define _HI847OFILMMIPI_SENSOR_H

#include "kd_camera_typedef.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"

#define MULTI_WRITE 1

#if MULTI_WRITE
kal_uint16 addr_data_pair_init_hi847ofilm[] = {
	0x0790, 0x0100,
	0x2000, 0x0000,
	0x2002, 0x0058,
	0x2006, 0x40B2,
	0x2008, 0xB05C,
	0x200A, 0x8446,
	0x200C, 0x40B2,
	0x200E, 0xB082,
	0x2010, 0x8450,
	0x2012, 0x40B2,
	0x2014, 0xB0A0,
	0x2016, 0x84C6,
	0x2018, 0x40B2,
	0x201A, 0xB10C,
	0x201C, 0x84BC,
	0x201E, 0x40B2,
	0x2020, 0xB33C,
	0x2022, 0x84B4,
	0x2024, 0x40B2,
	0x2026, 0xB378,
	0x2028, 0x84B0,
	0x202A, 0x40B2,
	0x202C, 0xB3A6,
	0x202E, 0x84B8,
	0x2030, 0x40B2,
	0x2032, 0xB0E6,
	0x2034, 0x8470,
	0x2036, 0x40B2,
	0x2038, 0xB3DC,
	0x203A, 0x847C,
	0x203C, 0x40B2,
	0x203E, 0xB64A,
	0x2040, 0x8478,
	0x2042, 0x40B2,
	0x2044, 0xB670,
	0x2046, 0x847E,
	0x2048, 0x40B2,
	0x204A, 0xB780,
	0x204C, 0x843A,
	0x204E, 0x40B2,
	0x2050, 0xB972,
	0x2052, 0x845C,
	0x2054, 0x40B2,
	0x2056, 0xB9A2,
	0x2058, 0x845E,
	0x205A, 0x4130,
	0x205C, 0x1292,
	0x205E, 0xD016,
	0x2060, 0xB3D2,
	0x2062, 0x0B00,
	0x2064, 0x2002,
	0x2066, 0xD2E2,
	0x2068, 0x0381,
	0x206A, 0x93C2,
	0x206C, 0x0263,
	0x206E, 0x2001,
	0x2070, 0x4130,
	0x2072, 0x422D,
	0x2074, 0x403E,
	0x2076, 0x888E,
	0x2078, 0x403F,
	0x207A, 0x192A,
	0x207C, 0x1292,
	0x207E, 0x843E,
	0x2080, 0x3FF7,
	0x2082, 0xB3D2,
	0x2084, 0x0267,
	0x2086, 0x2403,
	0x2088, 0xD0F2,
	0x208A, 0x0040,
	0x208C, 0x0381,
	0x208E, 0x90F2,
	0x2090, 0x0010,
	0x2092, 0x0260,
	0x2094, 0x2002,
	0x2096, 0x1292,
	0x2098, 0x84BC,
	0x209A, 0x1292,
	0x209C, 0xD020,
	0x209E, 0x4130,
	0x20A0, 0x1292,
	0x20A2, 0x8470,
	0x20A4, 0x1292,
	0x20A6, 0x8452,
	0x20A8, 0x0900,
	0x20AA, 0x7118,
	0x20AC, 0x1292,
	0x20AE, 0x848E,
	0x20B0, 0x0900,
	0x20B2, 0x7112,
	0x20B4, 0x0800,
	0x20B6, 0x7A20,
	0x20B8, 0x4292,
	0x20BA, 0x87DE,
	0x20BC, 0x7334,
	0x20BE, 0x0F00,
	0x20C0, 0x7304,
	0x20C2, 0x421F,
	0x20C4, 0x8718,
	0x20C6, 0x1292,
	0x20C8, 0x846E,
	0x20CA, 0x1292,
	0x20CC, 0x8488,
	0x20CE, 0x0B00,
	0x20D0, 0x7114,
	0x20D2, 0x0002,
	0x20D4, 0x1292,
	0x20D6, 0x848C,
	0x20D8, 0x1292,
	0x20DA, 0x8454,
	0x20DC, 0x43C2,
	0x20DE, 0x86EE,
	0x20E0, 0x1292,
	0x20E2, 0x8444,
	0x20E4, 0x4130,
	0x20E6, 0x4392,
	0x20E8, 0x7360,
	0x20EA, 0xB3D2,
	0x20EC, 0x0B00,
	0x20EE, 0x2402,
	0x20F0, 0xC2E2,
	0x20F2, 0x0381,
	0x20F4, 0x0900,
	0x20F6, 0x732C,
	0x20F8, 0x4382,
	0x20FA, 0x7360,
	0x20FC, 0x422D,
	0x20FE, 0x403E,
	0x2100, 0x87F0,
	0x2102, 0x403F,
	0x2104, 0x87E8,
	0x2106, 0x1292,
	0x2108, 0x843E,
	0x210A, 0x4130,
	0x210C, 0x120B,
	0x210E, 0x120A,
	0x2110, 0x4392,
	0x2112, 0x87FA,
	0x2114, 0x4392,
	0x2116, 0x760E,
	0x2118, 0x0900,
	0x211A, 0x760C,
	0x211C, 0x421B,
	0x211E, 0x760A,
	0x2120, 0x903B,
	0x2122, 0x0201,
	0x2124, 0x2408,
	0x2126, 0x903B,
	0x2128, 0x0102,
	0x212A, 0x2405,
	0x212C, 0x4292,
	0x212E, 0x030A,
	0x2130, 0x87F8,
	0x2132, 0x1292,
	0x2134, 0x849A,
	0x2136, 0x903B,
	0x2138, 0x0020,
	0x213A, 0x2010,
	0x213C, 0x403B,
	0x213E, 0x8498,
	0x2140, 0x422F,
	0x2142, 0x12AB,
	0x2144, 0x403F,
	0x2146, 0x0028,
	0x2148, 0x12AB,
	0x214A, 0x403B,
	0x214C, 0x84C4,
	0x214E, 0x407F,
	0x2150, 0xFFAA,
	0x2152, 0x12AB,
	0x2154, 0x407F,
	0x2156, 0x0055,
	0x2158, 0x12AB,
	0x215A, 0x3FDC,
	0x215C, 0x903B,
	0x215E, 0x0021,
	0x2160, 0x2890,
	0x2162, 0x903B,
	0x2164, 0x0100,
	0x2166, 0x200D,
	0x2168, 0x403F,
	0x216A, 0x0028,
	0x216C, 0x1292,
	0x216E, 0x8498,
	0x2170, 0x425F,
	0x2172, 0x0306,
	0x2174, 0x1292,
	0x2176, 0x84C4,
	0x2178, 0x4FC2,
	0x217A, 0x0318,
	0x217C, 0x0261,
	0x217E, 0x0000,
	0x2180, 0x3FC9,
	0x2182, 0x903B,
	0x2184, 0x0101,
	0x2186, 0x2858,
	0x2188, 0x903B,
	0x218A, 0x0200,
	0x218C, 0x2450,
	0x218E, 0x903B,
	0x2190, 0x0201,
	0x2192, 0x2C47,
	0x2194, 0x903B,
	0x2196, 0x0102,
	0x2198, 0x2041,
	0x219A, 0x93E2,
	0x219C, 0x0262,
	0x219E, 0x240A,
	0x21A0, 0x425F,
	0x21A2, 0x0306,
	0x21A4, 0x1292,
	0x21A6, 0x84C4,
	0x21A8, 0x4F4E,
	0x21AA, 0x4EC2,
	0x21AC, 0x0318,
	0x21AE, 0x0260,
	0x21B0, 0x0000,
	0x21B2, 0x3FB0,
	0x21B4, 0x403A,
	0x21B6, 0x8030,
	0x21B8, 0x4382,
	0x21BA, 0x0326,
	0x21BC, 0x4382,
	0x21BE, 0x0328,
	0x21C0, 0x421B,
	0x21C2, 0x030C,
	0x21C4, 0x930B,
	0x21C6, 0x2420,
	0x21C8, 0x4A5F,
	0x21CA, 0x0001,
	0x21CC, 0x1292,
	0x21CE, 0x84C4,
	0x21D0, 0x4F4E,
	0x21D2, 0x4A5F,
	0x21D4, 0x0001,
	0x21D6, 0x9F0E,
	0x21D8, 0x2402,
	0x21DA, 0x5392,
	0x21DC, 0x0326,
	0x21DE, 0x4ECA,
	0x21E0, 0x0001,
	0x21E2, 0x533B,
	0x21E4, 0x2411,
	0x21E6, 0x4A6F,
	0x21E8, 0x1292,
	0x21EA, 0x84C4,
	0x21EC, 0x4F4E,
	0x21EE, 0x4A6F,
	0x21F0, 0x9F0E,
	0x21F2, 0x2402,
	0x21F4, 0x5392,
	0x21F6, 0x0326,
	0x21F8, 0x4ECA,
	0x21FA, 0x0000,
	0x21FC, 0x533B,
	0x21FE, 0x532A,
	0x2200, 0x0260,
	0x2202, 0x0000,
	0x2204, 0x930B,
	0x2206, 0x23E0,
	0x2208, 0x40B2,
	0x220A, 0xAA55,
	0x220C, 0x0328,
	0x220E, 0xB0F2,
	0x2210, 0x0040,
	0x2212, 0x0381,
	0x2214, 0x277F,
	0x2216, 0xD3D2,
	0x2218, 0x0267,
	0x221A, 0x3F7C,
	0x221C, 0x0261,
	0x221E, 0x0000,
	0x2220, 0x3F79,
	0x2222, 0x903B,
	0x2224, 0x0201,
	0x2226, 0x23FA,
	0x2228, 0x1292,
	0x222A, 0x84C0,
	0x222C, 0x3F73,
	0x222E, 0x1292,
	0x2230, 0x84C0,
	0x2232, 0x0261,
	0x2234, 0x0000,
	0x2236, 0x3F6E,
	0x2238, 0x903B,
	0x223A, 0x0040,
	0x223C, 0x2018,
	0x223E, 0x422F,
	0x2240, 0x1292,
	0x2242, 0x8498,
	0x2244, 0x12B0,
	0x2246, 0xF0EA,
	0x2248, 0x907F,
	0x224A, 0xFFAA,
	0x224C, 0x240D,
	0x224E, 0x5392,
	0x2250, 0x0312,
	0x2252, 0x12B0,
	0x2254, 0xF0EA,
	0x2256, 0x907F,
	0x2258, 0x0055,
	0x225A, 0x2403,
	0x225C, 0x5392,
	0x225E, 0x0312,
	0x2260, 0x3F59,
	0x2262, 0x5392,
	0x2264, 0x0310,
	0x2266, 0x3F56,
	0x2268, 0x5392,
	0x226A, 0x0310,
	0x226C, 0x3FF2,
	0x226E, 0x903B,
	0x2270, 0x0080,
	0x2272, 0x23D4,
	0x2274, 0x4382,
	0x2276, 0x0312,
	0x2278, 0x4382,
	0x227A, 0x0310,
	0x227C, 0x0261,
	0x227E, 0x0000,
	0x2280, 0x3F49,
	0x2282, 0x932B,
	0x2284, 0x2005,
	0x2286, 0x403F,
	0x2288, 0x0028,
	0x228A, 0x1292,
	0x228C, 0x8498,
	0x228E, 0x3F42,
	0x2290, 0x903B,
	0x2292, 0x0003,
	0x2294, 0x284B,
	0x2296, 0x923B,
	0x2298, 0x2015,
	0x229A, 0x403F,
	0x229C, 0x0023,
	0x229E, 0x1292,
	0x22A0, 0x8498,
	0x22A2, 0x421B,
	0x22A4, 0x87F8,
	0x22A6, 0x421F,
	0x22A8, 0x030C,
	0x22AA, 0x9F0B,
	0x22AC, 0x2F33,
	0x22AE, 0x1292,
	0x22B0, 0x84BA,
	0x22B2, 0x930F,
	0x22B4, 0x2004,
	0x22B6, 0x5392,
	0x22B8, 0x0312,
	0x22BA, 0x531B,
	0x22BC, 0x3FF4,
	0x22BE, 0x5392,
	0x22C0, 0x0310,
	0x22C2, 0x3FFB,
	0x22C4, 0x903B,
	0x22C6, 0x0009,
	0x22C8, 0x2818,
	0x22CA, 0x903B,
	0x22CC, 0x0010,
	0x22CE, 0x23A6,
	0x22D0, 0x403F,
	0x22D2, 0x0027,
	0x22D4, 0x1292,
	0x22D6, 0x8498,
	0x22D8, 0x421B,
	0x22DA, 0x87F8,
	0x22DC, 0x421F,
	0x22DE, 0x030C,
	0x22E0, 0x9F0B,
	0x22E2, 0x2F18,
	0x22E4, 0x1292,
	0x22E6, 0x84BA,
	0x22E8, 0x930F,
	0x22EA, 0x2004,
	0x22EC, 0x5392,
	0x22EE, 0x0312,
	0x22F0, 0x531B,
	0x22F2, 0x3FF4,
	0x22F4, 0x5392,
	0x22F6, 0x0310,
	0x22F8, 0x3FFB,
	0x22FA, 0x922B,
	0x22FC, 0x238F,
	0x22FE, 0x421B,
	0x2300, 0x87F8,
	0x2302, 0x421F,
	0x2304, 0x030C,
	0x2306, 0x9F0B,
	0x2308, 0x2C0B,
	0x230A, 0x1292,
	0x230C, 0x84C2,
	0x230E, 0x934F,
	0x2310, 0x240A,
	0x2312, 0x5392,
	0x2314, 0x0312,
	0x2316, 0x531B,
	0x2318, 0x421F,
	0x231A, 0x030C,
	0x231C, 0x9F0B,
	0x231E, 0x2BF5,
	0x2320, 0x0261,
	0x2322, 0x0000,
	0x2324, 0x3EF7,
	0x2326, 0x5392,
	0x2328, 0x0310,
	0x232A, 0x3FF5,
	0x232C, 0x930B,
	0x232E, 0x277F,
	0x2330, 0x931B,
	0x2332, 0x277A,
	0x2334, 0x3F73,
	0x2336, 0x413A,
	0x2338, 0x413B,
	0x233A, 0x4130,
	0x233C, 0x4F0C,
	0x233E, 0x403F,
	0x2340, 0x0267,
	0x2342, 0xF0FF,
	0x2344, 0xFFDF,
	0x2346, 0x0000,
	0x2348, 0xF0FF,
	0x234A, 0xFFEF,
	0x234C, 0x0000,
	0x234E, 0x421D,
	0x2350, 0x84B0,
	0x2352, 0x403E,
	0x2354, 0x06F9,
	0x2356, 0x4C0F,
	0x2358, 0x1292,
	0x235A, 0x84AC,
	0x235C, 0x4F4E,
	0x235E, 0xB31E,
	0x2360, 0x2403,
	0x2362, 0xD0F2,
	0x2364, 0x0020,
	0x2366, 0x0267,
	0x2368, 0xB32E,
	0x236A, 0x2403,
	0x236C, 0xD0F2,
	0x236E, 0x0010,
	0x2370, 0x0267,
	0x2372, 0xC3E2,
	0x2374, 0x0267,
	0x2376, 0x4130,
	0x2378, 0x120B,
	0x237A, 0x120A,
	0x237C, 0x403A,
	0x237E, 0x1140,
	0x2380, 0x1292,
	0x2382, 0xD080,
	0x2384, 0x430B,
	0x2386, 0x4A0F,
	0x2388, 0x532A,
	0x238A, 0x1292,
	0x238C, 0x84A4,
	0x238E, 0x4F0E,
	0x2390, 0x430F,
	0x2392, 0x5E82,
	0x2394, 0x87FC,
	0x2396, 0x6F82,
	0x2398, 0x87FE,
	0x239A, 0x531B,
	0x239C, 0x923B,
	0x239E, 0x2BF3,
	0x23A0, 0x413A,
	0x23A2, 0x413B,
	0x23A4, 0x4130,
	0x23A6, 0xF0F2,
	0x23A8, 0x007F,
	0x23AA, 0x0267,
	0x23AC, 0x421D,
	0x23AE, 0x84B6,
	0x23B0, 0x403E,
	0x23B2, 0x01F9,
	0x23B4, 0x1292,
	0x23B6, 0x84AC,
	0x23B8, 0x4F4E,
	0x23BA, 0xF35F,
	0x23BC, 0x2403,
	0x23BE, 0xD0F2,
	0x23C0, 0xFF80,
	0x23C2, 0x0267,
	0x23C4, 0xB36E,
	0x23C6, 0x2404,
	0x23C8, 0xD0F2,
	0x23CA, 0x0040,
	0x23CC, 0x0267,
	0x23CE, 0x3C03,
	0x23D0, 0xF0F2,
	0x23D2, 0xFFBF,
	0x23D4, 0x0267,
	0x23D6, 0xC2E2,
	0x23D8, 0x0267,
	0x23DA, 0x4130,
	0x23DC, 0x120B,
	0x23DE, 0x120A,
	0x23E0, 0x8231,
	0x23E2, 0x430B,
	0x23E4, 0x93C2,
	0x23E6, 0x0C0A,
	0x23E8, 0x2404,
	0x23EA, 0xB3D2,
	0x23EC, 0x0B05,
	0x23EE, 0x2401,
	0x23F0, 0x431B,
	0x23F2, 0x422D,
	0x23F4, 0x403E,
	0x23F6, 0x192A,
	0x23F8, 0x403F,
	0x23FA, 0x888E,
	0x23FC, 0x1292,
	0x23FE, 0x843E,
	0x2400, 0x930B,
	0x2402, 0x20F4,
	0x2404, 0x93E2,
	0x2406, 0x0241,
	0x2408, 0x24EB,
	0x240A, 0x403A,
	0x240C, 0x0292,
	0x240E, 0x4AA2,
	0x2410, 0x0A00,
	0x2412, 0xB2E2,
	0x2414, 0x0361,
	0x2416, 0x2405,
	0x2418, 0x4A2F,
	0x241A, 0x1292,
	0x241C, 0x8474,
	0x241E, 0x4F82,
	0x2420, 0x0A1C,
	0x2422, 0x93C2,
	0x2424, 0x0360,
	0x2426, 0x34CD,
	0x2428, 0x430C,
	0x242A, 0x4C0F,
	0x242C, 0x5F0F,
	0x242E, 0x4F0D,
	0x2430, 0x510D,
	0x2432, 0x4F0E,
	0x2434, 0x5A0E,
	0x2436, 0x4E1E,
	0x2438, 0x0002,
	0x243A, 0x4F1F,
	0x243C, 0x192A,
	0x243E, 0x1202,
	0x2440, 0xC232,
	0x2442, 0x4303,
	0x2444, 0x4E82,
	0x2446, 0x0130,
	0x2448, 0x4F82,
	0x244A, 0x0138,
	0x244C, 0x421E,
	0x244E, 0x013A,
	0x2450, 0x421F,
	0x2452, 0x013C,
	0x2454, 0x4132,
	0x2456, 0x108E,
	0x2458, 0x108F,
	0x245A, 0xEF4E,
	0x245C, 0xEF0E,
	0x245E, 0xF37F,
	0x2460, 0xC312,
	0x2462, 0x100F,
	0x2464, 0x100E,
	0x2466, 0x4E8D,
	0x2468, 0x0000,
	0x246A, 0x531C,
	0x246C, 0x922C,
	0x246E, 0x2BDD,
	0x2470, 0xB3D2,
	0x2472, 0x1921,
	0x2474, 0x2403,
	0x2476, 0x410F,
	0x2478, 0x1292,
	0x247A, 0x847E,
	0x247C, 0x403B,
	0x247E, 0x843E,
	0x2480, 0x422D,
	0x2482, 0x410E,
	0x2484, 0x403F,
	0x2486, 0x1908,
	0x2488, 0x12AB,
	0x248A, 0x403D,
	0x248C, 0x0005,
	0x248E, 0x403E,
	0x2490, 0x0292,
	0x2492, 0x403F,
	0x2494, 0x86E4,
	0x2496, 0x12AB,
	0x2498, 0x421F,
	0x249A, 0x060E,
	0x249C, 0x9F82,
	0x249E, 0x8720,
	0x24A0, 0x288D,
	0x24A2, 0x9382,
	0x24A4, 0x060E,
	0x24A6, 0x248A,
	0x24A8, 0x90BA,
	0x24AA, 0x0010,
	0x24AC, 0x0000,
	0x24AE, 0x2C0B,
	0x24B0, 0x93C2,
	0x24B2, 0x86EE,
	0x24B4, 0x2008,
	0x24B6, 0x403F,
	0x24B8, 0x06A7,
	0x24BA, 0xD0FF,
	0x24BC, 0x0007,
	0x24BE, 0x0000,
	0x24C0, 0xF0FF,
	0x24C2, 0xFFF8,
	0x24C4, 0x0000,
	0x24C6, 0x4392,
	0x24C8, 0x8720,
	0x24CA, 0x403F,
	0x24CC, 0x06A7,
	0x24CE, 0xD2EF,
	0x24D0, 0x0000,
	0x24D2, 0xC2EF,
	0x24D4, 0x0000,
	0x24D6, 0x93C2,
	0x24D8, 0x87D3,
	0x24DA, 0x2068,
	0x24DC, 0xB0F2,
	0x24DE, 0x0040,
	0x24E0, 0x0B05,
	0x24E2, 0x2461,
	0x24E4, 0xD3D2,
	0x24E6, 0x0410,
	0x24E8, 0xB3E2,
	0x24EA, 0x0381,
	0x24EC, 0x2089,
	0x24EE, 0x90B2,
	0x24F0, 0x0030,
	0x24F2, 0x0A00,
	0x24F4, 0x2C52,
	0x24F6, 0x93C2,
	0x24F8, 0x86EE,
	0x24FA, 0x204F,
	0x24FC, 0x430E,
	0x24FE, 0x430C,
	0x2500, 0x4C0F,
	0x2502, 0x5F0F,
	0x2504, 0x5F0F,
	0x2506, 0x5F0F,
	0x2508, 0x4F1F,
	0x250A, 0x8668,
	0x250C, 0xF03F,
	0x250E, 0x07FF,
	0x2510, 0x903F,
	0x2512, 0x0400,
	0x2514, 0x343E,
	0x2516, 0x5F0E,
	0x2518, 0x531C,
	0x251A, 0x923C,
	0x251C, 0x2BF1,
	0x251E, 0x4E0F,
	0x2520, 0x930E,
	0x2522, 0x3834,
	0x2524, 0x110F,
	0x2526, 0x110F,
	0x2528, 0x110F,
	0x252A, 0x9382,
	0x252C, 0x86EE,
	0x252E, 0x2023,
	0x2530, 0x5F82,
	0x2532, 0x87D6,
	0x2534, 0x403B,
	0x2536, 0x87D6,
	0x2538, 0x4B2F,
	0x253A, 0x12B0,
	0x253C, 0xB616,
	0x253E, 0x4F8B,
	0x2540, 0x0000,
	0x2542, 0x430C,
	0x2544, 0x4C0D,
	0x2546, 0x5D0D,
	0x2548, 0x5D0D,
	0x254A, 0x5D0D,
	0x254C, 0x403A,
	0x254E, 0x87D8,
	0x2550, 0x421B,
	0x2552, 0x87D6,
	0x2554, 0x4B0F,
	0x2556, 0x8A2F,
	0x2558, 0x4F0E,
	0x255A, 0x4E0F,
	0x255C, 0x5F0F,
	0x255E, 0x7F0F,
	0x2560, 0xE33F,
	0x2562, 0x8E8D,
	0x2564, 0x8668,
	0x2566, 0x7F8D,
	0x2568, 0x866A,
	0x256A, 0x531C,
	0x256C, 0x923C,
	0x256E, 0x2BEA,
	0x2570, 0x4B8A,
	0x2572, 0x0000,
	0x2574, 0x3C45,
	0x2576, 0x9382,
	0x2578, 0x86F0,
	0x257A, 0x2005,
	0x257C, 0x4382,
	0x257E, 0x87D6,
	0x2580, 0x4382,
	0x2582, 0x87D8,
	0x2584, 0x3FD7,
	0x2586, 0x4F82,
	0x2588, 0x87D6,
	0x258A, 0x3FD4,
	0x258C, 0x503F,
	0x258E, 0x0007,
	0x2590, 0x3FC9,
	0x2592, 0x5F0E,
	0x2594, 0x503E,
	0x2596, 0xF800,
	0x2598, 0x3FBF,
	0x259A, 0x430F,
	0x259C, 0x12B0,
	0x259E, 0xB616,
	0x25A0, 0x4382,
	0x25A2, 0x87D6,
	0x25A4, 0x3C2D,
	0x25A6, 0xC3D2,
	0x25A8, 0x0410,
	0x25AA, 0x3F9E,
	0x25AC, 0x430D,
	0x25AE, 0x403E,
	0x25B0, 0x0050,
	0x25B2, 0x403F,
	0x25B4, 0x85C8,
	0x25B6, 0x1292,
	0x25B8, 0x844E,
	0x25BA, 0x3F90,
	0x25BC, 0x5392,
	0x25BE, 0x8720,
	0x25C0, 0x3F84,
	0x25C2, 0x403B,
	0x25C4, 0x843E,
	0x25C6, 0x4A0F,
	0x25C8, 0x532F,
	0x25CA, 0x422D,
	0x25CC, 0x4F0E,
	0x25CE, 0x403F,
	0x25D0, 0x0E08,
	0x25D2, 0x12AB,
	0x25D4, 0x422D,
	0x25D6, 0x403E,
	0x25D8, 0x192A,
	0x25DA, 0x410F,
	0x25DC, 0x12AB,
	0x25DE, 0x3F48,
	0x25E0, 0x93C2,
	0x25E2, 0x86EE,
	0x25E4, 0x2312,
	0x25E6, 0x403A,
	0x25E8, 0x86E4,
	0x25EA, 0x3F11,
	0x25EC, 0x403D,
	0x25EE, 0x0200,
	0x25F0, 0x422E,
	0x25F2, 0x403F,
	0x25F4, 0x192A,
	0x25F6, 0x1292,
	0x25F8, 0x844E,
	0x25FA, 0xC3D2,
	0x25FC, 0x1921,
	0x25FE, 0x3F02,
	0x2600, 0x422D,
	0x2602, 0x403E,
	0x2604, 0x888E,
	0x2606, 0x403F,
	0x2608, 0x192A,
	0x260A, 0x1292,
	0x260C, 0x843E,
	0x260E, 0x5231,
	0x2610, 0x413A,
	0x2612, 0x413B,
	0x2614, 0x4130,
	0x2616, 0x4382,
	0x2618, 0x052C,
	0x261A, 0x4F0D,
	0x261C, 0x930D,
	0x261E, 0x3402,
	0x2620, 0xE33D,
	0x2622, 0x531D,
	0x2624, 0xF03D,
	0x2626, 0x07F0,
	0x2628, 0x4D0E,
	0x262A, 0xC312,
	0x262C, 0x100E,
	0x262E, 0x110E,
	0x2630, 0x110E,
	0x2632, 0x110E,
	0x2634, 0x930F,
	0x2636, 0x3803,
	0x2638, 0x4EC2,
	0x263A, 0x052C,
	0x263C, 0x3C04,
	0x263E, 0x4EC2,
	0x2640, 0x052D,
	0x2642, 0xE33D,
	0x2644, 0x531D,
	0x2646, 0x4D0F,
	0x2648, 0x4130,
	0x264A, 0x1292,
	0x264C, 0xD048,
	0x264E, 0x93C2,
	0x2650, 0x86EE,
	0x2652, 0x200D,
	0x2654, 0xB0F2,
	0x2656, 0x0020,
	0x2658, 0x0381,
	0x265A, 0x2407,
	0x265C, 0x9292,
	0x265E, 0x8722,
	0x2660, 0x0384,
	0x2662, 0x2C03,
	0x2664, 0xD3D2,
	0x2666, 0x0649,
	0x2668, 0x4130,
	0x266A, 0xC3D2,
	0x266C, 0x0649,
	0x266E, 0x4130,
	0x2670, 0x120B,
	0x2672, 0x120A,
	0x2674, 0x1209,
	0x2676, 0x1208,
	0x2678, 0x1207,
	0x267A, 0x1206,
	0x267C, 0x1205,
	0x267E, 0x1204,
	0x2680, 0x8231,
	0x2682, 0x4F81,
	0x2684, 0x0000,
	0x2686, 0x4381,
	0x2688, 0x0002,
	0x268A, 0x4304,
	0x268C, 0x411C,
	0x268E, 0x0002,
	0x2690, 0x5C0C,
	0x2692, 0x4C0F,
	0x2694, 0x5F0F,
	0x2696, 0x5F0F,
	0x2698, 0x5F0F,
	0x269A, 0x5F0F,
	0x269C, 0x5F0F,
	0x269E, 0x503F,
	0x26A0, 0x1980,
	0x26A2, 0x440D,
	0x26A4, 0x5D0D,
	0x26A6, 0x4D0E,
	0x26A8, 0x5F0E,
	0x26AA, 0x4E2E,
	0x26AC, 0x4D05,
	0x26AE, 0x5505,
	0x26B0, 0x5F05,
	0x26B2, 0x4516,
	0x26B4, 0x0008,
	0x26B6, 0x4517,
	0x26B8, 0x000A,
	0x26BA, 0x460A,
	0x26BC, 0x470B,
	0x26BE, 0xF30A,
	0x26C0, 0xF32B,
	0x26C2, 0x4A81,
	0x26C4, 0x0004,
	0x26C6, 0x4B81,
	0x26C8, 0x0006,
	0x26CA, 0xB03E,
	0x26CC, 0x2000,
	0x26CE, 0x2404,
	0x26D0, 0xF03E,
	0x26D2, 0x1FFF,
	0x26D4, 0xE33E,
	0x26D6, 0x531E,
	0x26D8, 0xF317,
	0x26DA, 0x503E,
	0x26DC, 0x2000,
	0x26DE, 0x4E0F,
	0x26E0, 0x5F0F,
	0x26E2, 0x7F0F,
	0x26E4, 0xE33F,
	0x26E6, 0x512C,
	0x26E8, 0x4C28,
	0x26EA, 0x4309,
	0x26EC, 0x4E0A,
	0x26EE, 0x4F0B,
	0x26F0, 0x480C,
	0x26F2, 0x490D,
	0x26F4, 0x1202,
	0x26F6, 0xC232,
	0x26F8, 0x12B0,
	0x26FA, 0xFFC0,
	0x26FC, 0x4132,
	0x26FE, 0x108E,
	0x2700, 0x108F,
	0x2702, 0xEF4E,
	0x2704, 0xEF0E,
	0x2706, 0xF37F,
	0x2708, 0xC312,
	0x270A, 0x100F,
	0x270C, 0x100E,
	0x270E, 0x4E85,
	0x2710, 0x0018,
	0x2712, 0x4F85,
	0x2714, 0x001A,
	0x2716, 0x480A,
	0x2718, 0x490B,
	0x271A, 0x460C,
	0x271C, 0x470D,
	0x271E, 0x1202,
	0x2720, 0xC232,
	0x2722, 0x12B0,
	0x2724, 0xFFC0,
	0x2726, 0x4132,
	0x2728, 0x4E0C,
	0x272A, 0x4F0D,
	0x272C, 0x108C,
	0x272E, 0x108D,
	0x2730, 0xED4C,
	0x2732, 0xED0C,
	0x2734, 0xF37D,
	0x2736, 0xC312,
	0x2738, 0x100D,
	0x273A, 0x100C,
	0x273C, 0x411E,
	0x273E, 0x0004,
	0x2740, 0x411F,
	0x2742, 0x0006,
	0x2744, 0x5E0E,
	0x2746, 0x6F0F,
	0x2748, 0x5E0E,
	0x274A, 0x6F0F,
	0x274C, 0x5E0E,
	0x274E, 0x6F0F,
	0x2750, 0xDE0C,
	0x2752, 0xDF0D,
	0x2754, 0x4C85,
	0x2756, 0x002C,
	0x2758, 0x4D85,
	0x275A, 0x002E,
	0x275C, 0x5314,
	0x275E, 0x9224,
	0x2760, 0x2B95,
	0x2762, 0x5391,
	0x2764, 0x0002,
	0x2766, 0x92A1,
	0x2768, 0x0002,
	0x276A, 0x2B8F,
	0x276C, 0x5231,
	0x276E, 0x4134,
	0x2770, 0x4135,
	0x2772, 0x4136,
	0x2774, 0x4137,
	0x2776, 0x4138,
	0x2778, 0x4139,
	0x277A, 0x413A,
	0x277C, 0x413B,
	0x277E, 0x4130,
	0x2780, 0x120B,
	0x2782, 0x120A,
	0x2784, 0x1209,
	0x2786, 0x8031,
	0x2788, 0x000C,
	0x278A, 0x425F,
	0x278C, 0x0205,
	0x278E, 0xC312,
	0x2790, 0x104F,
	0x2792, 0x114F,
	0x2794, 0x114F,
	0x2796, 0x114F,
	0x2798, 0x114F,
	0x279A, 0x114F,
	0x279C, 0xF37F,
	0x279E, 0x4F0B,
	0x27A0, 0xF31B,
	0x27A2, 0x5B0B,
	0x27A4, 0x5B0B,
	0x27A6, 0x5B0B,
	0x27A8, 0x503B,
	0x27AA, 0xD194,
	0x27AC, 0x4219,
	0x27AE, 0x0508,
	0x27B0, 0xF039,
	0x27B2, 0x2000,
	0x27B4, 0x4F0A,
	0x27B6, 0xC312,
	0x27B8, 0x100A,
	0x27BA, 0xE31A,
	0x27BC, 0x421F,
	0x27BE, 0x87DE,
	0x27C0, 0x503F,
	0x27C2, 0xFF60,
	0x27C4, 0x903F,
	0x27C6, 0x00C8,
	0x27C8, 0x2C02,
	0x27CA, 0x403F,
	0x27CC, 0x00C8,
	0x27CE, 0x4F82,
	0x27D0, 0x7322,
	0x27D2, 0xB3D2,
	0x27D4, 0x0381,
	0x27D6, 0x2009,
	0x27D8, 0x421F,
	0x27DA, 0x86F0,
	0x27DC, 0xD21F,
	0x27DE, 0x86EE,
	0x27E0, 0x930F,
	0x27E2, 0x24B9,
	0x27E4, 0x40F2,
	0x27E6, 0xFF80,
	0x27E8, 0x0619,
	0x27EA, 0x1292,
	0x27EC, 0xD00A,
	0x27EE, 0xB3D2,
	0x27F0, 0x0385,
	0x27F2, 0x2405,
	0x27F4, 0x421F,
	0x27F6, 0x880A,
	0x27F8, 0x4F92,
	0x27FA, 0x0002,
	0x27FC, 0x8714,
	0x27FE, 0x430D,
	0x2800, 0x93C2,
	0x2802, 0x87D0,
	0x2804, 0x2003,
	0x2806, 0xB2F2,
	0x2808, 0x0360,
	0x280A, 0x2001,
	0x280C, 0x431D,
	0x280E, 0x425F,
	0x2810, 0x87D3,
	0x2812, 0xD25F,
	0x2814, 0x87D2,
	0x2816, 0xF37F,
	0x2818, 0x5F0F,
	0x281A, 0x425E,
	0x281C, 0x87CD,
	0x281E, 0xDE0F,
	0x2820, 0x5F0F,
	0x2822, 0x5B0F,
	0x2824, 0x4FA2,
	0x2826, 0x0402,
	0x2828, 0x930D,
	0x282A, 0x2007,
	0x282C, 0x930A,
	0x282E, 0x248E,
	0x2830, 0x4F5F,
	0x2832, 0x0001,
	0x2834, 0xF37F,
	0x2836, 0x4FC2,
	0x2838, 0x0403,
	0x283A, 0x93C2,
	0x283C, 0x87CD,
	0x283E, 0x2483,
	0x2840, 0xC2F2,
	0x2842, 0x0400,
	0x2844, 0xB2E2,
	0x2846, 0x0265,
	0x2848, 0x2407,
	0x284A, 0x421F,
	0x284C, 0x0508,
	0x284E, 0xF03F,
	0x2850, 0xFFDF,
	0x2852, 0xD90F,
	0x2854, 0x4F82,
	0x2856, 0x0508,
	0x2858, 0xB3D2,
	0x285A, 0x0383,
	0x285C, 0x2484,
	0x285E, 0x403F,
	0x2860, 0x0508,
	0x2862, 0x4FB1,
	0x2864, 0x0000,
	0x2866, 0x4FB1,
	0x2868, 0x0002,
	0x286A, 0x4FB1,
	0x286C, 0x0004,
	0x286E, 0x403F,
	0x2870, 0x0500,
	0x2872, 0x4FB1,
	0x2874, 0x0006,
	0x2876, 0x4FB1,
	0x2878, 0x0008,
	0x287A, 0x4FB1,
	0x287C, 0x000A,
	0x287E, 0xB3E2,
	0x2880, 0x0383,
	0x2882, 0x2412,
	0x2884, 0xC2E1,
	0x2886, 0x0002,
	0x2888, 0xB2E2,
	0x288A, 0x0383,
	0x288C, 0x434F,
	0x288E, 0x634F,
	0x2890, 0xF37F,
	0x2892, 0x4F4E,
	0x2894, 0x114E,
	0x2896, 0x434E,
	0x2898, 0x104E,
	0x289A, 0x415F,
	0x289C, 0x0007,
	0x289E, 0xF07F,
	0x28A0, 0x007F,
	0x28A2, 0xDE4F,
	0x28A4, 0x4FC1,
	0x28A6, 0x0007,
	0x28A8, 0xB2F2,
	0x28AA, 0x0383,
	0x28AC, 0x2415,
	0x28AE, 0xF0F1,
	0x28B0, 0xFFBF,
	0x28B2, 0x0000,
	0x28B4, 0xB0F2,
	0x28B6, 0x0010,
	0x28B8, 0x0383,
	0x28BA, 0x434E,
	0x28BC, 0x634E,
	0x28BE, 0x5E4E,
	0x28C0, 0x5E4E,
	0x28C2, 0x5E4E,
	0x28C4, 0x5E4E,
	0x28C6, 0x5E4E,
	0x28C8, 0x5E4E,
	0x28CA, 0x415F,
	0x28CC, 0x0006,
	0x28CE, 0xF07F,
	0x28D0, 0xFFBF,
	0x28D2, 0xDE4F,
	0x28D4, 0x4FC1,
	0x28D6, 0x0006,
	0x28D8, 0xB0F2,
	0x28DA, 0x0020,
	0x28DC, 0x0383,
	0x28DE, 0x2410,
	0x28E0, 0xF0F1,
	0x28E2, 0xFFDF,
	0x28E4, 0x0002,
	0x28E6, 0xB0F2,
	0x28E8, 0x0040,
	0x28EA, 0x0383,
	0x28EC, 0x434E,
	0x28EE, 0x634E,
	0x28F0, 0x5E4E,
	0x28F2, 0x5E4E,
	0x28F4, 0x415F,
	0x28F6, 0x0008,
	0x28F8, 0xC26F,
	0x28FA, 0xDE4F,
	0x28FC, 0x4FC1,
	0x28FE, 0x0008,
	0x2900, 0x93C2,
	0x2902, 0x0383,
	0x2904, 0x3412,
	0x2906, 0xF0F1,
	0x2908, 0xFFDF,
	0x290A, 0x0000,
	0x290C, 0x425E,
	0x290E, 0x0382,
	0x2910, 0xF35E,
	0x2912, 0x5E4E,
	0x2914, 0x5E4E,
	0x2916, 0x5E4E,
	0x2918, 0x5E4E,
	0x291A, 0x5E4E,
	0x291C, 0x415F,
	0x291E, 0x0006,
	0x2920, 0xF07F,
	0x2922, 0xFFDF,
	0x2924, 0xDE4F,
	0x2926, 0x4FC1,
	0x2928, 0x0006,
	0x292A, 0x410F,
	0x292C, 0x4FB2,
	0x292E, 0x0508,
	0x2930, 0x4FB2,
	0x2932, 0x050A,
	0x2934, 0x4FB2,
	0x2936, 0x050C,
	0x2938, 0x4FB2,
	0x293A, 0x0500,
	0x293C, 0x4FB2,
	0x293E, 0x0502,
	0x2940, 0x4FB2,
	0x2942, 0x0504,
	0x2944, 0x3C10,
	0x2946, 0xD2F2,
	0x2948, 0x0400,
	0x294A, 0x3F7C,
	0x294C, 0x4F6F,
	0x294E, 0xF37F,
	0x2950, 0x4FC2,
	0x2952, 0x0402,
	0x2954, 0x3F72,
	0x2956, 0x90F2,
	0x2958, 0x0011,
	0x295A, 0x0619,
	0x295C, 0x2B46,
	0x295E, 0x50F2,
	0x2960, 0xFFF0,
	0x2962, 0x0619,
	0x2964, 0x3F42,
	0x2966, 0x5031,
	0x2968, 0x000C,
	0x296A, 0x4139,
	0x296C, 0x413A,
	0x296E, 0x413B,
	0x2970, 0x4130,
	0x2972, 0x0900,
	0x2974, 0x7312,
	0x2976, 0x421F,
	0x2978, 0x0A08,
	0x297A, 0xF03F,
	0x297C, 0xF7FF,
	0x297E, 0x4F82,
	0x2980, 0x0A88,
	0x2982, 0x0900,
	0x2984, 0x7312,
	0x2986, 0x421F,
	0x2988, 0x0A0E,
	0x298A, 0xF03F,
	0x298C, 0x7FFF,
	0x298E, 0x4F82,
	0x2990, 0x0A8E,
	0x2992, 0x0900,
	0x2994, 0x7312,
	0x2996, 0x421F,
	0x2998, 0x0A1E,
	0x299A, 0xC31F,
	0x299C, 0x4F82,
	0x299E, 0x0A9E,
	0x29A0, 0x4130,
	0x29A2, 0x4292,
	0x29A4, 0x0A08,
	0x29A6, 0x0A88,
	0x29A8, 0x0900,
	0x29AA, 0x7312,
	0x29AC, 0x4292,
	0x29AE, 0x0A0E,
	0x29B0, 0x0A8E,
	0x29B2, 0x0900,
	0x29B4, 0x7312,
	0x29B6, 0x4292,
	0x29B8, 0x0A1E,
	0x29BA, 0x0A9E,
	0x29BC, 0x4130,
	0x29BE, 0x7400,
	0x29C0, 0x8058,
	0x29C2, 0x1807,
	0x29C4, 0x00E0,
	0x29C6, 0x7002,
	0x29C8, 0x17C7,
	0x29CA, 0x0045,
	0x29CC, 0x0006,
	0x29CE, 0x17CC,
	0x29D0, 0x0015,
	0x29D2, 0x1512,
	0x29D4, 0x216F,
	0x29D6, 0x005B,
	0x29D8, 0x005D,
	0x29DA, 0x00DE,
	0x29DC, 0x00DD,
	0x29DE, 0x5023,
	0x29E0, 0x00DE,
	0x29E2, 0x005B,
	0x29E4, 0x0410,
	0x29E6, 0x0091,
	0x29E8, 0x0015,
	0x29EA, 0x0040,
	0x29EC, 0x7023,
	0x29EE, 0x1653,
	0x29F0, 0x0156,
	0x29F2, 0x0001,
	0x29F4, 0x2081,
	0x29F6, 0x7020,
	0x29F8, 0x2F99,
	0x29FA, 0x005C,
	0x29FC, 0x0000,
	0x29FE, 0x5040,
	0x2A00, 0x0045,
	0x2A02, 0x213A,
	0x2A04, 0x0303,
	0x2A06, 0x0148,
	0x2A08, 0x0049,
	0x2A0A, 0x0045,
	0x2A0C, 0x0046,
	0x2A0E, 0x05DD,
	0x2A10, 0x00DE,
	0x2A12, 0x00DD,
	0x2A14, 0x00DC,
	0x2A16, 0x00DE,
	0x2A18, 0x04D6,
	0x2A1A, 0x2014,
	0x2A1C, 0x2081,
	0x2A1E, 0x7087,
	0x2A20, 0x2F99,
	0x2A22, 0x005C,
	0x2A24, 0x0002,
	0x2A26, 0x5060,
	0x2A28, 0x31C0,
	0x2A2A, 0x2122,
	0x2A2C, 0x7800,
	0x2A2E, 0xC08C,
	0x2A30, 0x0001,
	0x2A32, 0x9038,
	0x2A34, 0x59F7,
	0x2A36, 0x907A,
	0x2A38, 0x03D8,
	0x2A3A, 0x8D90,
	0x2A3C, 0x01C0,
	0x2A3E, 0x7400,
	0x2A40, 0x8058,
	0x2A42, 0x1807,
	0x2A44, 0x00E0,
	0x2A46, 0x7002,
	0x2A48, 0x17C7,
	0x2A4A, 0x0045,
	0x2A4C, 0x0006,
	0x2A4E, 0x17CC,
	0x2A50, 0x0015,
	0x2A52, 0x1512,
	0x2A54, 0x216F,
	0x2A56, 0x005B,
	0x2A58, 0x005D,
	0x2A5A, 0x00DE,
	0x2A5C, 0x00DD,
	0x2A5E, 0x5023,
	0x2A60, 0x00DE,
	0x2A62, 0x005B,
	0x2A64, 0x0410,
	0x2A66, 0x0091,
	0x2A68, 0x0015,
	0x2A6A, 0x0040,
	0x2A6C, 0x7023,
	0x2A6E, 0x1653,
	0x2A70, 0x0156,
	0x2A72, 0x0001,
	0x2A74, 0x2081,
	0x2A76, 0x7020,
	0x2A78, 0x2F99,
	0x2A7A, 0x005C,
	0x2A7C, 0x0000,
	0x2A7E, 0x5040,
	0x2A80, 0x0045,
	0x2A82, 0x213A,
	0x2A84, 0x0303,
	0x2A86, 0x0148,
	0x2A88, 0x0049,
	0x2A8A, 0x0045,
	0x2A8C, 0x0046,
	0x2A8E, 0x05DD,
	0x2A90, 0x00DE,
	0x2A92, 0x00DD,
	0x2A94, 0x00DC,
	0x2A96, 0x00DE,
	0x2A98, 0x0296,
	0x2A9A, 0x2014,
	0x2A9C, 0x2081,
	0x2A9E, 0x7087,
	0x2AA0, 0x2F99,
	0x2AA2, 0x005C,
	0x2AA4, 0x0002,
	0x2AA6, 0x5060,
	0x2AA8, 0x31C0,
	0x2AAA, 0x2122,
	0x2AAC, 0x7800,
	0x2AAE, 0xC08C,
	0x2AB0, 0x0001,
	0x2AB2, 0x9038,
	0x2AB4, 0x59F7,
	0x2AB6, 0x907A,
	0x2AB8, 0x03D8,
	0x2ABA, 0x8D90,
	0x2ABC, 0x01C0,
	0x2ABE, 0x7400,
	0x2AC0, 0x2002,
	0x2AC2, 0x70DF,
	0x2AC4, 0x2F21,
	0x2AC6, 0x04C1,
	0x2AC8, 0x0D80,
	0x2ACA, 0x7800,
	0x2ACC, 0x0041,
	0x2ACE, 0x7400,
	0x2AD0, 0x2004,
	0x2AD2, 0x70DF,
	0x2AD4, 0x2F21,
	0x2AD6, 0x04C2,
	0x2AD8, 0x0D80,
	0x2ADA, 0x7800,
	0x2ADC, 0x7400,
	0x2ADE, 0x2008,
	0x2AE0, 0x70DF,
	0x2AE2, 0x2F21,
	0x2AE4, 0x04C3,
	0x2AE6, 0x0D80,
	0x2AE8, 0x7800,
	0x2AEA, 0x7400,
	0x2AEC, 0x0004,
	0x2AEE, 0x70DF,
	0x2AF0, 0x2F22,
	0x2AF2, 0x7008,
	0x2AF4, 0x2F1F,
	0x2AF6, 0x7021,
	0x2AF8, 0x2F01,
	0x2AFA, 0x7800,
	0x2AFC, 0x7400,
	0x2AFE, 0x0002,
	0x2B00, 0x70DF,
	0x2B02, 0x3F5F,
	0x2B04, 0x703A,
	0x2B06, 0x2F01,
	0x2B08, 0x7800,
	0x2B0A, 0x7400,
	0x2B0C, 0x2010,
	0x2B0E, 0x70DF,
	0x2B10, 0x3F40,
	0x2B12, 0x700A,
	0x2B14, 0x0FC0,
	0x2B16, 0x7800,
	0x2B18, 0x7400,
	0x2B1A, 0x2004,
	0x2B1C, 0x70DF,
	0x2B1E, 0x2F21,
	0x2B20, 0x04C2,
	0x2B22, 0x0D80,
	0x2B24, 0x7800,
	0x2B26, 0x0041,
	0x2B28, 0x7400,
	0x2B2A, 0x2002,
	0x2B2C, 0x70DF,
	0x2B2E, 0x2F22,
	0x2B30, 0x04C1,
	0x2B32, 0x0D80,
	0x2B34, 0x7800,
	0x2B36, 0x7400,
	0x2B38, 0x0001,
	0x2B3A, 0x70DF,
	0x2B3C, 0x3F5F,
	0x2B3E, 0x703A,
	0x2B40, 0x2F01,
	0x2B42, 0x7800,
	0x2B44, 0x7400,
	0x2B46, 0x200A,
	0x2B48, 0x70DF,
	0x2B4A, 0x3F40,
	0x2B4C, 0x700A,
	0x2B4E, 0x0FC0,
	0x2B50, 0x7800,
	0x2B52, 0x7400,
	0x2B54, 0x2015,
	0x2B56, 0x70DF,
	0x2B58, 0x3F5F,
	0x2B5A, 0x703A,
	0x2B5C, 0x2F01,
	0x2B5E, 0x7800,
	0x2B60, 0x7400,
	0x2B62, 0x7800,
	0x2B64, 0x007F,
	0x2B66, 0x0000,
	0x2B68, 0xB9BE,
	0x2B6A, 0x0000,
	0x2B6C, 0xB9BE,
	0x2B6E, 0xBA2E,
	0x2B70, 0x0002,
	0x2B72, 0x0000,
	0x2B74, 0xBA3E,
	0x2B76, 0x0000,
	0x2B78, 0xBA3E,
	0x2B7A, 0xBAAE,
	0x2B7C, 0x0002,
	0x2B7E, 0x0063,
	0x2B80, 0xBB18,
	0x2B82, 0x0063,
	0x2B84, 0xBB28,
	0x2B86, 0x0063,
	0x2B88, 0xBADC,
	0x2B8A, 0x0063,
	0x2B8C, 0xBAEA,
	0x2B8E, 0xBACC,
	0x2B90, 0x0004,
	0x2B92, 0x0063,
	0x2B94, 0xBADC,
	0x2B96, 0x0063,
	0x2B98, 0xBB0A,
	0x2B9A, 0x0063,
	0x2B9C, 0xBB18,
	0x2B9E, 0x0063,
	0x2BA0, 0xBB36,
	0x2BA2, 0xBACC,
	0x2BA4, 0x0004,
	0x2BA6, 0x0063,
	0x2BA8, 0xBABE,
	0x2BAA, 0x0063,
	0x2BAC, 0xBACE,
	0x2BAE, 0x0063,
	0x2BB0, 0xBADC,
	0x2BB2, 0x0063,
	0x2BB4, 0xBAEA,
	0x2BB6, 0xBACC,
	0x2BB8, 0x0004,
	0x2BBA, 0x0063,
	0x2BBC, 0xBADC,
	0x2BBE, 0x0063,
	0x2BC0, 0xBB0A,
	0x2BC2, 0x0063,
	0x2BC4, 0xBABE,
	0x2BC6, 0x0063,
	0x2BC8, 0xBAFC,
	0x2BCA, 0xBACC,
	0x2BCC, 0x0004,
	0x2BCE, 0x0063,
	0x2BD0, 0xBABE,
	0x2BD2, 0x0063,
	0x2BD4, 0xBACE,
	0x2BD6, 0x0063,
	0x2BD8, 0xBADC,
	0x2BDA, 0x0063,
	0x2BDC, 0xBB0A,
	0x2BDE, 0xBACC,
	0x2BE0, 0x0004,
	0x2BE2, 0xFFFF,
	0x2BE4, 0xBB60,
	0x2BE6, 0x0000,
	0x2BE8, 0x0000,
	0x2BEA, 0x0000,
	0x2BEC, 0x0000,
	0x2BEE, 0x0000,
	0x2BF0, 0x0000,
	0x2BF2, 0xBB64,
	0x2BF4, 0x0001,
	0x2BF6, 0x0063,
	0x2BF8, 0xBB44,
	0x2BFA, 0x0063,
	0x2BFC, 0xBB52,
	0x2BFE, 0x0000,
	0x2C00, 0x0000,
	0x2C02, 0x0000,
	0x2C04, 0x0000,
	0x2C06, 0xBACC,
	0x2C08, 0x0002,
	0x2C0A, 0x0066,
	0x2C0C, 0x0067,
	0x2C0E, 0x00AF,
	0x2C10, 0x01CF,
	0x2C12, 0x0087,
	0x2C14, 0x0083,
	0x2C16, 0x011B,
	0x2C18, 0x035A,
	0x2C1A, 0x00FA,
	0x2C1C, 0x00F2,
	0x2C1E, 0x00A6,
	0x2C20, 0x00A4,
	0x2C22, 0xFFFF,
	0x2C24, 0x002C,
	0x2C26, 0x0058,
	0x2C28, 0x0000,
	0x2C2A, 0x0000,
	0x2C2C, 0xBC0A,
	0x2C2E, 0xBB66,
	0x2C30, 0xBB72,
	0x2C32, 0xBC24,
	0x2C34, 0xBB7E,
	0x2C36, 0xBB92,
	0x2C38, 0xBB7E,
	0x2C3A, 0xBB92,
	0x2C3C, 0xBBF6,
	0x2C3E, 0xBBF6,
	0x2C40, 0xBBE2,
	0x2C42, 0xBBE2,
	0x2C44, 0xBBA6,
	0x2C46, 0xBBBA,
	0x2C48, 0xBBA6,
	0x2C4A, 0xBBBA,
	0x2C4C, 0xBBF6,
	0x2C4E, 0xBBF6,
	0x2C50, 0xBBE2,
	0x2C52, 0xBBE2,
	0x2C54, 0xBB7E,
	0x2C56, 0xBB92,
	0x2C58, 0xBB7E,
	0x2C5A, 0xBB92,
	0x2C5C, 0xBBF6,
	0x2C5E, 0xBBF6,
	0x2C60, 0xBBE2,
	0x2C62, 0xBBE2,
	0x2C64, 0xBBA6,
	0x2C66, 0xBBBA,
	0x2C68, 0xBBA6,
	0x2C6A, 0xBBBA,
	0x2C6C, 0xBBF6,
	0x2C6E, 0xBBF6,
	0x2C70, 0xBBE2,
	0x2C72, 0xBBE2,
	0x3800, 0x880E,
	0x3802, 0xBC54,
	0x3804, 0xBC32,
	0x3806, 0xD13E,
	0x3808, 0xBC34,
	0x380A, 0xBC2E,
	0x380C, 0x0000,
	0x380E, 0x0040,
	0x3810, 0x0040,
	0x3812, 0x0040,
	0x3814, 0x0043,
	0x3816, 0x0046,
	0x3818, 0x004B,
	0x381A, 0x004D,
	0x381C, 0x0051,
	0x381E, 0x0055,
	0x3820, 0x005A,
	0x3822, 0x005E,
	0x3824, 0x0062,
	0x3826, 0x0067,
	0x3828, 0x006C,
	0x382A, 0x0070,
	0x382C, 0x0078,
	0x382E, 0x0086,
	0x3830, 0x0090,
	0x3832, 0x0096,
	0x3834, 0x009D,
	0x3836, 0x00A5,
	0x3838, 0x00AD,
	0x383A, 0x00B4,
	0x383C, 0x00B9,
	0x383E, 0x00BE,
	0x3840, 0x00C3,
	0x3842, 0x00C8,
	0x3844, 0x00CD,
	0x3846, 0x00D2,
	0x3848, 0x00D7,
	0x384A, 0x00DC,
	0x384C, 0x00DC,
	0x384E, 0x0000,
	0x3850, 0x0000,
	0x3852, 0x0000,
	0x3854, 0x0000,
	0x3856, 0x0000,
	0x3858, 0x0000,
	0x385A, 0x0000,
	0x385C, 0x0000,
	0x385E, 0x0000,
	0x3860, 0x0000,
	0x3862, 0x0000,
	0x3864, 0x0000,
	0x3866, 0x0000,
	0x3868, 0x0000,
	0x386A, 0x0000,
	0x386C, 0x0000,
	0x386E, 0x0000,
	0x3870, 0x0000,
	0x3872, 0x0000,
	0x3874, 0x0000,
	0x3876, 0x0000,
	0x3878, 0x0000,
	0x387A, 0x0000,
	0x387C, 0x0000,
	0x387E, 0x0000,
	0x3880, 0x0000,
	0x3882, 0x0000,
	0x3884, 0x0000,
	0x3886, 0x0000,
	0x3888, 0x0000,
	0x388A, 0x0000,
	0x388C, 0x0000,
	0x026A, 0xFFFF,
	0x026C, 0x00FF,
	0x026E, 0x0000,
	0x0360, 0x1E8E,
	0x040E, 0x01EB,
	0x0600, 0x1130,
	0x0602, 0x3112,
	0x0604, 0x8048,
	0x0606, 0x00E9,
	0x067A, 0x0404,
	0x067C, 0x0404,
	0x06A8, 0x0240,
	0x06AA, 0x00CA,
	0x06AC, 0x0041,
	0x06B4, 0x3FFF,
	0x06DE, 0x0404,
	0x06E0, 0x0404,
	0x06E2, 0xFF00,
	0x06E4, 0x8010,
	0x06E6, 0x80C0,
	0x06E8, 0x00C0,
	0x06EA, 0x02C0,
	0x052A, 0x0000,
	0x052C, 0x0000,
	0x0F06, 0x0002,
	0x0A04, 0xB4C5,
	0x0A06, 0xC400,
	0x0A08, 0x988A,
	0x0A0A, 0xA387,
	0x0A0E, 0xEEC0,
	0x0A12, 0x0000,
	0x0A18, 0x0010,
	0x0A1C, 0x0040,
	0x0A20, 0x0015,
	0x0C00, 0x0021,
	0x0C16, 0x0002,
	0x0708, 0x6FC0,
	0x070C, 0x0000,
	0x1244, 0x0000,
	0x105C, 0x0F0B,
	0x1958, 0x0000,
	0x195A, 0x004C,
	0x195C, 0x0097,
	0x195E, 0x0221,
	0x1960, 0x03FE,
	0x1980, 0x00E0,
	0x1982, 0x0010,
	0x1984, 0x2018,
	0x1986, 0x0008,
	0x1988, 0x0000,
	0x198A, 0x0000,
	0x198C, 0x0880,
	0x198E, 0x0000,
	0x1990, 0x1A00,
	0x1992, 0x0000,
	0x1994, 0x2800,
	0x1996, 0x0002,
	0x1962, 0x0000,
	0x1964, 0x004C,
	0x1966, 0x0097,
	0x1968, 0x0221,
	0x196A, 0x03FE,
	0x19C0, 0x00E0,
	0x19C2, 0x0010,
	0x19C4, 0x2018,
	0x19C6, 0x0008,
	0x19C8, 0x0000,
	0x19CA, 0x0000,
	0x19CC, 0x0880,
	0x19CE, 0x0000,
	0x19D0, 0x1A00,
	0x19D2, 0x0000,
	0x19D4, 0x2800,
	0x19D6, 0x0002,
	0x196C, 0x0000,
	0x196E, 0x004C,
	0x1970, 0x0097,
	0x1972, 0x0221,
	0x1974, 0x03FE,
	0x1A00, 0x00E0,
	0x1A02, 0x0010,
	0x1A04, 0x2018,
	0x1A06, 0x0008,
	0x1A08, 0x0000,
	0x1A0A, 0x0000,
	0x1A0C, 0x0880,
	0x1A0E, 0x0000,
	0x1A10, 0x1A00,
	0x1A12, 0x0000,
	0x1A14, 0x2800,
	0x1A16, 0x0002,
	0x1976, 0x0000,
	0x1978, 0x004C,
	0x197A, 0x0097,
	0x197C, 0x0221,
	0x197E, 0x03FE,
	0x1A40, 0x00E0,
	0x1A42, 0x0010,
	0x1A44, 0x2018,
	0x1A46, 0x0008,
	0x1A48, 0x0000,
	0x1A4A, 0x0000,
	0x1A4C, 0x0880,
	0x1A4E, 0x0000,
	0x1A50, 0x1A00,
	0x1A52, 0x0000,
	0x1A54, 0x2800,
	0x1A56, 0x0002,
	0x192A, 0x0201,
	0x0384, 0x0000,
	0x027E, 0x0100,

};

kal_uint16 addr_data_pair_cap_hi847ofilm[] = {
	0x0B00, 0x0000,
	0x0204, 0x0000,
	0x0206, 0x0384,
	0x020A, 0x0A66,
	0x020E, 0x0A6A,
	0x0214, 0x0200,
	0x0216, 0x0200,
	0x0218, 0x0200,
	0x021A, 0x0200,
	0x0224, 0x002E,
	0x022A, 0x0017,
	0x022C, 0x0E1B,
	0x022E, 0x09C1,
	0x0234, 0x1111,
	0x0236, 0x1111,
	0x0238, 0x1111,
	0x023A, 0x1111,
	0x0268, 0x00BE,
	0x0440, 0x0028,
	0x0F00, 0x0000,
	0x0F04, 0x0008,
	0x0B04, 0x00DC,
	0x0B12, 0x0CC0,
	0x0B14, 0x0990,
	0x0B20, 0x0100,
	0x1100, 0x1100,
	0x1102, 0x0008,
	0x1108, 0x0202,
	0x1118, 0x0000,
	0x0A10, 0xB040,
	0x0C14, 0x0008,
	0x0C18, 0x0CC0,
	0x0C1A, 0x0990,
	0x0730, 0x0001,
	0x0732, 0x0000,
	0x0734, 0x0300,
	0x0736, 0x005A,
	0x0738, 0x0002,
	0x073C, 0x0900,
	0x0740, 0x0000,
	0x0742, 0x0000,
	0x0744, 0x0300,
	0x0746, 0x00D2,
	0x0748, 0x0002,
	0x074A, 0x0901,
	0x074C, 0x0000,
	0x074E, 0x0100,
	0x0750, 0x0000,
	0x1200, 0x0946,
	0x1202, 0x1E00,
	0x120E, 0x6027,
	0x1210, 0x8027,
	0x1246, 0x00F0,
	0x1000, 0x0300,
	0x1002, 0x4311,
	0x1004, 0x2BAB,
	0x1010, 0x0100,
	0x1012, 0x01D7,
	0x1014, 0x0074,
	0x1016, 0x0074,
	0x101A, 0x0074,
	0x1020, 0xC10D,
	0x1022, 0x0D39,
	0x1024, 0x030D,
	0x1026, 0x1712,
	0x1028, 0x1C10,
	0x102A, 0x170A,
	0x102C, 0x2800,
	0x1038, 0x1100,
	0x103E, 0x0001,
	0x1042, 0x0108,
	0x1044, 0x00C8,
	0x1046, 0x0004,
	0x1048, 0x00C8,
	0x1066, 0x0100,
	0x1600, 0xE000,
	0x1608, 0x0028,
	0x160A, 0x0C80,
	0x160C, 0x001A,
	0x160E, 0x0960,
};
#endif

enum IMGSENSOR_MODE{
	IMGSENSOR_MODE_INIT,
	IMGSENSOR_MODE_PREVIEW,
	IMGSENSOR_MODE_CAPTURE,
	IMGSENSOR_MODE_VIDEO,
	IMGSENSOR_MODE_HIGH_SPEED_VIDEO,
	IMGSENSOR_MODE_SLIM_VIDEO,
	IMGSENSOR_MODE_CUSTOM1,
	IMGSENSOR_MODE_CUSTOM2,
	IMGSENSOR_MODE_CUSTOM3,
	IMGSENSOR_MODE_CUSTOM4,
	IMGSENSOR_MODE_CUSTOM5,
};

struct imgsensor_mode_struct {
	kal_uint32 pclk;
	kal_uint32 linelength;
	kal_uint32 framelength;

	kal_uint8 startx;
	kal_uint8 starty;

	kal_uint16 grabwindow_width;
	kal_uint16 grabwindow_height;

	kal_uint8 mipi_data_lp2hs_settle_dc;

	/*	 following for GetDefaultFramerateByScenario()	*/
	kal_uint16 max_framerate;
	kal_uint32 mipi_pixel_rate;

};

/* SENSOR PRIVATE STRUCT FOR VARIABLES*/
struct imgsensor_struct {
	kal_uint8 mirror;				//mirrorflip information

	kal_uint8 sensor_mode; //record IMGSENSOR_MODE enum value

	kal_uint32 shutter;				//current shutter
	kal_uint16 gain;				//current gain

	kal_uint32 pclk;				//current pclk

	kal_uint32 frame_length;		//current framelength
	kal_uint32 line_length;			//current linelength

	kal_uint32 min_frame_length; //current min framelength to max
	kal_int32 dummy_pixel;			//current dummypixel
	kal_int32 dummy_line;			//current dummline

	kal_uint16 current_fps;			//current max fps
	kal_bool   autoflicker_en; //record autoflicker enable or disable
	kal_bool test_pattern; //record test pattern mode or not
	enum MSDK_SCENARIO_ID_ENUM current_scenario_id; //current scenario
	kal_bool  ihdr_en;				//ihdr enable or disable

	kal_uint8 i2c_write_id; //record current sensor's i2c write id
	kal_uint8 pdaf_mode;
};

/* SENSOR PRIVATE STRUCT FOR CONSTANT*/
struct imgsensor_info_struct {
	kal_uint16 sensor_id; //record sensor id defined in Kd_imgsensor.h
	kal_uint32 checksum_value; //checksum value for Camera Auto Test
	struct imgsensor_mode_struct pre; //preview scenario
	struct imgsensor_mode_struct cap; //capture scenario
	struct imgsensor_mode_struct cap1; //capture for PIP 24fps
	struct imgsensor_mode_struct cap2; //capture for PIP 15ps
	struct imgsensor_mode_struct normal_video;//normal video info
	struct imgsensor_mode_struct hs_video; //high speed video relative info
	struct imgsensor_mode_struct slim_video; //slim video for VT
	struct imgsensor_mode_struct custom1; //custom1 scenario relative info
	struct imgsensor_mode_struct custom2; //custom2 scenario relative info
	struct imgsensor_mode_struct custom3; //custom3 scenario relative info
	struct imgsensor_mode_struct custom4; //custom4 scenario relative info
	struct imgsensor_mode_struct custom5; //custom5 scenario relative info

	kal_uint8  ae_shut_delay_frame; //shutter delay frame for AE cycle
	kal_uint8  ae_sensor_gain_delay_frame; //sensorgaindelfra for AEcycle
	kal_uint8  ae_ispGain_delay_frame; //ispgaindelayframe for AEcycle
	kal_uint8  frame_time_delay_frame;
	kal_uint8  ihdr_support;		//1, support; 0,not support
	kal_uint8  ihdr_le_firstline;	//1,le first ; 0, se first
	kal_uint8  sensor_mode_num;		//support sensor mode num

	kal_uint8  cap_delay_frame;		//enter capture delay frame num
	kal_uint8  pre_delay_frame;		//enter preview delay frame num
	kal_uint8  video_delay_frame; //enter video delay frame num
	kal_uint8  hs_video_delay_frame; //enter high speed videodelayframenum
	kal_uint8  slim_video_delay_frame; //enter slim video delay frame num
	kal_uint8  custom1_delay_frame;     //enter custom1 delay frame num
	kal_uint8  custom2_delay_frame;     //enter custom1 delay frame num
	kal_uint8  custom3_delay_frame;     //enter custom1 delay frame num
	kal_uint8  custom4_delay_frame;     //enter custom1 delay frame num
	kal_uint8  custom5_delay_frame;     //enter custom1 delay frame num

	kal_uint8  margin; //sensor framelength & shutter margin
	kal_uint32 min_shutter; //min shutter
	kal_uint32 min_gain;
	kal_uint32 max_gain;
	kal_uint32 min_gain_iso;
	kal_uint32 gain_step;
	kal_uint32 gain_type;
	kal_uint32 max_frame_length; //maxframelengthbysensor reg's limitation

	kal_uint8  isp_driving_current; //mclk driving current
	kal_uint8  sensor_interface_type;//sensor_interface_type
	kal_uint8  mipi_sensor_type;
	kal_uint8  mipi_settle_delay_mode;
	kal_uint8  sensor_output_dataformat;
	kal_uint8  mclk;

	kal_uint8  mipi_lane_num;		//mipi lane num
	kal_uint8  i2c_addr_table[5];
	kal_uint32  i2c_speed;     //i2c speed
};


extern int iReadRegI2C(u8 *a_pSendData, u16 a_sizeSendData, u8 *a_pRecvData,
				u16 a_sizeRecvData, u16 i2cId);
extern int iWriteRegI2C(u8 *a_pSendData, u16 a_sizeSendData, u16 i2cId);
extern void kdSetI2CSpeed(u16 i2cSpeed);
extern int iBurstWriteReg(u8 *pData, u32 bytes, u16 i2cId);

extern int iBurstWriteReg_multi(u8 *pData, u32 bytes, u16 i2cId,
	u16 transfer_length, u16 timing);

#endif

/* Copyright (c) 2018-2019, The Linux Foundation. All rights reserved.
 * Copyright (C) 2021 XiaoMi, Inc.

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _WCD937X_INTERNAL_H
#define _WCD937X_INTERNAL_H

#include "../wcd-clsh.h"
#include "../wcd-mbhc-v2.h"
#include "asoc/wcd-irq.h"
#include "wcd937x-mbhc.h"

#define WCD937X_MAX_MICBIAS 3

/* Convert from vout ctl to micbias voltage in mV */
#define  WCD_VOUT_CTL_TO_MICB(v)  (1000 + v * 50)
#define MAX_PORT 8
#define MAX_CH_PER_PORT 8

extern struct regmap_config wcd937x_regmap_config;

struct codec_port_info {
	u32 slave_port_type;
	u32 master_port_type;
	u32 ch_mask;
	u32 num_ch;
	u32 ch_rate;
};
struct wcd937x_priv {
	struct device *dev;

	int variant;
	struct snd_soc_codec *codec;
	struct device_node *rst_np;
	struct regmap *regmap;

	struct swr_device *rx_swr_dev;
	struct swr_device *tx_swr_dev;

	s32 micb_ref[WCD937X_MAX_MICBIAS];
	s32 pullup_ref[WCD937X_MAX_MICBIAS];

	struct fw_info *fw_data;
	struct device_node *wcd_rst_np;

	struct mutex micb_lock;
	s32 dmic_0_1_clk_cnt;
	s32 dmic_2_3_clk_cnt;
	s32 dmic_4_5_clk_cnt;
	/* class h specific info */
	struct wcd_clsh_cdc_info clsh_info;
	/* mbhc module */
	struct wcd937x_mbhc *mbhc;

	u32 hph_mode;
	bool comp1_enable;
	bool comp2_enable;

	struct irq_domain *virq;
	struct wcd_irq_info irq_info;
	u32 rx_clk_cnt;
	int num_irq_regs;
	/* to track the status */
	unsigned long status_mask;

	u8 num_tx_ports;
	u8 num_rx_ports;
	struct codec_port_info
			tx_port_mapping[MAX_PORT][MAX_CH_PER_PORT];
	struct codec_port_info
			rx_port_mapping[MAX_PORT][MAX_CH_PER_PORT];
	struct regulator_bulk_data *supplies;
	struct notifier_block nblock;
	/* wcd callback to bolero */
	void *handle;
	int (*update_wcd_event)(void *handle, u16 event, u32 data);
	int (*register_notifier)(void *handle,
				struct notifier_block *nblock,
				bool enable);
	int (*wakeup)(void *handle, bool enable);
	u32 version;
	/* Entry for version info */
	struct snd_info_entry *entry;
	struct snd_info_entry *version_entry;
	int ear_rx_path;
	int ana_clk_count;
#ifdef CONFIG_SND_SOC_FOR_ULTRASOUND_PATH
	int ana_adc_count; //for wcd937x_codec_enable_adc
	int ana_tx_req_count; //for wcd937x_enable_req
#endif
	struct mutex ana_tx_clk_lock;
};

struct wcd937x_micbias_setting {
	u8 ldoh_v;
	u32 cfilt1_mv;
	u32 micb1_mv;
	u32 micb2_mv;
	u32 micb3_mv;
	u8 bias1_cfilt_sel;
};

struct wcd937x_pdata {
	struct device_node *rst_np;
	struct device_node *rx_slave;
	struct device_node *tx_slave;
	struct wcd937x_micbias_setting micbias;

	struct cdc_regulator *regulator;
	int num_supplies;
};

struct wcd_ctrl_platform_data {
	void *handle;
	int (*update_wcd_event)(void *handle, u16 event, u32 data);
	int (*register_notifier)(void *handle,
				 struct notifier_block *nblock,
				 bool enable);
};

enum {
	WCD_RX1,
	WCD_RX2,
	WCD_RX3
};

enum {
	BOLERO_WCD_EVT_TX_CH_HOLD_CLEAR = 1,
	BOLERO_WCD_EVT_PA_OFF_PRE_SSR,
	BOLERO_WCD_EVT_SSR_DOWN,
	BOLERO_WCD_EVT_SSR_UP,
};

enum {
	WCD_BOLERO_EVT_RX_MUTE = 1,	/* for RX mute/unmute */
	WCD_BOLERO_EVT_IMPED_TRUE,	/* for imped true */
	WCD_BOLERO_EVT_IMPED_FALSE,	/* for imped false */
};

enum {
	/* INTR_CTRL_INT_MASK_0 */
	WCD937X_IRQ_MBHC_BUTTON_PRESS_DET = 0,
	WCD937X_IRQ_MBHC_BUTTON_RELEASE_DET,
	WCD937X_IRQ_MBHC_ELECT_INS_REM_DET,
	WCD937X_IRQ_MBHC_ELECT_INS_REM_LEG_DET,
	WCD937X_IRQ_MBHC_SW_DET,
	WCD937X_IRQ_HPHR_OCP_INT,
	WCD937X_IRQ_HPHR_CNP_INT,
	WCD937X_IRQ_HPHL_OCP_INT,

	/* INTR_CTRL_INT_MASK_1 */
	WCD937X_IRQ_HPHL_CNP_INT,
	WCD937X_IRQ_EAR_CNP_INT,
	WCD937X_IRQ_EAR_SCD_INT,
	WCD937X_IRQ_AUX_CNP_INT,
	WCD937X_IRQ_AUX_SCD_INT,
	WCD937X_IRQ_HPHL_PDM_WD_INT,
	WCD937X_IRQ_HPHR_PDM_WD_INT,
	WCD937X_IRQ_AUX_PDM_WD_INT,

	/* INTR_CTRL_INT_MASK_2 */
	WCD937X_IRQ_LDORT_SCD_INT,
	WCD937X_IRQ_MBHC_MOISTURE_INT,
	WCD937X_IRQ_HPHL_SURGE_DET_INT,
	WCD937X_IRQ_HPHR_SURGE_DET_INT,
	WCD937X_NUM_IRQS,
};

extern struct wcd937x_mbhc *wcd937x_soc_get_mbhc(struct snd_soc_codec *codec);
extern int wcd937x_mbhc_micb_adjust_voltage(struct snd_soc_codec *codec,
					int volt, int micb_num);
extern int wcd937x_get_micb_vout_ctl_val(u32 micb_mv);
extern int wcd937x_micbias_control(struct snd_soc_codec *codec, int micb_num,
			int req, bool is_dapm);
#endif

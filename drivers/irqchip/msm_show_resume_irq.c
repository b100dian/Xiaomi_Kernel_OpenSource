/*
 * @,@Author: ,: your name
 * @,@Date: ,: 2021-01-22 15:35:13
 * @,@LastEditTime: ,: 2021-01-22 19:26:54
 * @,@LastEditors: ,: your name
 * @,@Description: ,: In User Settings Edit
 * @,@FilePath: ,: /msm-4.14/drivers/irqchip/msm_show_resume_irq.c
 */
/* Copyright (c) 2011, 2014-2016 The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

int msm_show_resume_irq_mask;

module_param_named(
	debug_mask, msm_show_resume_irq_mask, int, 0664);

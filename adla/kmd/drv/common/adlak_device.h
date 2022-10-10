/*******************************************************************************
 * Copyright (C) 2021 Amlogic, Inc. All rights reserved.
 ******************************************************************************/

/*****************************************************************************/
/**
 *
 * @file adlak_device.h
 * @brief
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   	Who				Date				Changes
 * ----------------------------------------------------------------------------
 * 1.00a shiwei.sun@amlogic.com	2021/06/05	Initial release
 * </pre>
 *
 ******************************************************************************/

#ifndef __ADLAK_DEVICE_H__
#define __ADLAK_DEVICE_H__

/***************************** Include Files *********************************/
#include "adlak_common.h"
#include "adlak_hw.h"
#include "adlak_platform_device.h"
#include "adlak_queue.h"

#ifdef CONFIG_ADLAK_DEBUG_INNNER
#include "adlak_dbg.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif
struct adlak_hardware_res {
    void *adlak_reg_va; /*mapped cpu virtual address for AHB register memory base address*/
    resource_size_t adlak_reg_pa;    /*the AHB register physical base address*/
    u64             adlak_reg_size;  /*the size of AHB register memory*/
    resource_size_t adlak_sram_pa;   /*the AHB AXI SRAM physical base address*/
    u64             adlak_sram_size; /*the size of AXI SRAM memory*/
    int32_t         sram_wrap;
    resource_size_t adlak_resmem_pa;   /*the reserved memory physical base address*/
    u64             adlak_resmem_size; /*the size of reserved memory*/

    struct io_region *preg;
    u32               irqline;
};

struct adlak_device_caps {
    void * data;
    size_t size;
};

struct adlak_proc_info {
    u32 irq_status;
};

struct adlak_device {
#ifndef CONFIG_ADLA_FREERTOS
    struct class *class;
    struct platform_device *pdev;
    struct device *         dev;
    struct cdev             cdev;
    struct miscdevice *     misc;
    dev_t                   devt;
    struct file_operations  fops;
    int                     major;
    struct clk *            clk_axi;
    struct clk *            clk_core;
#endif
    int32_t                   net_id;
    adlak_os_mutex_t          dev_mutex;
    adlak_os_spinlock_t       spinlock;
    struct adlak_hardware_res hw_res;
    struct adlak_device_caps  dev_caps;
    struct adlak_mem *        mm;
    struct adlak_workqueue    queue;
    struct adlak_proc_info    proc;
    struct list_head          context_list;
    void *                    hw_info;
    u64                       smmu_entry;
    void *                    psmmu;
    u32                       hw_timeout_ms;  // unit is system tick
    int                       dependency_mode;
    int                       all_task_num;
    int                       all_task_num_max;
    bool                      is_clk_axi_enabled;
    bool                      is_clk_core_enabled;
    int                       clk_axi_freq_real;
    int                       clk_core_freq_real;
    int                       clk_axi_freq_set;
    int                       clk_core_freq_set;

    int is_suspend;
    int need_reset_queue;
    int is_reset;

    struct _cmq_buf_info {
        u32 size;       /*max size per invoke*/
        u32 total_size; /*cmq_total_size = 2 * cmq_size */

        u32                      cmq_wr_offset;
        u32                      cmq_rd_offset;
        struct adlak_mem_handle *cmq_mm_info;

    } cmq_buf_info;
    struct _hw_stat {
#define ADLAK_HW_STAT_UNKNOWN (0)
#define ADLAK_HW_STAT_INITED (1)
#define ADLAK_HW_STAT_ERR (2)
#define ADLAK_HW_STAT_NEED_RESET (3)
        u32 stat; /**/
#define ADLAK_PS_STAT_UNKNOWN (0)
#define ADLAK_PS_STAT_RESETED (1)
#define ADLAK_PS_STAT_STOP (2)
#define ADLAK_PS_STAT_RUN (3)
#define ADLAK_PS_STAT_ERR (4)
        u32 paser_stat;
    } hw_stat;
    adlak_os_sema_t paser_refresh;
    bool            smmu_en;

    int   dpm_en;
    int   dpm_period_set;
    void *pdpm;  // dynamic power management
};

/**
 * @brief init adlak device
 *
 * @param padlak
 * @return int
 */
int adlak_device_init(struct adlak_device *padlak);
int adlak_device_deinit(struct adlak_device *padlak);

/**
 * @brief called in top-half IRQ handler
 *
 * @param padlak
 * @return int
 */
int adlak_irq_proc(struct adlak_device *const padlak);

#ifdef __cplusplus
}
#endif

#endif /* __ADLAK_DEVICE_H__ end define*/

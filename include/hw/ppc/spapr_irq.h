/*
 * QEMU PowerPC sPAPR IRQ backend definitions
 *
 * Copyright (c) 2018, IBM Corporation.
 *
 * This code is licensed under the GPL version 2 or later. See the
 * COPYING file in the top-level directory.
 */

#ifndef HW_SPAPR_IRQ_H
#define HW_SPAPR_IRQ_H

/*
 * IRQ range offsets per device type
 */
#define SPAPR_IRQ_IPI        0x0
#define SPAPR_IRQ_EPOW       0x1000  /* XICS_IRQ_BASE offset */
#define SPAPR_IRQ_HOTPLUG    0x1001
#define SPAPR_IRQ_VIO        0x1100  /* 256 VIO devices */
#define SPAPR_IRQ_PCI_LSI    0x1200  /* 32+ PHBs devices */

#define SPAPR_IRQ_MSI        0x1300  /* Offset of the dynamic range covered
                                      * by the bitmap allocator */

typedef struct sPAPRMachineState sPAPRMachineState;

void spapr_irq_msi_init(sPAPRMachineState *spapr, uint32_t nr_msis);
int spapr_irq_msi_alloc(sPAPRMachineState *spapr, uint32_t num, bool align,
                        Error **errp);
void spapr_irq_msi_free(sPAPRMachineState *spapr, int irq, uint32_t num);
void spapr_irq_msi_reset(sPAPRMachineState *spapr);

typedef struct sPAPRIrq {
    uint32_t    nr_irqs;
    uint32_t    nr_msis;
    uint8_t     ov5;

    void (*init)(sPAPRMachineState *spapr, Error **errp);
    int (*claim)(sPAPRMachineState *spapr, int irq, bool lsi, Error **errp);
    void (*free)(sPAPRMachineState *spapr, int irq, int num);
    qemu_irq (*qirq)(sPAPRMachineState *spapr, int irq);
    void (*print_info)(sPAPRMachineState *spapr, Monitor *mon);
    void (*dt_populate)(sPAPRMachineState *spapr, uint32_t nr_servers,
                        void *fdt, uint32_t phandle);
    void (*cpu_intc_create)(sPAPRMachineState *spapr, PowerPCCPU *cpu,
                            Error **errp);
    int (*post_load)(sPAPRMachineState *spapr, int version_id);
    void (*reset)(sPAPRMachineState *spapr, Error **errp);
    void (*set_irq)(void *opaque, int srcno, int val);
} sPAPRIrq;

extern sPAPRIrq spapr_irq_xics;
extern sPAPRIrq spapr_irq_xics_legacy;
extern sPAPRIrq spapr_irq_xive;
extern sPAPRIrq spapr_irq_dual;

void spapr_irq_init(sPAPRMachineState *spapr, Error **errp);
int spapr_irq_claim(sPAPRMachineState *spapr, int irq, bool lsi, Error **errp);
void spapr_irq_free(sPAPRMachineState *spapr, int irq, int num);
qemu_irq spapr_qirq(sPAPRMachineState *spapr, int irq);
int spapr_irq_post_load(sPAPRMachineState *spapr, int version_id);
void spapr_irq_reset(sPAPRMachineState *spapr, Error **errp);

/*
 * XICS legacy routines
 */
int spapr_irq_find(sPAPRMachineState *spapr, int num, bool align, Error **errp);
#define spapr_irq_findone(spapr, errp) spapr_irq_find(spapr, 1, false, errp)

#endif

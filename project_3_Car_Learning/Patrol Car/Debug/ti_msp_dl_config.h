/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


/* Defines for PWM_R */
#define PWM_R_INST                                                        TIMG12
#define PWM_R_INST_IRQHandler                                  TIMG12_IRQHandler
#define PWM_R_INST_INT_IRQN                                    (TIMG12_INT_IRQn)
#define PWM_R_INST_CLK_FREQ                                              4000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_R_C0_PORT                                                 GPIOA
#define GPIO_PWM_R_C0_PIN                                         DL_GPIO_PIN_14
#define GPIO_PWM_R_C0_IOMUX                                      (IOMUX_PINCM36)
#define GPIO_PWM_R_C0_IOMUX_FUNC                    IOMUX_PINCM36_PF_TIMG12_CCP0
#define GPIO_PWM_R_C0_IDX                                    DL_TIMER_CC_0_INDEX

/* Defines for PWM_L */
#define PWM_L_INST                                                         TIMG0
#define PWM_L_INST_IRQHandler                                   TIMG0_IRQHandler
#define PWM_L_INST_INT_IRQN                                     (TIMG0_INT_IRQn)
#define PWM_L_INST_CLK_FREQ                                              4000000
/* GPIO defines for channel 1 */
#define GPIO_PWM_L_C1_PORT                                                 GPIOA
#define GPIO_PWM_L_C1_PIN                                         DL_GPIO_PIN_13
#define GPIO_PWM_L_C1_IOMUX                                      (IOMUX_PINCM35)
#define GPIO_PWM_L_C1_IOMUX_FUNC                     IOMUX_PINCM35_PF_TIMG0_CCP1
#define GPIO_PWM_L_C1_IDX                                    DL_TIMER_CC_1_INDEX



/* Defines for HC_06 */
#define HC_06_INST                                                         UART2
#define HC_06_INST_FREQUENCY                                            32000000
#define HC_06_INST_IRQHandler                                   UART2_IRQHandler
#define HC_06_INST_INT_IRQN                                       UART2_INT_IRQn
#define GPIO_HC_06_RX_PORT                                                 GPIOA
#define GPIO_HC_06_TX_PORT                                                 GPIOA
#define GPIO_HC_06_RX_PIN                                         DL_GPIO_PIN_22
#define GPIO_HC_06_TX_PIN                                         DL_GPIO_PIN_21
#define GPIO_HC_06_IOMUX_RX                                      (IOMUX_PINCM47)
#define GPIO_HC_06_IOMUX_TX                                      (IOMUX_PINCM46)
#define GPIO_HC_06_IOMUX_RX_FUNC                       IOMUX_PINCM47_PF_UART2_RX
#define GPIO_HC_06_IOMUX_TX_FUNC                       IOMUX_PINCM46_PF_UART2_TX
#define HC_06_BAUD_RATE                                                   (9600)
#define HC_06_IBRD_32_MHZ_9600_BAUD                                        (208)
#define HC_06_FBRD_32_MHZ_9600_BAUD                                         (21)





/* Port definition for Pin Group BSL */
#define BSL_PORT                                                         (GPIOA)

/* Defines for PIN_18: GPIOA.18 with pinCMx 40 on package pin 33 */
#define BSL_PIN_18_PIN                                          (DL_GPIO_PIN_18)
#define BSL_PIN_18_IOMUX                                         (IOMUX_PINCM40)
/* Port definition for Pin Group I2C */
#define I2C_PORT                                                         (GPIOA)

/* Defines for SDA: GPIOA.28 with pinCMx 3 on package pin 3 */
#define I2C_SDA_PIN                                             (DL_GPIO_PIN_28)
#define I2C_SDA_IOMUX                                             (IOMUX_PINCM3)
/* Defines for SCL: GPIOA.31 with pinCMx 6 on package pin 5 */
#define I2C_SCL_PIN                                             (DL_GPIO_PIN_31)
#define I2C_SCL_IOMUX                                             (IOMUX_PINCM6)
/* Defines for STBY: GPIOA.12 with pinCMx 34 on package pin 27 */
#define TB6612_STBY_PORT                                                 (GPIOA)
#define TB6612_STBY_PIN                                         (DL_GPIO_PIN_12)
#define TB6612_STBY_IOMUX                                        (IOMUX_PINCM34)
/* Defines for AIN1: GPIOB.6 with pinCMx 23 on package pin 20 */
#define TB6612_AIN1_PORT                                                 (GPIOB)
#define TB6612_AIN1_PIN                                          (DL_GPIO_PIN_6)
#define TB6612_AIN1_IOMUX                                        (IOMUX_PINCM23)
/* Defines for AIN2: GPIOB.7 with pinCMx 24 on package pin 21 */
#define TB6612_AIN2_PORT                                                 (GPIOB)
#define TB6612_AIN2_PIN                                          (DL_GPIO_PIN_7)
#define TB6612_AIN2_IOMUX                                        (IOMUX_PINCM24)
/* Defines for BIN1: GPIOA.8 with pinCMx 19 on package pin 16 */
#define TB6612_BIN1_PORT                                                 (GPIOA)
#define TB6612_BIN1_PIN                                          (DL_GPIO_PIN_8)
#define TB6612_BIN1_IOMUX                                        (IOMUX_PINCM19)
/* Defines for BIN2: GPIOA.9 with pinCMx 20 on package pin 17 */
#define TB6612_BIN2_PORT                                                 (GPIOA)
#define TB6612_BIN2_PIN                                          (DL_GPIO_PIN_9)
#define TB6612_BIN2_IOMUX                                        (IOMUX_PINCM20)
/* Defines for L_A: GPIOB.24 with pinCMx 52 on package pin 42 */
#define ENCODER_L_A_PORT                                                 (GPIOB)
#define ENCODER_L_A_PIN                                         (DL_GPIO_PIN_24)
#define ENCODER_L_A_IOMUX                                        (IOMUX_PINCM52)
/* Defines for L_B: GPIOB.20 with pinCMx 48 on package pin 41 */
#define ENCODER_L_B_PORT                                                 (GPIOB)
// pins affected by this interrupt request:["L_B"]
#define ENCODER_GPIOB_INT_IRQN                                  (GPIOB_INT_IRQn)
#define ENCODER_GPIOB_INT_IIDX                  (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define ENCODER_L_B_IIDX                                    (DL_GPIO_IIDX_DIO20)
#define ENCODER_L_B_PIN                                         (DL_GPIO_PIN_20)
#define ENCODER_L_B_IOMUX                                        (IOMUX_PINCM48)
/* Defines for R_A: GPIOA.15 with pinCMx 37 on package pin 30 */
#define ENCODER_R_A_PORT                                                 (GPIOA)
#define ENCODER_R_A_PIN                                         (DL_GPIO_PIN_15)
#define ENCODER_R_A_IOMUX                                        (IOMUX_PINCM37)
/* Defines for R_B: GPIOA.16 with pinCMx 38 on package pin 31 */
#define ENCODER_R_B_PORT                                                 (GPIOA)
// pins affected by this interrupt request:["R_B"]
#define ENCODER_GPIOA_INT_IRQN                                  (GPIOA_INT_IRQn)
#define ENCODER_GPIOA_INT_IIDX                  (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define ENCODER_R_B_IIDX                                    (DL_GPIO_IIDX_DIO16)
#define ENCODER_R_B_PIN                                         (DL_GPIO_PIN_16)
#define ENCODER_R_B_IOMUX                                        (IOMUX_PINCM38)






#define CPUCLK_FREQ                                                     32000000


/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_PWM_R_init(void);
void SYSCFG_DL_PWM_L_init(void);
void SYSCFG_DL_HC_06_init(void);

void SYSCFG_DL_SYSTICK_init(void);

void SYSCFG_DL_SYSCTL_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */

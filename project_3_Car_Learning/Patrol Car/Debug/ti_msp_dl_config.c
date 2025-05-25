/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 *  ============ ti_msp_dl_config.c =============
 *  Configured MSPM0 DriverLib module definitions
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */

#include "ti_msp_dl_config.h"

DL_TimerA_backupConfig gPWM_BuzzerBackup;
DL_TimerG_backupConfig gTIMER_Encoder_ReadBackup;
DL_TimerA_backupConfig gTIMER_Tracking_ReadBackup;

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform any initialization needed before using any board APIs
 */
SYSCONFIG_WEAK void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_PWM_R_init();
    SYSCFG_DL_PWM_L_init();
    SYSCFG_DL_PWM_Buzzer_init();
    SYSCFG_DL_TIMER_Encoder_Read_init();
    SYSCFG_DL_TIMER_Tracking_Read_init();
    SYSCFG_DL_HC_06_init();
    SYSCFG_DL_SYSTICK_init();
    SYSCFG_DL_SYSCTL_init();
    /* Ensure backup structures have no valid state */
	gPWM_BuzzerBackup.backupRdy 	= false;
	gTIMER_Encoder_ReadBackup.backupRdy 	= false;
	gTIMER_Tracking_ReadBackup.backupRdy 	= false;


}
/*
 * User should take care to save and restore register configuration in application.
 * See Retention Configuration section for more details.
 */
SYSCONFIG_WEAK bool SYSCFG_DL_saveConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_saveConfiguration(PWM_Buzzer_INST, &gPWM_BuzzerBackup);
	retStatus &= DL_TimerG_saveConfiguration(TIMER_Encoder_Read_INST, &gTIMER_Encoder_ReadBackup);
	retStatus &= DL_TimerA_saveConfiguration(TIMER_Tracking_Read_INST, &gTIMER_Tracking_ReadBackup);

    return retStatus;
}


SYSCONFIG_WEAK bool SYSCFG_DL_restoreConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_restoreConfiguration(PWM_Buzzer_INST, &gPWM_BuzzerBackup, false);
	retStatus &= DL_TimerG_restoreConfiguration(TIMER_Encoder_Read_INST, &gTIMER_Encoder_ReadBackup, false);
	retStatus &= DL_TimerA_restoreConfiguration(TIMER_Tracking_Read_INST, &gTIMER_Tracking_ReadBackup, false);

    return retStatus;
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_TimerG_reset(PWM_R_INST);
    DL_TimerG_reset(PWM_L_INST);
    DL_TimerA_reset(PWM_Buzzer_INST);
    DL_TimerG_reset(TIMER_Encoder_Read_INST);
    DL_TimerA_reset(TIMER_Tracking_Read_INST);
    DL_UART_Main_reset(HC_06_INST);

    DL_MathACL_reset(MATHACL);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_TimerG_enablePower(PWM_R_INST);
    DL_TimerG_enablePower(PWM_L_INST);
    DL_TimerA_enablePower(PWM_Buzzer_INST);
    DL_TimerG_enablePower(TIMER_Encoder_Read_INST);
    DL_TimerA_enablePower(TIMER_Tracking_Read_INST);
    DL_UART_Main_enablePower(HC_06_INST);

    DL_MathACL_enablePower(MATHACL);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_R_C0_IOMUX,GPIO_PWM_R_C0_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_R_C0_PORT, GPIO_PWM_R_C0_PIN);
    
	DL_GPIO_initPeripheralOutputFunctionFeatures(
		 GPIO_PWM_R_C0_IOMUX, GPIO_PWM_R_C0_IOMUX_FUNC,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_L_C1_IOMUX,GPIO_PWM_L_C1_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_L_C1_PORT, GPIO_PWM_L_C1_PIN);
    
	DL_GPIO_initPeripheralOutputFunctionFeatures(
		 GPIO_PWM_L_C1_IOMUX, GPIO_PWM_L_C1_IOMUX_FUNC,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_Buzzer_C0_IOMUX,GPIO_PWM_Buzzer_C0_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_Buzzer_C0_PORT, GPIO_PWM_Buzzer_C0_PIN);

    DL_GPIO_initPeripheralOutputFunction(
        GPIO_HC_06_IOMUX_TX, GPIO_HC_06_IOMUX_TX_FUNC);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_HC_06_IOMUX_RX, GPIO_HC_06_IOMUX_RX_FUNC);

    DL_GPIO_initDigitalOutputFeatures(BSL_PIN_18_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
		 DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);

    DL_GPIO_initDigitalOutput(I2C_SDA_IOMUX);

    DL_GPIO_initDigitalOutputFeatures(I2C_SCL_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);

    DL_GPIO_initDigitalOutput(TB6612_STBY_IOMUX);

    DL_GPIO_initDigitalOutput(TB6612_AIN1_IOMUX);

    DL_GPIO_initDigitalOutput(TB6612_AIN2_IOMUX);

    DL_GPIO_initDigitalOutput(TB6612_BIN1_IOMUX);

    DL_GPIO_initDigitalOutput(TB6612_BIN2_IOMUX);

    DL_GPIO_initDigitalInputFeatures(GPIO_Encoder_L_L_A_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(GPIO_Encoder_L_L_B_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(GPIO_Encoder_R_R_B_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(GPIO_Encoder_R_R_A_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(Tracking_HW_5_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(Tracking_HW_4_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(Tracking_HW_3_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(Tracking_HW_2_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(Tracking_HW_1_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_clearPins(GPIOA, BSL_PIN_18_PIN);
    DL_GPIO_setPins(GPIOA, I2C_SDA_PIN |
		I2C_SCL_PIN |
		TB6612_STBY_PIN |
		TB6612_BIN1_PIN |
		TB6612_BIN2_PIN);
    DL_GPIO_enableOutput(GPIOA, BSL_PIN_18_PIN |
		I2C_SDA_PIN |
		I2C_SCL_PIN |
		TB6612_STBY_PIN |
		TB6612_BIN1_PIN |
		TB6612_BIN2_PIN);
    DL_GPIO_setLowerPinsPolarity(GPIOA, DL_GPIO_PIN_15_EDGE_RISE);
    DL_GPIO_setUpperPinsPolarity(GPIOA, DL_GPIO_PIN_16_EDGE_RISE);
    DL_GPIO_setLowerPinsInputFilter(GPIOA, DL_GPIO_PIN_15_INPUT_FILTER_8_CYCLES);
    DL_GPIO_setUpperPinsInputFilter(GPIOA, DL_GPIO_PIN_16_INPUT_FILTER_8_CYCLES);
    DL_GPIO_clearInterruptStatus(GPIOA, GPIO_Encoder_R_R_B_PIN |
		GPIO_Encoder_R_R_A_PIN);
    DL_GPIO_enableInterrupt(GPIOA, GPIO_Encoder_R_R_B_PIN |
		GPIO_Encoder_R_R_A_PIN);
    DL_GPIO_setPins(GPIOB, TB6612_AIN1_PIN |
		TB6612_AIN2_PIN);
    DL_GPIO_enableOutput(GPIOB, TB6612_AIN1_PIN |
		TB6612_AIN2_PIN);
    DL_GPIO_setUpperPinsPolarity(GPIOB, DL_GPIO_PIN_24_EDGE_RISE |
		DL_GPIO_PIN_20_EDGE_RISE);
    DL_GPIO_setUpperPinsInputFilter(GPIOB, DL_GPIO_PIN_24_INPUT_FILTER_8_CYCLES |
		DL_GPIO_PIN_20_INPUT_FILTER_8_CYCLES);
    DL_GPIO_clearInterruptStatus(GPIOB, GPIO_Encoder_L_L_A_PIN |
		GPIO_Encoder_L_L_B_PIN);
    DL_GPIO_enableInterrupt(GPIOB, GPIO_Encoder_L_L_A_PIN |
		GPIO_Encoder_L_L_B_PIN);

}


/*
 * Timer clock configuration to be sourced by  / 8 (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   4000000 Hz = 4000000 Hz / (8 * (0 + 1))
 */
static const DL_TimerG_ClockConfig gPWM_RClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale = 0U
};

static const DL_TimerG_PWMConfig gPWM_RConfig = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN_UP,
    .period = 1000,
    .isTimerWithFourCC = false,
    .startTimer = DL_TIMER_START,
};

SYSCONFIG_WEAK void SYSCFG_DL_PWM_R_init(void) {

    DL_TimerG_setClockConfig(
        PWM_R_INST, (DL_TimerG_ClockConfig *) &gPWM_RClockConfig);

    DL_TimerG_initPWMMode(
        PWM_R_INST, (DL_TimerG_PWMConfig *) &gPWM_RConfig);

    // Set Counter control to the smallest CC index being used
    DL_TimerG_setCounterControl(PWM_R_INST,DL_TIMER_CZC_CCCTL0_ZCOND,DL_TIMER_CAC_CCCTL0_ACOND,DL_TIMER_CLC_CCCTL0_LCOND);

    DL_TimerG_setCaptureCompareOutCtl(PWM_R_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_0_INDEX);

    DL_TimerG_setCaptCompUpdateMethod(PWM_R_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
    DL_TimerG_setCaptureCompareValue(PWM_R_INST, 0, DL_TIMER_CC_0_INDEX);

    DL_TimerG_enableClock(PWM_R_INST);


    
    DL_TimerG_setCCPDirection(PWM_R_INST , DL_TIMER_CC0_OUTPUT );


}
/*
 * Timer clock configuration to be sourced by  / 8 (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   4000000 Hz = 4000000 Hz / (8 * (0 + 1))
 */
static const DL_TimerG_ClockConfig gPWM_LClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale = 0U
};

static const DL_TimerG_PWMConfig gPWM_LConfig = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN_UP,
    .period = 1000,
    .isTimerWithFourCC = false,
    .startTimer = DL_TIMER_START,
};

SYSCONFIG_WEAK void SYSCFG_DL_PWM_L_init(void) {

    DL_TimerG_setClockConfig(
        PWM_L_INST, (DL_TimerG_ClockConfig *) &gPWM_LClockConfig);

    DL_TimerG_initPWMMode(
        PWM_L_INST, (DL_TimerG_PWMConfig *) &gPWM_LConfig);

    // Set Counter control to the smallest CC index being used
    DL_TimerG_setCounterControl(PWM_L_INST,DL_TIMER_CZC_CCCTL1_ZCOND,DL_TIMER_CAC_CCCTL1_ACOND,DL_TIMER_CLC_CCCTL1_LCOND);

    DL_TimerG_setCaptureCompareOutCtl(PWM_L_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_1_INDEX);

    DL_TimerG_setCaptCompUpdateMethod(PWM_L_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_1_INDEX);
    DL_TimerG_setCaptureCompareValue(PWM_L_INST, 0, DL_TIMER_CC_1_INDEX);

    DL_TimerG_enableClock(PWM_L_INST);


    
    DL_TimerG_setCCPDirection(PWM_L_INST , DL_TIMER_CC1_OUTPUT );


}
/*
 * Timer clock configuration to be sourced by  / 1 (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   4000000 Hz = 4000000 Hz / (1 * (0 + 1))
 */
static const DL_TimerA_ClockConfig gPWM_BuzzerClockConfig = {
    .clockSel = DL_TIMER_CLOCK_MFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 0U
};

static const DL_TimerA_PWMConfig gPWM_BuzzerConfig = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN_UP,
    .period = 1000,
    .isTimerWithFourCC = true,
    .startTimer = DL_TIMER_START,
};

SYSCONFIG_WEAK void SYSCFG_DL_PWM_Buzzer_init(void) {

    DL_TimerA_setClockConfig(
        PWM_Buzzer_INST, (DL_TimerA_ClockConfig *) &gPWM_BuzzerClockConfig);

    DL_TimerA_initPWMMode(
        PWM_Buzzer_INST, (DL_TimerA_PWMConfig *) &gPWM_BuzzerConfig);

    // Set Counter control to the smallest CC index being used
    DL_TimerA_setCounterControl(PWM_Buzzer_INST,DL_TIMER_CZC_CCCTL0_ZCOND,DL_TIMER_CAC_CCCTL0_ACOND,DL_TIMER_CLC_CCCTL0_LCOND);

    DL_TimerA_setCaptureCompareOutCtl(PWM_Buzzer_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_0_INDEX);

    DL_TimerA_setCaptCompUpdateMethod(PWM_Buzzer_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_0_INDEX);
    DL_TimerA_setCaptureCompareValue(PWM_Buzzer_INST, 0, DL_TIMER_CC_0_INDEX);

    DL_TimerA_enableClock(PWM_Buzzer_INST);


    
    DL_TimerA_setCCPDirection(PWM_Buzzer_INST , DL_TIMER_CC0_OUTPUT );


}



/*
 * Timer clock configuration to be sourced by BUSCLK /  (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   320000 Hz = 32000000 Hz / (1 * (99 + 1))
 */
static const DL_TimerG_ClockConfig gTIMER_Encoder_ReadClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 99U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_Encoder_Read_INST_LOAD_VALUE = (20 ms * 320000 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMER_Encoder_ReadTimerConfig = {
    .period     = TIMER_Encoder_Read_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_START,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_Encoder_Read_init(void) {

    DL_TimerG_setClockConfig(TIMER_Encoder_Read_INST,
        (DL_TimerG_ClockConfig *) &gTIMER_Encoder_ReadClockConfig);

    DL_TimerG_initTimerMode(TIMER_Encoder_Read_INST,
        (DL_TimerG_TimerConfig *) &gTIMER_Encoder_ReadTimerConfig);
    DL_TimerG_enableInterrupt(TIMER_Encoder_Read_INST , DL_TIMERG_INTERRUPT_ZERO_EVENT);
	NVIC_SetPriority(TIMER_Encoder_Read_INST_INT_IRQN, 1);
    DL_TimerG_enableClock(TIMER_Encoder_Read_INST);





}

/*
 * Timer clock configuration to be sourced by BUSCLK /  (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   8000000 Hz = 32000000 Hz / (1 * (3 + 1))
 */
static const DL_TimerA_ClockConfig gTIMER_Tracking_ReadClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 3U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_Tracking_Read_INST_LOAD_VALUE = (5 ms * 8000000 Hz) - 1
 */
static const DL_TimerA_TimerConfig gTIMER_Tracking_ReadTimerConfig = {
    .period     = TIMER_Tracking_Read_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_START,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_Tracking_Read_init(void) {

    DL_TimerA_setClockConfig(TIMER_Tracking_Read_INST,
        (DL_TimerA_ClockConfig *) &gTIMER_Tracking_ReadClockConfig);

    DL_TimerA_initTimerMode(TIMER_Tracking_Read_INST,
        (DL_TimerA_TimerConfig *) &gTIMER_Tracking_ReadTimerConfig);
    DL_TimerA_enableInterrupt(TIMER_Tracking_Read_INST , DL_TIMERA_INTERRUPT_ZERO_EVENT);
	NVIC_SetPriority(TIMER_Tracking_Read_INST_INT_IRQN, 1);
    DL_TimerA_enableClock(TIMER_Tracking_Read_INST);





}



static const DL_UART_Main_ClockConfig gHC_06ClockConfig = {
    .clockSel    = DL_UART_MAIN_CLOCK_BUSCLK,
    .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1
};

static const DL_UART_Main_Config gHC_06Config = {
    .mode        = DL_UART_MAIN_MODE_NORMAL,
    .direction   = DL_UART_MAIN_DIRECTION_TX_RX,
    .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,
    .parity      = DL_UART_MAIN_PARITY_NONE,
    .wordLength  = DL_UART_MAIN_WORD_LENGTH_8_BITS,
    .stopBits    = DL_UART_MAIN_STOP_BITS_ONE
};

SYSCONFIG_WEAK void SYSCFG_DL_HC_06_init(void)
{
    DL_UART_Main_setClockConfig(HC_06_INST, (DL_UART_Main_ClockConfig *) &gHC_06ClockConfig);

    DL_UART_Main_init(HC_06_INST, (DL_UART_Main_Config *) &gHC_06Config);
    /*
     * Configure baud rate by setting oversampling and baud rate divisors.
     *  Target baud rate: 9600
     *  Actual baud rate: 9600.24
     */
    DL_UART_Main_setOversampling(HC_06_INST, DL_UART_OVERSAMPLING_RATE_16X);
    DL_UART_Main_setBaudRateDivisor(HC_06_INST, HC_06_IBRD_32_MHZ_9600_BAUD, HC_06_FBRD_32_MHZ_9600_BAUD);


    /* Configure Interrupts */
    DL_UART_Main_enableInterrupt(HC_06_INST,
                                 DL_UART_MAIN_INTERRUPT_RX);
    /* Setting the Interrupt Priority */
    NVIC_SetPriority(HC_06_INST_INT_IRQN, 1);


    DL_UART_Main_enable(HC_06_INST);
}

SYSCONFIG_WEAK void SYSCFG_DL_SYSTICK_init(void)
{
    /*
     * Initializes the SysTick period to 1.00 ms,
     * enables the interrupt, and starts the SysTick Timer
     */
    DL_SYSTICK_config(32000);
}


SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    
	DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
	/* Set default configuration */
	DL_SYSCTL_disableHFXT();
	DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_enableMFCLK();
    /* INT_GROUP1 Priority */
    NVIC_SetPriority(GPIOA_INT_IRQn, 1);

}



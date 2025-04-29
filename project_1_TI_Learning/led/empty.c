/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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

#include "ti_msp_dl_config.h"
#include "delay.h"
#include "key.h"

void key_cb(void)
{
    DL_GPIO_togglePins(LED_PORT, LED_PIN_14_PIN);
}

void key_cl_cb(uint8_t clicks)
{
    DL_GPIO_togglePins(LED_PORT, LED_PIN_14_PIN);
}
int main(void)
{
    uint32_t crurrent;
    SYSCFG_DL_init();
    Key_InitTypedef Key_InitStruct;
    Key_InitStruct.key_port = KEY_PORT;
    Key_InitStruct.key_pin = KEY_PIN_18_PIN;
    Key_InitStruct.key_pressed_cb = key_cb;
    Key_InitStruct.key_long_pressed_cb = key_cl_cb;
    Key_InitStruct.key_released_cb = key_cb;
    Key_TypeDef Key1;
    Key_Init(&Key1, &Key_InitStruct);
    while (1)
    {
        Key_Proc(&Key1);
        
    }
}



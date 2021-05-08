/***************************************************************************//**
 * @file main.c
 * @brief main() function.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include "playground.h"
#include "em_usart.h"
#include "em_cmu.h"
#include "movement.h"

extern bool UsartFlag;
extern uint8_t UsartData;
void Usart_init(void) {
	// Enable clock for GPIO
	CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;
	// Set PF7 to high
	GPIO_PinModeSet(gpioPortF, 7, gpioModePushPull, 1);
	// Configure UART0
	// (Now use the "emlib" functions whenever possible.)
	// Enable clock for UART0
	CMU_ClockEnable(cmuClock_UART0, true);
	CMU_ClockEnable(cmuClock_GPIO, true);
	// Initialize UART0 (115200 Baud, 8N1 frame format)
	// To initialize the UART0, we need a structure to hold
	// configuration data. It is a good practice to initialize it with
	// default values, then set individual parameters only where needed
	USART_InitAsync_TypeDef UART0_init = USART_INITASYNC_DEFAULT;
	USART_InitAsync(UART0, &UART0_init);
	// USART0: see in efm32ggf1024.h
	// Set TX (PE0) and RX (PE1) pins as push-pull output and input resp.
	// DOUT for TX is 1, as it is the idle state for UART communication
	GPIO_PinModeSet(gpioPortE, 0, gpioModePushPull, 1);
	// DOUT for RX is 0, as DOUT can enable a glitch filter for inputs,
	// and we are fine without such a filter
	GPIO_PinModeSet(gpioPortE, 1, gpioModeInput, 0);
	// Use PE0 as TX and PE1 as RX (Location 1, see datasheet (not refman))
	// Enable both RX and TX for routing
	//USART_Enable(UART0,usartEnable);
	UART0->ROUTE |= UART_ROUTE_LOCATION_LOC1;
	// Select "Location 1" as the routing configuration
	UART0->ROUTE |= UART_ROUTE_TXPEN | UART_ROUTE_RXPEN;
	//USART_Tx(UART0, '+');
	USART_IntClear(UART0, USART_IEN_RXDATAV);
	USART_IntEnable(UART0, USART_IEN_RXDATAV);
	NVIC_ClearPendingIRQ(UART0_RX_IRQn);
	NVIC_EnableIRQ(UART0_RX_IRQn);
}

int main(void)
{
	SegmentLCD_Init(false);
	SegmentLCD_AllOff();
	Usart_init();
	InitPlayGround();
	vTaskStartScheduler();
}

void UART0_RX_IRQHandler(void){
	UsartFlag = true;
	UsartData = USART_Rx(UART0);
	USART_Tx(UART0,UsartData);
	USART_IntClear(UART0, USART_IEN_RXDATAV);
}

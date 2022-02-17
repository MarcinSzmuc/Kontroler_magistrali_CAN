/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */

#include "stm32f1xx.h"
#include <string.h>
#include "Funkcje.h"

GPIO_InitTypeDef gpio;
UART_HandleTypeDef uart;
CAN_HandleTypeDef can;
CAN_FilterTypeDef canfilter;
CAN_RxHeaderTypeDef rxHeader;
CAN_TxHeaderTypeDef txHeader;
I2C_HandleTypeDef i2c;

uint32_t canMailbox = 0;

int main(void) {

	SystemCoreClock = 8000000;
	HAL_Init();
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;
	__HAL_RCC_CAN1_CLK_ENABLE()
	;
	__HAL_RCC_USART1_CLK_ENABLE()
	;
	 __HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_AFIO_REMAP_CAN1_2()
	;
	__HAL_RCC_I2C1_CLK_ENABLE()
	;

	//USART

	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pin = GPIO_PIN_9;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Mode = GPIO_MODE_AF_INPUT;
	gpio.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOA, &gpio);

	uart.Instance = USART1;
	uart.Init.BaudRate = 115200;
	uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart.Init.Parity = UART_PARITY_NONE;
	uart.Init.StopBits = UART_STOPBITS_1;
	uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart.Init.OverSampling = UART_OVERSAMPLING_16;
	uart.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&uart);

	//CAN

	gpio.Pin = GPIO_PIN_8;
		gpio.Mode = GPIO_MODE_INPUT;
		gpio.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &gpio);

		gpio.Pin = GPIO_PIN_9;
		gpio.Mode = GPIO_MODE_AF_PP;
		gpio.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &gpio);

//I2C pamiec
	gpio.Mode = GPIO_MODE_AF_OD;
	gpio.Pin = GPIO_PIN_6 | GPIO_PIN_7;		// SCL, SDA
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &gpio);

	i2c.Instance = I2C1;
	i2c.Init.ClockSpeed = 100000;
	i2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
	i2c.Init.OwnAddress1 = 0xff;
	i2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	i2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2c.Init.OwnAddress2 = 0xff;
	i2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&i2c);

	wyslij_string("POLACZONO\n\r");
	wczytaj_ostatnia_konfiguracje();

	while (1) {



		menu_can();



	}
}

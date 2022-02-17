/*
 * Funkcje.C
 *
 *  Created on: 11.10.2021
 *      Author: SaintHunter
 */
#include "Funkcje.h"
#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef uart;
extern CAN_HandleTypeDef can;
extern CAN_FilterTypeDef canfilter;
extern CAN_RxHeaderTypeDef rxHeader;
extern CAN_TxHeaderTypeDef txHeader;
extern uint32_t canMailbox;
extern I2C_HandleTypeDef i2c;

uint8_t canTx1[8] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
uint8_t canTx2[8] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
uint8_t canTx3[8] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
uint8_t preset_canTx[8] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
uint8_t preset_canRx[8] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
uint8_t preset_canTx1[8] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
uint8_t preset_canTx2[8] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
uint8_t preset_canTx3[8] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };

uint8_t preset_canRx1[8] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
uint8_t preset_canRx2[8] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
uint8_t preset_canRx3[8] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };

CAN_RxHeaderTypeDef rxHeader1;
CAN_RxHeaderTypeDef rxHeader2;
CAN_TxHeaderTypeDef txHeader1;
CAN_TxHeaderTypeDef txHeader2;

void wyslij_znak(char znak) {
	if (znak == 0)
		znak = '0';
	else if (znak == 1)
		znak = '1';
	else if (znak == 2)
		znak = '2';
	else if (znak == 3)
		znak = '3';
	else if (znak == 4)
		znak = '4';
	else if (znak == 5)
		znak = '5';
	else if (znak == 6)
		znak = '6';
	else if (znak == 7)
		znak = '7';
	else if (znak == 8)
		znak = '8';
	else if (znak == 9)
		znak = '9';
	else if (znak == 10)
		znak = 'A';
	else if (znak == 11)
		znak = 'B';
	else if (znak == 12)
		znak = 'C';
	else if (znak == 13)
		znak = 'D';
	else if (znak == 14)
		znak = 'E';
	else if (znak == 15)
		znak = 'F';

	HAL_UART_Transmit(&uart, (uint8_t*) &znak, 1, 1000);
}

void wyslij_string(char* string) {
	HAL_UART_Transmit(&uart, (uint8_t*) string, strlen(string), 1000);
}
void send_char(char c) {
	HAL_UART_Transmit(&uart, (uint8_t*) &c, 1, 1000);
}
int __io_putchar(int ch) {
	if (ch == '\n')
		send_char('\r');
	send_char(ch);
	return ch;
}

uint8_t pobierz_klawisz() {

	uint8_t klawisz = ' ';

	HAL_UART_Receive(&uart, &klawisz, 1, 1);
	if (klawisz == '0')
		klawisz = 0;
	else if (klawisz == '1')
		klawisz = 1;
	else if (klawisz == '2')
		klawisz = 2;
	else if (klawisz == '3')
		klawisz = 3;
	else if (klawisz == '4')
		klawisz = 4;
	else if (klawisz == '5')
		klawisz = 5;
	else if (klawisz == '6')
		klawisz = 6;
	else if (klawisz == '7')
		klawisz = 7;
	else if (klawisz == '8')
		klawisz = 8;
	else if (klawisz == '9')
		klawisz = 9;
	else if (klawisz == 'A' || klawisz == 'a')
		klawisz = 10;
	else if (klawisz == 'B' || klawisz == 'b')
		klawisz = 11;
	else if (klawisz == 'C' || klawisz == 'c')
		klawisz = 12;
	else if (klawisz == 'D' || klawisz == 'd')
		klawisz = 13;
	else if (klawisz == 'E' || klawisz == 'e')
		klawisz = 14;
	else if (klawisz == 'F' || klawisz == 'f')
		klawisz = 15;
	else
		klawisz = ' ';

	return klawisz;

}

void menu_predkosc() {

	uint8_t bufor[4] = { 0 };
	uint32_t Prescaler = ' ';
	uint8_t can_BRP1 = ' ';
	uint8_t can_BRP2 = ' ';
	uint8_t can_BRP3 = ' ';
	uint8_t can_BRP4 = ' ';

	uint32_t TimeSeg1 = ' ';
	int8_t can_tseg1a = ' ';
	int8_t can_tseg1b = ' ';

	uint32_t TimeSeg2 = ' ';
	uint32_t SyncJumpWidth = ' ';
	uint8_t wybor_predkosci = 1;
	uint8_t lista_predkosci = 0;
	uint8_t zapis = ' ';

	HAL_CAN_Stop(&can);
	wyslij_string(
			"Wybierz szybkosc: \n\r1.100 kbit/s\n\r2.500 kbit/s\n\r3.1000 kbit/s\n\r4.Wybierz wlasna szybkosc\n\r");
	while (lista_predkosci > 4 || lista_predkosci == 0)
		lista_predkosci = pobierz_klawisz();
	wyslij_znak(lista_predkosci);
	wyslij_string("\n\r");
	while (lista_predkosci < 5) {

		switch (lista_predkosci) {
		case 1:

			can.Init.SyncJumpWidth = CAN_SJW_1TQ;
			can.Init.Prescaler = 8;
			can.Init.TimeSeg1 = CAN_BS1_8TQ;
			can.Init.TimeSeg2 = CAN_BS2_1TQ;

			wyslij_string(
					"Zapisac konfiguracje do pamieci?\n\r1 - tak\n\r2 - nie\n\r");
			while (zapis != 1 && zapis != 2)
				zapis = pobierz_klawisz();
			wyslij_znak(zapis);
			wyslij_string("\n\r");
			if (zapis == 1) {
				zapisz_do_pamieci(0, can.Init.SyncJumpWidth);
				zapisz_do_pamieci(4, can.Init.Prescaler);
				zapisz_do_pamieci(8, can.Init.TimeSeg1);
				zapisz_do_pamieci(12, can.Init.TimeSeg2);
			}

			lista_predkosci = 5;
			break;
		case 2:

			can.Init.SyncJumpWidth = CAN_SJW_1TQ;
			can.Init.Prescaler = 1;
			can.Init.TimeSeg1 = CAN_BS1_13TQ;
			can.Init.TimeSeg2 = CAN_BS2_2TQ;

			wyslij_string(
					"Zapisac konfiguracje do pamieci?\n\r1 - tak\n\r2 - nie\n\r");
			while (zapis != 1 && zapis != 2)
				zapis = pobierz_klawisz();
			wyslij_znak(zapis);
			wyslij_string("\n\r");
			if (zapis == 1) {
				zapisz_do_pamieci(0, can.Init.SyncJumpWidth);
				zapisz_do_pamieci(4, can.Init.Prescaler);
				zapisz_do_pamieci(8, can.Init.TimeSeg1);
				zapisz_do_pamieci(12, can.Init.TimeSeg2);
			}

			lista_predkosci = 5;
			break;
		case 3:

			can.Init.SyncJumpWidth = CAN_SJW_1TQ;
			can.Init.Prescaler = 1;
			can.Init.TimeSeg1 = CAN_BS1_6TQ;
			can.Init.TimeSeg2 = CAN_BS2_1TQ;

			wyslij_string(
					"Zapisac konfiguracje do pamieci?\n\r1 - tak\n\r2 - nie\n\r");
			while (zapis != 1 && zapis != 2)
				zapis = pobierz_klawisz();
			wyslij_znak(zapis);
			wyslij_string("\n\r");
			if (zapis == 1) {
				zapisz_do_pamieci(0, can.Init.SyncJumpWidth);
				zapisz_do_pamieci(4, can.Init.Prescaler);
				zapisz_do_pamieci(8, can.Init.TimeSeg1);
				zapisz_do_pamieci(12, can.Init.TimeSeg2);
			}

			lista_predkosci = 5;
			break;
		case 4:
			wyslij_string(
					"clk zegara = 8MHz.\n\rSzybkosc CAN = 1/(BRP/clk * (TimeSeg1+TimeSeg2+1))\n\r");
			while (wybor_predkosci < 8) {

				switch (wybor_predkosci) {

				case 1:
					wyslij_string(
							"Podaj liczbe SyncJumpWidth max 4 (1 cyfra decymalna)\n\r");
					while (SyncJumpWidth > 4 || SyncJumpWidth == 0)
						SyncJumpWidth = pobierz_klawisz();

					bufor[0] = SyncJumpWidth;
					wyslij_znak(bufor[0]);
					wyslij_string("\n\r");
					if (SyncJumpWidth == 1)
						can.Init.SyncJumpWidth = CAN_SJW_1TQ;
					else if (SyncJumpWidth == 2)
						can.Init.SyncJumpWidth = CAN_SJW_2TQ;
					else if (SyncJumpWidth == 3)
						can.Init.SyncJumpWidth = CAN_SJW_3TQ;
					else if (SyncJumpWidth == 4)
						can.Init.SyncJumpWidth = CAN_SJW_4TQ;

					wybor_predkosci = 2;
					break;

				case 2:
					wyslij_string(
							"Podaj Prescaler max. 1024. (4 cyfry decymalne)\n\r");
					while (can_BRP1 > 9)
						can_BRP1 = pobierz_klawisz();

					bufor[0] = can_BRP1;
					wyslij_znak(bufor[0]);

					while (can_BRP2 > 9)
						can_BRP2 = pobierz_klawisz();
					bufor[1] = can_BRP2;
					wyslij_znak(bufor[1]);

					while (can_BRP3 > 9)
						can_BRP3 = pobierz_klawisz();
					bufor[2] = can_BRP3;
					wyslij_znak(bufor[2]);

					while (can_BRP4 > 9
							|| (can_BRP1 == 0 && can_BRP2 == 0 && can_BRP3 == 0
									&& can_BRP4 == 0))
						can_BRP4 = pobierz_klawisz();
					bufor[3] = can_BRP4;
					wyslij_znak(bufor[3]);
					wyslij_string("\n\r");

					Prescaler = can_BRP1 * 10 * 10 * 10 + can_BRP2 * 10 * 10
							+ can_BRP3 * 10 + can_BRP4;

					wybor_predkosci = 3;
					break;
				case 3:
					if (Prescaler > 1024) {
						wyslij_string(
								"predkosc zbyt duza wpisz liczbe z zakresu 0-1024\n\r");
						can_BRP1 = ' ';
						can_BRP2 = ' ';
						can_BRP3 = ' ';
						can_BRP4 = ' ';
						wybor_predkosci = 2;

					} else if (Prescaler <= 1024) {
						can.Init.Prescaler = Prescaler;
						wybor_predkosci = 4;
					}
					break;
				case 4:
					wyslij_string(
							"Podaj liczbe TimeSeg1 max 16 (2 cyfry decymalne)\n\r");
					while (can_tseg1a > 1)
						can_tseg1a = pobierz_klawisz();
					bufor[0] = can_tseg1a;
					wyslij_znak(bufor[0]);

					while (can_tseg1b > 9
							|| (can_tseg1a == 0 && can_tseg1b == 0)
							|| (can_tseg1b > 6 && can_tseg1a == 1))
						can_tseg1b = pobierz_klawisz();
					bufor[1] = can_tseg1b;
					wyslij_znak(bufor[1]);
					wyslij_string("\n\r");
					TimeSeg1 = can_tseg1a * 10 + can_tseg1b;

					if (TimeSeg1 == 1)
						can.Init.TimeSeg1 = CAN_BS1_1TQ;
					else if (TimeSeg1 == 2)
						can.Init.TimeSeg1 = CAN_BS1_2TQ;
					else if (TimeSeg1 == 3)
						can.Init.TimeSeg1 = CAN_BS1_3TQ;
					else if (TimeSeg1 == 4)
						can.Init.TimeSeg1 = CAN_BS1_4TQ;
					else if (TimeSeg1 == 5)
						can.Init.TimeSeg1 = CAN_BS1_5TQ;
					else if (TimeSeg1 == 6)
						can.Init.TimeSeg1 = CAN_BS1_6TQ;
					else if (TimeSeg1 == 7)
						can.Init.TimeSeg1 = CAN_BS1_7TQ;
					else if (TimeSeg1 == 8)
						can.Init.TimeSeg1 = CAN_BS1_8TQ;
					else if (TimeSeg1 == 9)
						can.Init.TimeSeg1 = CAN_BS1_9TQ;
					else if (TimeSeg1 == 10)
						can.Init.TimeSeg1 = CAN_BS1_10TQ;
					else if (TimeSeg1 == 11)
						can.Init.TimeSeg1 = CAN_BS1_11TQ;
					else if (TimeSeg1 == 12)
						can.Init.TimeSeg1 = CAN_BS1_12TQ;
					else if (TimeSeg1 == 13)
						can.Init.TimeSeg1 = CAN_BS1_13TQ;
					else if (TimeSeg1 == 14)
						can.Init.TimeSeg1 = CAN_BS1_14TQ;
					else if (TimeSeg1 == 15)
						can.Init.TimeSeg1 = CAN_BS1_15TQ;
					else if (TimeSeg1 == 16)
						can.Init.TimeSeg1 = CAN_BS1_16TQ;

					wybor_predkosci = 6;

					break;
				case 6:
					wyslij_string(
							"Podaj liczbe TimeSeg2 max 8 (1 cyfra decymalna)\n\r");
					while (TimeSeg2 > 8 || TimeSeg2 == 0)
						TimeSeg2 = pobierz_klawisz();
					bufor[0] = TimeSeg2;
					wyslij_znak(bufor[0]);
					wyslij_string("\n\r");

					if (TimeSeg2 == 1)
						can.Init.TimeSeg2 = CAN_BS2_1TQ;
					else if (TimeSeg2 == 2)
						can.Init.TimeSeg2 = CAN_BS2_2TQ;
					else if (TimeSeg2 == 3)
						can.Init.TimeSeg2 = CAN_BS2_3TQ;
					else if (TimeSeg2 == 4)
						can.Init.TimeSeg2 = CAN_BS2_4TQ;
					else if (TimeSeg2 == 5)
						can.Init.TimeSeg2 = CAN_BS2_5TQ;
					else if (TimeSeg2 == 6)
						can.Init.TimeSeg2 = CAN_BS2_6TQ;
					else if (TimeSeg2 == 7)
						can.Init.TimeSeg2 = CAN_BS2_7TQ;
					else if (TimeSeg2 == 8)
						can.Init.TimeSeg2 = CAN_BS2_8TQ;

					wybor_predkosci = 7;
					break;
				case 7:
					wyslij_string(
							"Zapisac konfiguracje do pamieci?\n\r1 - tak\n\r2 - nie\n\r");
					while (zapis != 1 && zapis != 2)
						zapis = pobierz_klawisz();
					wyslij_znak(zapis);
					wyslij_string("\n\r");
					if (zapis == 1) {
						zapisz_do_pamieci(0, can.Init.SyncJumpWidth);
						zapisz_do_pamieci(4, can.Init.Prescaler);
						zapisz_do_pamieci(8, can.Init.TimeSeg1);
						zapisz_do_pamieci(12, can.Init.TimeSeg2);
					}

					wybor_predkosci = 8;
					break;

				}

			}

			lista_predkosci = 5;

			break;
		}

	}

	HAL_CAN_Init(&can);
	HAL_CAN_Start(&can);
	wyslij_string("Wybrano szybkosc\n\r");

}
void wczytaj_ostatnia_konfiguracje() {

	canTx1[0] = wczytaj_z_pamieci8b(132);
	canTx1[1] = wczytaj_z_pamieci8b(135);
	canTx1[2] = wczytaj_z_pamieci8b(138);
	canTx1[3] = wczytaj_z_pamieci8b(141);
	canTx1[4] = wczytaj_z_pamieci8b(144);
	canTx1[5] = wczytaj_z_pamieci8b(147);
	canTx1[6] = wczytaj_z_pamieci8b(150);
	canTx1[7] = wczytaj_z_pamieci8b(153);

	canTx2[0] = wczytaj_z_pamieci8b(133);
	canTx2[1] = wczytaj_z_pamieci8b(136);
	canTx2[2] = wczytaj_z_pamieci8b(139);
	canTx2[3] = wczytaj_z_pamieci8b(142);
	canTx2[4] = wczytaj_z_pamieci8b(145);
	canTx2[5] = wczytaj_z_pamieci8b(148);
	canTx2[6] = wczytaj_z_pamieci8b(151);
	canTx2[7] = wczytaj_z_pamieci8b(154);

	canTx3[0] = wczytaj_z_pamieci8b(134);
	canTx3[1] = wczytaj_z_pamieci8b(137);
	canTx3[2] = wczytaj_z_pamieci8b(140);
	canTx3[3] = wczytaj_z_pamieci8b(143);
	canTx3[4] = wczytaj_z_pamieci8b(146);
	canTx3[5] = wczytaj_z_pamieci8b(149);
	canTx3[6] = wczytaj_z_pamieci8b(152);
	canTx3[7] = wczytaj_z_pamieci8b(155);

	preset_canRx1[0] = wczytaj_z_pamieci8b(156);
	preset_canRx1[1] = wczytaj_z_pamieci8b(159);
	preset_canRx1[2] = wczytaj_z_pamieci8b(162);
	preset_canRx1[3] = wczytaj_z_pamieci8b(165);
	preset_canRx1[4] = wczytaj_z_pamieci8b(168);
	preset_canRx1[5] = wczytaj_z_pamieci8b(171);
	preset_canRx1[6] = wczytaj_z_pamieci8b(174);
	preset_canRx1[7] = wczytaj_z_pamieci8b(177);

	preset_canRx2[0] = wczytaj_z_pamieci8b(157);
	preset_canRx2[1] = wczytaj_z_pamieci8b(160);
	preset_canRx2[2] = wczytaj_z_pamieci8b(163);
	preset_canRx2[3] = wczytaj_z_pamieci8b(166);
	preset_canRx2[4] = wczytaj_z_pamieci8b(169);
	preset_canRx2[5] = wczytaj_z_pamieci8b(172);
	preset_canRx2[6] = wczytaj_z_pamieci8b(175);
	preset_canRx2[7] = wczytaj_z_pamieci8b(178);

	preset_canRx3[0] = wczytaj_z_pamieci8b(158);
	preset_canRx3[1] = wczytaj_z_pamieci8b(161);
	preset_canRx3[2] = wczytaj_z_pamieci8b(164);
	preset_canRx3[3] = wczytaj_z_pamieci8b(167);
	preset_canRx3[4] = wczytaj_z_pamieci8b(170);
	preset_canRx3[5] = wczytaj_z_pamieci8b(173);
	preset_canRx3[6] = wczytaj_z_pamieci8b(176);
	preset_canRx3[7] = wczytaj_z_pamieci8b(179);

	preset_canTx1[0] = wczytaj_z_pamieci8b(180);
	preset_canTx1[1] = wczytaj_z_pamieci8b(183);
	preset_canTx1[2] = wczytaj_z_pamieci8b(186);
	preset_canTx1[3] = wczytaj_z_pamieci8b(189);
	preset_canTx1[4] = wczytaj_z_pamieci8b(192);
	preset_canTx1[5] = wczytaj_z_pamieci8b(195);
	preset_canTx1[6] = wczytaj_z_pamieci8b(198);
	preset_canTx1[7] = wczytaj_z_pamieci8b(201);

	preset_canTx2[0] = wczytaj_z_pamieci8b(181);
	preset_canTx2[1] = wczytaj_z_pamieci8b(184);
	preset_canTx2[2] = wczytaj_z_pamieci8b(187);
	preset_canTx2[3] = wczytaj_z_pamieci8b(190);
	preset_canTx2[4] = wczytaj_z_pamieci8b(193);
	preset_canTx2[5] = wczytaj_z_pamieci8b(196);
	preset_canTx2[6] = wczytaj_z_pamieci8b(199);
	preset_canTx2[7] = wczytaj_z_pamieci8b(202);

	preset_canTx3[0] = wczytaj_z_pamieci8b(182);
	preset_canTx3[1] = wczytaj_z_pamieci8b(185);
	preset_canTx3[2] = wczytaj_z_pamieci8b(188);
	preset_canTx3[3] = wczytaj_z_pamieci8b(191);
	preset_canTx3[4] = wczytaj_z_pamieci8b(194);
	preset_canTx3[5] = wczytaj_z_pamieci8b(197);
	preset_canTx3[6] = wczytaj_z_pamieci8b(200);
	preset_canTx3[7] = wczytaj_z_pamieci8b(203);

	txHeader1.DLC = wczytaj_z_pamieci(92);
	txHeader1.IDE = wczytaj_z_pamieci(100);
	txHeader1.RTR = wczytaj_z_pamieci(108);
	txHeader1.StdId = wczytaj_z_pamieci(116);
	txHeader1.ExtId = wczytaj_z_pamieci(124);
	txHeader2.DLC = wczytaj_z_pamieci(96);
	txHeader2.IDE = wczytaj_z_pamieci(104);
	txHeader2.RTR = wczytaj_z_pamieci(112);
	txHeader2.StdId = wczytaj_z_pamieci(120);
	txHeader2.ExtId = wczytaj_z_pamieci(128);

	can.Instance = CAN1;
	can.Init.Mode = CAN_MODE_NORMAL;
	can.Init.SyncJumpWidth = wczytaj_z_pamieci(0);
	can.Init.Prescaler = wczytaj_z_pamieci(4);
	can.Init.TimeSeg1 = wczytaj_z_pamieci(8);
	can.Init.TimeSeg2 = wczytaj_z_pamieci(12);
	HAL_CAN_Init(&can);

	canfilter.FilterBank = 0;
	canfilter.FilterMode = CAN_FILTERMODE_IDMASK;
	canfilter.FilterFIFOAssignment = 0;
	canfilter.FilterIdHigh = 0;
	canfilter.FilterIdLow = 0;
	canfilter.FilterMaskIdHigh = 0;
	canfilter.FilterMaskIdLow = 0;
	canfilter.FilterScale = CAN_FILTERSCALE_32BIT;
	canfilter.FilterActivation = ENABLE;
	canfilter.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(&can, &canfilter);

	txHeader.DLC = txHeader1.DLC;
	txHeader.IDE = txHeader1.IDE;
	txHeader.RTR = txHeader1.RTR;
	txHeader.StdId = txHeader1.StdId;
	txHeader.ExtId = txHeader1.ExtId;

	HAL_CAN_Start(&can);

}
void konfiguruj_filtr_can() {

	canfilter.FilterActivation = wczytaj_z_pamieci(16);
	canfilter.FilterBank = wczytaj_z_pamieci(20);
	canfilter.FilterMode = wczytaj_z_pamieci(24);
	canfilter.FilterScale = wczytaj_z_pamieci(28);
	canfilter.FilterFIFOAssignment = wczytaj_z_pamieci(32);
	canfilter.FilterIdHigh = wczytaj_z_pamieci(36);
	canfilter.FilterIdLow = wczytaj_z_pamieci(40);
	canfilter.FilterMaskIdHigh = wczytaj_z_pamieci(44);
	canfilter.FilterMaskIdLow = wczytaj_z_pamieci(48);

}

void menu_filtr() {
	uint8_t m_filtr = 1;
	uint8_t zapisf = ' ';
	uint8_t odczytf = ' ';
	uint8_t bufor[4] = { 0 };

	uint8_t wybor_filtr = ' ';
	uint32_t FilterBank = ' ';
	uint32_t FilterMode = ' ';
	uint32_t FilterScale = ' ';
	uint32_t FilterFIFOAssignment = ' ';

	uint8_t FilterIdHigh1 = ' ';
	uint8_t FilterIdHigh2 = ' ';
	uint8_t FilterIdHigh3 = ' ';
	uint8_t FilterIdHigh4 = ' ';

	uint8_t FilterIdLow1 = ' ';
	uint8_t FilterIdLow2 = ' ';
	uint8_t FilterIdLow3 = ' ';
	uint8_t FilterIdLow4 = ' ';

	uint8_t FilterMaskIdHigh1 = ' ';
	uint8_t FilterMaskIdHigh2 = ' ';
	uint8_t FilterMaskIdHigh3 = ' ';
	uint8_t FilterMaskIdHigh4 = ' ';

	uint8_t FilterMaskIdLow1 = ' ';
	uint8_t FilterMaskIdLow2 = ' ';
	uint8_t FilterMaskIdLow3 = ' ';
	uint8_t FilterMaskIdLow4 = ' ';

	HAL_CAN_Stop(&can);

	wyslij_string(
			"Czy chcesz wczytac ostatnia konfiguracje filtra?\n\r1 - tak\n\r2 - nie\n\r");
	while (odczytf != 1 && odczytf != 2)
		odczytf = pobierz_klawisz();
	wyslij_znak(odczytf);
	wyslij_string("\n\r");
	if (odczytf == 1) {
		konfiguruj_filtr_can();
		m_filtr = 11;

	}
	while (m_filtr < 11) {
		switch (m_filtr) {
		case 1:
			wyslij_string(
					"Czy chcesz dokonac filtracji odbieranych ramek CAN?\n\r1 - tak\n\r2 - nie\n\r");
			while (wybor_filtr != 1 && wybor_filtr != 2)
				wybor_filtr = pobierz_klawisz();

			wyslij_znak(wybor_filtr);
			wyslij_string("\n\r");
			if (wybor_filtr == 1) {
				canfilter.FilterActivation = ENABLE;
				m_filtr = 2;
			} else if (wybor_filtr == 2) {
				canfilter.FilterBank = 0;
				canfilter.FilterMode = CAN_FILTERMODE_IDMASK;
				canfilter.FilterFIFOAssignment = 0;
				canfilter.FilterIdHigh = 0;
				canfilter.FilterIdLow = 0;
				canfilter.FilterMaskIdHigh = 0;
				canfilter.FilterMaskIdLow = 0;
				canfilter.FilterScale = CAN_FILTERSCALE_32BIT;
				canfilter.FilterActivation = ENABLE;
				HAL_CAN_ConfigFilter(&can, &canfilter);
				m_filtr = 11;
			}

			break;

		case 2:
			wyslij_string("podaj FilterBank\n\r");
			while (FilterBank > 13)
				FilterBank = pobierz_klawisz();
			canfilter.FilterBank = FilterBank;

			bufor[0] = FilterBank;
			wyslij_znak(bufor[0]);
			wyslij_string("\n\r");
			m_filtr = 3;

			break;
		case 3:
			wyslij_string("podaj FilterMode\n\rIDMASK - 1\n\rIDLIST - 2\n\r");
			while (FilterMode != 1 && FilterMode != 2)
				FilterMode = pobierz_klawisz();

			wyslij_znak(FilterMode);
			wyslij_string("\n\r");
			if (FilterMode == 1)
				canfilter.FilterMode = CAN_FILTERMODE_IDMASK;
			else if (FilterMode == 2)
				canfilter.FilterMode = CAN_FILTERMODE_IDLIST;
			m_filtr = 4;

			break;

		case 4:
			wyslij_string("podaj FilterScale\n\r16BIT - 1\n\r32BIT - 2\n\r");
			while (FilterScale != 1 && FilterScale != 2)
				FilterScale = pobierz_klawisz();

			wyslij_znak(FilterScale);
			wyslij_string("\n\r");
			if (FilterScale == 1)
				canfilter.FilterScale = CAN_FILTERSCALE_16BIT;
			else if (FilterScale == 2)
				canfilter.FilterScale = CAN_FILTERSCALE_32BIT;
			m_filtr = 5;

			break;
		case 5:
			wyslij_string("podaj FilterFIFOAssignment\n\r");
			while (FilterFIFOAssignment != 0 && FilterFIFOAssignment != 1)
				FilterFIFOAssignment = pobierz_klawisz();
			canfilter.FilterFIFOAssignment = FilterFIFOAssignment;

			bufor[0] = FilterFIFOAssignment;
			wyslij_znak(bufor[0]);
			wyslij_string("\n\r");
			m_filtr = 6;
			break;
		case 6:
			if (canfilter.FilterScale == CAN_FILTERSCALE_16BIT
					&& canfilter.FilterMode == CAN_FILTERMODE_IDMASK)
				wyslij_string(
						"podaj 2. 16-bitowa maske (4 cyfry hexadecymalne)\n\r");

			if (canfilter.FilterScale == CAN_FILTERSCALE_16BIT
					&& canfilter.FilterMode == CAN_FILTERMODE_IDLIST)
				wyslij_string(
						"podaj 4. 16-bitowy identyfikator(4 cyfry hexadecymalne)\n\r");

			if (canfilter.FilterScale == CAN_FILTERSCALE_32BIT
					&& canfilter.FilterMode == CAN_FILTERMODE_IDMASK)
				wyslij_string(
						"podaj starsze 16 bitow 1. 32-bitowego identyfikatora (4 cyfry hexadecymalne)\n\r");

			if (canfilter.FilterScale == CAN_FILTERSCALE_32BIT
					&& canfilter.FilterMode == CAN_FILTERMODE_IDLIST)
				wyslij_string(
						"podaj starsze 16 bitow 1. 32-bitowego identyfikatora (4 cyfry hexadecymalne)\n\r");

			while (FilterIdHigh1 > 15)
				FilterIdHigh1 = pobierz_klawisz();

			bufor[0] = FilterIdHigh1;
			wyslij_znak(bufor[0]);

			while (FilterIdHigh2 > 15)
				FilterIdHigh2 = pobierz_klawisz();

			bufor[1] = FilterIdHigh2;
			wyslij_znak(bufor[1]);

			while (FilterIdHigh3 > 15)
				FilterIdHigh3 = pobierz_klawisz();

			bufor[2] = FilterIdHigh3;
			wyslij_znak(bufor[2]);

			while (FilterIdHigh4 > 15)
				FilterIdHigh4 = pobierz_klawisz();

			bufor[3] = FilterIdHigh4;
			wyslij_znak(bufor[3]);
			wyslij_string("\n\r");

			canfilter.FilterIdHigh = FilterIdHigh1 * 16 * 16 * 16
					+ FilterIdHigh2 * 16 * 16 + FilterIdHigh3 * 16
					+ FilterIdHigh4;

			m_filtr = 7;

			break;
		case 7:
			if (canfilter.FilterScale == CAN_FILTERSCALE_16BIT
					&& canfilter.FilterMode == CAN_FILTERMODE_IDMASK)
				wyslij_string(
						"podaj 1. 16-bitowa maske (4 cyfry hexadecymalne)\n\r");

			if (canfilter.FilterScale == CAN_FILTERSCALE_16BIT
					&& canfilter.FilterMode == CAN_FILTERMODE_IDLIST)
				wyslij_string(
						"podaj 2. 16-bitowy identyfikator(4 cyfry hexadecymalne)\n\r");

			if (canfilter.FilterScale == CAN_FILTERSCALE_32BIT
					&& canfilter.FilterMode == CAN_FILTERMODE_IDMASK)
				wyslij_string(
						"podaj mlodsze 16 bitow 1. 32-bitowego identyfikatora (4 cyfry hexadecymalne)\n\r");

			if (canfilter.FilterScale == CAN_FILTERSCALE_32BIT
					&& canfilter.FilterMode == CAN_FILTERMODE_IDLIST)
				wyslij_string(
						"podaj mlodsze 16 bitow 1. 32-bitowego identyfikatora (4 cyfry hexadecymalne)\n\r");

			while (FilterIdLow1 > 15)
				FilterIdLow1 = pobierz_klawisz();

			bufor[0] = FilterIdLow1;
			wyslij_znak(bufor[0]);
			while (FilterIdLow2 > 15)
				FilterIdLow2 = pobierz_klawisz();

			bufor[1] = FilterIdLow2;
			wyslij_znak(bufor[1]);
			while (FilterIdLow3 > 15)
				FilterIdLow3 = pobierz_klawisz();

			bufor[2] = FilterIdLow3;
			wyslij_znak(bufor[2]);
			while (FilterIdLow4 > 15)
				FilterIdLow4 = pobierz_klawisz();

			bufor[3] = FilterIdLow4;
			wyslij_znak(bufor[3]);
			wyslij_string("\n\r");

			canfilter.FilterIdLow = FilterIdLow1 * 16 * 16 * 16
					+ FilterIdLow2 * 16 * 16 + FilterIdLow3 * 16 + FilterIdLow4;
			m_filtr = 8;

			break;
		case 8:
			if (canfilter.FilterScale == CAN_FILTERSCALE_16BIT
					&& canfilter.FilterMode == CAN_FILTERMODE_IDMASK)
				wyslij_string(
						"podaj 2. 16-bitowy identyfikator (4 cyfry hexadecymalne)\n\r");

			if (canfilter.FilterScale == CAN_FILTERSCALE_16BIT
					&& canfilter.FilterMode == CAN_FILTERMODE_IDLIST)
				wyslij_string(
						"podaj 3. 16-bitowy identyfikator(4 cyfry hexadecymalne)\n\r");

			if (canfilter.FilterScale == CAN_FILTERSCALE_32BIT
					&& canfilter.FilterMode == CAN_FILTERMODE_IDMASK)
				wyslij_string(
						"podaj starsze 16 bitow 1. 32-bitowej maski (4 cyfry hexadecymalne)\n\r");

			if (canfilter.FilterScale == CAN_FILTERSCALE_32BIT
					&& canfilter.FilterMode == CAN_FILTERMODE_IDLIST)
				wyslij_string(
						"podaj starsze 16 bitow 2. 32-bitowego identyfikatora (4 cyfry hexadecymalne)\n\r");

			while (FilterMaskIdHigh1 > 15)
				FilterMaskIdHigh1 = pobierz_klawisz();

			bufor[0] = FilterMaskIdHigh1;
			wyslij_znak(bufor[0]);
			while (FilterMaskIdHigh2 > 15)
				FilterMaskIdHigh2 = pobierz_klawisz();

			bufor[1] = FilterMaskIdHigh2;
			wyslij_znak(bufor[1]);
			while (FilterMaskIdHigh3 > 15)
				FilterMaskIdHigh3 = pobierz_klawisz();

			bufor[2] = FilterMaskIdHigh3;
			wyslij_znak(bufor[2]);
			while (FilterMaskIdHigh4 > 15)
				FilterMaskIdHigh4 = pobierz_klawisz();

			bufor[3] = FilterMaskIdHigh4;
			wyslij_znak(bufor[3]);
			wyslij_string("\n\r");

			canfilter.FilterMaskIdHigh = FilterMaskIdHigh1 * 16 * 16 * 16
					+ FilterMaskIdHigh2 * 16 * 16 + FilterMaskIdHigh3 * 16
					+ FilterMaskIdHigh4;
			m_filtr = 9;

			break;

		case 9:
			if (canfilter.FilterScale == CAN_FILTERSCALE_16BIT
					&& canfilter.FilterMode == CAN_FILTERMODE_IDMASK)
				wyslij_string(
						"podaj 1. 16-bitowy identyfikator (4 cyfry hexadecymalne)\n\r");

			if (canfilter.FilterScale == CAN_FILTERSCALE_16BIT
					&& canfilter.FilterMode == CAN_FILTERMODE_IDLIST)
				wyslij_string(
						"podaj 1. 16-bitowy identyfikator(4 cyfry hexadecymalne)\n\r");

			if (canfilter.FilterScale == CAN_FILTERSCALE_32BIT
					&& canfilter.FilterMode == CAN_FILTERMODE_IDMASK)
				wyslij_string(
						"podaj mlodsze 16 bitow 1. 32-bitowej maski (4 cyfry hexadecymalne)\n\r");

			if (canfilter.FilterScale == CAN_FILTERSCALE_32BIT
					&& canfilter.FilterMode == CAN_FILTERMODE_IDLIST)
				wyslij_string(
						"podaj mlodsze 16 bitow 2. 32-bitowego identyfikatora (4 cyfry hexadecymalne)\n\r");

			while (FilterMaskIdLow1 > 15)
				FilterMaskIdLow1 = pobierz_klawisz();

			bufor[0] = FilterMaskIdLow1;
			wyslij_znak(bufor[0]);
			while (FilterMaskIdLow2 > 15)
				FilterMaskIdLow2 = pobierz_klawisz();

			bufor[1] = FilterMaskIdLow2;
			wyslij_znak(bufor[1]);
			while (FilterMaskIdLow3 > 15)
				FilterMaskIdLow3 = pobierz_klawisz();

			bufor[2] = FilterMaskIdLow3;
			wyslij_znak(bufor[2]);
			while (FilterMaskIdLow4 > 15)
				FilterMaskIdLow4 = pobierz_klawisz();

			bufor[3] = FilterMaskIdLow4;
			wyslij_znak(bufor[3]);
			wyslij_string("\n\r");

			canfilter.FilterMaskIdLow = FilterMaskIdLow1 * 16 * 16 * 16
					+ FilterMaskIdLow2 * 16 * 16 + FilterMaskIdLow3 * 16
					+ FilterMaskIdLow4;
			m_filtr = 10;

			break;
		case 10:
			wyslij_string(
					"Zapisac konfiguracje do pamieci?\n\r1 - tak\n\r2 - nie\n\r");
			while (zapisf != 1 && zapisf != 2)
				zapisf = pobierz_klawisz();
			wyslij_znak(zapisf);
			wyslij_string("\n\r");
			if (zapisf == 1) {
				zapisz_do_pamieci(16, canfilter.FilterActivation);
				zapisz_do_pamieci(20, canfilter.FilterBank);
				zapisz_do_pamieci(24, canfilter.FilterMode);
				zapisz_do_pamieci(28, canfilter.FilterScale);
				zapisz_do_pamieci(32, canfilter.FilterFIFOAssignment);
				zapisz_do_pamieci(36, canfilter.FilterIdHigh);
				zapisz_do_pamieci(40, canfilter.FilterIdLow);
				zapisz_do_pamieci(44, canfilter.FilterMaskIdHigh);
				zapisz_do_pamieci(48, canfilter.FilterMaskIdLow);

			}

			m_filtr = 11;
			break;

		}
	}
	HAL_CAN_ConfigFilter(&can, &canfilter);
	HAL_CAN_Start(&can);
	wyslij_string("Skonfigurowano filtr CAN\n\r");

}

void konfiguruj_Tx() {

	uint8_t bufor[4] = { 0 };
	uint8_t conf_tx = 1;
	uint8_t ktory_headertx = ' ';
	uint32_t DLC = ' ';
	uint32_t IDE = ' ';
	uint32_t RTR = ' ';

	uint32_t StdId1 = ' ';
	uint32_t StdId2 = ' ';
	uint32_t StdId3 = ' ';

	uint8_t ExtId1 = ' ';
	uint8_t ExtId2 = ' ';
	uint8_t ExtId3 = ' ';
	uint8_t ExtId4 = ' ';
	uint8_t ExtId5 = ' ';
	uint8_t ExtId6 = ' ';
	uint8_t ExtId7 = ' ';
	uint8_t ExtId8 = ' ';

	wyslij_string("Zapisac do 1. czy 2. Header'aTx? (1-2).\n\r");
	while (ktory_headertx != 1 && ktory_headertx != 2)
		ktory_headertx = pobierz_klawisz();
	wyslij_znak(ktory_headertx);
	wyslij_string("\n\r");

	while (conf_tx < 6) {
		switch (conf_tx) {
		case 1:
			wyslij_string("Podaj dlugosc ramki  max 8 (1 cyfra decymalna)\n\r");
			while (DLC > 8)
				DLC = pobierz_klawisz();
			txHeader.DLC = DLC;
			bufor[0] = DLC;
			wyslij_znak(bufor[0]);
			wyslij_string("\n\r");
			if (ktory_headertx == 1) {
				txHeader1.DLC = txHeader.DLC;
				zapisz_do_pamieci(92, txHeader1.DLC);
			} else if (ktory_headertx == 2) {
				txHeader2.DLC = txHeader.DLC;
				zapisz_do_pamieci(96, txHeader2.DLC);
			}

			conf_tx = 2;
			break;
		case 2:
			wyslij_string(
					"Podaj rodzaj identyfikatora\n\r1 - Identyfikator standardowy\n\r2 - Identyfikator rozszerzony\n\r (1 cyfra decymalna)\n\r");
			while (IDE != 1 && IDE != 2)
				IDE = pobierz_klawisz();
			bufor[0] = IDE;
			wyslij_znak(bufor[0]);
			if (IDE == 1)
				txHeader.IDE = CAN_ID_STD;
			if (IDE == 2)
				txHeader.IDE = CAN_ID_EXT;
			wyslij_string("\n\r");
			if (ktory_headertx == 1) {
				txHeader1.IDE = txHeader.IDE;
				zapisz_do_pamieci(100, txHeader1.IDE);
			} else if (ktory_headertx == 2) {
				txHeader2.IDE = txHeader.IDE;
				zapisz_do_pamieci(104, txHeader2.IDE);
			}

			conf_tx = 3;
			break;
		case 3:
			wyslij_string(
					"Podaj rodzaj ramki\n\r1 - Data frame\n\r2 - Remote frame\n\r  (1 cyfra decymalna)\n\r");
			while (RTR != 1 && RTR != 2)
				RTR = pobierz_klawisz();
			bufor[0] = RTR;
			wyslij_znak(bufor[0]);
			if (RTR == 1)
				txHeader.RTR = CAN_RTR_DATA;
			if (RTR == 2)
				txHeader.RTR = CAN_RTR_REMOTE;
			wyslij_string("\n\r");
			if (ktory_headertx == 1) {
				txHeader1.RTR = txHeader.RTR;
				zapisz_do_pamieci(108, txHeader1.RTR);
			} else if (ktory_headertx == 2) {
				txHeader2.RTR = txHeader.RTR;
				zapisz_do_pamieci(112, txHeader2.RTR);
			}

			conf_tx = 4;
			break;
		case 4:
			if (IDE == 2) {
				conf_tx = 5;
				break;
			}
			wyslij_string(
					"Podaj Standardowy identyfikator  max. 0x7FF (3 cyfry heksadecymalne)\n\r");
			while (StdId1 > 7)
				StdId1 = pobierz_klawisz();
			bufor[0] = StdId1;
			wyslij_znak(bufor[0]);

			while (StdId2 > 15)
				StdId2 = pobierz_klawisz();
			bufor[0] = StdId2;
			wyslij_znak(bufor[0]);

			while (StdId3 > 15)
				StdId3 = pobierz_klawisz();
			bufor[0] = StdId3;
			wyslij_znak(bufor[0]);
			wyslij_string("\n\r");

			txHeader.StdId = StdId1 * 16 * 16 + StdId2 * 16 + StdId3;
			if (ktory_headertx == 1) {
				txHeader1.StdId = txHeader.StdId;
				zapisz_do_pamieci(116, txHeader1.StdId);
			} else if (ktory_headertx == 2) {
				txHeader2.StdId = txHeader.StdId;
				zapisz_do_pamieci(120, txHeader2.StdId);
			}

			conf_tx = 5;
			break;
		case 5:
			if (IDE == 1) {
				conf_tx = 6;
				break;
			}
			wyslij_string(
					"Podaj Rozszerzony identyfikator  max. 1FFFFFFF (8 cyfr heksadecymalnych)\n\r");
			while (ExtId1 > 1)
				ExtId1 = pobierz_klawisz();
			bufor[0] = ExtId1;
			wyslij_znak(bufor[0]);

			while (ExtId2 > 15)
				ExtId2 = pobierz_klawisz();
			bufor[0] = ExtId2;
			wyslij_znak(bufor[0]);

			while (ExtId3 > 15)
				ExtId3 = pobierz_klawisz();
			bufor[0] = ExtId3;
			wyslij_znak(bufor[0]);

			while (ExtId4 > 15)
				ExtId4 = pobierz_klawisz();
			bufor[0] = ExtId4;
			wyslij_znak(bufor[0]);

			while (ExtId5 > 15)
				ExtId5 = pobierz_klawisz();
			bufor[0] = ExtId5;
			wyslij_znak(bufor[0]);

			while (ExtId6 > 15)
				ExtId6 = pobierz_klawisz();
			bufor[0] = ExtId6;
			wyslij_znak(bufor[0]);

			while (ExtId7 > 15)
				ExtId7 = pobierz_klawisz();
			bufor[0] = ExtId7;
			wyslij_znak(bufor[0]);

			while (ExtId8 > 15)
				ExtId8 = pobierz_klawisz();
			bufor[0] = ExtId8;
			wyslij_znak(bufor[0]);
			wyslij_string("\n\r");

			txHeader.ExtId = ExtId1 * 16 * 16 * 16 * 16 * 16 * 16 * 16
					+ ExtId2 * 16 * 16 * 16 * 16 * 16 * 16
					+ ExtId3 * 16 * 16 * 16 * 16 * 16
					+ ExtId4 * 16 * 16 * 16 * 16 + ExtId5 * 16 * 16 * 16
					+ ExtId6 * 16 * 16 + ExtId7 * 16 + ExtId8;
			if (ktory_headertx == 1) {
				txHeader1.ExtId = txHeader.ExtId;
				zapisz_do_pamieci(124, txHeader1.ExtId);
			} else if (ktory_headertx == 2) {
				txHeader2.ExtId = txHeader.ExtId;
				zapisz_do_pamieci(128, txHeader2.ExtId);
			}

			conf_tx = 6;
			break;

		}

	}
	wyslij_string("Skonfigurowano ramke tx\n\r");

}

void odbierz_wiadomosc() {

	uint8_t x = 0;
	uint8_t a = 0;
	uint8_t b = 0;

	wyslij_string("Wcisnij 1 aby wyjsc\n\r");
	while (x != 1) {

		uint8_t canRx[8] = { 0 };

		if (canfilter.FilterFIFOAssignment == 0) {
			if (HAL_CAN_GetRxFifoFillLevel(&can, CAN_RX_FIFO0)) {
				HAL_CAN_GetRxMessage(&can, CAN_RX_FIFO0, &rxHeader, canRx);
				wyslij_string("\n\r");
				if (rxHeader.IDE == CAN_ID_STD)
					printf("ID: %x (Hex)\n", rxHeader.StdId);
				else if (rxHeader.IDE == CAN_ID_EXT)
					printf("ID: %x (Hex)\n", rxHeader.ExtId);
				wyslij_string("DATA: ");
				if (rxHeader.DLC == 0)
					;
				else if (rxHeader.DLC == 1) {
					a = canRx[0] / 16;
					b = canRx[0] % 16;
					wyslij_znak(a);
					wyslij_znak(b);
					wyslij_string(" ");
				} else if (rxHeader.DLC == 2) {
					for (int i = 0; i < rxHeader.DLC; i++) {
						a = canRx[i] / 16;
						b = canRx[i] % 16;
						wyslij_znak(a);
						wyslij_znak(b);
						wyslij_string(" ");
					}
				} else if (rxHeader.DLC == 3) {
					for (int i = 0; i < rxHeader.DLC; i++) {
						a = canRx[i] / 16;
						b = canRx[i] % 16;
						wyslij_znak(a);
						wyslij_znak(b);
						wyslij_string(" ");
					}
				} else if (rxHeader.DLC == 4) {
					for (int i = 0; i < rxHeader.DLC; i++) {
						a = canRx[i] / 16;
						b = canRx[i] % 16;
						wyslij_znak(a);
						wyslij_znak(b);
						wyslij_string(" ");
					}
				} else if (rxHeader.DLC == 5) {
					for (int i = 0; i < rxHeader.DLC; i++) {
						a = canRx[i] / 16;
						b = canRx[i] % 16;
						wyslij_znak(a);
						wyslij_znak(b);
						wyslij_string(" ");
					}
				} else if (rxHeader.DLC == 6) {
					for (int i = 0; i < rxHeader.DLC; i++) {
						a = canRx[i] / 16;
						b = canRx[i] % 16;
						wyslij_znak(a);
						wyslij_znak(b);
						wyslij_string(" ");
					}
				} else if (rxHeader.DLC == 7) {
					for (int i = 0; i < rxHeader.DLC; i++) {
						a = canRx[i] / 16;
						b = canRx[i] % 16;
						wyslij_znak(a);
						wyslij_znak(b);
						wyslij_string(" ");
					}
				} else if (rxHeader.DLC == 8) {
					for (int i = 0; i < rxHeader.DLC; i++) {
						a = canRx[i] / 16;
						b = canRx[i] % 16;
						wyslij_znak(a);
						wyslij_znak(b);
						wyslij_string(" ");
					}

				}
				wyslij_string("\n\r");

			}
		} else if (canfilter.FilterFIFOAssignment == 1) {
			if (HAL_CAN_GetRxFifoFillLevel(&can, CAN_RX_FIFO1)) {
				HAL_CAN_GetRxMessage(&can, CAN_RX_FIFO1, &rxHeader, canRx);
				wyslij_string("\n\r");
				if (rxHeader.IDE == CAN_ID_STD)
					printf("ID: %x (Hex)\n", rxHeader.StdId);
				else if (rxHeader.IDE == CAN_ID_EXT)
					printf("ID: %x (Hex)\n", rxHeader.ExtId);
				wyslij_string("DATA: ");
				if (rxHeader.DLC == 0)
					;
				else if (rxHeader.DLC == 1) {
					a = canRx[0] / 16;
					b = canRx[0] % 16;
					wyslij_znak(a);
					wyslij_znak(b);
					wyslij_string(" ");
				} else if (rxHeader.DLC == 2) {
					for (int i = 0; i < rxHeader.DLC; i++) {
						a = canRx[i] / 16;
						b = canRx[i] % 16;
						wyslij_znak(a);
						wyslij_znak(b);
						wyslij_string(" ");
					}
				} else if (rxHeader.DLC == 3) {
					for (int i = 0; i < rxHeader.DLC; i++) {
						a = canRx[i] / 16;
						b = canRx[i] % 16;
						wyslij_znak(a);
						wyslij_znak(b);
						wyslij_string(" ");
					}
				} else if (rxHeader.DLC == 4) {
					for (int i = 0; i < rxHeader.DLC; i++) {
						a = canRx[i] / 16;
						b = canRx[i] % 16;
						wyslij_znak(a);
						wyslij_znak(b);
						wyslij_string(" ");
					}
				} else if (rxHeader.DLC == 5) {
					for (int i = 0; i < rxHeader.DLC; i++) {
						a = canRx[i] / 16;
						b = canRx[i] % 16;
						wyslij_znak(a);
						wyslij_znak(b);
						wyslij_string(" ");
					}
				} else if (rxHeader.DLC == 6) {
					for (int i = 0; i < rxHeader.DLC; i++) {
						a = canRx[i] / 16;
						b = canRx[i] % 16;
						wyslij_znak(a);
						wyslij_znak(b);
						wyslij_string(" ");
					}
				} else if (rxHeader.DLC == 7) {
					for (int i = 0; i < rxHeader.DLC; i++) {
						a = canRx[i] / 16;
						b = canRx[i] % 16;
						wyslij_znak(a);
						wyslij_znak(b);
						wyslij_string(" ");
					}
				} else if (rxHeader.DLC == 8) {
					for (int i = 0; i < rxHeader.DLC; i++) {
						a = canRx[i] / 16;
						b = canRx[i] % 16;
						wyslij_znak(a);
						wyslij_znak(b);
						wyslij_string(" ");
					}

				}
				wyslij_string("\n\r");
			}
		}
		x = pobierz_klawisz();
	}
}

void zapisz_wiadomosc() {
	uint8_t canTx[8] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	uint8_t canTxx[8] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	uint8_t conf_wyslij = 1;
	uint8_t komorka_pamieci = ' ';

	wyslij_string(
			"Wybierz do ktorej komorki pamieci zapisac wiadomosc (1-3).\n\r");
	while (komorka_pamieci != 1 && komorka_pamieci != 2 && komorka_pamieci != 3)
		komorka_pamieci = pobierz_klawisz();
	wyslij_znak(komorka_pamieci);
	wyslij_string("\n\r");

	while (conf_wyslij < 9) {
		switch (conf_wyslij) {
		case 1:
			wyslij_string("Podaj kolejne bajty wiadomosci do przeslania \n\r");
			while (canTx[0] > 15)
				canTx[0] = pobierz_klawisz();
			wyslij_znak(canTx[0]);
			while (canTxx[0] > 15)
				canTxx[0] = pobierz_klawisz();
			wyslij_znak(canTxx[0]);
			wyslij_string(" ");
			canTx[0] = canTx[0] * 16 + canTxx[0];
			conf_wyslij = 2;
			if (komorka_pamieci == 1) {
				canTx1[0] = canTx[0];
				zapisz_do_pamieci8b(132, canTx[0]);
			} else if (komorka_pamieci == 2) {
				canTx2[0] = canTx[0];
				zapisz_do_pamieci8b(133, canTx[0]);
			} else if (komorka_pamieci == 3) {
				canTx3[0] = canTx[0];
				zapisz_do_pamieci8b(134, canTx[0]);
			}
			break;
		case 2:
			while (canTx[1] > 15)
				canTx[1] = pobierz_klawisz();
			wyslij_znak(canTx[1]);
			while (canTxx[1] > 15)
				canTxx[1] = pobierz_klawisz();
			wyslij_znak(canTxx[1]);
			wyslij_string(" ");
			canTx[1] = canTx[1] * 16 + canTxx[1];
			conf_wyslij = 3;
			if (komorka_pamieci == 1) {
				canTx1[1] = canTx[1];
				zapisz_do_pamieci8b(135, canTx[1]);
			}

			else if (komorka_pamieci == 2) {
				canTx2[1] = canTx[1];
				zapisz_do_pamieci8b(136, canTx[1]);
			}

			else if (komorka_pamieci == 3) {
				canTx3[1] = canTx[1];
				zapisz_do_pamieci8b(137, canTx[1]);
			}

			break;
		case 3:

			while (canTx[2] > 15)
				canTx[2] = pobierz_klawisz();
			wyslij_znak(canTx[2]);
			while (canTxx[2] > 15)
				canTxx[2] = pobierz_klawisz();
			wyslij_znak(canTxx[2]);
			wyslij_string(" ");
			canTx[2] = canTx[2] * 16 + canTxx[2];
			conf_wyslij = 4;
			if (komorka_pamieci == 1) {
				canTx1[2] = canTx[2];
				zapisz_do_pamieci8b(138, canTx[2]);
			}

			else if (komorka_pamieci == 2) {
				canTx2[2] = canTx[2];
				zapisz_do_pamieci8b(139, canTx[2]);
			}

			else if (komorka_pamieci == 3) {
				canTx3[2] = canTx[2];
				zapisz_do_pamieci8b(140, canTx[2]);
			}

			break;
		case 4:

			while (canTx[3] > 15)
				canTx[3] = pobierz_klawisz();
			wyslij_znak(canTx[3]);
			while (canTxx[3] > 15)
				canTxx[3] = pobierz_klawisz();
			wyslij_znak(canTxx[3]);
			wyslij_string(" ");
			canTx[3] = canTx[3] * 16 + canTxx[3];
			conf_wyslij = 5;
			if (komorka_pamieci == 1) {
				canTx1[3] = canTx[3];
				zapisz_do_pamieci8b(141, canTx[3]);
			}

			else if (komorka_pamieci == 2) {
				canTx2[3] = canTx[3];
				zapisz_do_pamieci8b(142, canTx[3]);
			}

			else if (komorka_pamieci == 3) {
				canTx3[3] = canTx[3];
				zapisz_do_pamieci8b(143, canTx[3]);
			}

			break;
		case 5:

			while (canTx[4] > 15)
				canTx[4] = pobierz_klawisz();
			wyslij_znak(canTx[4]);
			while (canTxx[4] > 15)
				canTxx[4] = pobierz_klawisz();
			wyslij_znak(canTxx[4]);
			wyslij_string(" ");
			canTx[4] = canTx[4] * 16 + canTxx[4];
			conf_wyslij = 6;
			if (komorka_pamieci == 1) {
				canTx1[4] = canTx[4];
				zapisz_do_pamieci8b(144, canTx[4]);
			}

			else if (komorka_pamieci == 2) {
				canTx2[4] = canTx[4];
				zapisz_do_pamieci8b(145, canTx[4]);
			}

			else if (komorka_pamieci == 3) {
				canTx3[4] = canTx[4];
				zapisz_do_pamieci8b(146, canTx[4]);
			}

			break;
		case 6:

			while (canTx[5] > 15)
				canTx[5] = pobierz_klawisz();
			wyslij_znak(canTx[5]);
			while (canTxx[5] > 15)
				canTxx[5] = pobierz_klawisz();
			wyslij_znak(canTxx[5]);
			wyslij_string(" ");
			canTx[5] = canTx[5] * 16 + canTxx[5];
			conf_wyslij = 7;
			if (komorka_pamieci == 1) {
				canTx1[5] = canTx[5];
				zapisz_do_pamieci8b(147, canTx[5]);
			}

			else if (komorka_pamieci == 2) {
				canTx2[5] = canTx[5];
				zapisz_do_pamieci8b(148, canTx[5]);
			}

			else if (komorka_pamieci == 3) {
				canTx3[5] = canTx[5];
				zapisz_do_pamieci8b(149, canTx[5]);
			}

			break;
		case 7:

			while (canTx[6] > 15)
				canTx[6] = pobierz_klawisz();
			wyslij_znak(canTx[6]);
			while (canTxx[6] > 15)
				canTxx[6] = pobierz_klawisz();
			wyslij_znak(canTxx[6]);
			wyslij_string(" ");
			canTx[6] = canTx[6] * 16 + canTxx[6];
			conf_wyslij = 8;
			if (komorka_pamieci == 1) {
				canTx1[6] = canTx[6];
				zapisz_do_pamieci8b(150, canTx[6]);
			}

			else if (komorka_pamieci == 2) {
				canTx2[6] = canTx[6];
				zapisz_do_pamieci8b(151, canTx[6]);
			}

			else if (komorka_pamieci == 3) {
				canTx3[6] = canTx[6];
				zapisz_do_pamieci8b(152, canTx[6]);
			}

			break;
		case 8:

			while (canTx[7] > 15)
				canTx[7] = pobierz_klawisz();
			wyslij_znak(canTx[7]);
			while (canTxx[7] > 15)
				canTxx[7] = pobierz_klawisz();
			wyslij_znak(canTxx[7]);
			wyslij_string(" ");
			canTx[7] = canTx[7] * 16 + canTxx[7];
			wyslij_string("\n\r");
			conf_wyslij = 9;
			if (komorka_pamieci == 1) {
				canTx1[7] = canTx[7];
				zapisz_do_pamieci8b(153, canTx[7]);
			}

			else if (komorka_pamieci == 2) {
				canTx2[7] = canTx[7];
				zapisz_do_pamieci8b(154, canTx[7]);
			}

			else if (komorka_pamieci == 3) {
				canTx3[7] = canTx[7];
				zapisz_do_pamieci8b(155, canTx[7]);
			}

			break;

		}
	}

}

void anuluj_transmisje() {
	HAL_CAN_AbortTxRequest(&can, canMailbox);

}

void wyslij_wiadomosc() {

	uint8_t a = 0;
	uint8_t b = 0;
	uint8_t ktora_wiadomosc = ' ';
	wyslij_string("Ktora wiadomosc wyslac? (1-3)\n\r");
	while (ktora_wiadomosc != 1 && ktora_wiadomosc != 2 && ktora_wiadomosc != 3)
		ktora_wiadomosc = pobierz_klawisz();
	wyslij_znak(ktora_wiadomosc);
	wyslij_string("\n\r");
	if (txHeader.IDE == CAN_ID_STD)
		printf("ID: %x (Hex)\n", txHeader.StdId);
	else if (txHeader.IDE == CAN_ID_EXT)
		printf("ID: %x (Hex)\n", txHeader.ExtId);
	wyslij_string("DATA: ");

	if (ktora_wiadomosc == 1) {
		if (HAL_CAN_GetTxMailboxesFreeLevel(&can))
			HAL_CAN_AddTxMessage(&can, &txHeader, canTx1, &canMailbox);
		for (int i = 0; i < txHeader.DLC; i++) {
			a = canTx1[i] / 16;
			b = canTx1[i] % 16;
			wyslij_znak(a);
			wyslij_znak(b);
			wyslij_string(" ");
		}

		wyslij_string("\n\r");
	} else if (ktora_wiadomosc == 2) {
		if (HAL_CAN_GetTxMailboxesFreeLevel(&can))
			HAL_CAN_AddTxMessage(&can, &txHeader, canTx2, &canMailbox);
		for (int i = 0; i < txHeader.DLC; i++) {
			a = canTx2[i] / 16;
			b = canTx2[i] % 16;
			wyslij_znak(a);
			wyslij_znak(b);
			wyslij_string(" ");
		}
		wyslij_string("\n\r");
	} else if (ktora_wiadomosc == 3) {
		if (HAL_CAN_GetTxMailboxesFreeLevel(&can))
			HAL_CAN_AddTxMessage(&can, &txHeader, canTx3, &canMailbox);
		for (int i = 0; i < txHeader.DLC; i++) {
			a = canTx3[i] / 16;
			b = canTx3[i] % 16;
			wyslij_znak(a);
			wyslij_znak(b);
			wyslij_string(" ");
		}
		wyslij_string("\n\r");
	}

}
void menu_can() {
	uint8_t menu = 11;

	while (1) {

		switch (menu) {

		case 1:

			menu_predkosc();
			wyslij_string("Skonfigurowano CAN\n\r");
			menu = 11;
			break;
		case 2:

			menu_filtr();
			menu = 11;
			break;

		case 3:

			konfiguruj_Tx();
			menu = 11;
			break;
		case 4:

			odbierz_wiadomosc();
			menu = 11;
			break;
		case 5:

			wyslij_wiadomosc();
			menu = 11;
			break;
		case 6:

			anuluj_transmisje();
			menu = 11;
			break;
		case 7:

			zapisz_wiadomosc();
			menu = 11;
			break;
		case 8:
			wybierz_Tx();
			menu = 11;
			break;

		case 11:

			wyslij_string(
					"\t\tMENU\n\r1.Konfiguracja Szybkosci CAN\n\r2.Konfiguracja filtrowania\n\r3.Konfiguruacja ramki Tx\n\r4.Odbierz wiadomosc\n\r5.Wyslij wiadomosc\n\r6.Anuluj transmisje\n\r7.Zapisz wiadomosc\n\r8.Wybierz header Tx\n\r");
			while (menu > 10 || menu == 0)
				menu = pobierz_klawisz();
			wyslij_znak(menu);
			wyslij_string("\n\r");

			break;
		}
	}
}
void wybierz_Tx() {
	uint8_t ktory_tx = ' ';
	wyslij_string("ktory Header Tx wybrac? (1-2)\n\r");
	while (ktory_tx != 1 && ktory_tx != 2)
		ktory_tx = pobierz_klawisz();
	wyslij_znak(ktory_tx);
	wyslij_string("\n\r");
	if (ktory_tx == 1) {
		txHeader.DLC = txHeader1.DLC;
		txHeader.IDE = txHeader1.IDE;
		txHeader.RTR = txHeader1.RTR;
		txHeader.StdId = txHeader1.StdId;
		txHeader.ExtId = txHeader1.ExtId;
	} else if (ktory_tx == 2) {
		txHeader.DLC = txHeader2.DLC;
		txHeader.IDE = txHeader2.IDE;
		txHeader.RTR = txHeader2.RTR;
		txHeader.StdId = txHeader2.StdId;
		txHeader.ExtId = txHeader2.ExtId;

	}

}
void zapisz_do_pamieci(uint32_t rejestr, uint32_t dane) {

	HAL_I2C_Mem_Write(&i2c, 0xa0, rejestr, 1, (uint8_t*) &dane, sizeof(dane),
	HAL_MAX_DELAY);
	HAL_Delay(5);
}

uint32_t wczytaj_z_pamieci(uint32_t rejestr) {
	uint32_t zmienna = 0;
	HAL_I2C_Mem_Read(&i2c, 0xa0, rejestr, 1, (uint8_t*) &zmienna,
			sizeof(zmienna),
			HAL_MAX_DELAY);
	return zmienna;

}

void zapisz_do_pamieci8b(uint32_t rejestr, uint8_t dane) {

	HAL_I2C_Mem_Write(&i2c, 0xa0, rejestr, 1, (uint8_t*) &dane, sizeof(dane),
	HAL_MAX_DELAY);
	HAL_Delay(5);
}

uint8_t wczytaj_z_pamieci8b(uint32_t rejestr) {
	uint8_t zmienna = 0;
	HAL_I2C_Mem_Read(&i2c, 0xa0, rejestr, 1, (uint8_t*) &zmienna,
			sizeof(zmienna),
			HAL_MAX_DELAY);
	return zmienna;

}


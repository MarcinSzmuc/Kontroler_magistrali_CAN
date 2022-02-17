/*
 * Funkcje.h
 *
 *  Created on: 11.10.2021
 *      Author: SaintHunter
 */

#ifndef FUNKCJE_H_
#define FUNKCJE_H_

#include "stm32f1xx_hal.h"


void wyslij_string(char* string);
void delay(uint16_t us);
void send_char(char c);
int __io_putchar(int ch);
uint8_t pobierz_klawisz();
void menu_predkosc();
void konfiguruj_filtr_can();
void menu_filtr();
void menu_can();
void konfiguruj_Tx();
void odbierz_wiadomosc();
void zapisz_wiadomosc();
void wyslij_wiadomosc();
void konfiguruj_preset();
void wybierz_preset();
void anuluj_transmisje();
void wyslij_znak_can(char znak);
uint8_t pobierz_klawisz_can();
void zapisz_do_pamieci(uint32_t rejestr,uint32_t dane);
uint32_t wczytaj_z_pamieci(uint32_t rejestr);
void wczytaj_ostatnia_konfiguracje();
void wybierz_Tx();
void zapisz_do_pamieci8b(uint32_t rejestr, uint8_t dane);
uint8_t wczytaj_z_pamieci8b(uint32_t rejestr);
int __io_putchar(int ch);
void wyslij_string(char* string);
#endif /* FUNKCJE_H_ */

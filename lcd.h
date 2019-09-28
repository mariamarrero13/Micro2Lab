/*
 * lcd.h
 *
 *  Created on: Sep 19, 2019
 *      Author: maria
 */

#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>

#ifndef lcd_h
#define lcd_h

#define Clear           0b00000001
#define lcd_EntryMode   0b00000110
#define Display_off     0b00001000
#define Display_on      0b00001111
#define FunctionReset   0b00110000
#define FunctionSet     0b00101100
#define lcd_SetCursor   0b10000000
#define lcd_LineOne     0x00
#define lcd_LineTwo     0x40


void lcd_byte(uint8_t);
void lcd_command(uint8_t);
void lcd_character(uint8_t);
void lcd_string(uint8_t *);
void lcd_init_4bit(uint_least8_t D7, uint_least8_t D6, uint_least8_t D5, uint_least8_t D4, uint_least8_t Rs, uint_least8_t E);


uint8_t D7;
uint8_t D6;
uint8_t D5;
uint8_t D4;
uint8_t Rs;
uint8_t E;
#endif

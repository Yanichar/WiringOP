/*
 * cc2500.c:
 *	cc2500 Radio Transiever library for wiringOP
 *
 * Copyright (c) 2013 Gordon Henderson.
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <cc2500.h>

#define CC2500_CS 10

void spi_setup(void)
{
    wiringPiSetup () ;
    pinMode (CC2500_CS, OUTPUT);
    digitalWrite (CC2500_CS, HIGH);
    wiringPiSPISetup (0, 1000000);
}

void cc_write_reg(uint8_t addr, uint8_t value)
{
  digitalWrite (CC2500_CS, LOW);

  uint8_t * data = malloc(2);

  data[0] = addr;
  data[1] = value;
  wiringPiSPIDataRW(0, data, 2);

  free(data);

  digitalWrite (CC2500_CS, HIGH);
}

void cc_write_burst_reg(uint8_t addr, uint8_t *buffer, uint8_t count)
{
  digitalWrite (CC2500_CS, LOW);
  addr |= TI_CCxxx0_WRITE_BURST;
  wiringPiSPIDataRW(0, &addr, 1);
  wiringPiSPIDataRW(0, buffer, count);
  digitalWrite (CC2500_CS, HIGH);
}

uint8_t cc_read_reg(uint8_t addr)
{
  uint8_t x;

  digitalWrite (CC2500_CS, LOW);

  delay(1);

  uint8_t * data = malloc(2);

  data[0] = addr | TI_CCxxx0_READ_SINGLE;;
  data[1] = 0xFF;

  wiringPiSPIDataRW(0, data, 2);

  x = data[1];

  free(data);

  digitalWrite (CC2500_CS, HIGH);

  return x;
}

void cc_read_burst_reg(uint8_t addr, uint8_t *buffer, uint8_t count)
{
  digitalWrite (CC2500_CS, LOW);
  
  addr |= TI_CCxxx0_READ_BURST;
  wiringPiSPIDataRW(0, &addr, 1);
  wiringPiSPIDataRW(0, buffer, count);
  
  digitalWrite (CC2500_CS, HIGH);
}

uint8_t cc_read_status(uint8_t addr)
{
  uint8_t x;

  digitalWrite (CC2500_CS, LOW);
  delay(1);

  uint8_t * data = malloc(2);

  data[0] = addr | TI_CCxxx0_READ_BURST;
  data[1] = 0xFF;

  wiringPiSPIDataRW(0, data, 2);

  x = data[1];

  free(data);

  digitalWrite (CC2500_CS, HIGH);

  return x;
}

void cc_strobe(uint8_t strobe)
{
  digitalWrite (CC2500_CS, LOW);
  wiringPiSPIDataRW(0, &strobe, 1);
  digitalWrite (CC2500_CS, HIGH);
}

uint8_t cc_read_chip_status_byte(fifo_status_mode_enum fifo_mode)
{
  uint8_t result = 0; 
  if(fifo_mode == TX_FIFO_BYTES_FREE)
  {
	result = 1<<7;
  }
  
  digitalWrite (CC2500_CS, LOW);
  wiringPiSPIDataRW(0, &result, 1);
  digitalWrite (CC2500_CS, HIGH);
  return result;
}


void cc_powerup_reset(void)
{
  // TODO: implement me
}

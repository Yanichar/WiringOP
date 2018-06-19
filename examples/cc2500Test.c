/*
 * cc2500Test.c:
 *	Test low level acsess to 2500 in wiringPi. cc2500 connected
 *	to the SPI0
 *
 * Copyright (c) 2012-2013 Gordon Henderson. <projects@drogon.net>
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
#include <string.h> 
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <cc2500.h>
#include "cc2500Test.h"

// CC2500 chip select pin - wiringPi pin 0
#define CC2500_CS 10

int8_t copmare_and_print_result(uint8_t expected, uint8_t actual)
{
  if(expected == actual)
  {
	printf("OK\n"); 
	return 0;
  }
  else
  {
    printf("FAIL\n");
	return -1;
  }
}

int main (void)
{
  uint32_t testCounter = 0;
  uint32_t failCounter = 0;
  
  printf("Test for cc2500 low level acsess start...\n\n");
  
  wiringPiSetup () ;
  
  spi_setup();
  
  printf("Status register read...");
  uint8_t reg = cc_read_status(TI_CCxxx0_PARTNUM);
  
  testCounter++;
  if(copmare_and_print_result(128, reg))
  {
     failCounter++;
  }
  
  printf("Configuration register write and read back...");
  cc_write_reg(TI_CCxxx0_ADDR, 0x55);
  reg = cc_read_reg(TI_CCxxx0_ADDR);
  
  testCounter++;
  if(copmare_and_print_result(0x55, reg))
  {
     failCounter++;
  }
  
  printf("Try to reset chip with strobe command...");
  cc_strobe(TI_CCxxx0_SRES);
  delay(500);
  
  reg = cc_read_reg(TI_CCxxx0_ADDR);
  
  testCounter++;
  if(copmare_and_print_result(0, reg))
  {
     failCounter++;
  }
  
  printf("Register burst write and read back...");
  uint8_t testWrite[2];
  uint8_t testRead[2];
  
  testWrite[0] = 0x55;
  testWrite[1] = 0xAA;
  cc_write_burst_reg(TI_CCxxx0_ADDR, testWrite, sizeof(testWrite));
  cc_read_burst_reg(TI_CCxxx0_ADDR, testRead, sizeof(testRead));
  
  // re-fill array becouse in testWrite we have current status
  testWrite[0] = 0x55;
  testWrite[1] = 0xAA;
	
  testCounter++;
  if(memcmp(testRead, testWrite, sizeof(testRead)))
  {
    failCounter++;
	printf("FAIL\n");
  }
  else
  {
    printf("OK\n");
  }
    
  // all test done
  
  printf("\n");
    
  if(!failCounter)
  {
     printf("Testing finished, all test are OK\n");
  }
  else
  {
    printf("Testing finished, some tests are FAILED\n");
  }
  
  return 0 ;
}

/*
  SD card test

  This example shows how use the utility libraries on which the
  SD library is based in order to get info about your SD card.
  Very useful for testing a card when you're not sure whether its working or not.
  Pin numbers reflect the default SPI pins for Uno and Nano models.
  The circuit:
    SD card attached to SPI bus as follows:
 ** SDO - pin 11 on Arduino Uno/Duemilanove/Diecimila
 ** SDI - pin 12 on Arduino Uno/Duemilanove/Diecimila
 ** CLK - pin 13 on Arduino Uno/Duemilanove/Diecimila
 ** CS - depends on your SD card shield or module.
 		Pin 10 used here for consistency with other Arduino examples

  created  28 Mar 2011
  by Limor Fried
  modified 24 July 2020
  by Tom Igoe
*/
// include the SD library:
#include <SPI.h>
#include <SD.h>



#ifndef SRC_LCD_H_
#define SRC_LCD_H_

#include "em_device.h"
#include "em_chip.h"

/* If we want to use the LCD driver, we have to include "segmentlcd.h"
 * This file is located outside the project folder (on a system path).
 * Press F3 to locate it.
 */
#include "segmentlcd.h"

/* If we want to turn on or off segments individually, we also need to
 * include "segmentlcd_individual.h". The easiest way is to put this file
 * (and the corresponding C file) inside the project folder.
 */
#include "segmentlcd_individual.h"

/* The segment data (which segments to turn on, which segments to turn off)
 * should be stored in arrays (a separate array belongs to the upper and the
 * lower part of the LCD).
 *
 * The elements in the arrays store the segment data for a single character.
 * The upper display has 4, seven-segment digits (can be used to display numbers).
 * The lower display has seven, 14 segment character places (these can be used to
 * display alphanumeric characters).
 *
 */
SegmentLCD_UpperCharSegments_TypeDef upperCharSegments[SEGMENT_LCD_NUM_OF_UPPER_CHARS];
SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS];


void delay();

void demoLowerSegments();

void demoUpperSegments();

void clearUpperSegments();

void printNumberOneUpperSegment(uint16_t number, uint8_t numberOfSegments);

void printNumberAllUpperSegments(uint16_t number);

void printOneLowerSegment(uint8_t digitNumber, char segment, uint8_t onOff);

void ringOff();

void ringOn();

void ringBlink();

void ringFill();

void blinkOneLowerSegment(uint32_t digitNumber, char segment);


#endif /* SRC_LCD_H_ */

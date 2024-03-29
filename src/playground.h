#pragma once
#include "em_device.h"
#include "em_chip.h"
#include "em_usart.h"
#include <stdbool.h>
#include <stdint.h>
#include "stdio.h"
#include "lcd.h"

#include "FreeRTOS.h" // Ez legyen az első FreeRTOS header
#include "task.h"
#include "semphr.h"


#ifndef SRC_PLAYGROUND_H_
#define SRC_PLAYGROUND_H_


typedef struct {

    char segment;
    bool ship;
    bool shot;
    uint8_t digitNumber;

} field;

typedef struct {
    uint8_t x;
    uint8_t y;
} coordinate;

volatile field map[35][5];
SemaphoreHandle_t  SemaphoreShot,SemaphoreWin,SemaphoreUsart;
TaskHandle_t  HandleCreate,HandleShow;

uint8_t UsartData;
void InitPlayGround();
uint8_t checkShotedShip();
void ShipLoadMap();
int random;
#endif /* SRC_PLAYGROUND_H_ */

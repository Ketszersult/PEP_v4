
#include "playground.h"



bool UsartFlag;
uint8_t UsartData;
coordinate ship[16][8] = {
        {{2, 0}, {7, 0}, {17, 0}, {22, 0}, {2,  2}, {7,  2}, {17, 2}, {22, 2}},
        {{1, 1}, {3, 3}, {6,  1}, {8,  3}, {11, 1}, {13, 3}, {16, 1}, {18, 3}},
		{{2, 0}, {7, 0}, {17, 0}, {22, 0}, {2,  2}, {7,  2}, {17, 2}, {22, 2}},
		{{1, 1}, {3, 3}, {6,  1}, {8,  3}, {11, 1}, {13, 3}, {16, 1}, {18, 3}},
		{{2, 0}, {7, 0}, {17, 0}, {22, 0}, {2,  2}, {7,  2}, {17, 2}, {22, 2}},
		{{1, 1}, {3, 3}, {6,  1}, {8,  3}, {11, 1}, {13, 3}, {16, 1}, {18, 3}},
		{{2, 0}, {7, 0}, {17, 0}, {22, 0}, {2,  2}, {7,  2}, {17, 2}, {22, 2}},
		{{1, 1}, {3, 3}, {6,  1}, {8,  3}, {11, 1}, {13, 3}, {16, 1}, {18, 3}},
		{{2, 0}, {7, 0}, {17, 0}, {22, 0}, {2,  2}, {7,  2}, {17, 2}, {22, 2}},
	  	{{1, 1}, {3, 3}, {6,  1}, {8,  3}, {11, 1}, {13, 3}, {16, 1}, {18, 3}},
	   	{{2, 0}, {7, 0}, {17, 0}, {22, 0}, {2,  2}, {7,  2}, {17, 2}, {22, 2}},
		{{1, 1}, {3, 3}, {6,  1}, {8,  3}, {11, 1}, {13, 3}, {16, 1}, {18, 3}},
};

//inenn indul a jatek
coordinate actualCoordinate = {2,0};
uint16_t shotNumber = 0;

//Letrehozza a palyat egy sablonbol
static void TskCreateGameMap(void *pvParam){
	while(true)
	{
		vTaskSuspend(HandleCreate);
		USART_Tx(UART0,'j');
		//xSemaphoreTake(SemaphoreNewGame,portMAX_DELAY);

		field template[5][5];
		for (uint8_t i = 0; i < 5; ++i) {
			for (uint8_t j = 0; j < 5; ++j) {
				template[i][j].digitNumber = 0;
				template[i][j].ship = false;
				template[i][j].shot = false;
				template[i][j].segment = 'x';
			}
		}

		//A kijelzo szegmenseinek elhelezése a matrixban
		template[2][4].segment = 'a';
		template[0][3].segment = 'f';
		template[1][3].segment = 'h';
		template[2][3].segment = 'j';
		template[3][3].segment = 'k';
		template[4][3].segment = 'b';
		template[2][2].segment = 'g';
		template[0][1].segment = 'e';
		template[1][1].segment = 'q';
		template[2][1].segment = 'p';
		template[3][1].segment = 'n';
		template[4][1].segment = 'c';
		template[2][0].segment = 'd';

		//A tenyleges palyaforma letrehozasa a sablonbol
		for (uint8_t i = 0; i < 7; ++i) {
			for (uint8_t j = 0; j < 5; ++j) {
				for (uint8_t k = 0; k < 5; ++k) {
					map[i * 5 + j][k] = template[j][k];
					map[i * 5 + j][k].digitNumber = i;
				}
			}
		}


		TskShipLoadMap();

	}
}


//random szam alapjan valaszt egy palya elrendezest es feltolti az ures palyat hajokkal
void TskShipLoadMap() {

		USART_Tx(UART0,'!');
		//uint8_t random = 1;
		//xQueueReceive(q, (void *)&random, portMAX_DELAY);

		if(random < 0 || random>9)
			random = 1;

		for (uint8_t i = 0; i < 8; i++) {
			if ('x' != map[ship[random][i].x][ship[random][i].y].segment)
				map[ship[random][i].x][ship[random][i].y].ship = true;
		}
		//vTaskSuspend(NULL);

}

//Kirajzolja a palyat, azon elemekt ahol mar eltalaltak a hajokat
static void TskShowMap(void *pvParam) {
	bool x;
    while (true)
    {

    for (uint8_t i = 0; i < 35; ++i) {
        for (uint8_t j = 0; j < 5; ++j) {
            if (true == map[i][j].ship && true == map[i][j].shot) //csak az eltalalt hajokat rajzolja ki
                //if(true == map[i][j].ship) // Kirajzol minden hajot - teszteles
            {
                //itt meg csak a 'memoriaba irodik be'
                printOneLowerSegment(map[i][j].digitNumber, map[i][j].segment,1);
            } else {
            	printOneLowerSegment(map[i][j].digitNumber, map[i][j].segment,0);
            }
        }
    }

    printOneLowerSegment(map[actualCoordinate.x][actualCoordinate.y].digitNumber,map[actualCoordinate.x][actualCoordinate.y].segment,x);
    x = !x;
    SegmentLCD_LowerSegments(lowerCharSegments); //Itt jelennek meg tenylegesen
    vTaskDelay(configTICK_RATE_HZ/2); // 0.5 sec delay
    }
}
 

//Lo a hajora villog a Ring ha talalt, kialszik ha nem
static void TskShot(void *pvParam) {
	uint8_t shot = 0;
    while(true){
        xSemaphoreTake(SemaphoreShot, portMAX_DELAY);
        ringFill();
        char buffer[10] = "";
        if(true == map[actualCoordinate.x][actualCoordinate.y].ship){
            map[actualCoordinate.x][actualCoordinate.y].shot = true;
            //Ring párszori villogatatása
            for (uint8_t i = 0; i < 5; ++i) {
                ringBlink();
                shot = 1;
            }
        } else{
            ringOff();
            shot = 0;
        }
        sprintf(buffer,"%u,%u,%u\n",actualCoordinate.x,actualCoordinate.y,shot);
        for(int i = 0; buffer[i] != '\0'; i++){
        	USART_Tx(UART0,buffer[i]);
        }
        shotNumber++;
        SegmentLCD_Number(shotNumber);
        if(checkShotedShip()){
            xSemaphoreGive(SemaphoreWin);
        }
    }
}

static void TskWin(void *pvParam) {
char win[] = "You WIN!!!!!\n";
    while (true)
    {
USART_Tx(UART0,'w');

		xSemaphoreTake(SemaphoreWin, portMAX_DELAY);

        SegmentLCD_AllOff();
        SegmentLCD_Write("You Win");

        	for(int i = 0; win[i] != '\0'; i++){
        		USART_Tx(UART0,win[i]);
        	}
        	vTaskSuspend(HandleShow);

    }
    

}

//ellenőrzi hogy minden hajót le lőttek e
uint8_t checkShotedShip(){
    for (uint8_t i = 0; i < 35; ++i) {
        for (uint8_t j = 0; j < 5 ; ++j) {
            if (true == map[i][j].ship && false == map[i][j].shot){
                return 0;
            }
        }
    }
    return 1;
}

static void TskUsartMove(void *pvParam){
	while (true){
		if(UsartFlag){
			//xSemaphoreTake(SemaphoreUsart,portMAX_DELAY);
		//vTaskSuspend(NULL);
		//taskENTER_CRITICAL();
			NVIC_DisableIRQ(UART0_RX_IRQn);
			switch(UsartData){
			case 'w':
				up();
				break;
			case 'a':
				left();
				break;
			case 's':
				down();
				break;
			case 'd':
				right();
				break;
			case 'n': //ESC
					random = USART_Rx(UART0)-'0';
					SegmentLCD_AllOff();
				shotNumber=0;
				USART_IntClear(UART0, USART_IEN_RXDATAV);
				NVIC_ClearPendingIRQ(UART0_RX_IRQn);
					vTaskResume(HandleShow);
					vTaskResume(HandleCreate);

				break;
			default:
				xSemaphoreGive(SemaphoreShot);
			}

		UsartFlag = false;
		//taskEXIT_CRITICAL();
		NVIC_EnableIRQ(UART0_RX_IRQn);
		}

		vTaskDelay(configTICK_RATE_HZ/10);
	}
}


void InitPlayGround(){
	SemaphoreShot = xSemaphoreCreateBinary();
	SemaphoreWin = xSemaphoreCreateBinary();
	SemaphoreNewGame = xSemaphoreCreateBinary();
	SemaphoreUsart = xSemaphoreCreateBinary();

 xTaskCreate(
					      TskCreateGameMap,
					      "TskCreateGameMap",
					      configMINIMAL_STACK_SIZE,
					      NULL,
					      tskIDLE_PRIORITY + 10,
						  &HandleCreate
					  );
 xTaskCreate
  (
      TskShowMap,
      "TskShowMap",
      configMINIMAL_STACK_SIZE,
      NULL,
	  tskIDLE_PRIORITY+ 5,
      &HandleShow
  );

 xTaskCreate
  (
      TskShot,
      "TskShot",
      configMINIMAL_STACK_SIZE,
      NULL,
      tskIDLE_PRIORITY + 6,
	  NULL
  );

 /*xTaskCreate
  (
      TskActualPosion,
      "TskActualPosion",
      configMINIMAL_STACK_SIZE,
      NULL,
      tskIDLE_PRIORITY + 4,
      NULL
  );*/

 xTaskCreate
  (
      TskWin,
      "TskWin",
      configMINIMAL_STACK_SIZE,
      NULL,
      tskIDLE_PRIORITY + 7,
      NULL
  );
 xTaskCreate
   (
	   TskUsartMove,
       "TskUsartMove",
       configMINIMAL_STACK_SIZE,
       NULL,
       tskIDLE_PRIORITY + 11,
	   &HandleUsart
   );

 //vTaskSuspend(HandleShow);
 //vTaskSuspend(HandleCreate);
}

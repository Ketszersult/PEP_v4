#include "lcd.h"


/*
 * A simple software delay loop used by this demo to slow down changes.
 */
void delay() {
    for (int d = 0; d < 39999; d++);
}

// A memoriaban beallítja a kivalasztott karakter egy szegmenest ami vilagitani fog
void printOneLowerSegment(uint8_t digitNumber, char segment, uint8_t onOff) {

    switch (segment) {
        case 'a':
            lowerCharSegments[digitNumber].a = onOff;
            break;
        case 'b':
            lowerCharSegments[digitNumber].b = onOff;
            break;
        case 'c':
            lowerCharSegments[digitNumber].c = onOff;
            break;
        case 'd':
            lowerCharSegments[digitNumber].d = onOff;
            break;
        case 'e':
            lowerCharSegments[digitNumber].e = onOff;
            break;
        case 'f':
            lowerCharSegments[digitNumber].f = onOff;
            break;
        case 'g':
            lowerCharSegments[digitNumber].g = onOff;
            lowerCharSegments[digitNumber].m = onOff;
            break;
        case 'h':
            lowerCharSegments[digitNumber].h = onOff;
            break;
        case 'j':
            lowerCharSegments[digitNumber].j = onOff;
            break;
        case 'k':
            lowerCharSegments[digitNumber].k = onOff;
            break;
        case 'n':
            lowerCharSegments[digitNumber].n = onOff;
            break;
        case 'p':
            lowerCharSegments[digitNumber].p = onOff;
            break;
        case 'q':
            lowerCharSegments[digitNumber].q = onOff;
            break;
        default:
            break;
    }
}


// Clear all upper segments
void clearUpperSegments() {

    for (uint8_t p = 0; p < SEGMENT_LCD_NUM_OF_UPPER_CHARS; p++) {
        upperCharSegments[p].raw = 0;
        SegmentLCD_UpperSegments(upperCharSegments);
    }
}


/*
 *
 *  Kiir egy szamoz(0-9) kozott egy digitre a 7 szegmenses kijelzon
 */
void printNumberOneUpperSegment(uint16_t number, uint8_t numberOfSegments) {
    switch (number) {
        case 0:
            upperCharSegments[numberOfSegments].a = 1;
            upperCharSegments[numberOfSegments].b = 1;
            upperCharSegments[numberOfSegments].c = 1;
            upperCharSegments[numberOfSegments].d = 1;
            upperCharSegments[numberOfSegments].e = 1;
            upperCharSegments[numberOfSegments].f = 1;
            upperCharSegments[numberOfSegments].g = 0;
            break;
        case 1:
            upperCharSegments[numberOfSegments].a = 0;
            upperCharSegments[numberOfSegments].b = 1;
            upperCharSegments[numberOfSegments].c = 1;
            upperCharSegments[numberOfSegments].d = 0;
            upperCharSegments[numberOfSegments].e = 0;
            upperCharSegments[numberOfSegments].f = 0;
            upperCharSegments[numberOfSegments].g = 0;
            break;
        case 2:
            upperCharSegments[numberOfSegments].a = 1;
            upperCharSegments[numberOfSegments].b = 1;
            upperCharSegments[numberOfSegments].c = 0;
            upperCharSegments[numberOfSegments].d = 1;
            upperCharSegments[numberOfSegments].e = 1;
            upperCharSegments[numberOfSegments].f = 0;
            upperCharSegments[numberOfSegments].g = 1;
            break;
        case 3:
            upperCharSegments[numberOfSegments].a = 1;
            upperCharSegments[numberOfSegments].b = 1;
            upperCharSegments[numberOfSegments].c = 1;
            upperCharSegments[numberOfSegments].d = 1;
            upperCharSegments[numberOfSegments].e = 0;
            upperCharSegments[numberOfSegments].f = 0;
            upperCharSegments[numberOfSegments].g = 1;
            break;
        case 4:
            upperCharSegments[numberOfSegments].a = 0;
            upperCharSegments[numberOfSegments].b = 1;
            upperCharSegments[numberOfSegments].c = 1;
            upperCharSegments[numberOfSegments].d = 0;
            upperCharSegments[numberOfSegments].e = 0;
            upperCharSegments[numberOfSegments].f = 1;
            upperCharSegments[numberOfSegments].g = 1;
            break;
        case 5:
            upperCharSegments[numberOfSegments].a = 1;
            upperCharSegments[numberOfSegments].b = 0;
            upperCharSegments[numberOfSegments].c = 1;
            upperCharSegments[numberOfSegments].d = 1;
            upperCharSegments[numberOfSegments].e = 0;
            upperCharSegments[numberOfSegments].f = 1;
            upperCharSegments[numberOfSegments].g = 1;
            break;
        case 6:
            upperCharSegments[numberOfSegments].a = 1;
            upperCharSegments[numberOfSegments].b = 0;
            upperCharSegments[numberOfSegments].c = 1;
            upperCharSegments[numberOfSegments].d = 1;
            upperCharSegments[numberOfSegments].e = 1;
            upperCharSegments[numberOfSegments].f = 1;
            upperCharSegments[numberOfSegments].g = 1;
            break;
        case 7:
            upperCharSegments[numberOfSegments].a = 1;
            upperCharSegments[numberOfSegments].b = 1;
            upperCharSegments[numberOfSegments].c = 1;
            upperCharSegments[numberOfSegments].d = 0;
            upperCharSegments[numberOfSegments].e = 0;
            upperCharSegments[numberOfSegments].f = 0;
            upperCharSegments[numberOfSegments].g = 0;
            break;
        case 8:
            upperCharSegments[numberOfSegments].a = 1;
            upperCharSegments[numberOfSegments].b = 1;
            upperCharSegments[numberOfSegments].c = 1;
            upperCharSegments[numberOfSegments].d = 1;
            upperCharSegments[numberOfSegments].e = 1;
            upperCharSegments[numberOfSegments].f = 1;
            upperCharSegments[numberOfSegments].g = 1;
            break;
        case 9:
            upperCharSegments[numberOfSegments].a = 1;
            upperCharSegments[numberOfSegments].b = 1;
            upperCharSegments[numberOfSegments].c = 1;
            upperCharSegments[numberOfSegments].d = 1;
            upperCharSegments[numberOfSegments].e = 0;
            upperCharSegments[numberOfSegments].f = 1;
            upperCharSegments[numberOfSegments].g = 1;
            break;
    }
}

/*
 * Kiir egy 4 jegyu termeszetes szamot a 7 szegmenses kijelzore
 * Ha a szam nincs 4 karakteres akkor az elejen nem hasznalt helyiertekeket nem jeleniti meg
 * A 9999-nel nagyobb szamoknal csak az also 4 helyierteket jeleníti meg
 */
void printNumberAllUpperSegments(uint16_t number) {
    uint16_t tempNumber = number % 10000;
    clearUpperSegments();

    uint8_t digit;
    if (1000 <= number) {
        digit = tempNumber / 1000;
        printNumberOneUpperSegment(digit, 3);
    }
    if (100 <= number) {
        tempNumber %= 1000;
        digit = tempNumber / 100;
        printNumberOneUpperSegment(digit, 2);
    }
    if (10 <= number) {
        tempNumber %= 100;
        digit = tempNumber / 10;
        printNumberOneUpperSegment(digit, 1);
    }
    tempNumber %= 10;
    digit = tempNumber;
    printNumberOneUpperSegment(digit, 0);

    SegmentLCD_UpperSegments(upperCharSegments); // itt jelenik meg a kijelzon, eddig csak memoriaba toltodik

}

//Kikapcsola a ring osszes szegmenset
void ringOff() {
    for (uint8_t i = 0; i < 8; ++i) {
        SegmentLCD_ARing(i, 0);
    }
}

//Bekapcsolaja a ring osszes szegmenset
void ringOn() {
    for (uint8_t i = 0; i < 8; ++i) {
        SegmentLCD_ARing(i, 1);
    }
}

//villogtatja a ringet
void ringBlink() {

    ringOn();
    delay();
    ringOff();
    delay();
}

//Korbe feltolti szegmensenkent a ringet
void ringFill() {
    for (uint8_t i = 0; i < 8; ++i) {
        SegmentLCD_ARing(i, 1);
        delay();
    }
}

//egy szegmens villogtatasa az aktualis helyzet kijelzesehez
void blinkOneLowerSegment(uint32_t digitNumber, char segment){
    printOneLowerSegment(digitNumber,segment,1);
    SegmentLCD_LowerSegments(lowerCharSegments); //Itt jelennek meg tenylegesen
    delay();
    printOneLowerSegment(digitNumber,segment,0);
    SegmentLCD_LowerSegments(lowerCharSegments); //Itt jelennek meg tenylegesen
    delay();

}

#include <avr/io.h>
#include "SPI.h"

void initSPI(void) {
    //MISO <-> Serial Data Out
    //MOSI <-> Serial Data In

    SPI_MISO_DDR |= _BV(SPI_MISO); //output
    SPI_MISO_PORT &= ~_BV(SPI_MISO);

    SPI_MOSI_DDR &= ~_BV(SPI_MOSI); //input
    SPI_MOSI_PORT |= _BV(SPI_MOSI); // pull-up enable

    SPI_SCK_DDR |= _BV(SPI_SCK); //output
    SPI_SCK_PORT &= ~_BV(SPI_SCK);
}

uint8_t spiTransfer(uint8_t c) {
    USIDR = c; // USI Data Register
    USISR = _BV(USIOIF); // USI Status Register | Counter Overflow Interrupt Flag -> is 1 when the 4 bit counter (0->15) overflow
    while((USISR & _BV(USIOIF)) == 0) { // loop 16 times
        //USI Control register - USIWireMode0 (set to 1 means three-wire-mode SPI like) - Clock Source Select (Software clock strobe USITC) - Clock Strobe (shift register by one and increment clock) - Toggle Clock(set 1 to toggle clock on each loop)
        USICR = _BV(USIWM0) | _BV(USICS1) | _BV(USICLK) | _BV(USITC);
    }
    // now this contain the response from the slave
    return USIDR;
}

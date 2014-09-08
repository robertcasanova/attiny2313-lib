#include <avr/io.h>
#include <avr/pgmspace.h>
#include "SPI.h"
#include "SPI_EEPROM.h"

void initEEPROM(void) {
    SPI_EEPROM_DDR |= _BV(SPI_EEPROM_CS);
    SPI_EEPROM_PORT |= _BV(SPI_EEPROM_CS); // high is not selected
}

uint8_t EEPROM_readStatus(void) {
    uint8_t status;

    SLAVE_SELECT;
    spiTransfer(EEPROM_STATUS);
    status = spiTransfer(0);
    SLAVE_DESELECT;

    return status;
}

void EEPROM_enableWrite(void) {
    SLAVE_SELECT;
    spiTransfer(EEPROM_ENABLE_WRITE);
    SLAVE_DESELECT;
}

void EEPROM_disableWrite(void) {
    SLAVE_SELECT;
    spiTransfer(EEPROM_DISABLE_WRITE);
    SLAVE_DESELECT;
}

uint8_t EEPROM_readByte(uint16_t address) {
    uint8_t data;

    SLAVE_SELECT;
    spiTransfer(EEPROM_READ);
    spiTransfer(address >> 8);
    spiTransfer(address & 0xFF);
    data = spiTransfer(0);
    SLAVE_DESELECT;

    return data;
}

void EEPROM_writeByte(uint16_t address, uint8_t data) {

    while(EEPROM_readStatus() & _BV(EEPROM_WRITE_IN_PROGRESS)) { ; } // wait until bit of Writing in Progress is clear

    EEPROM_enableWrite();

    SLAVE_SELECT;
    spiTransfer(EEPROM_WRITE);
    spiTransfer(address >> 8);
    spiTransfer(address & 0xFF);
    spiTransfer(data);
    SLAVE_DESELECT;
}

uint8_t * EEPROM_readPage(uint16_t address) {
    uint8_t data[EEPROM_BYTES_PER_PAGE];
    uint16_t i;

    SLAVE_SELECT;
    spiTransfer(EEPROM_READ);
    spiTransfer(address >> 8);
    spiTransfer(address & 0xFF);
    for(i = 0; i < EEPROM_BYTES_PER_PAGE; i++) {
        data[i] = spiTransfer(0);
    }
    SLAVE_DESELECT;

    return data;
}

void EEPROM_writePage(uint16_t address, uint8_t *data) {
    while(EEPROM_readStatus() & _BV(EEPROM_WRITE_IN_PROGRESS)) { ; } // wait until bit of Writing in Progress is clear

    uint8_t i;

    EEPROM_enableWrite();

    SLAVE_SELECT;
    NOP;NOP;NOP;NOP;
    spiTransfer(EEPROM_WRITE);
    spiTransfer(address >> 8);
    spiTransfer(address & 0xFF);
    for(i=0; i < EEPROM_BYTES_PER_PAGE; i++) {
        spiTransfer(pgm_read_byte(&(data[i])));
    }
    SLAVE_DESELECT;
    NOP;NOP;NOP;NOP;

}



void EEPROM_clear(void) {
    uint8_t i = 0;
    uint16_t pageNum = 0;



    while(pageNum < EEPROM_TOT_PAGES) {
        uint16_t pageAddress = pageNum * EEPROM_BYTES_PER_PAGE;

        while(EEPROM_readStatus() & _BV(EEPROM_WRITE_IN_PROGRESS)) { ; }
        EEPROM_enableWrite();

        SLAVE_SELECT;
        spiTransfer(EEPROM_WRITE);
        spiTransfer(pageAddress >> 8);
        spiTransfer(pageAddress & 0xFF);
        for(i=0; i < EEPROM_BYTES_PER_PAGE; i++) { // write 32 bytes to 0
            spiTransfer(0);
        }
        SLAVE_DESELECT;
        pageNum++;
    }
}

void EEPROM_readIntoLeds(uint16_t address) {
    uint8_t x;

    SLAVE_SELECT;
    spiTransfer(EEPROM_READ);
    spiTransfer(address >> 8);
    spiTransfer(address & 0xFF);
    x = spiTransfer(0); // first read from EEPROM and save to x
    x = spiTransfer(x); // start shift first byte
    spiTransfer(x); // shift second byte


    SLAVE_DESELECT;

    DDRD |= _BV(PD5);

    PORTD |= _BV(PD5);
    NOP;NOP;NOP;NOP;
    PORTD &= ~_BV(PD5);

}

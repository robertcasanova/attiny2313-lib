// This code assume you are using a 25AA320A/25LC320A or similar EEPROM


#include <avr/io.h>

#define SPI_EEPROM_DDR DDRB
#define SPI_EEPROM_PORT PORTB
#define SPI_EEPROM_CS PB4 //i'm using PB4 as master CS pin for EEPROM

#define SLAVE_SELECT SPI_EEPROM_PORT &= ~_BV(SPI_EEPROM_CS) // put CS low -> activate slave
#define SLAVE_DESELECT SPI_EEPROM_PORT |= _BV(SPI_EEPROM_CS) // put CS high -> deactivate slave

//EEPROM Istructions
#define EEPROM_WRITE 0x02
#define EEPROM_READ 0x03
#define EEPROM_DISABLE_WRITE 0x04
#define EEPROM_ENABLE_WRITE 0x06
#define EEPROM_STATUS 0x05
#define EEPROM_SET_STATUS 0x01

//EEPROM Status Register
#define EEPROM_WRITE_IN_PROGRESS 0x00 // When set to a ‘1’, a write is in progress, when set to a ‘0’, no write is in progress.
#define EEPROM_WRITE_ENABLE 0x01 // 1-> Enable Writing, 0-> Disable writing ( To change this use EEPROM_ENABLE_WRITE command )
#define EEPROM_BLOCK_PROTECTION_0 0x02 // both 0 -> no write protection, 0-1 last 1/4, 1-0 last 1/2, 1-1 all
#define EEPROM_BLOCK_PROTECTION_1 0x03

//EEPROM General Info
#define EEPROM_BYTES_PER_PAGE 32
#define EEPROM_TOT_PAGES 1000

// stop one cpu clock
#define NOP asm("nop")


void initEEPROM(void); // set CS as output with pull-up enable

uint8_t EEPROM_readStatus(void); // return status register

void EEPROM_enableWrite(void);
void EEPROM_disableWrite(void);

uint8_t EEPROM_readByte(uint16_t address);
void EEPROM_writeByte(uint16_t address, uint8_t data);

uint8_t * EEPROM_readPage(uint16_t address);
void EEProm_writePage(uint16_t address, uint8_t *data);

void EEPROM_clear(void);

void EEPROM_readIntoLeds(uint16_t address);

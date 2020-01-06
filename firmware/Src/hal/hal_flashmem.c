/*
 * Thin layer around the STM32 flash write/erase to handle persistent setting storage.
 */

/* ----- System Includes ---------------------------------------------------- */

#include <math.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "hal_flashmem.h"
#include "stm32f4xx_hal.h"

/* -------------------------------------------------------------------------- */

// Helpers for flash sector addresses
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0,  16 Kbyte */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1,  16 Kbyte */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2,  16 Kbyte */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3,  16 Kbyte */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4,  64 Kbyte */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5,  128 Kbyte */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6,  128 Kbyte */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7,  128 Kbyte */
// Following pages not available on 429VET6 part
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8,  128 Kbyte */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9,  128 Kbyte */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbyte */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbyte */

#define FLASH_VOLTAGE_SETTING FLASH_VOLTAGE_RANGE_3

// Sectors reserved for non-volatile storage
#define FLASH_START_ADDRESS  ADDR_FLASH_SECTOR_2
#define FLASH_END_ADDRESS    (ADDR_FLASH_SECTOR_4 - 0x01)

#define PAGE_SIZE               0x4000  /* Page size = 16KByte */
#define PAGE_COUNT              0x02

#define PAGE0_BASE_ADDRESS    ((uint32_t)(FLASH_START_ADDRESS + 0x0000))
#define PAGE0_END_ADDRESS     ((uint32_t)(FLASH_START_ADDRESS + (PAGE_SIZE - 1)))
#define PAGE0_ID               FLASH_SECTOR_2

#define PAGE1_BASE_ADDRESS    ((uint32_t)(FLASH_START_ADDRESS + 0x4000))
#define PAGE1_END_ADDRESS     ((uint32_t)(EEPROM_START_ADDRESS + (2 * PAGE_SIZE - 1)))
#define PAGE1_ID               FLASH_SECTOR_3


/* -------------------------------------------------------------------------- */


PRIVATE void hal_flashmem_erase_sector(uint32_t address);

PRIVATE uint32_t hal_flashmem_sector_number(uint32_t address);
PRIVATE void hal_flashmem_unlock( void );
PRIVATE void hal_flashmem_lock( void );
uint8_t FLASH_OB_GetRDP(void);                                  // private to stm32f4xx_hal_flash_ex.c
PRIVATE bool hal_flashmem_readout_protection_enabled( void );


typedef enum {
    FLASH_BUSY = 1,
    FLASH_COMPLETE,
} FlashStatus_t;

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_flashmem_init(void )
{
//    HAL_FLASH_Unlock();
//    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_flashmem_store( uint16_t address, uint8_t *data, uint16_t len)
{
    uint16_t current_address = address;
    uint16_t bytes_written = 0;

//    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data);

}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_flashmem_retrieve(uint16_t address, uint8_t *data, uint16_t len)
{
    uint16_t current_address = address;
    uint16_t bytes_read = 0;


}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_flashmem_erase_sector(uint32_t address)
{
    uint32_t flash_status = 0x00;   // error sector address, 0xFFFFFFFFU written when ok

    FLASH_EraseInitTypeDef sErase;
    sErase.VoltageRange = FLASH_VOLTAGE_SETTING;
    sErase.TypeErase = FLASH_TYPEERASE_SECTORS;
    sErase.NbSectors = 1;
    sErase.Sector = hal_flashmem_sector_number(address);

    if( HAL_FLASHEx_Erase(&sErase, &flash_status) != HAL_OK )
    {
        // TODO handle flash erase error gracefully?
        _Error_Handler(__FILE__, __LINE__);
    }
}

/* -------------------------------------------------------------------------- */

// Return the sector number containing the input address
PRIVATE uint32_t
hal_flashmem_sector_number(uint32_t address)
{
    uint32_t sector = 0;

    if(address < ADDR_FLASH_SECTOR_1 && address >= ADDR_FLASH_SECTOR_0)
    {
        sector = FLASH_SECTOR_0;
    }
    else if(address < ADDR_FLASH_SECTOR_2 && address >= ADDR_FLASH_SECTOR_1)
    {
        sector = FLASH_SECTOR_1;
    }
    else if(address < ADDR_FLASH_SECTOR_3 && address >= ADDR_FLASH_SECTOR_2)
    {
        sector = FLASH_SECTOR_2;
    }
    else if(address < ADDR_FLASH_SECTOR_4 && address >= ADDR_FLASH_SECTOR_3)
    {
        sector = FLASH_SECTOR_3;
    }
    else if(address < ADDR_FLASH_SECTOR_5 && address >= ADDR_FLASH_SECTOR_4)
    {
        sector = FLASH_SECTOR_4;
    }
    else if(address < ADDR_FLASH_SECTOR_6 && address >= ADDR_FLASH_SECTOR_5)
    {
        sector = FLASH_SECTOR_5;
    }
    else if(address < ADDR_FLASH_SECTOR_7 && address >= ADDR_FLASH_SECTOR_6)
    {
        sector = FLASH_SECTOR_6;
    }
    else if(address < ADDR_FLASH_SECTOR_8 && address >= ADDR_FLASH_SECTOR_7)
    {
        sector = FLASH_SECTOR_7;
    }
    else if(address < ADDR_FLASH_SECTOR_9 && address >= ADDR_FLASH_SECTOR_8)
    {
        sector = FLASH_SECTOR_8;
    }
    else if(address < ADDR_FLASH_SECTOR_10 && address >= ADDR_FLASH_SECTOR_9)
    {
        sector = FLASH_SECTOR_9;
    }
    else if(address < ADDR_FLASH_SECTOR_11 && address >= ADDR_FLASH_SECTOR_10)
    {
        sector = FLASH_SECTOR_10;
    }
    else	/*(Address < FLASH_END_ADDR && Address >= ADDR_FLASH_SECTOR_11)*/
    {
        sector = FLASH_SECTOR_11;
    }
    return sector;
}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_flashmem_unlock( void )
{
    HAL_FLASH_Unlock();
}

PRIVATE void
hal_flashmem_lock( void )
{
    HAL_FLASH_Lock();
}

/* -------------------------------------------------------------------------- */

// OB_RDP_LEVEL_0 when protection off,
// OB_RDP_LEVEL_1 for memory protection,
// OB_RDP_LEVEL_2 when full chip protected
PRIVATE bool hal_flashmem_readout_protection_enabled( void )
{
    return !( FLASH_OB_GetRDP() == OB_RDP_LEVEL_0 );
}

/* ----- End ---------------------------------------------------------------- */

/*
 * Thin layer around the STM32 flash write/erase to handle persistent setting storage.
 */

/* ----- System Includes ---------------------------------------------------- */

#include "string.h"

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

#define PAGE_ACTIVE_MARKER  0xDEADBEEF
#define PAGE_MARKER_LENGTH  4

/* -------------------------------------------------------------------------- */

PRIVATE void hal_flashmem_prepare( void );

PRIVATE uint32_t hal_flashmem_find_end_address( uint8_t sector );

PRIVATE void hal_flashmem_migrate_sector( uint8_t new_sector );

PRIVATE void hal_flashmem_erase_sector(uint32_t address);

PRIVATE bool hal_flashmem_is_sector_active( uint8_t sector_to_check );

PRIVATE uint32_t hal_flashmem_get_sector_number(uint32_t address);

PRIVATE uint32_t hal_flashmem_get_sector_address(uint8_t sector);

PRIVATE void hal_flashmem_unlock( void );

PRIVATE void hal_flashmem_lock( void );

uint8_t FLASH_OB_GetRDP(void);              // private to stm32f4xx_hal_flash_ex.c

PRIVATE bool hal_flashmem_readout_protection_enabled( void );


/* -------------------------------------------------------------------------- */

typedef struct {
    uint16_t id;
    uint16_t len;
    uint32_t new_address;
} StoredVariableHeader_t;

uint8_t sector_in_use = 0;
uint32_t address_of_end = 0;

/* -------------------------------------------------------------------------- */
//    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

PUBLIC void
hal_flashmem_init(void )
{
    // Check if the flash is accessible before we try to init
    if( hal_flashmem_readout_protection_enabled() )
    {
        // uh what?
        _Error_Handler(__FILE__, __LINE__);
    }

    // Work out which bank of memory is the current one
    if( hal_flashmem_is_sector_active(PAGE0_ID) )
    {
        sector_in_use = PAGE0_ID;
    }
    else if( hal_flashmem_is_sector_active(PAGE1_ID) )
    {
        sector_in_use = PAGE1_ID;
    }
    else    // neither sector is in use, therefore we should get one ready
    {
        hal_flashmem_prepare();
    }

    // Walk through the data and find the 'end' of the written data
    address_of_end = hal_flashmem_find_end_address( sector_in_use );

}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_flashmem_store( uint16_t id, uint8_t *data, uint16_t len)
{
    // Find the address of the last record for the given ID


    // Check the new data is different


    // Check the area we want to write into is empty




    // Prepare the header information for the new entry
    StoredVariableHeader_t metadata;

    metadata.id = id;
    metadata.len = len;
    metadata.new_address = 0xFFFFFFFF;  // don't write anything there right now

    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address_of_end, metadata.id);
    address_of_end += 2;
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address_of_end, metadata.len);
    address_of_end += 2;
    // don't write anything into the address field yet
    address_of_end += 4;


    //    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data);


}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_flashmem_retrieve(uint16_t id, uint8_t *data, uint16_t len)
{
    // Find the latest copy of the variable

    // Check that it's valid

    // Copy the valid data into the user's buffer/whatever

    memcpy( &data, &address, len);
}

/* -------------------------------------------------------------------------- */



// Wipe both sectors, put the magic word in the first sector
PRIVATE void
hal_flashmem_prepare( void )
{
    // Wipe the two storage sectors
    hal_flashmem_erase_sector( hal_flashmem_get_sector_address( PAGE0_ID ) );
    hal_flashmem_erase_sector( hal_flashmem_get_sector_address( PAGE1_ID ) );

    // Write the magic word into the base of the sector to indicate active use
    uint32_t sector_marker = PAGE_ACTIVE_MARKER;
    hal_flashmem_store( PAGE0_ID, &sector_marker, sizeof(sector_marker) );
    sector_in_use = PAGE0_ID;
}

/* -------------------------------------------------------------------------- */

PRIVATE uint32_t
hal_flashmem_find_end_address( uint8_t sector )
{
    // first entry starts one FULLWORD from the sector base address
    uint32_t scan_addr = hal_flashmem_get_sector_address( sector ) + PAGE_MARKER_LENGTH;
    uint32_t sector_limit_addr = hal_flashmem_get_sector_address( sector+1 );

    bool end_found = false;
    StoredVariableHeader_t *tmp_entry;

    // Walk though entries until we find blank space
    while( scan_addr < sector_limit_addr && !end_found )
    {
        // 'Read' the entry metadata
        tmp_entry = (StoredVariableHeader_t *)scan_addr;

        // Check the entry at this address has data (id and size both must be non-zero
        if( tmp_entry->id && tmp_entry->len )
        {
            // there's data, move the address to that of the next entry
            scan_addr += 4 + tmp_entry->len;  // remember to skip 4-bytes for the address
            // todo follow data to new address based on assumption instead of searching every entry
        }
        else
        {
            end_found = true;
        }
    }

    if( scan_addr >= sector_limit_addr )
    {
        // We shouldn't have found the data after the bank of flash ends...
        _Error_Handler(__FILE__, __LINE__);
    }

    return scan_addr;
}

/* -------------------------------------------------------------------------- */

// Copies the most recent data from the current sector into the new sector, marks it as active, and wipes the old one
PRIVATE void
hal_flashmem_migrate_sector( uint8_t new_sector )
{
    uint8_t old_sector = sector_in_use;

    // Find the most recent data and copy to the other sector
    //memcpy( &new_sector_address, &old_sector_address, data_size );


    // Mark the new sector as active
    uint32_t sector_marker = PAGE_ACTIVE_MARKER;
    hal_flashmem_store( new_sector, &sector_marker, sizeof(sector_marker) );

    // Wipe the old sector
    hal_flashmem_erase_sector( hal_flashmem_get_sector_address( old_sector ) );

    // We are now using a new sector
    sector_in_use = new_sector;
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
    sErase.Sector = hal_flashmem_get_sector_number(address);

    if( HAL_FLASHEx_Erase(&sErase, &flash_status) != HAL_OK )
    {
        // TODO handle flash erase error gracefully?
        _Error_Handler(__FILE__, __LINE__);
    }
}

PRIVATE bool
hal_flashmem_is_sector_active( uint8_t sector_to_check )
{
    uint32_t *sector_start;
    sector_start = hal_flashmem_get_sector_address( sector_to_check );

    return ( *sector_start == PAGE_ACTIVE_MARKER );
}

/* -------------------------------------------------------------------------- */

// Return the sector number containing the input address
PRIVATE uint32_t
hal_flashmem_get_sector_number(uint32_t address)
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

PRIVATE uint32_t
hal_flashmem_get_sector_address(uint8_t sector)
{
    switch( sector )
    {
        case FLASH_SECTOR_0:
            return ADDR_FLASH_SECTOR_0;
        case FLASH_SECTOR_1:
            return ADDR_FLASH_SECTOR_1;
        case FLASH_SECTOR_2:
            return ADDR_FLASH_SECTOR_2;
        case FLASH_SECTOR_3:
            return ADDR_FLASH_SECTOR_3;
        case FLASH_SECTOR_4:
            return ADDR_FLASH_SECTOR_4;
        case FLASH_SECTOR_5:
            return ADDR_FLASH_SECTOR_5;
        case FLASH_SECTOR_6:
            return ADDR_FLASH_SECTOR_6;
        case FLASH_SECTOR_7:
            return ADDR_FLASH_SECTOR_7;
        case FLASH_SECTOR_8:
            return ADDR_FLASH_SECTOR_8;
        case FLASH_SECTOR_9:
            return ADDR_FLASH_SECTOR_9;
        case FLASH_SECTOR_10:
            return ADDR_FLASH_SECTOR_10;
        case FLASH_SECTOR_11:
            return ADDR_FLASH_SECTOR_11;
    }
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

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

#define PAGE_SIZE   0x4000  // Page size = 16KByte
#define PAGE_COUNT  2       // 2 sectors reserved for NV storage

#define PAGE0_ID    FLASH_SECTOR_2
#define PAGE1_ID    FLASH_SECTOR_3

#define PAGE_ACTIVE_MARKER  (uint32_t)0xDEADBEEF
#define PAGE_MARKER_LENGTH  4

#define FLASH_WORD_UNWRITTEN 0xFFFFFFFF

/* -------------------------------------------------------------------------- */

PRIVATE void hal_flashmem_prepare( void );

PRIVATE uint32_t hal_flashmem_find_end_address( uint8_t sector );

PRIVATE uint32_t hal_flashmem_find_variable_entry(uint16_t identifier );

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
    hal_flashmem_unlock();

    // Find the address of the last record for the given ID
    uint32_t entry_addr = hal_flashmem_find_variable_entry( id );
    StoredVariableHeader_t *existing_metadata;
    existing_metadata = (StoredVariableHeader_t *)entry_addr;

    uint8_t store_state = 0;    // 0 = null, 1 = identical data early exit, 2 = no previous entry, 3 = previous entry updated

    // If we've got a previous entry for the same ID, we should update the metadata with the address of the new entry
    // or just write nothing if the data is identical to the existing version in storage
    if( entry_addr && existing_metadata->id )
    {
        entry_addr += sizeof(StoredVariableHeader_t);   // payload is after the header
        uint32_t *entry_payload_addr;
        entry_payload_addr = entry_addr+sizeof(StoredVariableHeader_t);

        // Check if the entry payload is different from the requested store
        if( existing_metadata->len == len && memcmp( entry_payload_addr, data, len ) == 0 )
        {
            store_state = 1;  // the existing entry is identical
        }
        else
        {
            // Write the address of the new entry into the current entry
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, entry_addr+4, address_of_end );
            store_state = 3;
        }

    }
    else
    {
        // A copy with that ID must not exist in storage yet...
        store_state = 2;
    }

    // Write a new entry to store the data
    if(store_state != 1 )
    {
        // Prepare the header information for the new entry
        StoredVariableHeader_t new_metadata;
        new_metadata.id = id;
        new_metadata.len = len;
        new_metadata.new_address = 0xFFFFFFFF;  // don't write anything there right now

        uint16_t payload_bytes_written = 0;

        uint32_t new_entry_addr = address_of_end;
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, new_entry_addr, new_metadata.id);
        new_entry_addr += 2;
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, new_entry_addr, new_metadata.len);
        new_entry_addr += 2;
        // don't write anything into the address field yet
        new_entry_addr += 4;

        // Write the payload into flash
        for( payload_bytes_written = 0; payload_bytes_written <= len; )
        {
            // TODO flash words until near end of payload, then cleanly word align with padding zeros
            HAL_FLASH_Program( FLASH_TYPEPROGRAM_WORD, new_entry_addr+payload_bytes_written, data[payload_bytes_written] );
            payload_bytes_written += 4;
        }

        address_of_end = new_entry_addr + payload_bytes_written + 1;
    }

    hal_flashmem_lock();

}

/* -------------------------------------------------------------------------- */

// Find the latest copy of the variable, and copy it into the user's buffer

PUBLIC uint16_t
hal_flashmem_retrieve(uint16_t id, uint8_t *buffer, uint16_t buff_len)
{
    uint32_t entry_addr = hal_flashmem_find_variable_entry( id );
    StoredVariableHeader_t *ret_metadata;
    ret_metadata = (StoredVariableHeader_t *)entry_addr;

    uint16_t bytes_to_copy = 0;

    // Check that the entry is valid
    if( entry_addr && ret_metadata->id )
    {
        entry_addr += sizeof(StoredVariableHeader_t);   // payload is after the header

        if( ret_metadata->len > buff_len )
        {
            // uh oh, they don't want the full thing...
            bytes_to_copy = buff_len;
        }
        else
        {
            bytes_to_copy = ret_metadata->len;
        }

        // Copy the valid data into the user's buffer
        memcpy( &buffer, &entry_addr, bytes_to_copy);
    }

    return bytes_to_copy;
}

/* -------------------------------------------------------------------------- */

// Wipe both sectors, put the magic word in the first sector
PRIVATE void
hal_flashmem_prepare( void )
{
    hal_flashmem_unlock();

    // Wipe the two storage sectors
    hal_flashmem_erase_sector( hal_flashmem_get_sector_address( PAGE0_ID ) );
    hal_flashmem_erase_sector( hal_flashmem_get_sector_address( PAGE1_ID ) );

    // Write the magic word into the base of the sector to indicate active use
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, hal_flashmem_get_sector_address(PAGE0_ID), PAGE_ACTIVE_MARKER );

    sector_in_use = PAGE0_ID;

    hal_flashmem_lock();
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
            // TODO Follow data to new address based on assumption instead of searching every entry
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

PRIVATE uint32_t
hal_flashmem_find_variable_entry(uint16_t identifier )
{
    uint32_t scan_addr = hal_flashmem_get_sector_address( sector_in_use ) + PAGE_MARKER_LENGTH;
    uint32_t sector_limit_addr = hal_flashmem_get_sector_address( sector_in_use+1 );

    bool id_found = false;
    StoredVariableHeader_t *tmp_entry;

    // Walk though entries until we find our ID, then follow the chain of address to the last one
    while( scan_addr < sector_limit_addr && !id_found )
    {
        // 'Read' the entry metadata
        tmp_entry = (StoredVariableHeader_t *)scan_addr;

        // Check the entry at this address has data (id and size both must be non-zero
        if( tmp_entry->id && tmp_entry->len )
        {
            // Is the current entry the right ID?
            if( tmp_entry->id == identifier)
            {
                // check the address metadata field to see if there's a newer entry
                if( tmp_entry->new_address != FLASH_WORD_UNWRITTEN)
                {
                    scan_addr = tmp_entry->new_address; // search there next pass
                }
                else
                {
                    id_found = true;  // this entry is the latest version of the data
                }
            }
            else
            {
                // check the next entry in the block next pass
                scan_addr += 4 + tmp_entry->len;  // remember to skip 4-bytes for the address
            }
        }
        else    // found the end of data somehow
        {
            scan_addr = 0;
            id_found = true;    // just exit with the 'error address'
        }
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

#ifndef HAL_FLASHMEM
#define HAL_FLASHMEM

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_flashmem_init(void );

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_flashmem_store(uint16_t address, uint8_t *data, uint16_t len);

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_flashmem_retrieve(uint16_t address, uint8_t *data, uint16_t len);

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_FLASHMEM */

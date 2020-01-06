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
hal_flashmem_store(uint16_t id, uint8_t *data, uint16_t len);

/* -------------------------------------------------------------------------- */

PUBLIC uint16_t
hal_flashmem_retrieve(uint16_t id, uint8_t *buffer, uint16_t buff_len);

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_FLASHMEM */

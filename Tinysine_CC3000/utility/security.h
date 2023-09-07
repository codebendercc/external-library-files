
#ifndef __SECURITY__
#define __SECURITY__

#include "nvmem.h"

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef  __cplusplus
extern "C" {
#endif


#define AES128_KEY_SIZE		16

#ifndef CC3000_UNENCRYPTED_SMART_CONFIG


//*****************************************************************************
//
//!  aes_encrypt
//!
//!  @param[in]  key   AES128 key of size 16 bytes
//!  @param[in\out] state   16 bytes of plain text and cipher text
//!
//!  @return  none
//!
//!  @brief   AES128 encryption:
//!           Given AES128 key and  16 bytes plain text, cipher text of 16 bytes
//!           is computed. The AES implementation is in mode ECB (Electronic 
//!           Code Book). 
//!	 
//!
//*****************************************************************************
extern void aes_encrypt(unsigned char *state, unsigned char *key);

//*****************************************************************************
//
//!  aes_decrypt
//!
//!  @param[in]  key   AES128 key of size 16 bytes
//!  @param[in\out] state   16 bytes of cipher text and plain text
//!
//!  @return  none
//!
//!  @brief   AES128 decryption:
//!           Given AES128 key and  16 bytes cipher text, plain text of 16 bytes
//!           is computed The AES implementation is in mode ECB 
//!           (Electronic Code Book).
//!	 
//!
//*****************************************************************************
extern void aes_decrypt(unsigned char *state, unsigned char *key);


//*****************************************************************************
//
//!  aes_read_key
//!
//!  @param[out]  key   AES128 key of size 16 bytes
//!
//!  @return  on success 0, error otherwise.
//!
//!  @brief   Reads AES128 key from EEPROM
//!           Reads the AES128 key from fileID #12 in EEPROM
//!           returns an error if the key does not exist. 
//!	 
//!
//*****************************************************************************
extern signed long aes_read_key(unsigned char *key);

//*****************************************************************************
//
//!  aes_write_key
//!
//!  @param[out]  key   AES128 key of size 16 bytes
//!
//!  @return  on success 0, error otherwise.
//!
//!  @brief   writes AES128 key from EEPROM
//!           Writes the AES128 key to fileID #12 in EEPROM
//!	 
//!
//*****************************************************************************
extern signed long aes_write_key(unsigned char *key);

#endif //CC3000_UNENCRYPTED_SMART_CONFIG

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef  __cplusplus
}
#endif // __cplusplus

#endif

/*********************************************************************
*
* Library of SGLfence: A library aiming to add a better software-based solution to preventing spectre leakage
* 
* Taking inspiration from SpectreGuard, InvisiSpec, SafeSpec, oo7
* 

Ideas: 
-	Make a C library and have flags implemented software wise so it can be updated
-	Mark data blocks as tainted/secret/invisible
-	Data marked as secret must be handed to a secret-dependent instruction to be decoded
-	Processes marked as invisible are processed/stored in separate data structure when being speculatively executed. Processes are stored in separate structure until approved and added to CPU Cache. If not approved, processes are squashed.
-	Some sections ARE completely blocked from speculative execution, if deemed of high importance.
-	Perhaps some way to assess code for spectre likelihood based on its structure/vulnerability, observe it closely when being executed?




**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <string.h>     //for memcm
#include <math.h>
#include <memory.h> //These include FetchChar, FetchInt, FetchLong, FetchFloat, StoreChar, StoreInt, StoreLong, and StoreFloat. To move blocks of data from one location to another in paged or common RAM, use the CmoveMany, CmoveManyCheck, or ToMemory routines.
#include <x86intrin.h> // for _mm_lfence();
#include <immintrin.h>
#include "sglfence.h"
#include "c_language_hashtable.h"
//#include <eeprom.c> 

//https://software.intel.com/sites/landingpage/IntrinsicsGuide/#

//Try storing into DRAM?
//Using register keyword to store into cpu???

//https://en.cppreference.com/w/cpp/atomic/atomic_thread_fence 
//might be useful for ordering

int label_important_data(){
    //Do not allow for ANY speculative execution here
    //Label 1

    if (data_section.flag_on) //if is true, then lfence has already been called
    {
        //There's really not much else to do, as I think this dooms all following commands from here on involved with this input to be serialized.
        //to mess with this more might require doing this with volatile threads, lfencing them and then combining their work together later. 
        break;
        
    }
    else
    {
        _mm_lfence();
        //Memory is synchronized but also now immune to preemptive execution
    }
    

}

int label_medium_important_data(){
    // Allocate things onto Ram with malloc() or things in memory.h ??
    //Label 2

    if (data_section.flag_on) //if is true, then between flags
    {
        /* code */
    }
    else
    {
        /* code */
    }
    
    

}

int label_unimportant_data(){
    //Label 0
    //allow for as much leakage as we want. Likely will not need to do anything in this section.

    /*if (data_section.flag_on) //if is true, then between flags
    {

    }
    else
    {

    }
    */


}


//look at uints?
//_qv or _eeprom 
//_eeprom should allow for read-only memory
//Make all values in the section established Volatile so its in RAM?

//readMemoryByte in spectre_sglfence to see how to read memory byte
//   flush_memory_sse AND _mm_clflush ????

// http://www.mosaic-industries.com/embedded-systems/sbc-single-board-computers/hcs12-9s12-freescale-development-board/c-application-programming
//Dang I wish I had access to those parts of storage that the OS obvs shouldn't have access to.

//https://www.ti.com/lit/an/spra177a/spra177a.pdf?ts=1607287421984&ref_url=https%253A%252F%252Fwww.google.com%252F 
void beginning_or_ending_flip(int data_label){
    
    
    switch(data_label) {
        case 0 :
            label_unimportant_data();
            break; //optional
        case 1  :
            label_important_data();
            break; //optional
        case 2:
            label_medium_important_data();
            break; //optional
        
        default: //Optional
            //Just to be safe, if we don't know if a section is important, we may as well secure it... though, it will be kinda slow.
            label_important_data();
        }
}



// https://stackoverflow.com/questions/32297088/how-to-implement-aes128-encryption-decryption-using-aes-ni-instructions-and-gcc 
// Ref the following repo https://github.com/sebastien-riou/aes-brute-force 
//_mm_aesenc_si128 for encryption. Needs a (__m256i a, __m256i RoundKey)
//also, *uint8_t can seemingly point to place
void encrypt_section(uint8_t plain_text[], uint8_t enc_key[]){
    int default_size = sizeof(plain_text);
    uint8_t computed_cipher[default_size];
    //uint8_t computed_plain[16];
    __m128i key_schedule[default_size+4]; //unsure why in the orig text I found  it was default_size + 4, but I'll stick with it
    aes128_load_key(enc_key,key_schedule);
    //aes128_enc(key_schedule,plain,computed_cipher); >> (__m128i *key_schedule, uint8_t *plainText,uint8_t *cipherText)

    //aes128_dec(key_schedule,cipher,computed_plain);

    __m128i m = _mm_loadu_si128((__m128i *) plain);

    DO_ENC_BLOCK(m,key_schedule);

    _mm_storeu_si128((__m128i *) computed_cipher, m);
    _mm_sfence(); //my addition?
    //break;
}

//_mm_aesdec_si128 for encryption. Needs a (__m256i a, __m256i RoundKey)
void decrypt_section(uint8_t cipher_text[], uint8_t enc_key[]){
    //enc_key needs to be an array of addresses the same size of cipher-text.
    int default_size = sizeof(plain_text);
    uint8_t computed_plain[default_size];
    __m128i key_schedule[default_size+4]; //unsure why in the orig text I found  it was default_size + 4, but I'll stick with it
    aes128_load_key(enc_key,key_schedule);
    //aes128_dec(key_schedule,cipher,computed_plain); >> (__m128i *key_schedule, uint8_t *cipherText,uint8_t *plainText)

    __m128i m = _mm_loadu_si128((__m128i *) cipher_text);

    DO_DEC_BLOCK(m,key_schedule);

    _mm_storeu_si128((__m128i *) computed_plain, m);

    _mm_sfence(); //my addition?
    //break;
}


//sfence is like loadfence but storing. mfence is loading and storing
//Needs to store/protect a char *[] ???
//Thankfully, arrays are stored sequentially
void sglfence(int security_level){//This should be called whenever the flag is called... with input on security level but not what exactly to protect
    //security_level is 0, 1, or 2
    
    if (data_section.flag_on)
    {
        beginning_or_ending_flip(data_section.level_of_importance);
        data_section.flag_on = false;
    }
    else{
        data_section.level_of_importance = security_level;
        data_section.flag_on = true;
        beginning_or_ending_flip(security_level);
    }
    
    return 0;
}

//Needs to store/protect a char *[] ???
//Thankfully, arrays are stored sequentially
void sglfence(int security_level, char* char_array_to_proc){//This should be called whenever the flag is called... with input of level and specific array to protect
    //security_level is 0, 1, or 2

    if (data_section.flag_on)
    {
        beginning_or_ending_flip(data_section.level_of_importance);
        data_section.flag_on = false;
    }
    else{
        data_section.level_of_importance = security_level;
        data_section.flag_on = true;
        data_section.char_array = char_array_to_proc;
        //= (char*)malloc(sizeof(char_array_to_protect)*sizeof(char));
        beginning_or_ending_flip(security_level);
    }
    return 0;
}


void sglfence(){//This should be called whenever the flag is called... without input. So, should be end of section.
    if (data_section.flag_on)
    {
        printf("You need to add a security level of 0, 1, or 2.");
        return 0;
    }
    else{
        beginning_or_ending_flip(data_section.level_of_importance);
        //security_level is 0, 1, or 2
        data_section.flag_on = false;

        return 0;        
    }
    
}

//Shouldnt need a main() ?? Should just test in separate file.

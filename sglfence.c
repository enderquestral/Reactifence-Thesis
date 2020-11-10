/*********************************************************************
*
* Library of SGLfence: A library aiming to add a better software-based solution to preventing spectre leakage
* 
* Taking inspiration from SpectreGuard, InvisiSpec, SafeSpec, oo7
* 
**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <math.h>
#include <x86intrin.h> // for _mm_lfence();


int main(int argc, char const *argv[])
{
    /* code */
    _mm_lfence();
    return 0;
}

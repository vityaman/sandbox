#include <stdio.h>
#include <emmintrin.h>

int main()
{
    int16_t input[16] __attribute__ ((aligned (16))) = {0, 0, 1, -1, 2, -2, 3, -3}; // ...
    
    int16_t ref[8] __attribute__ ((aligned (16))) = {-1,-1,-1,-1,-1,-1,-1,-1};
    
    int16_t output[8]  __attribute__ ((aligned (16)));
    
    // Load the first 128 bits (8 16-bit elements) of the input array into an XMM register
    __m128i reg_input = _mm_load_si128((__m128i*)input);
    // Load the 8 reference elements for comparison
    __m128i reg_ref = _mm_load_si128((__m128i*)ref);
    // Compare 8 input elements to reference elements (input[i] > ref[i])
    __m128i reg_output = _mm_cmpgt_epi16(reg_input, reg_ref);
    // Store the output register to memory
    _mm_store_si128((__m128i*)output, reg_output);

    for (int i = 0; i < 8; ++i)
      printf("%d ", input[i]);
     
    printf("\n");
    
    for (int i = 0; i < 8; ++i)
      printf("%d ", output[i]);

    return 0;
}
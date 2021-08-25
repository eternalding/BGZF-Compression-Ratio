#ifndef CONVERSION_HPP
#define CONVERSION_HPP

#include <stdint.h>


static int big_endian;

static inline int 
is_big_endian()
{
    long one= 1;
    big_endian = !(*((char *)(&one)));
    return big_endian;
}


// Adapted from bam_endian developed by Heng Li
static inline uint16_t 
swap_endian_2(uint16_t v)
{
    return (uint16_t)(((v & 0x00FF00FFU) << 8) | ((v & 0xFF00FF00U) >> 8));
}
static inline uint32_t 
swap_endian_4(uint32_t v)
{
    v = ((v & 0x0000FFFFU) << 16) | (v >> 16);
    return ((v & 0x00FF00FFU) << 8) | ((v & 0xFF00FF00U) >> 8);
}

uint16_t 
to_uint16(const void *pv) {
    uint16_t k = *(uint16_t *)pv;
    return (big_endian)? swap_endian_2(k) : k;
}

uint32_t 
to_uint32(const void *pv) {
    uint32_t k = *(uint32_t *)pv;
    return (big_endian)? swap_endian_4(k) : k;
}
#endif
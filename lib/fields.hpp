#include <iostream>
#include "conversion.hpp"


//FLAG
struct FLG
{
    static bool FTEXT,  // true for ASCII and false for binary file instead
                FHCRC,  // If FHCRC is set, a CRC16 for the gzip header is present, immediately before the compressed data.
                FEXTRA, // If FEXTRA is set, optional extra fields are present
                FNAME,  // If FNAME is set, an original file name is present, terminated by a zero byte.
                FCOMMENT, // If FCOMMENT is set, a zero-terminated file comment is present. 
                reserved_bit5, // Should be zero
                reserved_bit6, // Shuold be zero
                reserved_bit7; // Shuold be zero  

    void
    ParseFlag(const char flag)
    {
        this->reserved_bit7 = ((flag >> 7) & 0x1);
        this->reserved_bit6 = ((flag >> 6) & 0x1);
        this->reserved_bit5 = ((flag >> 5) & 0x1);
        this->FCOMMENT      = ((flag >> 4) & 0x1);
        this->FNAME         = ((flag >> 3) & 0x1);
        this->FEXTRA        = ((flag >> 2) & 0x1);
        this->FHCRC         = ((flag >> 1) & 0x1);
        this->FTEXT         = ((flag >> 0) & 0x1);
        
        //reserved bits should be zero
        if(this->reserved_bit5||this->reserved_bit6||this->reserved_bit7)
            perror("[Error] Reserved bits not zero!");
    }
};

/*
 * Check different fields
 */

//Field default values are achieved from spec:
//https://samtools.github.io/hts-specs/SAMv1.pdf
static constexpr auto ID1  {0x1F}, 
                      ID2  {0x8B}, 
                      CM   {0x8} ,
                      FLG  {0x4} ,
                      SI1  {0x42},
                      SI2  {0x43},
                      SLEN {0x2} ;

void 
CheckHeader(const uint8_t* buffer)
{
    if(buffer[0]!=ID1)
        perror("[Error] Wrong ID1 field.");
    else if(buffer[1]!=ID2)
        perror("[Error] Wrong ID2 field.");
    else if(buffer[2]!=CM)
        perror("[Error] Wrong CM field.");
    else if(buffer[3]!=FLG)
        perror("[Error] Wrong FLG field.");
    else if(buffer[12]!=SI1)
        perror("[Error] Wrong SI1 field.");
    else if(buffer[13]!=SI2)
        perror("[Error] Wrong SI2 field.");
    else if(to_uint16(&buffer[14])!=SLEN)
        perror("[Error] Wrong SLEN field.");
}
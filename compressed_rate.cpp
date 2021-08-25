#include <iostream>
#include <fstream>
#include <string_view>
#include <cstring>

//BINARY FILE
#include <fcntl.h>
#include <unistd.h>

//Custom FILEs
#include "lib/fields.hpp"
#include "lib/conversion.hpp"

using ll = long long;

/**
 * DEFINE MACROS
 */
#if !defined(O_BINARY) && defined(_O_BINARY)
    #define O_BINARY  _O_BINARY
#elif !defined(O_BINARY)
    #define O_BINARY  0
#endif

static constexpr auto HEADER_LEN {18},
                      FOOTER_LEN {8};

static constexpr 
std::string_view helpmsg {R"(
    Usage
    -h: Display help message.
    -i: Input bgzip file (.bgz)

    Example usage:
    <Executable> -i <Input bgzip file>
    )"};

// Thanks to the clear explanation made by John Marshall 
// in https://www.biostars.org/p/475646/
// Extra error indication functions are added
double
get_compressed_ratio(const char* FILENAME)
{
    uint8_t header_buffer[HEADER_LEN], footer_buffer[FOOTER_LEN];
    ll total_uncompressed_size {0}, total_block_size {0}, block_cnt {0};
    uint16_t BSIZE, XLEN;
    uint32_t ISIZE;
    is_big_endian();


    //Open File
    int fd = open(FILENAME, O_RDONLY | O_BINARY);
    if(fd<0)
        perror("[Error] Error opening file!");

    //Read file 
    while(true)
    {
        //Read Header
        auto nhread {read(fd,header_buffer,HEADER_LEN)};
        if(nhread<0)
            perror("[Error] Error reading file.");
        else if(nhread==0)
            break;
        else if(nhread<HEADER_LEN)
            perror("[Error] Incomplete Header!");

        //Header sanity check
        CheckHeader(header_buffer);

        //Read remaining data
        BSIZE = to_uint16(&header_buffer[16]) + 1;
        XLEN  = to_uint16(&header_buffer[10]);

        if(lseek(fd,BSIZE - XLEN - 20,SEEK_CUR)<0)
            perror("[Error] Error reading compressed data.");
        
        //Read footer
        auto nfread {read(fd,footer_buffer,FOOTER_LEN)};
        if(nfread<0)
            perror("[Error] Error reading footer!");
        else if(nfread<FOOTER_LEN)
            perror("[Error] Incomplete footer!");
        
        ISIZE = to_uint32(&footer_buffer[4]);

        total_uncompressed_size += ISIZE;
        total_block_size += BSIZE;
        block_cnt ++;
    }

    std::cout<<"Total Block count:"<<block_cnt<<std::endl;
    std::cout<<"Total uncompressed size:"<<total_uncompressed_size<<std::endl;
    std::cout<<"Total block size:"<<total_block_size<<std::endl;
    return double(total_block_size)/total_uncompressed_size;
}

int main(int argc, char** argv)
{
    //Error usage
    if(argc < 2)
    {
        std::cerr<<helpmsg<<std::endl;
        return EXIT_FAILURE;
    }

    //Check arguments
    for(int i = 1;i<argc;++i)
    {
        if(strcmp(argv[i],"-h")==0)
        {
            std::cout<<helpmsg<<std::endl;
            return EXIT_SUCCESS;
        }
        else if(strcmp(argv[i],"-i")==0)
        {
            auto fstat = open(argv[i+1], O_RDONLY | O_BINARY);
            if(fstat<0)
            {
                perror("Error message");
                return EXIT_FAILURE;
            }

            auto ratio {get_compressed_ratio(argv[i+1])};

            std::cout<<"Compressed ratio:"<<ratio<<std::endl;
            return EXIT_SUCCESS;
        }
    }

    return 0;


}

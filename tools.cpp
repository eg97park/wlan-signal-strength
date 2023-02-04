#include "pch.h"
#include "tools.h"


void usage(char* argv[])
{
    printf("syntax: %s <interface> <mac>\n", argv[0]);
    printf("sample: %s wlp45s0 00:11:22:33:44:55\n", argv[0]);
}


bool parse(Param* param, int argc, char* argv[])
{
    if (argc != 3)
    {
        usage(argv);
        return false;
    }
    param->if_ = argv[1];
    param->mac_ = parse_mac_addr(argv[2]);

    return true;
}


void dump(void* p, size_t n)
{
    uint8_t* u8 = static_cast<uint8_t*>(p);
    size_t i = 0;
    while (true) {
        printf("%02X ", *u8++);
        if (++i >= n) break;
        if (i % 8 == 0) printf(" ");
        if (i % 16 == 0) printf("\n");
    }
    printf("\n");
}


uint8_t* parse_mac_addr(const char* mac_addr)
{
    uint8_t* parsed_mac_addr = (uint8_t*)malloc(sizeof(uint8_t) * 6);
    std::sscanf(
        mac_addr,
        "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
        &parsed_mac_addr[0],
        &parsed_mac_addr[1],
        &parsed_mac_addr[2],
        &parsed_mac_addr[3],
        &parsed_mac_addr[4],
        &parsed_mac_addr[5]
    );
    
    return parsed_mac_addr;
}

void clear()
{
    std::cout << "\x1B[2J\x1B[H";
}
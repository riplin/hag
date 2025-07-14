//Copyright 2023-Present riplin

#include <stdio.h>
#include <has/types.h>

int main(void)
{
    for (uint32_t i = 0x000000; i < 0x100000; i += 0x10000)
    {
        uint8_t* ptr = (uint8_t*)i;
        char name[15];
        sprintf(name, "%05X.bin", i);
        FILE* fp = fopen(name, "wb");

        fwrite(ptr, 1, 0x10000, fp);

        fclose(fp);
    }
    return 0;
}

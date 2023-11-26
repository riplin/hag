//Copyright 2023-Present riplin

#include <dos.h>
#include <i86.h>
#include <stdlib.h>
#include <string.h>

#include <support/allocatr.h>

namespace Support
{

void* Allocator::Allocate(uint32_t size)
{
    return malloc(size);
}

void Allocator::Free(void* memory)
{
    free(memory);
}

//Memory in the first 1MiB area.
void far* Allocator::AllocateLow(uint16_t size)
{
    void far* ret = 0;
    REGS regs;
    SREGS sregs;
    memset(&sregs,0,sizeof(sregs));
    regs.w.ax = 0x0100;//DPMI call, allocate DOS memory
    regs.w.bx = size >> 4;
    int386x(0x31, &regs, &regs, &sregs);
    return MK_FP(regs.w.dx, 0);
}

void Allocator::FreeLow(void far* memory)
{
    REGS regs;
    SREGS sregs;
    memset(&sregs,0,sizeof(sregs));
    regs.w.ax = 0x0101;//DPMI call, free DOS memory
    regs.w.dx = FP_SEG(memory);
    int386x(0x31, &regs, &regs, &sregs);
}

}

//Copyright 2023-Present riplin

#include <dos.h>
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

bool Allocator::AllocDosMem(uint16_t size, uint16_t& selector, uint16_t& segment)
{
    bool ret = false;
    selector = segment = 0;

    REGS r;
    memset(&r, 0, sizeof(r));

    r.w.ax = 0x0100;
    r.w.bx = (size + 15) >> 4;
    int386(0x31, &r, &r);

    if ((r.w.flags & 0x01) == 0)
    {
        segment = r.w.ax;
        selector = r.w.dx;
        ret = true;
    }

    return ret;
}

bool Allocator::FreeDosMem(uint16_t selector)
{
    bool ret = false;
    if (selector != 0)
    {
        REGS r;
        memset(&r, 0, sizeof(r));
        r.w.ax = 0x0101;
        r.w.dx = selector;
        int386(0x31, &r, &r);
        ret = (r.w.flags & 0x01) == 0;
    }

    return ret;
}

}

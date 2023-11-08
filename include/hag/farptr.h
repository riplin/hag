//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/testing/mock.h>

struct FARPointer
{
    inline FARPointer(uint16_t seg, uint16_t offs) : Offset(offs), Segment(seg){}
    inline bool IsNull() { return (Offset == 0x0000) && (Segment == 0x0000);}

#ifdef MOCK
    template<typename T> T*ToPointer(uint32_t size = sizeof(T)) { return (T*)(&Hag::Testing::Mock::Memory::Ref((uint32_t(Segment) << 4) + Offset, size)); }
#else
    template<typename T> T*ToPointer(uint32_t size = sizeof(T)) { return (T*)((uint32_t(Segment) << 4) + Offset); }
#endif

    uint16_t Offset;
    uint16_t Segment;
};

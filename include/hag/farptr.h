//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/testing/mock.h>
#include <sys/nearptr.h>

struct FARPointer
{
    inline FARPointer() : Offset(0), Segment(0) {}
    inline FARPointer(uint16_t seg, uint16_t offs) : Offset(offs), Segment(seg){}
    inline bool IsNull() { return (Offset == 0x0000) && (Segment == 0x0000);}
    inline void Set(uint16_t seg, uint16_t offs) { Segment = seg; Offset = offs; }

#ifdef MOCK
    template<typename T> T*ToPointer(uint32_t size = sizeof(T)) { return (T*)(&Hag::Testing::Mock::Memory::Ref((uint32_t(Segment) << 4) + Offset, size)); }
#else
    template<typename T> T*ToPointer(uint32_t size = sizeof(T)) { return (T*)(((uint32_t(Segment) << 4) + Offset) + __djgpp_conventional_base); }
#endif

    uint16_t Offset;
    uint16_t Segment;
};

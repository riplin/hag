//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>

void SYS_ClearInterrupts();
#pragma aux SYS_ClearInterrupts = "cli";

void SYS_RestoreInterrupts();
#pragma aux SYS_RestoreInterrupts = "sti";

void SYS_SpinWait(uint16_t cycles);//This is a time sensitive loop!!!!
#pragma aux SYS_SpinWait = \
    "waitloop:"         \
    "loop waitloop"     \
    parm [cx];

#ifdef MOCK

#include <hag/testing/mock.h>

inline uint8_t SYS_ReadPortByte(uint16_t reg)
{
    return Hag::Testing::Mock::Port::Read8(reg);
}

inline void SYS_WritePortByte(uint16_t reg, uint8_t val)
{
    Hag::Testing::Mock::Port::Write8(reg, val);
}

inline void SYS_WritePortBytes(uint16_t reg, uint8_t val1, uint8_t val2)
{
    Hag::Testing::Mock::Port::Write8(reg, val1, val2);
}

inline uint16_t SYS_ReadPortShort(uint16_t reg)
{
    return Hag::Testing::Mock::Port::Read16(reg);
}

inline void SYS_WritePortShort(uint16_t reg, uint16_t val)
{
    Hag::Testing::Mock::Port::Write16(reg, val);
}

inline void SYS_WritePortDouble(uint16_t reg, uint32_t val)
{
    SYS_WritePortShort(reg, uint16_t(val));
    SYS_WritePortShort(reg + 2, uint16_t(val >> 16));
}

inline uint32_t SYS_ReadPortDouble(uint16_t reg)
{
    return uint32_t(SYS_ReadPortShort(reg)) | (uint32_t(SYS_ReadPortShort(reg + 2)) << 16);
}

#else

uint8_t SYS_ReadPortByte(uint16_t reg);
#pragma aux SYS_ReadPortByte = \
    "in al, dx"         \
    parm [dx]           \
    value [al];

void SYS_WritePortByte(uint16_t reg, uint8_t val);
#pragma aux SYS_WritePortByte = \
    "out dx, al"        \
    parm [dx] [al];

void SYS_WritePortBytes(uint16_t reg, uint8_t val1, uint8_t val2);
#pragma aux SYS_WritePortBytes = \
    "out dx, ax"        \
    parm [dx] [al] [ah];

uint16_t SYS_ReadPortShort(uint16_t reg);
#pragma aux SYS_ReadPortShort = \
    "in ax, dx"         \
    parm [dx]           \
    value [ax];

void SYS_WritePortShort(uint16_t reg, uint16_t val);
#pragma aux SYS_WritePortShort = \
    "out dx, ax"        \
    parm [dx] [ax];

void SYS_WritePortDouble(uint16_t reg, uint32_t val);
#pragma aux SYS_WritePortDouble = \
    "out dx, eax"       \
    parm [dx] [eax];

uint32_t SYS_ReadPortDouble(uint16_t reg);
#pragma aux SYS_ReadPortDouble = \
    "in eax, dx"        \
    parm [dx]           \
    value [eax];

#endif
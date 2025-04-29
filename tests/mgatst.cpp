//Copyright 2023-Present riplin

#include <hag/types.h>
#include <i86.h>
#include <stdio.h>
#include <string.h>

uint8_t Data0x7d24 = 0xFF;
uint8_t Data0x7d34 = 0x00;

void CalculatePLL_NMPS_AndNearestClockRate(uint32_t requestedFrequencyInKHz, uint32_t& closestFrequencyInKHz, uint8_t& N, uint8_t& M, uint8_t& P, uint8_t& S)
{
    uint32_t maxFreq = Data0x7d24 != 0xff ? (Data0x7d24 + 100) * 1000 : 220000;
    uint32_t inFreq = requestedFrequencyInKHz < 6262 ? 6262 : (requestedFrequencyInKHz > maxFreq ? maxFreq : requestedFrequencyInKHz);
    uint32_t workFreq = 0;
    uint32_t saveFreq = 0;
    uint8_t saveN = 0;
    uint8_t saveM = 0;
    int count = 3;
    int saveCount = 0;
    uint32_t saveError = 0xffffffff;
    uint32_t minError = (inFreq * 5) / 1000;
    bool fullBreak = false;
    uint32_t workError = 0;
    uint32_t currFreq = 0;

    do
    {
        workError = inFreq << count;

        if (workError < 50000)
            break;

        if (workError <= maxFreq)
        {
            M = 1;
            N = 7;

            do
            {
                do
                {
                    workError = (Data0x7d34 & 0x20) != 0x00 ? 14318 : 27000;
                    workError = (workError * (N + 1)) / (M + 1);

                    if (workError > maxFreq)
                        break;

                    if (workError >= 50000)
                    {
                        workFreq = workError;
                        workError >>= count;
                        uint32_t tmpFreq = inFreq;

                        if (inFreq >= workError)
                        {
                            tmpFreq = workError;
                            workError = inFreq;
                        }

                        workError -= tmpFreq;
                        currFreq = workFreq;

                        if (workError <= minError)
                        {
                            fullBreak = true;
                            break;
                        }

                        if (workError < saveError)
                        {
                            saveError = workError;
                            saveN = N;
                            saveM = M;
                            saveCount = count;
                            saveFreq = currFreq;
                        }
                    }
                    ++N;

                } while (N <= 127);

                if (fullBreak)
                    break;

                N = 7;
                ++M;

            } while (M <= 31);

            if (fullBreak)
                break;
        }

        --count;
    } while (count >= 0);

    if (workError > minError)
    {
        N = saveN;
        M = saveM;
        count = saveCount;
        currFreq = saveFreq;
    }

    P = (1 << count) - 1;
    S = currFreq <= 100000 ? 0 : (currFreq <= 140000 ? 1 : (currFreq <= 180000 ? 2 : 3));
    closestFrequencyInKHz = currFreq >> count;
}

void CalculatePLL_NMPS_AndNearestClockRate_ASM(uint32_t requestedFrequencyInKHz, uint32_t& closestFrequencyInKHz, uint32_t& NMPS)
{
    REGPACK r;
    memset(&r, 0, sizeof(r));

    uint32_t bp_04h = 0;
    uint32_t bp_08h = 0;
    uint32_t bp_0ch = 0;
    uint32_t bp_10h = 0;
    uint32_t bp_14h = 0;
    uint32_t bp_18h = 0;
    uint32_t bp_1ch = 0;

//     push   bp
//     mov    bp, sp
//     sub    sp, 001ch
//     push   ecx
//     push   edx
//     mov    dword ptr [bp - 18h], 0ffffffffh
    bp_18h = 0xffffffff;

//     mov    dword ptr [bp - 04h], eax
    bp_04h = requestedFrequencyInKHz;

//     xor    eax, eax
    r.x.eax ^= r.x.eax;

//     mov    al, byte ptr cs:[Data0x7d24] ;Offset 0x7d24
    r.h.al = Data0x7d24;

//     cmp    al, 0ffh
//     je     Label0x6b7c                  ;Offset 0x6b7c
    if (r.h.al == 0xff)
        goto Label0x6b7c;
                
//     add    ax, 64h
    r.w.ax += 0x64;

//     mov    ebx, 000003e8h               ;1,000
    r.x.ebx = 0x000003e8;

//     mul    ebx
    r.x.eax *= r.x.ebx;

//     mov    dword ptr [bp - 08h], eax
    bp_08h = r.x.eax;

//     jmp    Label0x6b84                  ;Offset 0x6b84
    goto Label0x6b84;

// Label0x6b7c:                            ;Offset 0x6b7c
Label0x6b7c:

//     mov    dword ptr [bp - 08h], 00035b60h;220,000
    bp_08h = 0x00035b60;

// Label0x6b84:                            ;Offset 0x6b84
Label0x6b84:

//     mov    eax, dword ptr [bp - 04h]
    r.x.eax = bp_04h;

//     cmp    eax, 00001876h               ;6,262
//     jae    Label0x6b96                  ;Offset 0x6b96
    if (r.x.eax >= 0x00001876)
        goto Label0x6b96;

//     mov    eax, 00001876h               ;6,262
    r.x.eax = 0x00001876;

// Label0x6b96:                            ;Offset 0x6b96
Label0x6b96:

//     cmp    eax, dword ptr [bp - 08h]
//     jbe    Label0x6ba0                  ;Offset 0x6ba0
    if (r.x.eax <= bp_08h)
        goto Label0x6ba0;

//     mov    eax, dword ptr [bp - 08h]
    r.x.eax = bp_08h;

// Label0x6ba0:                            ;Offset 0x6ba0
Label0x6ba0:

//     mov    dword ptr [bp - 04h], eax
    bp_04h = r.x.eax;

//     xor    edx, edx
    r.x.edx ^= r.x.edx;

//     mov    ebx, 00000005h
    r.x.ebx = 0x00000005;

//     mul    ebx
    r.x.eax *= r.x.ebx;

//     mov    ebx, 000003e8h               ;1,000
    r.x.ebx = 0x000003e8;

//     div    ebx
    r.x.eax /= r.x.ebx;

//     mov    dword ptr [bp - 1ch], eax
    bp_1ch = r.x.eax;

//     mov    ecx, 00000003h
    r.x.ecx = 0x00000003;

// Label0x6bc3:                            ;Offset 0x6bc3
Label0x6bc3:

//     mov    eax, dword ptr [bp - 04h]
    r.x.eax = bp_04h;

//     shl    eax, cl
    r.x.eax <<= r.h.cl;

//     shl    ecx, 10h
    r.x.ecx <<= 0x10;

//     cmp    eax, 0000c350h               ;50,000
//     jb     Label0x6c7d                  ;Offset 0x6c7d
    if (r.x.eax < 0x0000c350)
        goto Label0x6c7d;

//     cmp    eax, dword ptr [bp - 08h]
//     ja     Label0x6c73                  ;Offset 0x6c73
    if (r.x.eax > bp_08h)
        goto Label0x6c73;

//     mov    ch, 01h
    r.h.ch = 0x01;

//     mov    cl, 07h
    r.h.cl = 0x07;

// Label0x6be4:                            ;Offset 0x6be4
Label0x6be4:

//     xor    edx, edx
    r.x.edx ^= r.x.edx;

//     xor    ebx, ebx
    r.x.ebx ^= r.x.ebx;

//     mov    eax, 00006978h               ;27,000
    r.x.eax = 0x00006978;

//     test   byte ptr cs:[Data0x7d34], 20h;Offset 0x7d34
//     je     Label0x6c00                  ;Offset 0x6c00
    if ((Data0x7d34 & 0x20) == 0x00)
        goto Label0x6c00;

//     mov    eax, 000037eeh               ;14,318
    r.x.eax = 0x000037ee;

// Label0x6c00:                            ;Offset 0x6c00
Label0x6c00:

//     mov    bl, cl
    r.h.bl = r.h.cl;

//     inc    bl
    ++r.h.bl;

//     mul    ebx
    r.x.eax *= r.x.ebx;

//     mov    bl, ch
    r.h.bl = r.h.ch;

//     inc    bl
    ++r.h.bl;

//     div    ebx
    r.x.eax /= r.x.ebx;

//     cmp    eax, dword ptr [bp - 08h]
//     ja     Label0x6c68                  ;Offset 0x6c68
    if (r.x.eax > bp_08h)
        goto Label0x6c68;

//     cmp    eax, 0000c350h               ;50,000
//     jb    Label0x6c5f                   ;Offset 0x6c5f
    if (r.x.eax <0x0000c350)
        goto Label0x6c5f;

//     mov    dword ptr [bp - 0ch], eax
    bp_0ch = r.x.eax;

//     ror    ecx, 10h
    r.x.ecx = (r.x.ecx >> 0x10) | (r.x.ecx << 0x10);

//     shr    eax, cl
    r.x.eax >>= r.h.cl;

//     rol    ecx, 10h
    r.x.ecx = (r.x.ecx >> 0x10) | (r.x.ecx << 0x10);

//     mov    ebx, dword ptr [bp - 04h]
    r.x.ebx = bp_04h;

//     cmp    ebx, eax
//     jb     Label0x6c3c                  ;Offset 0x6c3c
    if (r.x.ebx < r.x.eax)
        goto Label0x6c3c;

//     xchg   eax, ebx
    {
        uint32_t tmp = r.x.eax;
        r.x.eax = r.x.ebx;
        r.x.ebx = tmp;
    }

    // Label0x6c3c:                            ;Offset 0x6c3c
Label0x6c3c:

//     sub    eax, ebx
    r.x.eax -= r.x.ebx;
    
//     mov    edx, dword ptr [bp - 0ch]
    r.x.edx = bp_0ch;

//     cmp    eax, dword ptr [bp - 1ch]
//     jbe    Label0x6c85                  ;Offset 0x6c85
    if (r.x.eax <= bp_1ch)
        goto Label0x6c85;

//     cmp    eax, dword ptr [bp - 18h]
//     jae    Label0x6c5f                  ;Offset 0x6c5f
    if (r.x.eax >= bp_18h)
        goto Label0x6c5f;

//     mov    dword ptr [bp - 18h], eax
    bp_18h = r.x.eax;

//     mov    dword ptr [bp - 14h], ecx
    bp_14h = r.x.ecx;

//     mov    dword ptr [bp - 10h], edx
    bp_10h = r.x.edx;

// Label0x6c5f:                            ;Offset 0x6c5f
Label0x6c5f:

//     inc    cl
    ++r.h.cl;

//     cmp    cl, 7fh
//     jbe    Label0x6be4                  ;Offset 0x6be4
    if (r.h.cl <= 0x7f)
        goto Label0x6be4;

// Label0x6c68:                            ;Offset 0x6c68
Label0x6c68:

//     mov    cl, 07h
    r.h.cl = 0x07;

//     inc    ch
    ++r.h.ch;

//     cmp    ch, 1fh
//     jbe    Label0x6be4                  ;Offset 0x6be4
    if (r.h.ch <= 0x1f)
        goto Label0x6be4;

// Label0x6c73:                            ;Offset 0x6c73
Label0x6c73:

//     shr    ecx, 10h
    r.x.ecx >>= 0x10;

//     dec    cl
    --r.h.cl;

//     jns    Label0x6bc3                  ;Offset 0x6bc3
    if ((r.h.cl & 0x80) == 0x00)
        goto Label0x6bc3;

// Label0x6c7d:                            ;Offset 0x6c7d
Label0x6c7d:

//     mov    ecx, dword ptr [bp - 14h]
    r.x.ecx = bp_14h;

//     mov    edx, dword ptr [bp - 10h]
    r.x.edx = bp_10h;

// Label0x6c85:                            ;Offset 0x6c85
Label0x6c85:

//     mov    ebx, ecx
    r.x.ebx = r.x.ecx;

//     and    ebx, 00001fffh
    r.x.ebx &= 0x00001fff;

//     shr    ecx, 10h
    r.x.ecx >>= 0x10;

//     mov    ax, 01h
    r.w.ax = 0x0001;

//     shl    ax, cl
    r.w.ax <<= r.h.cl;

//     dec    ax
    --r.w.ax;

//     shl    ax, 0dh
    r.w.ax <<= 0x0d;

//     or     bx, ax
    r.w.bx |= r.w.ax;

//     xor    eax, eax
    r.x.eax ^= r.x.eax;

//     cmp    edx, 000186a0h               ;100,000
//     ja     Label0x6caf                  ;Offset 0x6caf
    if (r.x.edx > 0x000186a0)
        goto Label0x6caf;

//     mov    ax, 0000h                    ;PixPLL S
    r.w.ax = 0x0000;

//     jmp    Label0x6cce                  ;Offset 0x6cce
    goto Label0x6cce;

// Label0x6caf:                            ;Offset 0x6caf
Label0x6caf:

//     cmp    edx, 000222e0h               ;140,000
//     ja     Label0x6cbd                  ;Offset 0x6cbd
    if (r.x.edx > 0x000222e0)
        goto Label0x6cbd;

//     mov    ax, 0001h                    ;PixPLL S
    r.w.ax = 0x0001;

//     jmp    Label0x6cce                  ;Offset 0x6cce
    goto Label0x6cce;

// Label0x6cbd:                            ;Offset 0x6cbd
Label0x6cbd:

//     cmp    edx, 0002bf20h               ;180,000
//     ja     Label0x6ccb                  ;Offset 0x6ccb
    if (r.x.edx > 0x0002bf20)
        goto Label0x6ccb;

//     mov    ax, 0002h                    ;PixPLL S
    r.w.ax = 0x0002;

//     jmp    Label0x6cce                  ;Offset 0x6cce
    goto Label0x6cce;

// Label0x6ccb:                            ;Offset 0x6ccb
Label0x6ccb:

//     mov    ax, 0003h                    ;PixPLL S
    r.w.ax = 0x0003;

// Label0x6cce:                            ;Offset 0x6cce
Label0x6cce:

//     shl    eax, 10h
    r.x.eax <<= 0x10;

//     or     ebx, eax                     ;Merge S value
    r.x.ebx |= r.x.eax;

//     mov    eax, edx
    r.x.eax = r.x.edx;

//     shr    eax, cl
    r.x.eax >>= r.h.cl;

//     pop    edx
//     pop    ecx
//     mov    sp, bp
//     pop    bp
//     ret
    closestFrequencyInKHz = r.x.eax;
    NMPS = r.x.ebx;
}

int main(void)
{
    uint32_t requestedFrequencies[] = 
    {
        0x00000000,
        0x00006257,
        0x00006ea2,
        0x00009858,
        0x00009C40,
        0x0000A078,
        0x0000FDE8,
        0x00011d28,
        0x0001A5E0,
        0xFFFFFFFF,
    };
    uint32_t count = sizeof(requestedFrequencies) / sizeof(uint32_t);

    bool failure = false;
    for (uint32_t i = 0; i < count; ++i)
    {
        uint8_t n = 0;
        uint8_t m = 0;
        uint8_t p = 0;
        uint8_t s = 0;
        uint32_t nmps1 = 0;
        uint32_t nmps2 = 0;
        uint32_t outfreq1 = 0;
        uint32_t outfreq2 = 0;
        CalculatePLL_NMPS_AndNearestClockRate_ASM(requestedFrequencies[i], outfreq1, nmps1);
        CalculatePLL_NMPS_AndNearestClockRate(requestedFrequencies[i], outfreq2, n, m, p, s);
        nmps2 = (uint32_t(s) << 16) | (uint32_t(p) << 13) | (uint32_t(m) << 8) | n;
        if ((outfreq1 != outfreq2) || (nmps1 != nmps2))
        {
            failure = true;
            printf("Input freq: 0x%08X, Output freq: 0x%08X, mnps: 0x%08X\n", requestedFrequencies[i], outfreq1, nmps1);
            printf("Input freq: 0x%08X, Output freq: 0x%08X, mnps: 0x%08X\n", requestedFrequencies[i], outfreq2, nmps2);
            printf("\n");
        }
    }
    if (!failure)
    {
        printf("Success!\n");
    }
}

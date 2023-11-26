//Copyright 2023-Present riplin

#pragma once

#include <hag/farptr.h>
#include <hag/drivers/s3/shared/regtype.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register
{

enum
{
    GraphicsProcessorStatus = 0x9AE8,                       //GP_STAT
};

}

typedef uint16_t GraphicsProcessorStatus_t;

namespace GraphicsProcessorStatus
{

    enum //Mask
    {
        FIFOStatusLow = 0x00FF,         //FIFO-STATUS
                                        //          In the following table for the Trio32, the leftmost column represents the value of FIFO
                                        //          status bit 1 (register bit 7). Each column to the right represents the value of the next
                                        //          higher FIFO status bit. A value of 0 read from any particular status bit position guarantees
                                        //          at least as many open FIFO slots as the numer of that status bit. For example, if
                                        //          a 0 is read from status bit 6 (register bit 2), then there are at least 6 open FIFO slots.
                                        //          See Bits 15-11 for a table for the Trio 64.
                                        //
                                        //          00000000 = 8 FIFO slots available
                                        //          00000001 = 7 FIFO slots available
                                        //          00000011 = 6 FIFO slots available
                                        //          00000111 = 5 FIFO slots available
                                        //          00001111 = 4 FIFO slots available
                                        //          00011111 = 3 FIFO slots available
                                        //          00111111 = 2 FIFO slots available
                                        //          01111111 = 1 FIFO slots available
                                        //          11111111 = 0 FIFO slots available
        GraphicsEngineBusy = 0x0200,    //HDW BSY 0 = not busy, 1 = busy - graphics command is executing
        AllFIFOSlotsEmpty = 0x0400,     //AE 0 = At least one FIFO slot is occupied
                                        //   1 = All FIFO slots empty
        FIFOStatusHigh = 0xF800,        //FIFO-STATUS
                                        //          These are the upper 5 bits of the FIFO status for the Trio64. See bits 7-0 for the interpretation.
                                        //          The following table applies only to the Trio64.
                                        //
                                        //          0000000000000 = 13 FIFO slots available
                                        //          0000000000001 = 12 FIFO slots available
                                        //          0000000000011 = 11 FIFO slots available
                                        //          0000000000111 = 10 FIFO slots available
                                        //          0000000001111 = 9 FIFO slots available
                                        //          0000000011111 = 8 FIFO slots available
                                        //          0000000111111 = 7 FIFO slots available
                                        //          0000001111111 = 6 FIFO slots available
                                        //          0000011111111 = 5 FIFO slots available
                                        //          0000111111111 = 4 FIFO slots available
                                        //          0001111111111 = 3 FIFO slots available
                                        //          0011111111111 = 2 FIFO slots available
                                        //          0111111111111 = 1 FIFO slots available
                                        //          1111111111111 = 0 FIFO slots available
        FIFOStatusInvalid = 0xFFFF
    };

    namespace Shift
    {
        enum
        {
            FIFOStatusLow = 0x00,
            GraphicsEngineBusy = 0x09,
            AllFIFOSlotsEmpty = 0x0A,
            FIFOStatusHigh = 0x0B,
        };
    }

    inline GraphicsProcessorStatus_t Read()
    {
        return GraphicsProcessorStatus_t(SYS_ReadPortShort(Register::GraphicsProcessorStatus));
    }

    inline void Wait()
    {
        GraphicsProcessorStatus_t value;
        do
        {
            value = Read();
        } while ((value & GraphicsEngineBusy) != 0);        
    }

}

namespace MMIO
{

    namespace GraphicsProcessorStatus
    {
        inline GraphicsProcessorStatus_t Read()
        {
            return *FARPointer(0xA000, Register::GraphicsProcessorStatus).
                ToPointer<GraphicsProcessorStatus_t>(sizeof(GraphicsProcessorStatus_t));
        }
    }

}

}}}

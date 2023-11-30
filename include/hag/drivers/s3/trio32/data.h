//Copyright 2023-Present riplin

#pragma once

#include <hag/system/bda.h>
#include <hag/drivers/vga/data.h>
#include <hag/drivers/s3/shared/data.h>
#include <hag/drivers/s3/trio32/trio32.h>

#pragma pack(push, 1);

namespace Hag { namespace S3 { namespace Trio32
{

namespace Data
{

extern VGA::VideoParameters* LegacyVideoModesV1[];
extern VGA::VideoParameters* LegacyVideoModesV2[];
extern VGA::VideoParameters* LegacyVideoModesV3[];

extern VGA::VideoParameters* VesaResolutions[];

extern Shared::VESAVideoModeData VesaVideoModes[];
extern int VesaVideoModesCount;

extern uint8_t SequenceInitData[];

extern uint8_t ClockData[];

extern Shared::FirmwareFlag_t FirmwareFlags;

}

}}}

#pragma pack(pop);

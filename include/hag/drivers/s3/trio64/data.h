//Copyright 2023-Present riplin

#pragma once

#include <hag/system/bda.h>
#include <hag/drivers/vga/data.h>
#include <hag/drivers/s3/shared/data.h>
#include <hag/drivers/s3/trio64/trio64.h>

#pragma pack(push, 1);

namespace Hag { namespace S3 { namespace Trio64
{

namespace Data
{

extern VGA::VideoParameters* LegacyVideoModesV1[];
extern VGA::VideoParameters* LegacyVideoModesV2[];
extern VGA::VideoParameters* LegacyVideoModesV3[];

extern VGA::VideoParameters* VesaResolutions[];

extern Shared::VESAVideoModeData VesaVideoModes[];
extern int VesaVideoModesCount;

extern Shared::VESAModeTranslation VideoModeTranslation[];
extern int VideoModeTranslationCount;

extern uint8_t SequenceInitData[];
extern uint8_t PCISystemConfig;
extern uint8_t VLBSystemConfig;

extern uint8_t ClockData[];
extern uint8_t ClockDataRev3[];

extern Shared::FirmwareFlag_t FirmwareFlags;

}

}}}

#pragma pack(pop);

//Copyright 2023-Present riplin

#pragma once

#include <hag/system/bda.h>
#include <hag/drivers/s3/shared/data.h>
#include <hag/drivers/s3/trio32/data.h>
#include <hag/drivers/s3/trio32/trio32.h>

namespace Hag::S3::Trio32
{

void ConfigureDCLKAndMCLK(uint8_t idx, uint8_t* data);
void SetupClocks(VGA::Register_t crtcPort, uint8_t clockConfig);

}

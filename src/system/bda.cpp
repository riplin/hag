//Copyright 2023-Present riplin

#include <hag/system/bda.h>
#include <hag/drivers/vga/crtc/index.h>

namespace Hag::System::BDA
{

static Instance* s_currentInstance = &SystemBDA();

Instance& CurrentInstance()
{
    return *s_currentInstance;
}

void SetInstance(Instance& instance)
{
    s_currentInstance = &instance;
}

}

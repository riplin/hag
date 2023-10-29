//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Vesa { namespace PM {

/*
    Hardware API:
    struct Hardware
    {
        static void GetSyncControl(bool& canControlHSync, bool& canControlVSync);
        static void GetSyncStatus(bool& statusHSync, bool& statusVSync);
        static void SetSyncStatus(bool statusHSync, bool statusVSync);
    };
*/
    template<typename Hardware>
    void SetOn()
    {
        bool canControlHSync = false;
        bool canControlVSync = false;
        Hardware::GetSyncControl(canControlHSync, canControlVSync);
        if (canControlHSync || canControlVSync)
        {
            Hardware::SetSyncStatus(true, true);
        }
    }

    template<typename Hardware>
    void SetStandby()
    {
        if (SupportsStandby<Hardware>())
        {
            Hardware::SetSyncStatus(false, true);
        }
    }

    template<typename Hardware>
    void SetSuspended()
    {
        if (SupportsSuspend<Hardware>())
        {
            Hardware::SetSyncStatus(true, false);
        }
    }

    template<typename Hardware>
    void SetOff()
    {
        if (SupportsOff<Hardware>())
        {
            Hardware::SetSyncStatus(false, false);
        }
    }

    template<typename Hardware>
    void SetReducedOn()
    {
        //I haven't looked at any hardware yet that supports this.
    }

    template<typename Hardware>
    bool IsOn()
    {
        bool ret = true;
        bool canControlHSync = false;
        bool canControlVSync = false;
        Hardware::GetSyncControl(canControlHSync, canControlVSync);
        if (canControlHSync || canControlVSync)
        {
            bool statusHSync = false;
            bool statusVSync = false;
            Hardware::GetSyncStatus(statusHSync, statusVSync);
            ret = statusVSync && statusHSync;
        }
        return ret;
    }

    template<typename Hardware>
    bool IsStandby()
    {
        bool ret = false;
        if (SupportsStandby<Hardware>())
        {
            bool statusHSync = false;
            bool statusVSync = false;
            Hardware::GetSyncStatus(statusHSync, statusVSync);
            ret = !statusHSync;
        }
        return ret;
    }

    template<typename Hardware>
    bool IsSuspended()
    {
        bool ret = false;
        if (SupportsSuspend<Hardware>())
        {
            bool statusHSync = false;
            bool statusVSync = false;
            Hardware::GetSyncStatus(statusHSync, statusVSync);
            ret = !statusVSync;
        }
        return ret;
    }

    template<typename Hardware>
    bool IsOff()
    {
        bool ret = false;
        if (SupportsOff<Hardware>())
        {
            bool statusHSync = false;
            bool statusVSync = false;
            Hardware::GetSyncStatus(statusHSync, statusVSync);
            ret = !statusVSync && !statusHSync;
        }
        return ret;
    }

    template<typename Hardware>
    bool IsReducedOn()
    {
        return false; //I haven't looked at any hardware yet that supports this.
    }

    template<typename Hardware>
    bool SupportsStandby()
    {
        bool canControlHSync = false;
        bool canControlVSync = false;
        Hardware::GetSyncControl(canControlHSync, canControlVSync);
        return canControlHSync;
    }

    template<typename Hardware>
    bool SupportsSuspend()
    {
        bool canControlHSync = false;
        bool canControlVSync = false;
        Hardware::GetSyncControl(canControlHSync, canControlVSync);
        return canControlVSync;
    }

    template<typename Hardware>
    bool SupportsOff()
    {
        bool canControlHSync = false;
        bool canControlVSync = false;
        Hardware::GetSyncControl(canControlHSync, canControlVSync);
        return canControlVSync && canControlHSync;
    }

    template<typename Hardware>
    bool SupportsReducedOn()
    {
        return false; //I haven't looked at any hardware yet that supports this.
    }

}}}

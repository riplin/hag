//Copyright 2023-Present riplin

#include <stdio.h>
#include <hag/testing/mock.h>
#include <hag/drivers/s3/sqrc/regs.h>
#include <hag/drivers/s3/sqrc/unlexseq.h>
#include <hag/drivers/s3/crtc/reglock1.h>
#include <hag/drivers/s3/crtc/reglock2.h>
#include <hag/drivers/s3/crtc/regs.h>
#include <hag/support/allocatr.h>
#include <hag/system/bda.h>

////////////////////////////////////////////////////////////////////////////////
void UnlockExtendedSequencerRegisters();
void LockExtendedSequencerRegisters();
uint16_t GetCRTControllerIndexRegister();
void LockExtendedCRTRegisters();
bool UnlockExtendedCRTRegistersSafe();
bool FindVideoModeData(uint8_t mode, uint32_t& offset);
bool GetVideoModeFlags(uint8_t mode, uint8_t& flags);
bool GetVESAVideoModeOverrideTable(uint8_t mode, uint8_t*& overrideTable, uint8_t& modeDataIndex);
uint8_t rol(uint8_t value, uint8_t rolval);
void ModeSetBDA(uint8_t& al);
bool VerifyBDAOrDeactivate(uint8_t& al);
bool GetVideoParameterBlockElement(uint16_t index, uint8_t*& returnPointer);
void GetVideoModeOverrideTable(uint8_t mode, uint8_t*& overrideTable, uint8_t& modeDataIndex);
void GetCurrentVideoModeOverrideTable(uint8_t*& overrideTable, uint8_t& modeDataIndex);
void SetTextModeBiosData(uint8_t mode, uint8_t*& selectedFont, uint8_t*& overrideTable, uint8_t& modeDataIndex);

void ApplyVideoParameters(uint8_t* overrideTable);
void SaveDynamicParameterData(uint8_t* overrideTable);
void PrepareAttributeController();
uint8_t ReadDataWithIndexRegister(uint16_t port, uint8_t index);
uint8_t FetchBusSpecificSystemConfig(uint16_t crtcPort);
void InitializeCRTControllerAndSequencer(uint8_t* CRTCInitData, uint8_t crtcPort);
void WaitGraphicsEngineReady();
void ClearMemory();
void ReadDeviceIDAndRevision(uint8_t& deviceId, uint8_t& revision);
void ConfigureDCLKAndMCLK(uint8_t idx, uint8_t* data);
void SetupClocks(uint8_t clockConfig);
bool FindModeFromVESAModeData(uint8_t mode, uint8_t*& vesaModeData, uint16_t& vesaVideoModeIdsOffset);
void ConfigureExtraVESAModeSettings(uint16_t crtc, uint8_t* modeData);
void EnableOver256KAddressingAndSetAddressWindow(uint8_t mode, uint16_t crtcReg);
void SetColorMode(uint8_t mode);
void ApplyVESAOverrideData(uint8_t mode, uint8_t* overrideTable, uint8_t modeDataIndex);
bool SetVideoMode(uint8_t mode);
////////////////////////////////////////////////////////////////////////////////

void S3Trio64MockConfigSetup()
{
    Hag::Testing::Mock::AddIndexedPort("CRT Controller B", Hag::VGA::Register::CRTControllerIndexB, 0xff, Hag::VGA::Register::CRTControllerDataB, 0x100);
    Hag::Testing::Mock::AddIndexedPort("CRT Controller D", Hag::VGA::Register::CRTControllerIndexD, 0xff, Hag::VGA::Register::CRTControllerDataD, 0x100);
    Hag::Testing::Mock::AddIndexedPort("Graphics Controller", Hag::VGA::Register::GraphicsControllerIndex, 0x0f, Hag::VGA::Register::SequencerData, 0x09);
    Hag::Testing::Mock::AddIndexedPort("Sequence Controller", Hag::VGA::Register::SequencerIndex, 0x1f, Hag::VGA::Register::SequencerData, 0x19);
}

int MockSetupTeardownTest()
{
    int ret = 0;

    for (int i = 0; i < 100; ++i)
    {
        Support::Allocator allocator;
        Hag::Testing::Mock::Initialize(allocator);
        S3Trio64MockConfigSetup();

        Hag::Testing::Mock::Reset();
        Hag::Testing::Mock::Shutdown();
    }

    return ret;
}

int SquenceRegisterLockingTest()
{
    int ret = 6;
    Support::Allocator allocator;
    Hag::Testing::Mock::Initialize(allocator);
    S3Trio64MockConfigSetup();

    {
        UnlockExtendedSequencerRegisters();

        Hag::Testing::Mock::PortAndValue modifiedPorts[] =
        {
            {
                Hag::VGA::Register::SequencerIndex,
                Hag::S3::SequencerRegister::UnlockExtendedSequencer
            },
            {
                Hag::VGA::Register::SequencerData,
                Hag::S3::Sequencer::UnlockExtendedSequencer::UnlockCode
            }
        };

        uint16_t readPorts[] = { 0x0000 };

        Hag::Testing::Mock::PortAndIndexAndValue modifiedIndexedPorts[] = 
        {
            {
                Hag::VGA::Register::SequencerData,
                Hag::S3::SequencerRegister::UnlockExtendedSequencer,
                Hag::S3::Sequencer::UnlockExtendedSequencer::UnlockCode
            }
        };

        Hag::Testing::Mock::PortAndIndex readIndexedPorts[] = { {0, 0} };

        ret -= VERIFYPORTCONTENT(0, modifiedPorts, readPorts, modifiedIndexedPorts, readIndexedPorts);
    }

    {
        LockExtendedSequencerRegisters();

        Hag::Testing::Mock::PortAndValue modifiedPorts[] =
        {
            {
                Hag::VGA::Register::SequencerIndex,
                Hag::S3::SequencerRegister::UnlockExtendedSequencer
            },
            {
                Hag::VGA::Register::SequencerData,
                Hag::S3::Sequencer::UnlockExtendedSequencer::LockCode
            }
        };

        uint16_t readPorts[] = { 0x0000 };

        Hag::Testing::Mock::PortAndIndexAndValue modifiedIndexedPorts[] = 
        {
            {
                Hag::VGA::Register::SequencerData,
                Hag::S3::SequencerRegister::UnlockExtendedSequencer,
                Hag::S3::Sequencer::UnlockExtendedSequencer::LockCode
            }
        };

        Hag::Testing::Mock::PortAndIndex readIndexedPorts[] = { {0, 0} };

        ret -= VERIFYPORTCONTENT(0, modifiedPorts, readPorts, modifiedIndexedPorts, readIndexedPorts);
    }

    return ret + Hag::Testing::Mock::Shutdown();
}

int CRTCIndexRegisterTest()
{
    int ret = 2;
    Support::Allocator allocator;
    Hag::Testing::Mock::Initialize(allocator);
    S3Trio64MockConfigSetup();

    uint16_t value = GetCRTControllerIndexRegister();
    
    if ((value == Hag::VGA::Register::CRTControllerIndexB) ||
        (value == Hag::VGA::Register::CRTControllerIndexD))
        --ret;

        Hag::Testing::Mock::PortAndValue modifiedPorts[] =
        {
            {
                0x0000,
                0x00
            }
        };
        
        uint16_t readPorts[] =
        {
            Hag::VGA::Register::MiscellaneousR
        };
        
        Hag::Testing::Mock::PortAndIndexAndValue modifiedIndexedPorts[] =
        {
            {
                0x0000,
                0x00,
                0x00
            }
        };
        
        Hag::Testing::Mock::PortAndIndex readIndexedPorts[] = { {0, 0} };

        ret -= VERIFYPORTCONTENT(0, modifiedPorts, readPorts, modifiedIndexedPorts, readIndexedPorts);

    return ret + Hag::Testing::Mock::Shutdown();
}

int CRTCRegisterLockingTest()
{
    int ret = 11;
    uint16_t crtIndex = GetCRTControllerIndexRegister();

    Support::Allocator allocator;
    Hag::Testing::Mock::Initialize(allocator);
    S3Trio64MockConfigSetup();

    {
        bool success = UnlockExtendedCRTRegistersSafe();
        if (success)
        {
            --ret;
        }else
        {
            printf("UnlockExtendedCRTRegistersSafe returned false\n");
        }

        Hag::Testing::Mock::PortAndValue modifiedPorts[] =
        {
            {
                crtIndex,
                Hag::S3::CRTControllerRegister::RegisterLock2
            },
            {
                crtIndex + 1,
                Hag::S3::CRTController::RegisterLock2::UnlockCode
            }
        };
        
        uint16_t readPorts[] =
        {
            Hag::VGA::Register::MiscellaneousR
        };
        
        Hag::Testing::Mock::PortAndIndexAndValue modifiedIndexedPorts[] =
        {
            {
                crtIndex + 1,
                Hag::S3::CRTControllerRegister::RegisterLock1,
                Hag::S3::CRTController::RegisterLock1::UnlockCode
            },
            {
                crtIndex + 1,
                Hag::S3::CRTControllerRegister::RegisterLock2,
                Hag::S3::CRTController::RegisterLock2::UnlockCode
            }
        };
        
        Hag::Testing::Mock::PortAndIndex readIndexedPorts[] = { {0, 0} };

        ret -= VERIFYPORTCONTENT(0, modifiedPorts, readPorts, modifiedIndexedPorts, readIndexedPorts);
    }

    {
        LockExtendedCRTRegisters();

        Hag::Testing::Mock::PortAndValue modifiedPorts[] =
        {
            {
                crtIndex,
                Hag::S3::CRTControllerRegister::RegisterLock2
            },
            {
                crtIndex + 1,
                Hag::S3::CRTController::RegisterLock2::LockCode
            }
        };
        
        uint16_t readPorts[] =
        {
            Hag::VGA::Register::MiscellaneousR
        };
        
        Hag::Testing::Mock::PortAndIndexAndValue modifiedIndexedPorts[] =
        {
            {
                crtIndex + 1,
                Hag::S3::CRTControllerRegister::RegisterLock1,
                Hag::S3::CRTController::RegisterLock1::LockCode
            },
            {
                crtIndex + 1,
                Hag::S3::CRTControllerRegister::RegisterLock2,
                Hag::S3::CRTController::RegisterLock2::LockCode
            }
        };
        
        Hag::Testing::Mock::PortAndIndex readIndexedPorts[] = { {0, 0} };

        ret -= VERIFYPORTCONTENT(0, modifiedPorts, readPorts, modifiedIndexedPorts, readIndexedPorts);
    }

    return ret + Hag::Testing::Mock::Shutdown();
}

extern uint8_t modes[];
extern uint16_t modesCount;
extern uint8_t legacyModesOnly[];
extern uint16_t legacyModesOnlyCount;
extern uint8_t vesaModesOnly[];
extern uint16_t vesaModesOnlyCount;
extern uint8_t VESAVideoModeData[];

int FindVideoModeDataTest()
{
    int ret = 30;

    for (uint8_t i = 0; i < modesCount; ++i)
    {
        bool isVesaMode = false;
        for (uint8_t j = 0; j < vesaModesOnlyCount; ++j)
        {
            if (vesaModesOnly[j] == modes[i])
            {
                isVesaMode = true;
                break;
            }
        }

        uint32_t offset = 0;
        bool returnValue = FindVideoModeData(modes[i], offset);
        
        if (returnValue == isVesaMode)
        {
            if (VESAVideoModeData[offset] == modes[i])
            {
                --ret;
            }
        }
    }

    return ret;
}

int GetVideoModeFlagsTest()
{
    int ret = 30;

    for (uint8_t i = 0; i < modesCount; ++i)
    {
        bool isVesaMode = false;
        for (uint8_t j = 0; j < vesaModesOnlyCount; ++j)
        {
            if (vesaModesOnly[j] == modes[i])
            {
                isVesaMode = true;
                break;
            }
        }

        uint32_t offset = 0;
        bool returnValue = FindVideoModeData(modes[i], offset);

        uint8_t flags = 0;
        bool returnValue2 = GetVideoModeFlags(modes[i], flags);
        
        if (returnValue == isVesaMode &&
            returnValue2 == isVesaMode)
        {
            if (VESAVideoModeData[offset] == modes[i] &&
                VESAVideoModeData[offset + 3] == flags)
            {
                --ret;
            }
        }
    }

    return ret;
}

int GetVESAVideoModeOverrideTableTest()
{
    int ret = 30;

    for (uint8_t i = 0; i < modesCount; ++i)
    {
        bool isVesaMode = false;
        for (uint8_t j = 0; j < vesaModesOnlyCount; ++j)
        {
            if (vesaModesOnly[j] == modes[i])
            {
                isVesaMode = true;
                break;
            }
        }
        
        uint8_t* overrideTable = NULL;
        uint8_t modeDataIndex = 0xff;

        bool returnValue = GetVESAVideoModeOverrideTable(modes[i], overrideTable, modeDataIndex);
        if (returnValue == isVesaMode)
        {
            if (overrideTable != NULL &&
                modeDataIndex != 0xff)
            {
                --ret;
            }
        }
    }

    return ret;
}

extern Hag::Testing::Mock::BDAFieldsAndValues ModeSetBDA_modifiedDefault[];
extern uint16_t ModeSetBDA_modifiedDefaultCount;
extern Hag::Testing::Mock::BDAFieldsAndValues ModeSetBDA_modified_0x07_0x0F[];
extern uint16_t ModeSetBDA_modified_0x07_0x0FCount;
extern uint8_t ModeSetBDA_readDefault[];
extern uint16_t ModeSetBDA_readDefaultCount;
extern uint8_t ModeSetBDA_read_0x03[];
extern uint16_t ModeSetBDA_read_0x03Count;
extern uint8_t ModeSetBDA_read_0x07_0x0F[];
extern uint16_t ModeSetBDA_read_0x07_0x0FCount;

int ModeSetBDATest()
{
    int ret = 349;
    Support::Allocator allocator;
    Hag::Testing::Mock::Initialize(allocator);
    S3Trio64MockConfigSetup();

    for (uint8_t i = 0; i < modesCount; ++i)
    {
        uint8_t mode = modes[i];
        ModeSetBDA(mode);
        
        if (mode == modes[i])
            --ret;

        Hag::Testing::Mock::BDAFieldsAndValues* modifiedFields = ModeSetBDA_modifiedDefault;
        int modifiedFieldsSize = 0;

        uint8_t* readFields = ModeSetBDA_readDefault;
        int readFieldsSize = ModeSetBDA_readDefaultCount;

        if (mode == 0x03)
        {
            readFields = ModeSetBDA_read_0x03;
            readFieldsSize = ModeSetBDA_read_0x03Count;
        }

        if ((mode == 0x07) || (mode == 0x0F))
        {
            modifiedFields = ModeSetBDA_modified_0x07_0x0F;
            modifiedFieldsSize = ModeSetBDA_modified_0x07_0x0FCount;

            readFields = ModeSetBDA_read_0x07_0x0F;
            readFieldsSize = ModeSetBDA_read_0x07_0x0FCount;
        }

        ret -= Hag::Testing::Mock::VerifyBDAFieldsAndValues(0, modifiedFields, modifiedFieldsSize, readFields, readFieldsSize);
        
        Hag::Testing::Mock::Reset();
    }

    return ret + Hag::Testing::Mock::Shutdown();
}

extern uint8_t VerifyBDAOrDeactivate_readDefault[];
extern uint16_t VerifyBDAOrDeactivate_readDefaultCount;
extern uint8_t VerifyBDAOrDeactivate_read_0x03[];
extern uint16_t VerifyBDAOrDeactivate_read_0x03Count;
extern uint8_t VerifyBDAOrDeactivate_read_0x07_0x0F[];
extern uint16_t VerifyBDAOrDeactivate_read_0x07_0x0FCount;

int VerifyBDAOrDeactivateTest()
{
    int ret = 156;
    
    Support::Allocator allocator;
    Hag::Testing::Mock::Initialize(allocator);
    S3Trio64MockConfigSetup();
    
    for (uint8_t i = 0; i < legacyModesOnlyCount; ++i)
    {
        uint8_t mode = legacyModesOnly[i];

        ModeSetBDA(mode);
        bool retValue = VerifyBDAOrDeactivate(mode);

        if (retValue && (mode == modes[i]))
            --ret;

        Hag::Testing::Mock::BDAFieldsAndValues* modifiedFields = ModeSetBDA_modifiedDefault;
        int modifiedFieldsSize = 0;

        uint8_t* readFields = VerifyBDAOrDeactivate_readDefault;
        int readFieldsSize = VerifyBDAOrDeactivate_readDefaultCount;

        if (mode == 0x03)
        {
            readFields = VerifyBDAOrDeactivate_read_0x03;
            readFieldsSize = VerifyBDAOrDeactivate_read_0x03Count;
        }

        if ((mode == 0x07) || (mode == 0x0F))
        {
            modifiedFields = ModeSetBDA_modified_0x07_0x0F;
            modifiedFieldsSize = ModeSetBDA_modified_0x07_0x0FCount;

            readFields = VerifyBDAOrDeactivate_read_0x07_0x0F;
            readFieldsSize = VerifyBDAOrDeactivate_read_0x07_0x0FCount;
        }

        ret -= Hag::Testing::Mock::VerifyBDAFieldsAndValues(0, modifiedFields, modifiedFieldsSize, readFields, readFieldsSize);
        
        Hag::Testing::Mock::Reset();
    }

    return ret + Hag::Testing::Mock::Shutdown();
}

struct SetTextModeBiosDataTest_DataVerify
{
    uint8_t* font;
    uint8_t* overrideTable;
    uint8_t modeDataIndex;
};

extern SetTextModeBiosDataTest_DataVerify SetTextModeBiosDataTest_Verify[];
extern uint8_t SetTextModeBiosDataTest_idxToData[];
extern Hag::Testing::Mock::BDAFieldsAndValues* SetTextModeBiosDataTest_modified[];
extern int SetTextModeBiosDataTest_modifiedSize[];
extern uint8_t* SetTextModeBiosDataTest_read[];
extern int SetTextModeBiosDataTest_readSize[];
extern uint8_t SetTextModeBiosDataTest_ignore[];
extern uint16_t SetTextModeBiosDataTest_ignoreCount;


int SetTextModeBiosDataTest()
{
    int ret = 1726;
    
    Support::Allocator allocator;
    Hag::Testing::Mock::Initialize(allocator);
    S3Trio64MockConfigSetup();

    for (uint8_t i = 0; i < modesCount; ++i)
    {
        uint8_t mode = modes[i];

        //The following code replicates parts of the SetVideoMode function
        
        uint32_t offset = 0x00;
        if (mode > 0x13 && !FindVideoModeData(mode, offset))
            continue; //Failure.

        //Reset clear screen on mode switch flag.
        Hag::System::BDA::VideoModeOptions::Get() &= 0x7f;

        ModeSetBDA(mode);

        if (mode <= 0x13 && !VerifyBDAOrDeactivate(mode))
            continue; //Failure.

        Hag::System::BDA::DisplayMode::Get() = mode;

        uint8_t* selectedFont = NULL;
        uint8_t* overrideTable = NULL;
        uint8_t modeDataIndex = 0;
        SetTextModeBiosData(mode, selectedFont, overrideTable, modeDataIndex);

        if (SetTextModeBiosDataTest_Verify[i].font == selectedFont &&
            SetTextModeBiosDataTest_Verify[i].overrideTable == overrideTable &&
            SetTextModeBiosDataTest_Verify[i].modeDataIndex == modeDataIndex)
            --ret;
        else
        printf("SetTextModeBiosDataTest failed! 0x%02X", mode);

        uint8_t modeIdx = SetTextModeBiosDataTest_idxToData[i];

        ret -= Hag::Testing::Mock::VerifyBDAFieldsAndValues(0, 
            SetTextModeBiosDataTest_modified[modeIdx], SetTextModeBiosDataTest_modifiedSize[modeIdx],
            SetTextModeBiosDataTest_read[modeIdx], SetTextModeBiosDataTest_readSize[modeIdx],
            SetTextModeBiosDataTest_ignore, SetTextModeBiosDataTest_ignoreCount);
        
        Hag::Testing::Mock::Reset();
    }

    return ret + Hag::Testing::Mock::Shutdown();
}

Hag::Testing::Mock::PortAndValue ApplyVideoParametersTest_modifiedPorts[] =
{
    {
        0,
        0
    }
};

uint16_t ApplyVideoParametersTest_readPorts[] =
{
    0
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts[] =
{
    {
        0,
        0,
        0
    }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts[] = { {0, 0} };

int ApplyVideoParametersTest()
{
    int ret = 0;

    Support::Allocator allocator;
    Hag::Testing::Mock::Initialize(allocator);
    S3Trio64MockConfigSetup();


    for (uint8_t i = 0; i < modesCount; ++i)
    {
        uint8_t mode = modes[i];

        printf("\n%i: 0x%02X\n", i, mode);
        //The following code replicates parts of the SetVideoMode function

        if (!UnlockExtendedCRTRegistersSafe())
            continue;
        
        uint32_t offset = 0x00;
        if (mode > 0x13 && !FindVideoModeData(mode, offset))
            continue; //Failure.

        //Reset clear screen on mode switch flag.
        Hag::System::BDA::VideoModeOptions::Get() &= 0x7f;

        ModeSetBDA(mode);

        if (mode <= 0x13 && !VerifyBDAOrDeactivate(mode))
            continue; //Failure.

        Hag::System::BDA::DisplayMode::Get() = mode;

        uint8_t* selectedFont = NULL;
        uint8_t* overrideTable = NULL;
        uint8_t modeDataIndex = 0;
        SetTextModeBiosData(mode, selectedFont, overrideTable, modeDataIndex);

        Hag::System::BDA::VideoModeOptions::Get() &= 
                    ~(Hag::System::BDA::VideoModeOptions::Unknown |
                    Hag::System::BDA::VideoModeOptions::Inactive);

        //SaveDynamicParameterData(overrideTable);

        ApplyVideoParameters(overrideTable);

        ret -= VERIFYPORTCONTENT(0, ApplyVideoParametersTest_modifiedPorts, ApplyVideoParametersTest_readPorts,
                                 ApplyVideoParametersTest_modifiedIndexedPorts, ApplyVideoParametersTest_readIndexedPorts);

        Hag::Testing::Mock::Reset();
    }

    return ret + Hag::Testing::Mock::Shutdown();   
}

/*



    for (uint8_t i = 0; i < sizeof(modes) / sizeof(uint8_t); ++i)
    {
        printf("\n----------------------------------------\n");
        uint8_t mode = modes[i];
        Hag::Testing::Mock::SelectInstance(1);

        //uint32_t offset = 0;
        //bool ret1 = FindVideoModeData(mode, offset);
        //printf("FindVideoModeData(0x%02X) returned %s, %i\n", mode, ret1 ? "true" : "false", offset);

        //uint8_t flags = 0;
        //bool ret2 = GetVideoModeFlags(mode, flags);
        //printf("GetVideoModeFlags(0x%02X) returned %s, 0x%02X\n", mode, ret2 ? "true" : "false", flags);

        //uint8_t newMode = mode;
        //bool ret3 = SetBDACursorSize(newMode);
        //printf("SetBDACursorSize(0x%02X) returned %s, 0x%02X\n", mode, ret3 ? "true" : "false", newMode);


        bool ret = SetVideoMode(mode);
        printf("SetVideoMode(0x%02X) returned: %s\n", mode, ret ? "true" : "false");

        if (Hag::Testing::Mock::HasDifferences())
            Hag::Testing::Mock::Report();
        
        Hag::Testing::Mock::Reset();
    }
*/

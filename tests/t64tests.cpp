//Copyright 2023-Present riplin

#include <stdio.h>
#include <hag/testing/mock.h>
#include <hag/drivers/s3/trio.h>
#include <hag/drivers/s3/regs.h>
#include <hag/drivers/s3/sqrc/regs.h>
#include <hag/drivers/s3/sqrc/unlexseq.h>
#include <hag/drivers/s3/crtc/reglock1.h>
#include <hag/drivers/s3/crtc/reglock2.h>
#include <hag/drivers/s3/crtc/regs.h>
#include <hag/support/allocatr.h>
#include <hag/system/bda.h>

namespace Clean
{
    uint16_t GetCRTControllerIndexRegister();
    bool UnlockExtendedCRTRegistersSafe();
    void LockExtendedCRTRegisters();
    void ModeSetBDA(uint8_t& al);
    bool VerifyBDAOrDeactivate(uint8_t& al);
    Hag::S3::VideoParameters* SetTextModeBiosData(uint8_t mode);
    void ApplyVideoParameters(Hag::S3::VideoParameters* overrideTable);
    uint8_t FetchBusSpecificSystemConfig(uint16_t crtcPort);
    void InitializeCRTControllerAndSequencer(uint8_t* CRTCInitData, uint16_t crtcPort);
    Hag::S3::VideoParameters* GetVideoModeOverrideTable(uint8_t mode);
    void SetupClocks(uint8_t clockConfig);
    void ConfigureExtraVESAModeSettings(uint8_t mode, uint16_t crtc, Hag::S3::VESAVideoModeData* overrideTable, Hag::S3::VESAResolutionVariant* modeData);
    Hag::S3::VideoParameters* GetVideoModeOverrideTable(uint8_t mode);
    void EnableOver256KAddressingAndSetAddressWindow(uint8_t mode, uint16_t crtcReg);
    void SetColorMode(uint8_t mode);
    void ClearMemory();
    void ApplyVESAOverrideData(uint8_t mode);
    void SetPalette();
    void SetFont();
}

////////////////////////////////////////////////////////////////////////////////
void UnlockExtendedSequencerRegisters();
void LockExtendedSequencerRegisters();
uint16_t GetCRTControllerIndexRegister();
void LockExtendedCRTRegisters();
bool UnlockExtendedCRTRegistersSafe();
bool FindVideoModeData(uint8_t mode, uint32_t& offset);
bool GetVideoModeFlags(uint8_t mode, uint8_t& flags);
bool GetVESAVideoModeOverrideTable(uint8_t mode, uint8_t*& overrideTable, uint8_t& modeDataIndex);
void ModeSetBDA(uint8_t& al);
bool VerifyBDAOrDeactivate(uint8_t& al);
void GetVideoModeOverrideTable(uint8_t mode, uint8_t*& overrideTable, uint8_t& modeDataIndex);
void SetTextModeBiosData(uint8_t mode, uint8_t*& selectedFont, uint8_t*& overrideTable, uint8_t& modeDataIndex);
void ApplyVideoParameters(uint8_t* overrideTable);
uint8_t ReadDataWithIndexRegister(uint16_t port, uint8_t index);
uint8_t FetchBusSpecificSystemConfig(uint16_t crtcPort);
void InitializeCRTControllerAndSequencer(uint8_t* CRTCInitData, uint16_t crtcPort);
void SetupClocks(uint8_t clockConfig);
void ConfigureExtraVESAModeSettings(uint16_t crtc, uint8_t* modeData);
void EnableOver256KAddressingAndSetAddressWindow(uint8_t mode, uint16_t crtcReg);
void SetColorMode(uint8_t mode);
void ClearMemory();
void ApplyVESAOverrideData(uint8_t mode, uint8_t* overrideTable, uint8_t modeDataIndex);
void SetPalette();
void SetFont();
void PatchCharacterSet(uint8_t flags);

void SetPaletteProfile();
void EnablePaletteBasedVideo();
void SetTextFontAndAddressing(uint8_t* font, uint16_t someOffset, uint16_t numCharacters, uint8_t charHeight, uint8_t ramBank);
void ConfigureCursorPropertiesAndVerticalDisplayEnd(uint8_t characterPointHeight);
bool FetchCheckedVideoParameterBlockElement(uint16_t paramTableIdx, uint16_t subIdx, uint8_t*& parameterBlockElement, uint16_t size = sizeof(FARPointer));
bool GetVideoParameterBlockElement(uint16_t index, uint8_t*& returnPointer, uint16_t size = sizeof(FARPointer));
void SetGraphicsCharacterFont(uint8_t* graphicsCharacterFontDefinition);
void ClearScreen();
void TurnOnScreen();
void SaveDynamicParameterData(uint8_t* overrideTable);
bool CheckCurrentModeExists(uint8_t* ptr, uint16_t offset);
void ConfigureFontAndCursor(uint8_t* fontDefinition);
void ConfigureFontRamBank(uint8_t* fontDefinition);
bool SetVideoMode(uint8_t mode);
////////////////////////////////////////////////////////////////////////////////

extern Hag::Testing::Mock::PortAndValue S3_86C764x20_DefaultPortValues[];
extern uint16_t S3_86C764x20_DefaultPortValuesCount;
extern uint8_t S3_86C764x20_AttributeControllerRegisters[];
extern uint8_t S3_86C764x20_RAMDACRegisters[];
extern uint8_t S3_86C764x20_SequencerControllerRegisters[];
extern uint8_t S3_86C764x20_GraphicsControllerRegisters[];
extern uint8_t S3_86C764x20_CRTControllerRegisters[];
extern uint8_t S3_86C764x20_BDA[];
extern uint8_t S3_86C764x20_BIOS[];

void S3Trio64MockConfigSetup(Hag::IAllocator& allocator, uint8_t* ramdacRegisters = S3_86C764x20_RAMDACRegisters)
{
    Hag::Testing::Mock::Initialize(allocator, S3_86C764x20_DefaultPortValues, S3_86C764x20_DefaultPortValuesCount, S3_86C764x20_AttributeControllerRegisters, ramdacRegisters);
    Hag::Testing::Mock::AddIndexedPort("CRT Controller B", Hag::VGA::Register::CRTControllerIndexB, 0xff, Hag::VGA::Register::CRTControllerDataB, 0x100, S3_86C764x20_CRTControllerRegisters);
    Hag::Testing::Mock::AddIndexedPort("CRT Controller D", Hag::VGA::Register::CRTControllerIndexD, 0xff, Hag::VGA::Register::CRTControllerDataD, 0x100, S3_86C764x20_CRTControllerRegisters);
    Hag::Testing::Mock::AddIndexedPort("Graphics Controller", Hag::VGA::Register::GraphicsControllerIndex, 0x0f, Hag::VGA::Register::GraphicsControllerData, 0x09, S3_86C764x20_GraphicsControllerRegisters);
    Hag::Testing::Mock::AddIndexedPort("Sequence Controller", Hag::VGA::Register::SequencerIndex, 0x1f, Hag::VGA::Register::SequencerData, 0x19, S3_86C764x20_SequencerControllerRegisters);
    Hag::Testing::Mock::SetDefaultMemory(S3_86C764x20_BDA, 0x0400, 0x100);
    Hag::Testing::Mock::SetDefaultMemory(S3_86C764x20_BIOS, 0xC0000, 0x8000);
    Hag::Testing::Mock::AddReadOnlyIndexedRegister(Hag::VGA::Register::CRTControllerIndexB, Hag::S3::CRTControllerRegister::ChipIDRevision);
    Hag::Testing::Mock::AddReadOnlyIndexedRegister(Hag::VGA::Register::CRTControllerIndexD, Hag::S3::CRTControllerRegister::ChipIDRevision);
}

int MockSetupTeardownTest()
{
    int ret = 0;

    for (int i = 0; i < 25; ++i)
    {
        Support::Allocator allocator;
        S3Trio64MockConfigSetup(allocator);

        Hag::Testing::Mock::Reset();
        Hag::Testing::Mock::Shutdown();
    }

    return ret;
}

int SquenceRegisterLockingTest()
{
    int ret = 12;
    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator);

    {
        Hag::Testing::Mock::SelectInstance(0);
        UnlockExtendedSequencerRegisters();

        Hag::Testing::Mock::SelectInstance(1);
        Hag::S3::Sequencer::UnlockExtendedSequencer::Unlock();

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
        ret -= VERIFYPORTCONTENT(1, modifiedPorts, readPorts, modifiedIndexedPorts, readIndexedPorts);
    }

    {
        Hag::Testing::Mock::SelectInstance(0);
        LockExtendedSequencerRegisters();

        Hag::Testing::Mock::SelectInstance(1);
        Hag::S3::Sequencer::UnlockExtendedSequencer::Lock();

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
        ret -= VERIFYPORTCONTENT(1, modifiedPorts, readPorts, modifiedIndexedPorts, readIndexedPorts);
    }

    return ret + Hag::Testing::Mock::Shutdown();
}

int CRTCIndexRegisterTest()
{
    int ret = 3;
    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator);

    Hag::Testing::Mock::SelectInstance(0);
    uint16_t value1 = GetCRTControllerIndexRegister();

    Hag::Testing::Mock::SelectInstance(1);
    uint16_t value2 = Clean::GetCRTControllerIndexRegister();

    if (((value1 == Hag::VGA::Register::CRTControllerIndexB) ||
         (value1 == Hag::VGA::Register::CRTControllerIndexD)) &&
         ((value2 == Hag::VGA::Register::CRTControllerIndexB) ||
         (value2 == Hag::VGA::Register::CRTControllerIndexD)) &&
         value1 == value2)
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
        ret -= VERIFYPORTCONTENT(1, modifiedPorts, readPorts, modifiedIndexedPorts, readIndexedPorts);

    return ret + Hag::Testing::Mock::Shutdown();
}

int CRTCRegisterLockingTest()
{
    int ret = 9;
    uint16_t crtIndex = GetCRTControllerIndexRegister();

    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator);

    {
        Hag::Testing::Mock::SelectInstance(0);
        bool success1 = UnlockExtendedCRTRegistersSafe();

        Hag::Testing::Mock::SelectInstance(1);
        bool success2 = Clean::UnlockExtendedCRTRegistersSafe();
        if (success1 && success2)
        {
            --ret;
        }else
        {
            printf("UnlockExtendedCRTRegistersSafe returned false\n");
        }

        uint16_t ignorePorts[] =
        {
            Hag::VGA::Register::MiscellaneousR,
            crtIndex,
            crtIndex + 1
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
        
        ret -= Hag::Testing::Mock::VerifyPortsAndValues(0, NULL, 0,
                                                           NULL, 0,
                                                           modifiedIndexedPorts, sizeof(modifiedIndexedPorts) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue),
                                                           NULL, 0,
                                                           ignorePorts, sizeof(ignorePorts) / sizeof(uint16_t));
        ret -= Hag::Testing::Mock::VerifyPortsAndValues(1, NULL, 0,
                                                           NULL, 0,
                                                           modifiedIndexedPorts, sizeof(modifiedIndexedPorts) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue),
                                                           NULL, 0,
                                                           ignorePorts, sizeof(ignorePorts) / sizeof(uint16_t));
    }

    {
        Hag::Testing::Mock::SelectInstance(0);
        LockExtendedCRTRegisters();

        Hag::Testing::Mock::SelectInstance(1);
        Clean::LockExtendedCRTRegisters();

        uint16_t ignorePorts[] =
        {
            Hag::VGA::Register::MiscellaneousR,
            crtIndex,
            crtIndex + 1
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
        
        ret -= Hag::Testing::Mock::VerifyPortsAndValues(0, NULL, 0,
                                                           NULL, 0,
                                                           modifiedIndexedPorts, sizeof(modifiedIndexedPorts) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue),
                                                           NULL, 0,
                                                           ignorePorts, sizeof(ignorePorts) / sizeof(uint16_t));
        ret -= Hag::Testing::Mock::VerifyPortsAndValues(1, NULL, 0,
                                                           NULL, 0,
                                                           modifiedIndexedPorts, sizeof(modifiedIndexedPorts) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue),
                                                           NULL, 0,
                                                           ignorePorts, sizeof(ignorePorts) / sizeof(uint16_t));
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
    int ret = 60;

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
        bool returnValue1 = FindVideoModeData(modes[i], offset);
        if ((returnValue1 == isVesaMode) && isVesaMode)
        {
            if (VESAVideoModeData[offset] == modes[i])
            {
                --ret;
            }
        }

        Hag::S3::VESAVideoModeData* videoModeData = Hag::S3::TrioBase::FindVideoModeData(modes[i]);
        if (isVesaMode && (videoModeData != NULL))
        {
            if (videoModeData->Mode == modes[i])
                --ret;
        }
        if (isVesaMode && (videoModeData == NULL))
        {
            printf("Could not find video mode data for mode 0x%02X : %i\n", modes[i], i);
        }
    }

    return ret;
}

int GetVideoModeFlagsTest()
{
    int ret = 60;

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

        uint8_t flags1 = 0;
        bool returnValue2 = GetVideoModeFlags(modes[i], flags1);
        
        if ((returnValue == isVesaMode) &&
            (returnValue2 == isVesaMode) && isVesaMode)
        {
            if (VESAVideoModeData[offset] == modes[i] &&
                VESAVideoModeData[offset + 3] == flags1)
            {
                --ret;
            }
        }

        Hag::S3::VESAVideoModeData* videoModeData = Hag::S3::TrioBase::FindVideoModeData(modes[i]);

        uint8_t flags2 = 0;
        bool returnValue3 = Hag::S3::TrioBase::GetVideoModeFlags(modes[i], flags2);

        if (((videoModeData != NULL) == isVesaMode) &&
            (returnValue3 == isVesaMode) && isVesaMode)
        {
            if (videoModeData->Mode == modes[i] &&
                videoModeData->Flags == flags1)
            {
                --ret;
            }
        }

    }

    return ret;
}

int GetVESAVideoModeOverrideTableTest()
{
    int ret = 60;

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
        
        uint8_t* overrideTable1 = NULL;
        uint8_t modeDataIndex = 0xff;

        bool returnValue1 = GetVESAVideoModeOverrideTable(modes[i], overrideTable1, modeDataIndex);
        if ((returnValue1 == isVesaMode) && isVesaMode)
        {
            if (overrideTable1 != NULL &&
                modeDataIndex != 0xff)
            {
                --ret;
            }
        }

        Hag::S3::VESAVideoModeData* vesaModeData = Hag::S3::TrioBase::FindVideoModeData(modes[i]);
        if ((vesaModeData != NULL) && isVesaMode)
        {
            if ((overrideTable1[0] == vesaModeData->OverrideTable->DisplayedCharacterColumns) &&
                (overrideTable1[1] == vesaModeData->OverrideTable->DisplayedRowsMinus1) &&
                (overrideTable1[2] == vesaModeData->OverrideTable->CharacterMatrixHeightPoints) &&
                (overrideTable1[5] == vesaModeData->OverrideTable->SequencerRegisters[0]))
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
    int ret = 698;
    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator);

    for (uint8_t i = 0; i < modesCount; ++i)
    {
        uint8_t mode = modes[i];

        Hag::Testing::Mock::SelectInstance(0);
        ModeSetBDA(mode);

        if (mode == modes[i])
            --ret;

        Hag::Testing::Mock::SelectInstance(1);
        Clean::ModeSetBDA(mode);

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
        ret -= Hag::Testing::Mock::VerifyBDAFieldsAndValues(1, modifiedFields, modifiedFieldsSize, readFields, readFieldsSize);
        
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
    int ret = 312;
    
    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator);
    
    for (uint8_t i = 0; i < legacyModesOnlyCount; ++i)
    {
        uint8_t mode = legacyModesOnly[i];

        Hag::Testing::Mock::SelectInstance(0);
        ModeSetBDA(mode);
        bool retValue1 = VerifyBDAOrDeactivate(mode);

        if (retValue1 && (mode == modes[i]))
            --ret;

        Hag::Testing::Mock::SelectInstance(1);
        Clean::ModeSetBDA(mode);
        bool retValue2 = Clean::VerifyBDAOrDeactivate(mode);

        if (retValue2 && (mode == modes[i]))
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
        ret -= Hag::Testing::Mock::VerifyBDAFieldsAndValues(1, modifiedFields, modifiedFieldsSize, readFields, readFieldsSize);
        
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
    int ret = 3452;
    
    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator);

    for (uint8_t i = 0; i < modesCount; ++i)
    {
        uint8_t mode = modes[i];

        //The following code replicates parts of the SetVideoMode function
        
        //printf("\n%i: 0x%02X instance 0\n", i, mode);
        Hag::Testing::Mock::SelectInstance(0);
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

        //printf("\n%i: 0x%02X instance 1\n", i, mode);
        Hag::Testing::Mock::SelectInstance(1);
        if (mode > 0x13 && Hag::S3::TrioBase::FindVideoModeData(mode) == NULL)
            continue; //Failure.

        //Reset clear screen on mode switch flag.
        Hag::System::BDA::VideoModeOptions::Get() &= 0x7f;

        Clean::ModeSetBDA(mode);

        if (mode <= 0x13 && !Clean::VerifyBDAOrDeactivate(mode))
            continue; //Failure.

        Hag::System::BDA::DisplayMode::Get() = mode;

        Hag::S3::VideoParameters* parameters = Clean::SetTextModeBiosData(mode);

        if (SetTextModeBiosDataTest_Verify[i].font == selectedFont &&
            SetTextModeBiosDataTest_Verify[i].overrideTable == overrideTable &&
            SetTextModeBiosDataTest_Verify[i].modeDataIndex == modeDataIndex)
            --ret;
        else
        {
            printf("SetTextModeBiosDataTest failed! 0x%02X\n", mode);
            printf("font: 0x%08X - 0x%08X\n", SetTextModeBiosDataTest_Verify[i].font, selectedFont);
            printf("overrideTable: 0x%08X - 0x%08X\n", SetTextModeBiosDataTest_Verify[i].overrideTable, overrideTable);
            printf("modeDataIndex: %i - %i\n", SetTextModeBiosDataTest_Verify[i].modeDataIndex, modeDataIndex);
        }

        if ((parameters->DisplayedCharacterColumns == overrideTable[0]) &&
            (parameters->DisplayedRowsMinus1 == overrideTable[1]) &&
            (parameters->CharacterMatrixHeightPoints == overrideTable[2]) &&
            (parameters->SequencerRegisters[0] == overrideTable[5]))
            --ret;
        else
        {
            if (parameters->DisplayedCharacterColumns != overrideTable[0])
                printf("DisplayedCharacterColumns do not match raw port: %i ! = %i\n", parameters->DisplayedCharacterColumns, overrideTable[0]);
            if (parameters->DisplayedRowsMinus1 != overrideTable[1])
                printf("DisplayedRowsMinus1 do not match raw port: %i ! = %i\n", parameters->DisplayedRowsMinus1, overrideTable[1]);
            if (parameters->CharacterMatrixHeightPoints != overrideTable[2])
                printf("CharacterMatrixHeightPoints do not match raw port: %i ! = %i\n", parameters->CharacterMatrixHeightPoints, overrideTable[2]);
            if (parameters->SequencerRegisters[0] != overrideTable[5])
                printf("SequencerRegisters[0] do not match raw port: %i ! = %i\n", parameters->SequencerRegisters[0], overrideTable[5]);
        }

        uint8_t modeIdx = SetTextModeBiosDataTest_idxToData[i];

        ret -= Hag::Testing::Mock::VerifyBDAFieldsAndValues(0,
            SetTextModeBiosDataTest_modified[modeIdx], SetTextModeBiosDataTest_modifiedSize[modeIdx],
            SetTextModeBiosDataTest_read[modeIdx], SetTextModeBiosDataTest_readSize[modeIdx],
            SetTextModeBiosDataTest_ignore, SetTextModeBiosDataTest_ignoreCount);

        ret -= Hag::Testing::Mock::VerifyBDAFieldsAndValues(1,
            SetTextModeBiosDataTest_modified[modeIdx], SetTextModeBiosDataTest_modifiedSize[modeIdx],
            SetTextModeBiosDataTest_read[modeIdx], SetTextModeBiosDataTest_readSize[modeIdx],
            SetTextModeBiosDataTest_ignore, SetTextModeBiosDataTest_ignoreCount);

        Hag::Testing::Mock::Reset();
    }

    return ret + Hag::Testing::Mock::Shutdown();
}

extern Hag::Testing::Mock::PortAndValue* ApplyVideoParametersTest_modifiedPorts[];
extern int ApplyVideoParametersTest_modifiedPortsCount[];
extern uint16_t ApplyVideoParametersTest_readPorts[];
extern int ApplyVideoParametersTest_readPortsCount;
extern Hag::Testing::Mock::PortAndIndexAndValue* ApplyVideoParametersTest_modifiedIndexedPorts[];
extern int ApplyVideoParametersTest_modifiedIndexedPortsCount[];
extern Hag::Testing::Mock::PortAndIndex* ApplyVideoParametersTest_readIndexedPorts[];
extern int ApplyVideoParametersTest_readIndexedPortsCount[];

extern uint16_t ApplyVideoParametersTest_ignorePorts[];
extern uint16_t ApplyVideoParametersTest_ignorePortsCount;
extern Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_ignoreIndexedPorts[];
extern uint16_t ApplyVideoParametersTest_ignoreIndexedPortsCount;

int ApplyVideoParametersTest()
{
    int ret = 5986;

    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator);


    for (uint8_t i = 0; i < modesCount; ++i)
    {
        uint8_t mode = modes[i];
        //The following code replicates parts of the SetVideoMode function
        {
            //printf("\n%i: 0x%02X instance 0\n", i, mode);
            Hag::Testing::Mock::SelectInstance(0);
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
        }

        {
            //printf("\n%i: 0x%02X instance 1\n", i, mode);
            Hag::Testing::Mock::SelectInstance(1);
            if (!Clean::UnlockExtendedCRTRegistersSafe())
                continue;
            
            if (mode > 0x13 && Hag::S3::TrioBase::FindVideoModeData(mode) == NULL)
                continue; //Failure.

            //Reset clear screen on mode switch flag.
            Hag::System::BDA::VideoModeOptions::Get() &= 0x7f;

            Clean::ModeSetBDA(mode);

            if (mode <= 0x13 && !Clean::VerifyBDAOrDeactivate(mode))
                continue; //Failure.

            Hag::System::BDA::DisplayMode::Get() = mode;

            Hag::S3::VideoParameters* parameters = Clean::SetTextModeBiosData(mode);

            Hag::System::BDA::VideoModeOptions::Get() &= 
                        ~(Hag::System::BDA::VideoModeOptions::Unknown |
                        Hag::System::BDA::VideoModeOptions::Inactive);

            //SaveDynamicParameterData(overrideTable);

            Clean::ApplyVideoParameters(parameters);
        }

        ret -= Hag::Testing::Mock::VerifyPortsAndValues(0, ApplyVideoParametersTest_modifiedPorts[i], 
                                                           ApplyVideoParametersTest_modifiedPortsCount[i],
                                                           ApplyVideoParametersTest_readPorts,
                                                           ApplyVideoParametersTest_readPortsCount,
                                                           ApplyVideoParametersTest_modifiedIndexedPorts[i],
                                                           ApplyVideoParametersTest_modifiedIndexedPortsCount[i],
                                                           ApplyVideoParametersTest_readIndexedPorts[i],
                                                           ApplyVideoParametersTest_readIndexedPortsCount[i],
                                                           ApplyVideoParametersTest_ignorePorts,
                                                           ApplyVideoParametersTest_ignorePortsCount,
                                                           ApplyVideoParametersTest_ignoreIndexedPorts,
                                                           ApplyVideoParametersTest_ignoreIndexedPortsCount);

        ret -= Hag::Testing::Mock::VerifyPortsAndValues(1, ApplyVideoParametersTest_modifiedPorts[i], 
                                                           ApplyVideoParametersTest_modifiedPortsCount[i],
                                                           ApplyVideoParametersTest_readPorts,
                                                           ApplyVideoParametersTest_readPortsCount,
                                                           ApplyVideoParametersTest_modifiedIndexedPorts[i],
                                                           ApplyVideoParametersTest_modifiedIndexedPortsCount[i],
                                                           ApplyVideoParametersTest_readIndexedPorts[i],
                                                           ApplyVideoParametersTest_readIndexedPortsCount[i],
                                                           ApplyVideoParametersTest_ignorePorts,
                                                           ApplyVideoParametersTest_ignorePortsCount,
                                                           ApplyVideoParametersTest_ignoreIndexedPorts,
                                                           ApplyVideoParametersTest_ignoreIndexedPortsCount);

        Hag::Testing::Mock::Reset();
    }

    return ret + Hag::Testing::Mock::Shutdown();   
}

extern uint8_t PCISystemConfig;
extern uint8_t VLBSystemConfig;

int FetchBusSpecificSystemConfigTest()
{
    int ret = 4;

    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator);

    uint16_t crtcPort = GetCRTControllerIndexRegister();
    uint8_t val = ReadDataWithIndexRegister(crtcPort, 0x36);
    uint8_t pciVal = (val & (~0x03)) | 0x02;
    uint8_t vlbVal = (val & (~0x03)) | 0x01;
    
    Hag::Testing::Mock::SelectInstance(0);
    SYS_WritePortBytes(crtcPort, 0x36, pciVal);

    if (FetchBusSpecificSystemConfig(crtcPort) == PCISystemConfig)
        --ret;
    else printf("PCI fail.\n");

    SYS_WritePortBytes(crtcPort, 0x36, vlbVal);

    if (FetchBusSpecificSystemConfig(crtcPort) == VLBSystemConfig)
        --ret;
    else printf("VLB fail.\n");

    Hag::Testing::Mock::SelectInstance(1);
    SYS_WritePortBytes(crtcPort, 0x36, pciVal);

    if (Clean::FetchBusSpecificSystemConfig(crtcPort) == PCISystemConfig)
        --ret;
    else printf("PCI fail.\n");

    SYS_WritePortBytes(crtcPort, 0x36, vlbVal);

    if (Clean::FetchBusSpecificSystemConfig(crtcPort) == VLBSystemConfig)
        --ret;
    else printf("VLB fail.\n");

    return ret + Hag::Testing::Mock::Shutdown();   
}

extern uint8_t CRTControllerInitData[];

extern Hag::Testing::Mock::PortAndValue CRTControllerInitData_modifiedPorts[];
extern int CRTControllerInitData_modifiedPortsCount;
extern uint16_t CRTControllerInitData_readPorts[];
extern int CRTControllerInitData_readPortsCount;
extern Hag::Testing::Mock::PortAndIndexAndValue CRTControllerInitData_modifiedIndexedPorts[];
extern int CRTControllerInitData_modifiedIndexedPortsCount;
extern Hag::Testing::Mock::PortAndIndex CRTControllerInitData_readIndexedPorts[];
extern int CRTControllerInitData_readIndexedPortsCount;
extern uint16_t CRTControllerInitData_ignorePorts[];
extern int CRTControllerInitData_ignorePortsCount;

int InitializeCRTControllerAndSequencerTest()
{
    int ret = 64;

    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator);

    {
        Hag::Testing::Mock::SelectInstance(0);
        uint16_t crtcPort = GetCRTControllerIndexRegister();

        InitializeCRTControllerAndSequencer(CRTControllerInitData, crtcPort);

        ret -= Hag::Testing::Mock::VerifyPortsAndValues(0, CRTControllerInitData_modifiedPorts, 
                                                        CRTControllerInitData_modifiedPortsCount,
                                                        CRTControllerInitData_readPorts,
                                                        CRTControllerInitData_readPortsCount,
                                                        CRTControllerInitData_modifiedIndexedPorts,
                                                        CRTControllerInitData_modifiedIndexedPortsCount,
                                                        CRTControllerInitData_readIndexedPorts,
                                                        CRTControllerInitData_readIndexedPortsCount,
                                                        CRTControllerInitData_ignorePorts,
                                                        CRTControllerInitData_ignorePortsCount,
                                                        NULL,
                                                        0);
    }

    {
        Hag::Testing::Mock::SelectInstance(1);
        uint16_t crtcPort = Clean::GetCRTControllerIndexRegister();

        Clean::InitializeCRTControllerAndSequencer(CRTControllerInitData, crtcPort);

        ret -= Hag::Testing::Mock::VerifyPortsAndValues(1, CRTControllerInitData_modifiedPorts, 
                                                        CRTControllerInitData_modifiedPortsCount,
                                                        CRTControllerInitData_readPorts,
                                                        CRTControllerInitData_readPortsCount,
                                                        CRTControllerInitData_modifiedIndexedPorts,
                                                        CRTControllerInitData_modifiedIndexedPortsCount,
                                                        CRTControllerInitData_readIndexedPorts,
                                                        CRTControllerInitData_readIndexedPortsCount,
                                                        CRTControllerInitData_ignorePorts,
                                                        CRTControllerInitData_ignorePortsCount,
                                                        NULL,
                                                        0);
    }

    return ret + Hag::Testing::Mock::Shutdown();   
}

extern uint8_t ModeData[][12];

extern Hag::Testing::Mock::PortAndIndexAndValue* SetupClocksTest_modifiedIndexedPorts[];
extern int SetupClocksTest_modifiedIndexedPortsCount[];

extern uint16_t SetupClocksTest_ignorePorts[];
extern uint16_t SetupClocksTest_ignorePortsCount;
extern Hag::Testing::Mock::PortAndIndex SetupClocksTest_ignoreIndexedPorts[];
extern uint16_t SetupClocksTest_ignoreIndexedPortsCount;

int SetupClocksTest()
{
    int ret = 300;

    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator);

    for (uint8_t i = 0; i < vesaModesOnlyCount; ++i)
    {
        uint8_t mode = vesaModesOnly[i];

        {
            uint8_t* overrideTable = NULL;
            uint8_t modeDataIndex = 0;

            Hag::Testing::Mock::SelectInstance(0);
            GetVideoModeOverrideTable(mode, overrideTable, modeDataIndex);

            uint8_t* modeData = ModeData[modeDataIndex];
            SetupClocks(modeData[0x03]);
        }

        {
            Hag::Testing::Mock::SelectInstance(1);
            Hag::S3::VESAVideoModeData* vesaModeData = Hag::S3::TrioBase::FindVideoModeData(mode);
            Clean::SetupClocks(vesaModeData->VariantData->ClockConfigIndex);
        }

        ret -= Hag::Testing::Mock::VerifyPortsAndValues(0, NULL,
                                                           0,
                                                           NULL,
                                                           0,
                                                           SetupClocksTest_modifiedIndexedPorts[i],
                                                           SetupClocksTest_modifiedIndexedPortsCount[i],
                                                           NULL,
                                                           0,
                                                           SetupClocksTest_ignorePorts,
                                                           SetupClocksTest_ignorePortsCount,
                                                           SetupClocksTest_ignoreIndexedPorts,
                                                           SetupClocksTest_ignoreIndexedPortsCount
                                                           );

        ret -= Hag::Testing::Mock::VerifyPortsAndValues(1, NULL,
                                                           0,
                                                           NULL,
                                                           0,
                                                           SetupClocksTest_modifiedIndexedPorts[i],
                                                           SetupClocksTest_modifiedIndexedPortsCount[i],
                                                           NULL,
                                                           0,
                                                           SetupClocksTest_ignorePorts,
                                                           SetupClocksTest_ignorePortsCount,
                                                           SetupClocksTest_ignoreIndexedPorts,
                                                           SetupClocksTest_ignoreIndexedPortsCount
                                                           );

        Hag::Testing::Mock::Reset();
    }

    return ret + Hag::Testing::Mock::Shutdown();   
}

extern Hag::Testing::Mock::PortAndValue* ConfigureExtraVESAModeSettingsTest_modifiedPorts[];
extern int ConfigureExtraVESAModeSettingsTest_modifiedPortsCount[];
extern Hag::Testing::Mock::PortAndIndexAndValue* ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts[];
extern int ConfigureExtraVESAModeSettingsTest_modifiedIndexedPortsCount[];
extern Hag::Testing::Mock::PortAndIndex* ConfigureExtraVESAModeSettingsTest_readIndexedPorts[];
extern int ConfigureExtraVESAModeSettingsTest_readIndexedPortsCount[];
extern uint16_t ConfigureExtraVESAModeSettingsTest_ignorePorts[];
extern uint16_t ConfigureExtraVESAModeSettingsTest_ignorePortsCount;

int ConfigureExtraVESAModeSettingsTest()
{
    int ret = 3170;

    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator);

    uint16_t crtcPort = GetCRTControllerIndexRegister();
    Hag::Testing::Mock::Reset();
    int idx = 0;
    for (uint8_t i = 0; i < vesaModesOnlyCount; ++i)
    {
        uint8_t mode = vesaModesOnly[i];

        uint8_t* overrideTable = NULL;
        uint8_t modeDataIndex = 0;

        GetVideoModeOverrideTable(mode, overrideTable, modeDataIndex);
        uint8_t* modeData1 = NULL;
        uint8_t baseIdx = modeDataIndex;

        Hag::S3::VESAVideoModeData* vesaModeData = Hag::S3::TrioBase::FindVideoModeData(mode);
        if (vesaModeData == NULL)
            continue;

        Hag::S3::VESAResolutionVariant* modeData2 = vesaModeData->VariantData;

        do
        {
            modeData1 = ModeData[modeDataIndex];
            if (modeData1[0x02] != modeData2->FlagsAndFilter)
                continue;

            Hag::Testing::Mock::SelectInstance(0);
            Hag::System::BDA::DisplayMode::Get() = mode;
            ConfigureExtraVESAModeSettings(crtcPort, modeData1);

            Hag::Testing::Mock::SelectInstance(1);
            Clean::ConfigureExtraVESAModeSettings(mode, crtcPort, vesaModeData, modeData2);

            //printf("\n%i: 0x%02X, idx: %i instance 0\n", i, mode, idx);
            ret -= Hag::Testing::Mock::VerifyPortsAndValues(0, ConfigureExtraVESAModeSettingsTest_modifiedPorts[idx],
                                                            ConfigureExtraVESAModeSettingsTest_modifiedPortsCount[idx],
                                                            NULL,
                                                            0,
                                                            ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts[idx],
                                                            ConfigureExtraVESAModeSettingsTest_modifiedIndexedPortsCount[idx],
                                                            ConfigureExtraVESAModeSettingsTest_readIndexedPorts[idx],
                                                            ConfigureExtraVESAModeSettingsTest_readIndexedPortsCount[idx],
                                                            ConfigureExtraVESAModeSettingsTest_ignorePorts,
                                                            ConfigureExtraVESAModeSettingsTest_ignorePortsCount,
                                                            NULL,
                                                            0
                                                            ); 

            //printf("\n%i: 0x%02X, idx: %i instance 1\n", i, mode, idx);
            ret -= Hag::Testing::Mock::VerifyPortsAndValues(1, ConfigureExtraVESAModeSettingsTest_modifiedPorts[idx],
                                                            ConfigureExtraVESAModeSettingsTest_modifiedPortsCount[idx],
                                                            NULL,
                                                            0,
                                                            ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts[idx],
                                                            ConfigureExtraVESAModeSettingsTest_modifiedIndexedPortsCount[idx],
                                                            ConfigureExtraVESAModeSettingsTest_readIndexedPorts[idx],
                                                            ConfigureExtraVESAModeSettingsTest_readIndexedPortsCount[idx],
                                                            ConfigureExtraVESAModeSettingsTest_ignorePorts,
                                                            ConfigureExtraVESAModeSettingsTest_ignorePortsCount,
                                                            NULL,
                                                            0
                                                            ); 

            Hag::Testing::Mock::Reset();

            ++idx;
            --modeDataIndex;
            ++modeData2;
            //modeData points to progressively lower frequency versions. top bit of byte 2 is terminate.
        } while ((modeData1[0x02] & 0x80) == 0x00);

    }
    return ret + Hag::Testing::Mock::Shutdown();   
}

extern Hag::Testing::Mock::PortAndIndex* EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts[];
extern int EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPortsCount[];
extern uint16_t EnableOver256KAddressingAndSetAddressWindowTest_ignorePorts[];
extern int EnableOver256KAddressingAndSetAddressWindowTest_ignorePortsCount;

int EnableOver256KAddressingAndSetAddressWindowTest()
{
    int ret = 460;

    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator);

    uint16_t crtcPort = GetCRTControllerIndexRegister();
    Hag::Testing::Mock::Reset();

    for (uint8_t i = 0; i < modesCount; ++i)
    {
        uint8_t mode = modes[i];

        Hag::Testing::Mock::SelectInstance(0);
        EnableOver256KAddressingAndSetAddressWindow(mode, crtcPort);

        Hag::Testing::Mock::SelectInstance(1);
        Clean::EnableOver256KAddressingAndSetAddressWindow(mode, crtcPort);

        ret -= Hag::Testing::Mock::VerifyPortsAndValues(0, NULL,
                                                        0,
                                                        NULL,
                                                        0,
                                                        NULL,
                                                        0,
                                                        EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts[i],
                                                        EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPortsCount[i],
                                                        EnableOver256KAddressingAndSetAddressWindowTest_ignorePorts,
                                                        EnableOver256KAddressingAndSetAddressWindowTest_ignorePortsCount,
                                                        NULL,
                                                        0
                                                        ); 

        ret -= Hag::Testing::Mock::VerifyPortsAndValues(1, NULL,
                                                        0,
                                                        NULL,
                                                        0,
                                                        NULL,
                                                        0,
                                                        EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts[i],
                                                        EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPortsCount[i],
                                                        EnableOver256KAddressingAndSetAddressWindowTest_ignorePorts,
                                                        EnableOver256KAddressingAndSetAddressWindowTest_ignorePortsCount,
                                                        NULL,
                                                        0
                                                        ); 

        Hag::Testing::Mock::Reset();
    }

    return ret + Hag::Testing::Mock::Shutdown();   
}

extern Hag::Testing::Mock::PortAndIndexAndValue* SetColorModeTest_modifiedIndexedPorts[];
extern int SetColorModeTest_modifiedIndexedPortsCount[];
extern Hag::Testing::Mock::PortAndIndex* SetColorModeTest_readIndexedPorts[];
extern int SetColorModeTest_readIndexedPortsCount[];
extern uint16_t SetColorModeTest_ignorePorts[];
extern int SetColorModeTest_ignorePortsCount;

int SetColorModeTest()
{
    int ret = 400;

    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator);

    for (uint8_t i = 0; i < modesCount; ++i)
    {
        uint8_t mode = modes[i];

        Hag::Testing::Mock::SelectInstance(0);
        SetColorMode(mode);

        Hag::Testing::Mock::SelectInstance(1);
        Clean::SetColorMode(mode);

        ret -= Hag::Testing::Mock::VerifyPortsAndValues(0, NULL,
                                                        0,
                                                        NULL,
                                                        0,
                                                        SetColorModeTest_modifiedIndexedPorts[i],
                                                        SetColorModeTest_modifiedIndexedPortsCount[i],
                                                        SetColorModeTest_readIndexedPorts[i],
                                                        SetColorModeTest_readIndexedPortsCount[i],
                                                        SetColorModeTest_ignorePorts,
                                                        SetColorModeTest_ignorePortsCount,
                                                        NULL,
                                                        0
                                                        ); 

        ret -= Hag::Testing::Mock::VerifyPortsAndValues(1, NULL,
                                                        0,
                                                        NULL,
                                                        0,
                                                        SetColorModeTest_modifiedIndexedPorts[i],
                                                        SetColorModeTest_modifiedIndexedPortsCount[i],
                                                        SetColorModeTest_readIndexedPorts[i],
                                                        SetColorModeTest_readIndexedPortsCount[i],
                                                        SetColorModeTest_ignorePorts,
                                                        SetColorModeTest_ignorePortsCount,
                                                        NULL,
                                                        0
                                                        ); 

        Hag::Testing::Mock::Reset();
    }

    return ret + Hag::Testing::Mock::Shutdown();   
}

extern Hag::Testing::Mock::PortAndValue ClearMemoryTest_modifiedPorts[];
extern int ClearMemoryTest_modifiedPortsCount;
extern Hag::Testing::Mock::PortAndIndexAndValue ClearMemoryTest_modifiedIndexedPorts[];
extern int ClearMemoryTest_modifiedIndexedPortsCount;
extern Hag::Testing::Mock::PortAndIndex ClearMemoryTest_readIndexedPorts[];
extern int ClearMemoryTest_readIndexedPortsCount;
extern uint16_t ClearMemoryTest_ignorePorts[];
extern int ClearMemoryTest_ignorePortsCount;

int ClearMemoryTest()
{
    int ret = 46;

    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator);

    Hag::Testing::Mock::SelectInstance(0);
    ClearMemory();

    Hag::Testing::Mock::SelectInstance(1);
    Clean::ClearMemory();

    ret -= Hag::Testing::Mock::VerifyPortsAndValues(0,
                                                    ClearMemoryTest_modifiedPorts,
                                                    ClearMemoryTest_modifiedPortsCount,
                                                    NULL,
                                                    0,
                                                    ClearMemoryTest_modifiedIndexedPorts,
                                                    ClearMemoryTest_modifiedIndexedPortsCount,
                                                    ClearMemoryTest_readIndexedPorts,
                                                    ClearMemoryTest_readIndexedPortsCount,
                                                    ClearMemoryTest_ignorePorts,
                                                    ClearMemoryTest_ignorePortsCount,
                                                    NULL,
                                                    0
                                                    ); 

    ret -= Hag::Testing::Mock::VerifyPortsAndValues(1,
                                                    ClearMemoryTest_modifiedPorts,
                                                    ClearMemoryTest_modifiedPortsCount,
                                                    NULL,
                                                    0,
                                                    ClearMemoryTest_modifiedIndexedPorts,
                                                    ClearMemoryTest_modifiedIndexedPortsCount,
                                                    ClearMemoryTest_readIndexedPorts,
                                                    ClearMemoryTest_readIndexedPortsCount,
                                                    ClearMemoryTest_ignorePorts,
                                                    ClearMemoryTest_ignorePortsCount,
                                                    NULL,
                                                    0
                                                    ); 

    Hag::Testing::Mock::Reset();

    return ret + Hag::Testing::Mock::Shutdown();   
}

extern uint16_t* ApplyVESAOverrideDataTest_readPorts[];
extern int ApplyVESAOverrideDataTest_readPortsCount[];
extern Hag::Testing::Mock::PortAndValue* ApplyVESAOverrideDataTest_modifiedPorts[];
extern int ApplyVESAOverrideDataTest_modifiedPortsCount[];
extern Hag::Testing::Mock::PortAndIndex* ApplyVESAOverrideDataTest_readIndexedPorts[];
extern int ApplyVESAOverrideDataTest_readIndexedPortsCount[];
extern Hag::Testing::Mock::PortAndIndexAndValue* ApplyVESAOverrideDataTest_modifiedIndexedPorts[];
extern int ApplyVESAOverrideDataTest_modifiedIndexedPortsCount[];

extern uint16_t ApplyVESAOverrideDataTest_ignorePorts[];
extern uint16_t ApplyVESAOverrideDataTest_ignorePortsCount;
extern Hag::Testing::Mock::PortAndIndex ApplyVESAOverrideDataTest_ignoreIndexedPorts[];
extern uint16_t ApplyVESAOverrideDataTest_ignoreIndexedPortsCount;

int ApplyVESAOverrideDataTest()
{
    int ret = 6784;

    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator);

    for (uint8_t i = 0; i < modesCount; ++i)
    {
        uint8_t mode = modes[i];
        //The following code replicates parts of the SetVideoMode function

        //printf("\n%i: 0x%02X instance 0\n", i, mode);
        {
            Hag::Testing::Mock::SelectInstance(0);
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

            Hag::Testing::Mock::Snapshot();

            ApplyVESAOverrideData(mode, overrideTable, modeDataIndex);
        }

        ret -= Hag::Testing::Mock::VerifyPortsAndValues(0, ApplyVESAOverrideDataTest_modifiedPorts[i], 
                                                           ApplyVESAOverrideDataTest_modifiedPortsCount[i],
                                                           ApplyVESAOverrideDataTest_readPorts[i],
                                                           ApplyVESAOverrideDataTest_readPortsCount[i],
                                                           ApplyVESAOverrideDataTest_modifiedIndexedPorts[i],
                                                           ApplyVESAOverrideDataTest_modifiedIndexedPortsCount[i],
                                                           ApplyVESAOverrideDataTest_readIndexedPorts[i],
                                                           ApplyVESAOverrideDataTest_readIndexedPortsCount[i],
                                                           ApplyVESAOverrideDataTest_ignorePorts,
                                                           ApplyVESAOverrideDataTest_ignorePortsCount,
                                                           ApplyVESAOverrideDataTest_ignoreIndexedPorts,
                                                           ApplyVESAOverrideDataTest_ignoreIndexedPortsCount);

        //printf("\n%i: 0x%02X instance 1\n", i, mode);
        {
            Hag::Testing::Mock::SelectInstance(1);
            if (!Clean::UnlockExtendedCRTRegistersSafe())
                continue;
            
            uint32_t offset = 0x00;
            if (mode > 0x13 && (Hag::S3::TrioBase::FindVideoModeData(mode) == NULL))
                continue; //Failure.

            //Reset clear screen on mode switch flag.
            Hag::System::BDA::VideoModeOptions::Get() &= 0x7f;

            Clean::ModeSetBDA(mode);

            if (mode <= 0x13 && !Clean::VerifyBDAOrDeactivate(mode))
                continue; //Failure.

            Hag::System::BDA::DisplayMode::Get() = mode;

            Hag::S3::VideoParameters* parameters = Clean::SetTextModeBiosData(mode);

            Hag::System::BDA::VideoModeOptions::Get() &= 
                        ~(Hag::System::BDA::VideoModeOptions::Unknown |
                        Hag::System::BDA::VideoModeOptions::Inactive);

            //SaveDynamicParameterData(overrideTable);

            Clean::ApplyVideoParameters(parameters);

            Hag::Testing::Mock::Snapshot();

            Clean::ApplyVESAOverrideData(mode);
        }

        ret -= Hag::Testing::Mock::VerifyPortsAndValues(1, ApplyVESAOverrideDataTest_modifiedPorts[i], 
                                                           ApplyVESAOverrideDataTest_modifiedPortsCount[i],
                                                           ApplyVESAOverrideDataTest_readPorts[i],
                                                           ApplyVESAOverrideDataTest_readPortsCount[i],
                                                           ApplyVESAOverrideDataTest_modifiedIndexedPorts[i],
                                                           ApplyVESAOverrideDataTest_modifiedIndexedPortsCount[i],
                                                           ApplyVESAOverrideDataTest_readIndexedPorts[i],
                                                           ApplyVESAOverrideDataTest_readIndexedPortsCount[i],
                                                           ApplyVESAOverrideDataTest_ignorePorts,
                                                           ApplyVESAOverrideDataTest_ignorePortsCount,
                                                           ApplyVESAOverrideDataTest_ignoreIndexedPorts,
                                                           ApplyVESAOverrideDataTest_ignoreIndexedPortsCount);

        Hag::Testing::Mock::Reset();
    }

    return ret + Hag::Testing::Mock::Shutdown();   
}

extern uint16_t* SetPaletteTest_readPorts[];
extern int SetPaletteTest_readPortsCount[];
extern Hag::Testing::Mock::PortAndIndexAndValue* SetPaletteTest_modifiedIndexedPorts[];
extern int SetPaletteTest_modifiedIndexedPortsCount[];

extern uint16_t SetPaletteTest_ignorePorts[];
extern uint16_t SetPaletteTest_ignorePortsCount;
extern Hag::Testing::Mock::PortAndIndex SetPaletteTest_ignoreIndexedPorts[];
extern uint16_t SetPaletteTest_ignoreIndexedPortsCount;

extern uint8_t SetPaletteTest_RAMDACRegisters[];

int SetPaletteTest()
{
    int ret = 44306;

    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator, SetPaletteTest_RAMDACRegisters);

    for (uint8_t i = 0; i < modesCount; ++i)
    {
        uint8_t mode = modes[i];
        //The following code replicates parts of the SetVideoMode function

        //printf("\n%i: 0x%02X instance 0\n", i, mode);
        {
            Hag::Testing::Mock::SelectInstance(0);
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

            ApplyVESAOverrideData(mode, overrideTable, modeDataIndex);

            Hag::Testing::Mock::Snapshot();

            SetPalette();
        }

        ret -= Hag::Testing::Mock::VerifyPortsAndValues(0, NULL,
                                                           0,
                                                           SetPaletteTest_readPorts[i],
                                                           SetPaletteTest_readPortsCount[i],
                                                           SetPaletteTest_modifiedIndexedPorts[i],
                                                           SetPaletteTest_modifiedIndexedPortsCount[i],
                                                           NULL,
                                                           0,
                                                           SetPaletteTest_ignorePorts,
                                                           SetPaletteTest_ignorePortsCount,
                                                           SetPaletteTest_ignoreIndexedPorts,
                                                           SetPaletteTest_ignoreIndexedPortsCount);

        //printf("\n%i: 0x%02X instance 1\n", i, mode);
        {
            Hag::Testing::Mock::SelectInstance(1);
            if (!Clean::UnlockExtendedCRTRegistersSafe())
                continue;
            
            uint32_t offset = 0x00;
            if (mode > 0x13 && (Hag::S3::TrioBase::FindVideoModeData(mode) == NULL))
                continue; //Failure.

            //Reset clear screen on mode switch flag.
            Hag::System::BDA::VideoModeOptions::Get() &= 0x7f;

            Clean::ModeSetBDA(mode);

            if (mode <= 0x13 && !Clean::VerifyBDAOrDeactivate(mode))
                continue; //Failure.

            Hag::System::BDA::DisplayMode::Get() = mode;

            Hag::S3::VideoParameters* parameters = Clean::SetTextModeBiosData(mode);

            Hag::System::BDA::VideoModeOptions::Get() &= 
                        ~(Hag::System::BDA::VideoModeOptions::Unknown |
                        Hag::System::BDA::VideoModeOptions::Inactive);

            //SaveDynamicParameterData(overrideTable);

            Clean::ApplyVideoParameters(parameters);

            Clean::ApplyVESAOverrideData(mode);

            Hag::Testing::Mock::Snapshot();

            Clean::SetPalette();
        }

        ret -= Hag::Testing::Mock::VerifyPortsAndValues(1, NULL,
                                                           0,
                                                           SetPaletteTest_readPorts[i],
                                                           SetPaletteTest_readPortsCount[i],
                                                           SetPaletteTest_modifiedIndexedPorts[i],
                                                           SetPaletteTest_modifiedIndexedPortsCount[i],
                                                           NULL,
                                                           0,
                                                           SetPaletteTest_ignorePorts,
                                                           SetPaletteTest_ignorePortsCount,
                                                           SetPaletteTest_ignoreIndexedPorts,
                                                           SetPaletteTest_ignoreIndexedPortsCount);

        Hag::Testing::Mock::Reset();
    }

    return ret + Hag::Testing::Mock::Shutdown();   
}

struct SFT_VerifyCallback
{
    Hag::Testing::Mock::MemoryAccess* MemoryAccesses;
    uint32_t MemoryAccessesCount;
    int Verified;
};

void SetFontTest_VerifyCallback(uint32_t offset, uint32_t size, void* ctx)
{
    SFT_VerifyCallback& context = *(SFT_VerifyCallback*)ctx;
    for (uint32_t i = 0; i < context.MemoryAccessesCount; ++i)
    {
        if (context.MemoryAccesses[i].Offset == offset &&
            context.MemoryAccesses[i].Size == size)
        {
            ++context.Verified;
            return;
        }
    }
    printf("Unknown memory access offset 0x%08X, size 0x%08X\n", offset, size);
}

extern Hag::Testing::Mock::MemoryAccess SetFontTest_memoryAccess0[];
extern int SetFontTest_memoryAccessCount0;
extern Hag::Testing::Mock::MemoryAccess SetFontTest_memoryAccess1[];
extern int SetFontTest_memoryAccessCount1;
extern Hag::Testing::Mock::PortAndIndex* SetFontTest_readIndexedPorts[];
extern int SetFontTest_readIndexedPortsCount[];
extern Hag::Testing::Mock::PortAndIndexAndValue* SetFontTest_modifiedIndexedPorts[];
extern int SetFontTest_modifiedIndexedPortsCount[];

extern uint16_t SetFontTest_ignorePorts[];
extern uint16_t SetFontTest_ignorePortsCount;
extern Hag::Testing::Mock::PortAndIndex SetFontTest_ignoreIndexedPorts[];
extern uint16_t SetFontTest_ignoreIndexedPortsCount;

int SetFontTest()
{
    int ret = 242;

    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator, SetPaletteTest_RAMDACRegisters);
    int idx1 = 0;
    int idx2 = 0;
    for (uint8_t i = 0; i < modesCount; ++i)
    {
        uint8_t mode = modes[i];
        //The following code replicates parts of the SetVideoMode function
        
        {
            Hag::Testing::Mock::SelectInstance(0);
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

            ApplyVESAOverrideData(mode, overrideTable, modeDataIndex);

            SetPalette();

            uint16_t crtReg = Hag::System::BDA::VideoBaseIOPort::Get();

            uint8_t flags = 0;
            if ((mode < 0x04) ||
                (mode == 0x07) ||
                (mode > 0x13 && GetVideoModeFlags(mode, flags) && ((flags & 0x01) == 0x01)))
            {
                Hag::Testing::Mock::Snapshot();

                //printf("\n%i: 0x%02X instance 0\n", idx, mode);

                SFT_VerifyCallback context =
                {
                    mode == 0x54 ? SetFontTest_memoryAccess1 : SetFontTest_memoryAccess0,
                    mode == 0x54 ? SetFontTest_memoryAccessCount1 : SetFontTest_memoryAccessCount0,
                    0
                };

                Hag::Testing::Mock::SetMemoryAccessCallback(SetFontTest_VerifyCallback, &context);
                SetFont();
                Hag::Testing::Mock::SetMemoryAccessCallback(NULL, NULL);
                ret -= context.Verified;

                ret -= Hag::Testing::Mock::VerifyPortsAndValues(0, NULL,
                                                                0,
                                                                NULL,
                                                                0,
                                                                SetFontTest_modifiedIndexedPorts[idx1],
                                                                SetFontTest_modifiedIndexedPortsCount[idx1],
                                                                SetFontTest_readIndexedPorts[idx1],
                                                                SetFontTest_readIndexedPortsCount[idx1],
                                                                SetFontTest_ignorePorts,
                                                                SetFontTest_ignorePortsCount,
                                                                SetFontTest_ignoreIndexedPorts,
                                                                SetFontTest_ignoreIndexedPortsCount);
                ++idx1;
            }
        }

        {
            Hag::Testing::Mock::SelectInstance(1);
            if (!Clean::UnlockExtendedCRTRegistersSafe())
                continue;
            
            uint32_t offset = 0x00;
            if (mode > 0x13 && (Hag::S3::TrioBase::FindVideoModeData(mode) == NULL))
                continue; //Failure.

            //Reset clear screen on mode switch flag.
            Hag::System::BDA::VideoModeOptions::Get() &= 0x7f;

            Clean::ModeSetBDA(mode);

            if (mode <= 0x13 && !Clean::VerifyBDAOrDeactivate(mode))
                continue; //Failure.

            Hag::System::BDA::DisplayMode::Get() = mode;

            Hag::S3::VideoParameters* parameters = Clean::SetTextModeBiosData(mode);

            Hag::System::BDA::VideoModeOptions::Get() &= 
                        ~(Hag::System::BDA::VideoModeOptions::Unknown |
                        Hag::System::BDA::VideoModeOptions::Inactive);

            //SaveDynamicParameterData(overrideTable);

            Clean::ApplyVideoParameters(parameters);

            Clean::ApplyVESAOverrideData(mode);

            Hag::Testing::Mock::Snapshot();

            Clean::SetPalette();

            uint16_t crtReg = Hag::System::BDA::VideoBaseIOPort::Get();

            uint8_t flags = 0;
            if ((mode < 0x04) ||
                (mode == 0x07) ||
                (mode > 0x13 && GetVideoModeFlags(mode, flags) && ((flags & 0x01) == 0x01)))
            {
                Hag::Testing::Mock::Snapshot();

                //printf("\n%i: 0x%02X instance 1\n", idx, mode);

                SFT_VerifyCallback context =
                {
                    mode == 0x54 ? SetFontTest_memoryAccess1 : SetFontTest_memoryAccess0,
                    mode == 0x54 ? SetFontTest_memoryAccessCount1 : SetFontTest_memoryAccessCount0,
                    0
                };

                Hag::Testing::Mock::SetMemoryAccessCallback(SetFontTest_VerifyCallback, &context);
                Clean::SetFont();
                Hag::Testing::Mock::SetMemoryAccessCallback(NULL, NULL);
                ret -= context.Verified;

                ret -= Hag::Testing::Mock::VerifyPortsAndValues(1, NULL,
                                                                0,
                                                                NULL,
                                                                0,
                                                                SetFontTest_modifiedIndexedPorts[idx2],
                                                                SetFontTest_modifiedIndexedPortsCount[idx2],
                                                                SetFontTest_readIndexedPorts[idx2],
                                                                SetFontTest_readIndexedPortsCount[idx2],
                                                                SetFontTest_ignorePorts,
                                                                SetFontTest_ignorePortsCount,
                                                                SetFontTest_ignoreIndexedPorts,
                                                                SetFontTest_ignoreIndexedPortsCount);
                ++idx2;
            }
        }

        Hag::Testing::Mock::Reset();
    }

    return ret + Hag::Testing::Mock::Shutdown();
}

void SetVideoModeInternalsTest_callback(uint32_t offset, uint32_t size, void* ctx)
{
    //printf("Memory access offset: 0x%08X, size: 0x%08X\n", offset, size);
}

int SetVideoModeInternalsTest()
{
    int ret = 0;

    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator, SetPaletteTest_RAMDACRegisters);
    for (uint8_t i = 0; i < modesCount; ++i)
    {
        uint8_t mode = modes[i];
        //printf("\n%i: 0x%02X\n", i, mode);

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

        SaveDynamicParameterData(overrideTable);
        ApplyVideoParameters(overrideTable);
        ApplyVESAOverrideData(mode, overrideTable, modeDataIndex);
        SetPalette();

        uint16_t crtReg = Hag::System::BDA::VideoBaseIOPort::Get();

        uint8_t flags = 0;
        if ((mode < 0x04) ||
            (mode == 0x07) ||
            (mode > 0x13 && GetVideoModeFlags(mode, flags) && ((flags & 0x01) == 0x01)))
        {
            SetFont();

            uint8_t* fontDefinition = NULL;
            if (FetchCheckedVideoParameterBlockElement(8, 11, fontDefinition, 0x0B + 0x14))
            {
                ConfigureFontAndCursor(fontDefinition);
            }

            uint8_t* paramBlock = NULL;
            if (GetVideoParameterBlockElement(0x0010, paramBlock, 0x20) &&
                !((FARPointer*)(paramBlock + 0x06))->IsNull())
            {

                fontDefinition = ((FARPointer*)(paramBlock + 0x06))->ToPointer<uint8_t>(0x0B + 0x14);//there's a 0xFF terminated list of modes. max is 14h
                if (CheckCurrentModeExists(fontDefinition, 7))
                    ConfigureFontRamBank(fontDefinition);
            }
        }
        else
        {
            Hag::System::BDA::CursorScanLines::Get() = Hag::System::BDA::CursorScanLines_t(0x00, 0x00);
            
            uint8_t* graphicsCharacterFontDefinition = NULL;
            if (FetchCheckedVideoParameterBlockElement(12, 7, graphicsCharacterFontDefinition, 0x07 + 0x14))
                SetGraphicsCharacterFont(graphicsCharacterFontDefinition); //Sets the pointer in the interrupt table.
        }
        Hag::Testing::Mock::Snapshot();

        //Hag::Testing::Mock::SetMemoryAccessCallback(SetVideoModeInternalsTest_callback, NULL);

        if (((Hag::System::BDA::VideoModeOptions::Get() & 0x80) == 0x00) &&
            (Hag::System::BDA::VideoBufferSize::Get() != 0x0000))
            ClearScreen();

        SetPaletteProfile();
        EnablePaletteBasedVideo();
        TurnOnScreen();
        
        //Hag::Testing::Mock::SetMemoryAccessCallback(NULL, NULL);

/*
        ret -= context.Verified;

        ret -= Hag::Testing::Mock::VerifyPortsAndValues(0, NULL,
                                                        0,
                                                        NULL,
                                                        0,
                                                        SetFontTest_modifiedIndexedPorts[idx],
                                                        SetFontTest_modifiedIndexedPortsCount[idx],
                                                        SetFontTest_readIndexedPorts[idx],
                                                        SetFontTest_readIndexedPortsCount[idx],
                                                        SetFontTest_ignorePorts,
                                                        SetFontTest_ignorePortsCount,
                                                        SetFontTest_ignoreIndexedPorts,
                                                        SetFontTest_ignoreIndexedPortsCount);
*/
        Hag::Testing::Mock::Reset();
    }

    return ret + Hag::Testing::Mock::Shutdown();
}

int SetVideoModeTest()
{
    int ret = 0;

    Support::Allocator allocator;
    S3Trio64MockConfigSetup(allocator, SetPaletteTest_RAMDACRegisters);
    for (uint8_t i = 0; i < modesCount; ++i)
    {
        uint8_t mode = modes[i];
        //printf("\n%i: 0x%02X\n", i, mode);

        SetVideoMode(mode);
/*
        ret -= context.Verified;

        ret -= Hag::Testing::Mock::VerifyPortsAndValues(0, NULL,
                                                        0,
                                                        NULL,
                                                        0,
                                                        SetFontTest_modifiedIndexedPorts[idx],
                                                        SetFontTest_modifiedIndexedPortsCount[idx],
                                                        SetFontTest_readIndexedPorts[idx],
                                                        SetFontTest_readIndexedPortsCount[idx],
                                                        SetFontTest_ignorePorts,
                                                        SetFontTest_ignorePortsCount,
                                                        SetFontTest_ignoreIndexedPorts,
                                                        SetFontTest_ignoreIndexedPortsCount);
*/
        Hag::Testing::Mock::Reset();
    }

    return ret + Hag::Testing::Mock::Shutdown();
}

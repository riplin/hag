#include <stdio.h>
#include <string.h>
#include "allhdrs.h"

int MockSetupTeardownTest();
int SquenceRegisterLockingTest();
int CRTCIndexRegisterTest();
int FindVideoModeDataTest();
int GetVideoModeFlagsTest();
int GetVESAVideoModeOverrideTableTest();
int ModeSetBDATest();
int VerifyBDAOrDeactivateTest();
int SetTextModeBiosDataTest();
int ApplyVideoParametersTest();
int FetchBusSpecificSystemConfigTest();
int InitializeCRTControllerAndSequencerTest();
int SetupClocksTest();
int ConfigureExtraVESAModeSettingsTest();
int EnableOver256KAddressingAndSetAddressWindowTest();
int SetColorModeTest();
int ClearMemoryTest();
int ApplyVESAOverrideDataTest();
int SetPaletteTest();
int SetFontTest();
int SetVideoModeInternalsTest();
int SetVideoModeTest();

#define TESTFUNCTION(F)                             \
{                                                   \
    int l = strlen(#F);                             \
    printf("RUNNING [");                            \
    printf(#F);                                     \
    printf("]");                                    \
    for (int i = 0; i < 79 - (l + 10); ++i)         \
        printf("-");                                \
    printf("\n");                                   \
    int fret = F();                                 \
    printf(fret != 0 ? "FAILED  [" : "PASSED  [");  \
    printf(#F);                                     \
    printf("]");                                    \
    if (fret != 0)                                  \
    {                                               \
        printf("[%02i]", fret);                     \
    }                                               \
    else                                            \
    {                                               \
        printf("----");                             \
    }                                               \
    for (int i = 0; i < 79 - (l + 14); ++i)         \
        printf("-");                                \
    printf("\n");                                   \
    ret += fret;                                    \
}

int main(void)
{
    int ret = 0;
    TESTFUNCTION(MockSetupTeardownTest);
    TESTFUNCTION(SquenceRegisterLockingTest);
    TESTFUNCTION(CRTCIndexRegisterTest);
    TESTFUNCTION(FindVideoModeDataTest);
    TESTFUNCTION(GetVideoModeFlagsTest);
    TESTFUNCTION(GetVESAVideoModeOverrideTableTest);
    TESTFUNCTION(ModeSetBDATest);
    TESTFUNCTION(VerifyBDAOrDeactivateTest);
    TESTFUNCTION(SetTextModeBiosDataTest);
    TESTFUNCTION(ApplyVideoParametersTest);
    TESTFUNCTION(FetchBusSpecificSystemConfigTest);
    TESTFUNCTION(InitializeCRTControllerAndSequencerTest);
    TESTFUNCTION(SetupClocksTest);
    TESTFUNCTION(ConfigureExtraVESAModeSettingsTest);
    TESTFUNCTION(EnableOver256KAddressingAndSetAddressWindowTest);
    TESTFUNCTION(SetColorModeTest);
    TESTFUNCTION(ClearMemoryTest);
    TESTFUNCTION(ApplyVESAOverrideDataTest);
    TESTFUNCTION(SetPaletteTest);
    TESTFUNCTION(SetFontTest);
    TESTFUNCTION(SetVideoModeInternalsTest);
    TESTFUNCTION(SetVideoModeTest);

    return ret;
}

//Copyright 2023-Present riplin

#include <stdio.h>
#include <hag/drivers/s3/trio64/trio64.h>
#include <hag/drivers/vga/funcs.h>
#include <has/system/sysasm.h>

Hag::VGA::Register_t Registers[] =
{
    Hag::S3::Trio64::Register::InputStatus0,
    Hag::S3::Trio64::Register::FeatureControlR,
    Hag::S3::Trio64::Register::MiscellaneousR,
    Hag::S3::Trio64::Register::InputStatus1B,
    Hag::S3::Trio64::Register::InputStatus1D,
    0x3C3,//Hag::VGA::Register::VideoSubsystemEnable
    Hag::S3::Trio64::Register::DACMask,
    Hag::S3::Trio64::Register::DACStatus,
    Hag::S3::Trio64::Register::SetupOptionSelect,
    Hag::S3::Trio64::Register::VideoSubsystemEnable,
    Hag::S3::Trio64::Register::SubsystemStatus,
    Hag::S3::Trio64::Register::AdvancedFunctionControl,
    Hag::S3::Trio64::Register::GraphicsProcessorStatus,
};

//CRTControllerIndexD           //256 regs
//CRTControllerDataD

int main(void)
{
    using namespace Hag::S3::Trio64;
    FILE* fp = fopen("s3regs.txt", "w");

    //Write code to CR38 to provide access to the S3 VGA registers (CR30-CR3F)
    CRTController::RegisterLock1::SoftUnlock rl1(Hag::VGA::GetCRTControllerIndexRegister());
    
    //Write code to CR39 to provide access to the System Control and System Extension registers (CR40-CRFF)
    CRTController::RegisterLock2::SoftUnlock rl2(Hag::VGA::GetCRTControllerIndexRegister());

    //Set bit 0 in CR40 to enable access to the Enhanced Commands registers.
    CRTController::SystemConfiguration::SoftUnlock sc(Hag::VGA::GetCRTControllerIndexRegister());

    //Enable write access to bits 1 and 6 of CR7
    //Enable access to RAMDAC register
    //Enable access to Palette/Overscan registers
    CRTController::BackwardCompatibility2::SoftUnlock bc2(Hag::VGA::GetCRTControllerIndexRegister());

    //Enable write access to CR0-CR6, CR7 (bits 7,5,3,2,0), CR9 (bit5), CR10, CR11 (bits 3-0), CR15-CR16, CR17 (bit 2)
    CRTController::CRTRegisterLock::SoftUnlock crl(Hag::VGA::GetCRTControllerIndexRegister());

    //Enable write access to CR0-CR7
    CRTController::VerticalRetraceEnd::SoftUnlock vre(Hag::VGA::GetCRTControllerIndexRegister());

    //write code to SR8 to provide access to SR9-SR18.
    Sequencer::UnlockExtendedSequencer::SoftUnlock ues;

    fprintf(fp, "VGA and S3 registers:\n");
    for (uint32_t i = 0; i < sizeof(Registers) / sizeof(Hag::VGA::Register_t); ++i)
    {
        Hag::VGA::Register_t reg = Registers[i];
        uint8_t value = SYS_ReadPortByte(reg);
        fprintf(fp, "{ 0x%04X, 0x%02X },\n", reg, value);
    }

    fprintf(fp, "\nAttribute Controller registers:\n");
    SYS_ReadPortByte(Hag::VGA::Register::InputStatus1D);//Reset attribute controller to index register.
    uint8_t orgAttribIdx = SYS_ReadPortByte(Hag::VGA::Register::AttributeControllerIndex);
    for (uint8_t i = 0; i < 32; ++i)
    {
        uint8_t idx = (orgAttribIdx & 0xE0) | i;
        SYS_ReadPortByte(Hag::VGA::Register::InputStatus1D);//Reset attribute controller to index register.
        SYS_WritePortByte(Hag::VGA::Register::AttributeControllerIndex, idx);
        uint8_t value = SYS_ReadPortByte(Hag::VGA::Register::AttributeControllerDataR);
        fprintf(fp, "{ 0x%02X, 0x%02X },\n", i, value);
    }
    SYS_ReadPortByte(Hag::VGA::Register::InputStatus1D);//Reset attribute controller to index register.
    SYS_WritePortByte(Hag::VGA::Register::AttributeControllerIndex, orgAttribIdx);

    fprintf(fp, "\nRAMDAC registers:\n");
    SYS_WritePortByte(Hag::VGA::Register::DACReadIndex, 0x00);
    for (uint16_t i = 0; i < 256; ++i)
    {
        uint8_t red = SYS_ReadPortByte(Hag::VGA::Register::RAMDACData);
        uint8_t green = SYS_ReadPortByte(Hag::VGA::Register::RAMDACData);
        uint8_t blue = SYS_ReadPortByte(Hag::VGA::Register::RAMDACData);
        fprintf(fp, "{ 0x%02X, 0x%02X, 0x%02X }, //%i\n", red, green, blue, i);
    }

    fprintf(fp, "\nSequencer Controller registers:\n");
    for (uint8_t i = 0; i < 32; ++i)
    {
        SYS_WritePortByte(Hag::VGA::Register::SequencerIndex, i);
        uint8_t value = SYS_ReadPortByte(Hag::VGA::Register::SequencerData);
        fprintf(fp, "{ 0x%02X, 0x%02X },\n", i, value);
    }

    fprintf(fp, "\nGraphics Controller registers:\n");
    for (uint8_t i = 0; i < 16; ++i)
    {
        SYS_WritePortByte(Hag::VGA::Register::GraphicsControllerIndex, i);
        uint8_t value = SYS_ReadPortByte(Hag::VGA::Register::GraphicsControllerData);
        fprintf(fp, "{ 0x%02X, 0x%02X },\n", i, value);
    }

    fprintf(fp, "\nCRT Controller registers:\n");
    for (uint16_t i = 0; i < 256; ++i)
    {
        SYS_WritePortByte(Hag::VGA::Register::CRTControllerIndexD, uint8_t(i));
        uint8_t value = SYS_ReadPortByte(Hag::VGA::Register::CRTControllerDataD);
        fprintf(fp, "{ 0x%02X, 0x%02X },\n", uint8_t(i), value);
    }

    fclose(fp);
    return 0;
}
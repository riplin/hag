//Copyright 2023-Present riplin

#include <stdio.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>
#include <hag/drivers/s3/crtc/reglock1.h>
#include <hag/drivers/s3/crtc/reglock2.h>
#include <hag/drivers/s3/crtc/sysconf.h>
#include <hag/drivers/s3/crtc/bkwcomp2.h>
#include <hag/drivers/s3/crtc/crtreglk.h>
#include <hag/drivers/vga/crtc/verrtcen.h>
#include <hag/drivers/s3/sqrc/unlexseq.h>

Hag::VGA::Register_t Registers[] =
{
    Hag::VGA::Register::InputStatus0,
    Hag::VGA::Register::FeatureControlR,
    Hag::VGA::Register::MiscellaneousR,
    Hag::VGA::Register::InputStatus1D,
    Hag::VGA::Register::VideoSubsystemEnable,
    Hag::VGA::Register::DACMask,
    Hag::VGA::Register::DACStatus,
    Hag::S3::Register::SetupOptionSelect,
    Hag::S3::Register::VideoSubsystemEnableS3,
    Hag::S3::Register::SubsystemStatus,
    Hag::S3::Register::AdvancedFunctionControl,
    Hag::S3::Register::CurrentYPosition,
    Hag::S3::Register::CurrentYPosition2,
    Hag::S3::Register::CurrentXPosition,
    Hag::S3::Register::CurrentXPosition2,
    Hag::S3::Register::DestinationYPositionAxialStepConstant,
    Hag::S3::Register::YCoordinate2AxialStepConstant2,
    Hag::S3::Register::DestinationXPositionDiagonalStepConstant,
    Hag::S3::Register::XCoordinate2,
    Hag::S3::Register::LineErrorTerm,
    Hag::S3::Register::LineErrorTerm2,
    Hag::S3::Register::MajorAxisPixelCount,
    Hag::S3::Register::MajorAxisPixelCount2,
    Hag::S3::Register::GraphicsProcessorStatus,
    Hag::S3::Register::DrawingCommand,
    Hag::S3::Register::DrawingCommand2,
    Hag::S3::Register::ShortStrokeVectorTransfer,
    Hag::S3::Register::BackgroundColor,
    Hag::S3::Register::ForegroundColor,
    Hag::S3::Register::BitplaneWriteMask,
    Hag::S3::Register::BitplaneReadMask,
    Hag::S3::Register::ColorCompareRegister,
    Hag::S3::Register::BackgroundMix,
    Hag::S3::Register::ForegroundMix,
    Hag::S3::Register::ReadRegisterData,
    Hag::S3::Register::WriteRegisterData,
    Hag::S3::Register::PixelDataTransfer,
    Hag::S3::Register::PixelDataTransferExtension,
    Hag::S3::Register::PatternY,
    Hag::S3::Register::PatternX
};

//CRTControllerIndexD           //256 regs
//CRTControllerDataD

int main(void)
{
    FILE* fp = fopen("s3regs.txt", "w");

    //Write code to CR38 to provide access to the S3 VGA registers (CR30-CR3F)
    Hag::S3::CRTController::RegisterLock1::SoftUnlock<Hag::VGA::Register::CRTControllerIndexD> rl1;
    
    //Write code to CR39 to provide access to the System Control and System Extension registers (CR40-CRFF)
    Hag::S3::CRTController::RegisterLock2::SoftUnlock<Hag::VGA::Register::CRTControllerIndexD> rl2;

    //Set bit 0 in CR40 to enable access to the Enhanced Commands registers.
    Hag::S3::CRTController::SystemConfiguration::SoftUnlock<Hag::VGA::Register::CRTControllerIndexD> sc;

    //Enable write access to bits 1 and 6 of CR7
    //Enable access to RAMDAC register
    //Enable access to Palette/Overscan registers
    Hag::S3::CRTController::BackwardCompatibility2::SoftUnlock<Hag::VGA::Register::CRTControllerIndexD> bc2;

    //Enable write access to CR0-CR6, CR7 (bits 7,5,3,2,0), CR9 (bit5), CR10, CR11 (bits 3-0), CR15-CR16, CR17 (bit 2)
    Hag::S3::CRTController::CRTRegisterLock::SoftUnlock<Hag::VGA::Register::CRTControllerIndexD> crl;

    //Enable write access to CR0-CR7
    Hag::VGA::CRTController::VerticalRetraceEnd::SoftUnlock<Hag::VGA::Register::CRTControllerIndexD> vre;

    //write code to SR8 to provide access to SR9-SR18.
    Hag::S3::Sequencer::UnlockExtendedSequencer::SoftUnlock ues;

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
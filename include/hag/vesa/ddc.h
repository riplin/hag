//Copyright 2023-Present riplin

//https://en.wikipedia.org/wiki/Extended_Display_Identification_Data

#pragma once

#include <hag/types.h>

#pragma pack(push, 1);

namespace Hag { namespace Vesa { namespace DDC
{
    
    typedef uint8_t VideoInputParameters_t;
    namespace VideoInputParameters
    {
        enum
        {
            DigitalInput = 0x80,                    //bit 7
            //If DigitalInput is 1
            DigitalBitDepthMask = 0x70,             //bits 6-4
            DigitalBitDepthUndefined = 0x00,        // 000
            DigitalBitDepth6 = 0x10,                // 001
            DigitalBitDepth8 = 0x20,                // 010
            DigitalBitDepth10 = 0x30,               // 011
            DigitalBitDepth12 = 0x40,               // 100
            DigitalBitDepth14 = 0x50,               // 101
            DigitalBitDepth16 = 0x60,               // 110
            DigitalBitDepthReserved = 0x70,         // 111
            DigitalInterfaceMask = 0x0f,            // bits 3-0
            DigitalInterfaceUndefined = 0x00,       // 0000
            DigitalInterfaceDVI = 0x01,             // 0001
            DigitalInterfaceHDMIa = 0x02,           // 0010
            DigitalInterfaceHDMIb = 0x03,           // 0011
            DigitalInterfaceMDDI = 0x04,            // 0100
            DigitalInterfaceDisplayPort = 0x05,     // 0101
            //If DigitalInput is 0 - Analog
            VideoWhiteAndSyncLevelsMask = 0x60,     // bits 6-5
            VideoLevel07_03 = 0x00,                 // 00 - +0.7/-0.3 V
            VideoLevel0714_0286 = 0x20,             // 01 - +0.714/-0.286 V
            VideoLevel10_04 = 0x40,                 // 10 - +1.0/-0.4 V
            VideoLevel07_0 = 0x60,                  // 11 - +0.7/0 V
            BlankToBlackPedestalExpected = 0x10,    // 
            SeparateSyncSupported = 0x08,           //
            CompositeSyncOnHSyncSupported = 0x04,   //
            SyncOnGreenSupported = 0x02,            //
            SerratedVSyncPulseSyncOnGreen = 0x01    //VSync pulse must be serrated when composite or sync-on-green is used. 
        };
    }

    typedef uint8_t SupportedFeatures_t;
    namespace SupportedFeatures
    {
        enum
        {
            DpmsStandbySupported = 0x80,
            DpmsSuspendSupported = 0x40,
            DpmsActiveOffSupported = 0x20,
            DisplayTypeMask = 0x18,                             // bits 4-3
            DigitalDisplayTypeRGB444 = 0x00,                    // 00 = RGB 4:4:4
            DigitalDisplayTypeRGB444YCrCb444 = 0x08,            // 01 = RGB 4:4:4 + YCrCb 4:4:4
            DigitalDisplayTypeRGB444YCrCb422 = 0x10,            // 10 = RGB 4:4:4 + YCrCb 4:2:2
            DigitalDisplayTypeRGB444YCrCb444YCrCb422 = 0x18,    // 11 = RGB 4:4:4 + YCrCb 4:4:4 + YCrCb 4:2:2
            AnalogDisplayTypeMonochrome = 0x00,                 // 00 = Monochrome or Grayscale
            AnalogDisplayTypeRGBColor = 0x08,                   // 01 = RGB Color
            AnalogDisplayTypeNonRGBColor = 0x10,                // 10 = Non-RGB Color
            AnalogDisplayTypeUndefined = 0x18,                  // 11 = Undefined
            StandardsRGBColorSpace = 0x04,                      // Standard sRGB colour space. Bytes 25–34 must contain sRGB standard values. 
            PreferredTimingMode = 0x02,                         // Preferred timing mode specified in descriptor block 1.
                                                                // For EDID 1.3+ the preferred timing mode is always in the first Detailed Timing Descriptor.
                                                                // In that case, this bit specifies whether the preferred timing mode includes
                                                                // native pixel format and refresh rate.
            ContinuousTimings = 0x01                            // Continuous timings with GTF or CVT
        };
    }

    typedef uint8_t EstablishedTimings1_t;
    namespace EstablishedTimings1
    {
        enum
        {
            P720x400x70 = 0x80,
            P720x400x88 = 0x40,
            P640x480x60 = 0x20,
            P640x480x67 = 0x10,
            P640x480x72 = 0x08,
            P640x480x75 = 0x04,
            P800x600x56 = 0x02,
            P800x600x60 = 0x01
        };
    }
    typedef uint8_t EstablishedTimings2_t;
    namespace EstablishedTimings2
    {
        enum
        {
            P800x600x72 = 0x80,
            P800x600x75 = 0x40,
            P832x624x75 = 0x20,
            I1024x768x87 = 0x10,
            P1024x768x60 = 0x08,
            P1024x768x70 = 0x04,
            P1024x768x75 = 0x02,
            P1280x1024x75 = 0x01
        };
    }

    typedef uint8_t EstablishedTimings3_t;
    namespace EstablishedTimings3
    {
        enum
        {
            P1152x870x75 = 0x80,
            //The rest are manufacturer specific
        };
    }

    typedef uint8_t AspectRatioYFrequency_t;
    namespace AspectRatioYFrequency
    {
        enum
        {
            AspectRatioMask = 0xc0,         //
            Aspect1x1 = 0x00,               // 00 1:1 - prior to 1.3
            Aspect16x10 = 0x00,             // 00 16:10 - 1.3 and over
            Aspect4x3 = 0x40,               // 01 4:3
            Aspect5x4 = 0x80,               // 10 5:4
            Aspect16x9 = 0xc0,              // 11 16:9
            VerticalFrequencyMask = 0x3f    // Vertical frequency, datavalue + 60 (60–123 Hz)
        };
    }

    struct StandardTimingInformation
    {
        uint8_t m_XResolution; //0x00 = reserved; otherwise, (datavalue + 31) × 8 (256–2288 pixels). 
        AspectRatioYFrequency_t m_AspectRatioYFrequency;
    };

    typedef uint8_t DetailedTimingDescriptorFeatures_t;
    namespace DetailedTiminDescriptorFeatures
    {
        enum
        {
            SignalInterfaceInterlaced = 0x80,           // bit 7 = 1 - Interlaced, 0 - Non-interlaced
            StereoModeMask = 0x61,                      // bits 6-5,0
            StereoModeNone1 = 0x00,                     // 00 x none, bit 0 is "don't care"
            StereoModeNone2 = 0x01,                     // 00 x none, bit 0 is "don't care"
            StereoModeFieldSequentialRight = 0x20,      // 01 0 field sequential, right during stereo sync
            StereoModeFieldSequentialLeft = 0x40,       // 10 0 field sequential, left during stereo sync
            StereoMode2WayInterleavedRightEven = 0x21,  // 01 1 2-way interleaved, right image on even lines
            StereoMode2WayInterleavedLeftEven = 0x41,   // 10 1 2-way interleaved, left image on even lines
            StereoMode4WayInterleaved = 0x60,           // 11 0 4-way interleaved
            StereoModeSideBySideInterleaved = 0x61,     // 11 1 Side-by-side interleaved
            AnalogSyncMask = 0x10,                      // bit 4 = 0 - Analog Sync
            AnalogSync = 0x00,                          // And with AnalogSyncMask, then compare to this. If equal, the following bits apply.
            AnalogSyncComposite = 0x08,                 // bit 3 = 0 - Analog Composite, 1 - Bipolar Analog Composite
            AnalogSerration = 0x04,                     // bit 2 = 0 - Without Serration, 1 - With Serrations (H-sync during V-sync)
            AnalogSyncOnAllColors = 0x02,               // bit 1 = 0 - Sync on green only, 1 - Sync on all three (RGB) video signals
            DigitalSyncMask = 0x18,                     // bits 4-3 = 10
            DigitalSync = 0x10,                         // And with DigitalSyncMask, then compare to this. If equal, the following bits apply.
            DigitalSerration = 0x04,                    // bit 2 = 0 - without serration, 1 - with serration (H-sync during V-sync)
            DigitalHSyncPolarity = 0x02,                // bit 1 = 0 - Negative, 1 - Positive.
            DigitalSyncSeparateMask = 0x18,             // bits 4-3 = 11
            DigitalSyncSeparate = 0x18,                 // And with DigitalSyncSeparateMask, then compare to this. If equal, the following bits apply.
            DigitalVSyncPolarity = 0x04,                // bit 2 = 0 - Negative, 1 - Positive.
            DigitalHSyncPolarity2 = 0x02,               // bit 1 = 0 - Negative, 1 - Positive. - Same as DigitalSync above.
        };
    }

    struct DetailedTimingDescriptor
    {
        uint16_t m_PixelClock;                          //00 = reserved; otherwise in 10 kHz units (0.01–655.35 MHz, little-endian). 
        uint8_t m_HorizontalActivePixelsLsb;            //lowest 8 bits
        uint8_t m_HorizontalBlankingPixelsLsb;          //lowest 8 bits
        uint8_t m_HorizontalActiveBlankingMsb;          //bits 7-4 Horizontal active pixels msb, bits 3-0 Horizontal blanking pixels msb
        uint8_t m_VerticalAciveLinesLsb;                //lowest 8 bits
        uint8_t m_VerticalBlankingLinesLsb;             //lowest 8 bits
        uint8_t m_VerticalActiveBlankingLinesMsb;       //bits 7-4 Vertical active lines msb, bits 3-0 Vertical blanking lines msb
        uint8_t m_HorizontalFrontPorchLsb;              //Horizontal front porch (sync offset) pixels 8 lsbits (0–255) From blanking start 
        uint8_t m_HorizontalSyncPulseWidthPixelsLsb;    //lowest 8 bits
        uint8_t m_VerticalFrontPorchSyncPulseLsb;       //bits 7-4 Vertical front porch (sync offset) lines lsb, bits 3-0 Vertical sync pulse width lines lsb
        uint8_t m_HVFrontPorchSyncPulseMsb;             //bits 7-6 Horizontal front porch (sync offset) pixels msb
                                                        //bits 5-4 Horizontal sync pulse width pixels msb
                                                        //bits 3-2 Vertical front porch (sync offset) lines msb
                                                        //bits 1-0 Vertical sync pulse width lines msb
        uint8_t m_HorizontalImageSizeMMLsb;
        uint8_t m_VerticalImageSizeMMLsb;
        uint8_t m_HorizontalBorderPixels;               //One side, total is twice this
        uint8_t m_VerticalBorderLines;                  //One side, total is twice this
        DetailedTimingDescriptorFeatures_t m_Features;
    };

    typedef uint8_t DescriptorType_t;
    namespace DescriptorType
    {
        enum
        {
            DisplaySerialNumber = 0xff,                 //ASCII text
            UnspecifiedText = 0xfe,                     //ASCII text
            DisplayRangeLimits = 0xfd,                  //6- or 13-byte (with additional timing) binary descriptor
            DisplayName = 0xfc,                         //ASCII text
            AdditionalWhitePointData = 0xfb,            //2x 5-byte descriptors, padded with 0x0a, 0x20, 0x20
            AdditionalStandardTimingIdentifiers = 0xfa, //6x 2-byte descriptors, padded with 0x0a
            DisplayColorManagement = 0xf9,              //DCM
            CVT3ByteTimingCodes = 0xf8,                 //
            AdditionalStandardTiming3 = 0xf7,           //
            DummyIdentifier = 0x10,                     //
            //0x00 - 0x0F = Manufacturer reserved.
        };
    }

    struct ASCIIDisplayDescriptor
    {
        uint16_t m_DisplayDescriptor;                   //Should be 0 for valid DisplayDescriptor
        uint8_t m_Reserved1;                            //Should be 0
        DescriptorType_t m_DescriptorType;              //
        uint8_t m_Reserved2;
        char m_Text[13];
    };

    typedef ASCIIDisplayDescriptor SerialNumberDisplayDescriptor;//0xff
    typedef ASCIIDisplayDescriptor UnspecifiedTextDisplayDescriptor;//0xfe
    typedef ASCIIDisplayDescriptor DisplayNameDisplayDescriptor;//0xfc
    typedef ASCIIDisplayDescriptor DummyIdentifierDisplayDescriptor;//0x10


    typedef uint8_t RangeLimitsRatesOffsets_t;
    namespace RangeLimitsRatesOffsets
    {
        enum
        {
            HorizontalRateMaxPlus255 = 0x08,            // bit 3 = 1 - +255
            HorizontalRateMinPlus255 = 0x04,            // bit 2 = 1 - +255
            VerticalRateMaxPlus255 = 0x02,              // bit 1 = 1 - +255
            VerticalRateMinPlus255 = 0x01               // bit 0 = 1 - +255
        };
    }

    typedef uint8_t ExtendedTimingInformationType_t;
    namespace ExtendedTimingInformationType
    {
        enum
        {
            DefaultGTF = 0x00,                          //When Edid::m_SupportedFeatures::ContinuousTimings is 1
            NoTimingInformation = 0x01,
            SecondaryGTFSupported = 0x02,
            CVT = 0x04
        };
    }

    struct GTFSecondaryCurve
    {
        uint8_t m_Reserved;
        uint8_t m_StartFrequency;                       //divided by 2KHz (0-510KHz)
        uint8_t m_CValue;                               //multiplied by 2 (0-127.5)
        uint16_t m_MValue;                              //0-65535
        uint8_t m_KValue;                               //0-255
        uint8_t m_JValue;                               //multiplied by 2 (0-127.5)
    };

    typedef uint8_t CVTClocksAndPixels_t;
    namespace CVTClocksAndPixels
    {
        enum
        {
            AdditionalClockPrecisionMask = 0xfc,        //Additional clock precision in 0.25 MHz increments
                                                        //(to be subtracted from m_MaximumPixelClockRate maximum pixel clock rate)
            MaximumActivePixelsPerLineMsbMask = 0x03    //Maximum active pixels per line, 2-bit msb 
        };
    }

    typedef uint8_t CVTAspectRatios_t;
    namespace CVTAspectRatios
    {
        enum
        {
            Aspect4x3 = 0x80,
            Aspect16x9 = 0x40,
            Aspect16x10 = 0x20,
            Aspect5x4 = 0x10,
            Aspect15x9 = 0x08
        };
    }

    typedef uint8_t CVTFlags_t;
    namespace CVTFlags
    {
        enum
        {
            AspectRatioPreferenceMask = 0xe0,   //bits 7-5
            Aspect4x3 = 0x00,                   // 000 = 4:3
            Aspect16x9 = 0x20,                  // 001 = 16:9
            Aspect16x10 = 0x40,                 // 010 = 16:10
            Aspect5x4 = 0x60,                   // 011 = 5:4
            Aspect15x9 = 0x80,                  // 100 = 15:9
            ReducedBlankingPreferred = 0x10,    // CVT-RB reduced blanking (preferred)
            StandardBlanking = 0x08,            //
        };
    }

    typedef uint8_t CVTScaling_t;
    namespace CVTScaling
    {
        enum
        {
            HorizontalShrink = 0x80,
            HorizontalStretch = 0x40,
            VerticalShrink = 0x20,
            VerticalStretch = 0x10
        };
    }

    struct CVT
    {
        uint8_t m_Version;                              //bits 7-4 CVT major version (1-15)
                                                        //bits 3-0 CVT minor version (0-15)
        CVTClocksAndPixels_t m_ClockAdjustmentAndPixelMsb;
        uint8_t m_MaximumActivePixelsPerLineLsb;        //no limit if 0
        CVTAspectRatios_t m_AspectRatios;
        CVTFlags_t m_Flags;
        CVTScaling_t m_Scaling;
        uint8_t m_PreferredVerticalRefreshRate;         //1-255
    };

    struct DisplayRangeLimitsDescriptor
    {
        uint16_t m_DisplayDescriptor;                   //Should be 0 for valid DisplayDescriptor
        uint8_t m_Reserved;                             //Should be 0
        DescriptorType_t m_DescriptorType;              //Should be 0xfd
        RangeLimitsRatesOffsets_t m_RateLimitOffsets;
        uint8_t m_HorizontalMinimum;
        uint8_t m_HorizontalMaximum;
        uint8_t m_VerticalMinimum;
        uint8_t m_VerticalMaximum;
        uint8_t m_MaximumPixelClockRate;                //Rounded up to 10MHz multiple (10-2550 MHz)
        ExtendedTimingInformationType_t m_ExtendedTimingInformaiton;
        union
        {
            GTFSecondaryCurve m_GTFSecondaryCurve;
            CVT m_CVT;
        };
    };
    
    struct WhitePoint
    {
        uint8_t m_IndexNumber;                      //1-255. 0 indicates descriptor not used
        uint8_t m_PointXYLsb;                       //bits 3-2 x value lsb
                                                    //bits 1-0 y value lsb
        uint8_t m_pointXMsb;                        //
        uint8_t m_pointYMsb;
        uint8_t m_Value;                            // (gamma − 1)×100 (1.0–3.54, like Edid m_DisplayGamma) 
    };

    struct AdditionalWhitePointDescriptor
    {
        uint16_t m_DisplayDescriptor;               //Should be 0 for valid DisplayDescriptor
        uint8_t m_Reserved1;                        //Should be 0
        DescriptorType_t m_DescriptorType;          //Should be 0xfb
        uint8_t m_Reserved2;                        //Should be 0
        WhitePoint m_Points[2];
        uint8_t m_Padding[3];
    };

    struct ColorManagementDataDescriptor
    {
        uint16_t m_DisplayDescriptor;               //Should be 0 for valid DisplayDescriptor
        uint8_t m_Reserved1;                        //Should be 0
        DescriptorType_t m_DescriptorType;          //Should be 0xf9
        uint8_t m_Reserved2;                        //Should be 0
        uint8_t m_Version;                          //Should be 3
        uint8_t m_RedA3Lsb;
        uint8_t m_RedA3Msb;
        uint8_t m_RedA2Lsb;
        uint8_t m_RedA2Msb;
        uint8_t m_GreenA3Lsb;
        uint8_t m_GreenA3Msb;
        uint8_t m_GreenA2Lsb;
        uint8_t m_GreenA2Msb;
        uint8_t m_BlueA3Lsb;
        uint8_t m_BlueA3Msb;
        uint8_t m_BlueA2Lsb;
        uint8_t m_BlueA2Msb;
    };

    typedef uint8_t AspectAndLines_t;
    namespace AspectAndLines
    {
        enum
        {
            AddressableLinesPerFieldMsbMask = 0xf0,
            AspectRatioMask = 0x0c,
            AspectRatio4x3 = 0x00,
            AspectRatio16x9 = 0x04,
            AspectRatio16x10 = 0x08,
            AspectRatio15x9 = 0x0c
        };
    }

    typedef uint8_t Rates_t;
    namespace Rates
    {
        enum
        {
            PreferredRatesMask = 0x60,
            Preferred50Hz = 0x00,
            Preferred60Hz = 0x20,
            Preferred75Hz = 0x40,
            Preferred85Hz = 0x80,
            VerticalRate50HzCVT = 0x10,
            VerticalRate60HzCVT = 0x08,
            VerticalRate75HzCVT = 0x04,
            VerticalRate85HzCVT = 0x02,
            VerticalRate60HzCVTReducedBlanking = 0x01
        };
    }

    struct CVTTiming
    {
        uint8_t m_AddressableLinesPerFieldLsb;
        AspectAndLines_t m_AspectRatioAndAddressableLinesMsb;
        Rates_t m_Rates;
    };

    struct CVTTimingCodeDescriptor
    {
        uint16_t m_DisplayDescriptor;               //Should be 0 for valid DisplayDescriptor
        uint8_t m_Reserved1;                        //Should be 0
        DescriptorType_t m_DescriptorType;          //Should be 0xf8
        uint8_t m_Reserved2;                        //Should be 0
        uint8_t m_Version;                          //Should be 1
        CVTTiming m_TimingDescriptors[4];
    };

    typedef uint8_t AdditionalStandardTimings1_t;
    namespace AdditionalStandardTimings1
    {
        enum
        {
            P640x350x85 = 0x80,
            P640x400x85 = 0x40,
            P720x400x85 = 0x20,
            P640x480x85 = 0x10,
            P848x480x60 = 0x08,
            P800x600x85 = 0x04,
            P1024x768x85 = 0x02,
            P1152x864x85 = 0x01
        };
    }

    typedef uint8_t AdditionalStandardTimings2_t;
    namespace AdditionalStandardTimings2
    {
        enum
        {
            P1280x768x60CVT_RB = 0x80,
            P1280x768x60 = 0x40,
            P1280x768x75 = 0x20,
            P1280x768x85 = 0x10,
            P1280x960x60 = 0x08,
            P1280x960x85 = 0x04,
            P1280x1024x60 = 0x02,
            P1280x1024x85 = 0x01
        };
    }

    typedef uint8_t AdditionalStandardTimings3_t;
    namespace AdditionalStandardTimings3
    {
        enum
        {
            P1360x768x60CVT_RB = 0x80,
            P1280x768x60 = 0x40,
            P1440x900x60CVT_RB = 0x20,
            P1440x900x75 = 0x10,
            P1440x900x85 = 0x08,
            P1400x1050x60CVT_RB = 0x04,
            P1400x1050x60 = 0x02,
            P1400x1050x75 = 0x01
        };
    }

    typedef uint8_t AdditionalStandardTimings4_t;
    namespace AdditionalStandardTimings4
    {
        enum
        {
            P1400x1050x85 = 0x80,
            P1680x1050x60CVT_RB = 0x40,
            P1680x1050x60 = 0x20,
            P1680x1050x75 = 0x10,
            P1680x1050x85 = 0x08,
            P1600x1200x60 = 0x04,
            P1600x1200x65 = 0x02,
            P1600x1200x70 = 0x01
        };
    }

    typedef uint8_t AdditionalStandardTimings5_t;
    namespace AdditionalStandardTimings5
    {
        enum
        {
            P1600x1200x75 = 0x80,
            P1600x1200x85 = 0x40,
            P1792x1344x60 = 0x20,
            P1792x1344x75 = 0x10,
            P1856x1392x60 = 0x08,
            P1856x1392x75 = 0x04,
            P1920x1200x60CVT_RB = 0x02,
            P1920x1200x60 = 0x01
        };
    }

    typedef uint8_t AdditionalStandardTimings6_t;
    namespace AdditionalStandardTimings6
    {
        enum
        {
            P1920x1200x75 = 0x80,
            P1920x1200x85 = 0x40,
            P1920x1440x60 = 0x20,
            P1920x1440x75 = 0x10,
        };
    }

    struct AdditionalStandardTimingsDescriptor
    {
        uint16_t m_DisplayDescriptor;               //Should be 0 for valid DisplayDescriptor
        uint8_t m_Reserved1;                        //Should be 0
        DescriptorType_t m_DescriptorType;          //Should be 0xf7
        uint8_t m_Reserved2;                        //Should be 0
        uint8_t m_Version;                          //Should be 10
        AdditionalStandardTimings1_t m_AdittionalStandardTimings1;
        AdditionalStandardTimings2_t m_AdittionalStandardTimings2;
        AdditionalStandardTimings3_t m_AdittionalStandardTimings3;
        AdditionalStandardTimings4_t m_AdittionalStandardTimings4;
        AdditionalStandardTimings5_t m_AdittionalStandardTimings5;
        AdditionalStandardTimings6_t m_AdittionalStandardTimings6;
        uint8_t m_Padding[6];
    };

    struct Edid
    {
        //Header Information
        uint8_t m_HeaderPattern[8];//0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
        uint16_t m_ManufacturerId;
        uint16_t m_ManufacturerProductCode;
        uint32_t m_SerialNumber;
        uint8_t m_ManufactureWeek;
        uint8_t m_ManufactureYear;//since 1990
        uint8_t m_EdidVersion;
        uint8_t m_EdidRevision;

        //Basic display parameters
        VideoInputParameters_t m_VideoInputParameters;
        uint8_t m_HorizontalScreenSize; //Horizontal screen size, in centimetres (range 1–255). 
                                        //If vertical screen size is 0, landscape aspect ratio (range 1.00–3.54), 
                                        //datavalue = (AR×100) − 99 (example: 16:9, 79; 4:3, 34.) 
        uint8_t m_VerticalScreenSize;   //Vertical screen size, in centimetres.
                                        //If horizontal screen size is 0, portrait aspect ratio (range 0.28–0.99),
                                        //datavalue = (100/AR) − 99 (example: 9:16, 79; 3:4, 34.) If both bytes are 0,
                                        //screen size and aspect ratio are undefined (e.g. projector)
        uint8_t m_DisplayGamma;         //Display gamma, factory default (range 1.00–3.54),
                                        //datavalue = (gamma×100) − 100 = (gamma − 1)×100. If 255, gamma is defined by DI-EXT block. 
        SupportedFeatures_t m_SupportedFeatures;

        //Chromaticity coordinates
        //10-bit 2° CIE 1931 xy coordinates for red, green, blue, and white point 
        uint8_t m_RedGreenLsb;      //Bits 7–6 	Red x value least-significant 2 bits
                                    //Bits 5–4 	Red y value least-significant 2 bits
                                    //Bits 3–2 	Green x value least-significant 2 bits
                                    //Bits 1–0 	Green y value least-significant 2 bits
        uint8_t m_BlueWhiteLsb;     //Bits 7–6 	Blue x value least-significant 2 bits
                                    //Bits 5–4 	Blue y value least-significant 2 bits
                                    //Bits 3–2 	White x value least-significant 2 bits
                                    //Bits 1–0 	White y value least-significant 2 bits
        uint8_t m_RedXMsb;          //Red x value most significant 8 bits (2^−1, …, 2^−8).
                                    //0–255 encodes fractional 0–0.996 (255/256); 0–0.999 (1023/1024) with lsbits 
        uint8_t m_RedYMsb;
        uint8_t m_GreenXMsb;
        uint8_t m_GreenYMsb;
        uint8_t m_BlueXMsb;
        uint8_t m_BlueYMsb;
        uint8_t m_DefaultWhitePointXMsb;
        uint8_t m_DefaultWhitePointYMsb;

        //Established timing bitmap. Supported bitmap for very common timing modes.
        EstablishedTimings1_t m_EstablishedTimings1;
        EstablishedTimings2_t m_EstablishedTimings2;
        EstablishedTimings3_t m_EstablishedTimings3;

        //Standard timing information. Up to 8 2-byte fields describing standard display modes.
        //Unused fields are filled with 0x01, 0x01. The following definitions applay to each record:
        StandardTimingInformation m_StandardTimingInformation[8];
        union
        {
            DetailedTimingDescriptor m_DetailedTiming;
            SerialNumberDisplayDescriptor m_SerialNumber;
            UnspecifiedTextDisplayDescriptor m_UnspecifiedText;
            DisplayNameDisplayDescriptor m_DisplayName;
            DummyIdentifierDisplayDescriptor m_Dummy;
            DisplayRangeLimitsDescriptor m_DisplayRangeLimits;
            AdditionalWhitePointDescriptor m_AdditionalWitePoint;
            ColorManagementDataDescriptor m_ColorManagementData;
            CVTTimingCodeDescriptor m_CVTTiming;
            AdditionalStandardTimingsDescriptor m_AdditionalStandardTimings;
        } Descriptors[4];
        uint8_t m_Extensions;
        uint8_t m_Checksum; //Sum of all 128 bytes mod 256 should be 0
        //
    };

    typedef uint8_t Version_t;
    namespace Version
    {
        enum
        {
            None = 0x00,
            Ddc1 = 0x01,
            Ddc2 = 0x02
        };
    }
    
    /*
        Hardware API:
        struct Hardware
        {
            static void Begin();                                //Do any setup needed to begin communication
            static void WriteClockAndData(uint2_t clockData);   //bit 1 = clock, bit 0 = data
            static uint1_t ReadData();                          //Return a single bit
            static void WaitForVSync();                         //Assumes normal vsync operation
            static void VSyncControlBegin();                    //Setup vsync control
            static void SendVSyncPulse();                       //Pulse the vsync control line
            static void VSyncControlEnd();                      //Return vsync to normal operation
            static void End();                                  //Do any teardown needed stop communication
        };
    */

    template<typename Hardware>
    void SendBegin()
    {
        Hardware::WriteClockAndData(0x01);
        Hardware::WriteClockAndData(0x03);
        Hardware::WriteClockAndData(0x02);
        Hardware::WriteClockAndData(0x00);
    }

    template<typename Hardware>
    void SendEnd()
    {
        Hardware::WriteClockAndData(0x00);
        Hardware::WriteClockAndData(0x02);
        Hardware::WriteClockAndData(0x03);
        Hardware::WriteClockAndData(0x01);
        Hardware::WriteClockAndData(0x03);
    }

    template<typename Hardware>
    void SendBit(uint1_t value)
    {
        Hardware::WriteClockAndData(uint2_t(value));
        Hardware::WriteClockAndData(uint2_t(value) | 0x02);
        Hardware::WriteClockAndData(uint2_t(value));
    }

    template<typename Hardware>
    uint1_t ReadBit()
    {
        Hardware::WriteClockAndData(0x01);
        Hardware::WriteClockAndData(0x03);
        uint1_t value = Hardware::ReadData();
        Hardware::WriteClockAndData(0x01);
        return value;
    }

    template<typename Hardware>
    void SendByte(uint8_t value)
    {
        for (int i = 7; i >= 0; --i)
        {
            SendBit<Hardware>(uint1_t((value >> i) & 0x01));
        }
        SendBit<Hardware>(0x01);
    }

    template<typename Hardware>
    uint8_t ReadByte()
    {
        uint8_t ret = 0;
        for (int i = 0; i < 8; ++i)
        {
            ret <<= 1;
            ret |= ReadBit<Hardware>();
        }
        return ret;
    }

    template<typename Hardware>
    void SendAck()
    {
        Hardware::WriteClockAndData(0x00);
    }

    template<typename Hardware>
    bool ReadDdc2Buffer(uint8_t offset, uint8_t size, bool performCheck, uint8_t* buffer)
    {
        bool ret = true;

        SendBegin<Hardware>();
        SendByte<Hardware>(0xa0);//Device ID - write
        SendByte<Hardware>(offset);//word offset
        SendBegin<Hardware>();
        SendByte<Hardware>(0xa1);//Device ID - read

        uint8_t check = 0;
        for (int i = 0; i < size - 1; ++i)
        {
            *buffer = ReadByte<Hardware>();
            check += *buffer;
            SendAck<Hardware>();
            ++buffer;
        }
        *buffer = ReadByte<Hardware>();
        check += *buffer;
        SendEnd<Hardware>();

        if (performCheck)
            ret = check == 0;

        return ret;
    }

    template<typename Hardware>
    uint8_t Ddc1ReadByte()
    {
        uint8_t ret = 0;
        for (int i = 7; i >= 0; -- i)
        {
            Hardware::SendVSyncPulse();
            ret |= Hardware::ReadData() << i;
        }
        return ret;
    }

    template<typename Hardware>
    bool FindDdc1Header(uint8_t findByte, uint32_t ffCount)
    {
        bool ret = false;
        uint8_t value = Ddc1ReadByte<Hardware>();
        int count = 0x480;
        while (true)
        {
            bool found = false;
            for (; count > 0; --count)
            {
                if (value == findByte)
                {
                    found = true;
                    break;
                }
                Hardware::SendVSyncPulse();
                value = (value << 1) | Hardware::ReadData();;
            }
            if (!found)
            break;

            //Find the FF bytes
            bool nonFFbyte = false;
            for (int i = 0; i < ffCount; ++i)
            {
                Hardware::SendVSyncPulse();
                value = Ddc1ReadByte<Hardware>();
                if (value != 0xff)
                {
                    nonFFbyte = true;
                    break;
                }
            }

            if (nonFFbyte)
                continue;

            //Found the FF bytes.
            Hardware::SendVSyncPulse();
            if (Ddc1ReadByte<Hardware>() != 0x00)
                continue;
            
            //Found the header
            if (findByte == 0x00)
            {
                ret = true;
                break;
            }
            
            //Skip bits
            for (int i = 0; i < 0x438; ++i)
                Hardware::SendVSyncPulse();

            if (Ddc1ReadByte<Hardware>() != 0x00)
                continue;
            
            //Skip more bits
            for (int i = 0; i < 0x40; ++i)
                Hardware::SendVSyncPulse();
            
            ret = true;
            break;
        }
        return ret;
    }

    template <typename Hardware>
    bool Ddc1ReadData(uint8_t* buffer)
    {
        uint8_t check = 0xfa;

        //Recreate 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00 pattern
        *buffer = 0;
        ++buffer;
        for (int i = 0; i < 6; ++i)
        {
            *buffer = 0xff;
            ++buffer;
        }
        *buffer = 0;
        ++buffer;

        for (int i = 0; i < 120; ++i)
        {
            Hardware::SendVSyncPulse();
            *buffer = Ddc1ReadByte<Hardware>();
            check += *buffer;
            ++buffer;
        }

        return check == 0;
    }

    template<typename Hardware>
    bool ReadEdidBuffer(Version_t version, Edid& edid)
    {
        bool ret = false;
        Hardware::Begin();

        if (version == Version::Ddc2)
        {
            ret = ReadDdc2Buffer<Hardware>(0, 0x80, true, (uint8_t*)&edid);
        }
        if (version == Version::Ddc1)
        {
            Hardware::VSyncControlBegin();

            if (FindDdc1Header<Hardware>(0xff, 6) || FindDdc1Header<Hardware>(0x00, 7))
            {
                ret = Ddc1ReadData<Hardware>((uint8_t*)&edid);
            }

            Hardware::VSyncControlEnd();
        }

        Hardware::End();
        return ret;
    }

    template<typename Hardware>
    Version_t InstallationCheck()
    {
        Version_t ret = Version::None;
        Hardware::Begin();

        SendBegin<Hardware>();
        SendByte<Hardware>(0xa0);//Device ID - write
        SendByte<Hardware>(0x00);//word offset
        SendBegin<Hardware>();
        SendByte<Hardware>(0xa1);//Device ID - read
        uint8_t shouldBe00 = ReadByte<Hardware>();
        SendAck<Hardware>();
        uint8_t shouldBeff = ReadByte<Hardware>();
        SendEnd<Hardware>();
        if (shouldBe00 == 0x00 && shouldBeff == 0xff)
        {
            ret = Version::Ddc2;
        }
        else
        {
            //Test for ddc1
            Hardware::WaitForVSync();
            uint1_t firstValue = Hardware::ReadData();
            for (int i = 0; i < 50; ++i)
            {
                Hardware::WaitForVSync();
                uint1_t secondValue = Hardware::ReadData();
                if (firstValue != secondValue)
                {
                    ret = Version::Ddc1;
                    break;
                }
            }
        }
        Hardware::End();
        return ret;
    }

}}}

#pragma pack(pop);

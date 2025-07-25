//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Testing::TestPatterns
{

extern void TestVideoModes();

extern void DrawTextPattern(uint16_t width, uint16_t height, uint8_t* videoMemory);
extern void Draw1BppPattern(uint16_t width, uint16_t height, uint8_t* videoMemory);
extern void Draw1BppPattern2(uint16_t width, uint16_t height, uint8_t* videoMemory);
extern void Draw2BppPattern(uint16_t width, uint16_t height, uint8_t* videoMemory);
extern void Draw2BppPlanarPattern(uint16_t width, uint16_t height, uint8_t* videoMemory);
extern void Draw4BppPattern(uint16_t width, uint16_t height, uint8_t* videoMemory);
extern void Draw8BppPattern(uint16_t width, uint16_t height, uint8_t* videoMemory);
extern void Draw8BppPlanarPattern(uint16_t width, uint16_t height, uint8_t* videoMemory);
extern void Draw15BppPattern(uint16_t width, uint16_t height, uint8_t* videoMemory);
extern void Draw16BppPattern(uint16_t width, uint16_t height, uint8_t* videoMemory);
extern void Draw24BppPattern(uint16_t width, uint16_t height, uint16_t stride, uint8_t* videoMemory);
extern void Draw32BppPattern(uint16_t width, uint16_t height, uint8_t* videoMemory);

}
#include <ov7725.hpp>

static const OV7725_cmd
    rgb[] = {
        // Manual output format, RGB, use RGB565 and full 0-255 output range
        {OV7725_REG_COM7, OV7725_COM7_RGB},
        {OV7725_REG_RGB444, 0x00},
        {OV7725_REG_COM15, OV7725_COM15_RGB565|OV7725_COM15_R00FF}},
    yuv[] = {
        // Manual output format, YUV, use full output range
        {OV7725_REG_COM7, OV7725_COM7_YUV},
        {OV7725_REG_COM15, OV7725_COM15_R00FF},
        {0xFF, 0xFF},
    },
    init[] = {
        {OV7725_REG_TSLB,  OV7725_TSLB_YLAST},    // No auto window
        {OV7725_REG_COM10, OV7725_COM10_VS_NEG}, // -VSYNC (req by SAMD PCC)
        {OV7725_REG_SLOP,       0x20},
        {OV7725_REG_GAM_BASE,   0x1C},
        {OV7725_REG_GAM_BASE + 1,  0x28},
        {OV7725_REG_GAM_BASE + 2,  0x3C},
        {OV7725_REG_GAM_BASE + 3,  0x55},
        {OV7725_REG_GAM_BASE + 4,  0x68},
        {OV7725_REG_GAM_BASE + 5,  0x76},
        {OV7725_REG_GAM_BASE + 6,  0x80},
        {OV7725_REG_GAM_BASE + 7,  0x88},
        {OV7725_REG_GAM_BASE + 8,  0x8F},
        {OV7725_REG_GAM_BASE + 9,  0x96},
        {OV7725_REG_GAM_BASE + 10, 0xA3},
        {OV7725_REG_GAM_BASE + 11, 0xAF},
        {OV7725_REG_GAM_BASE + 12, 0xC4},
        {OV7725_REG_GAM_BASE + 13, 0xD7},
        {OV7725_REG_GAM_BASE + 14, 0xE8},
        {OV7725_REG_COM8,
         OV7725_COM8_FASTAEC | OV7725_COM8_AECSTEP | OV7725_COM8_BANDING},
        {OV7725_REG_GAIN, 0x00},
        {OV7725_COM2_SSLEEP, 0x00},
        {OV7725_REG_COM4, 0x00},
        {OV7725_REG_COM9, 0x20}, // Max AGC value
        {OV7725_REG_BD50MAX, 0x05},
        {OV7725_REG_BD60MAX, 0x07},
        {OV7725_REG_AEW, 0x75},
        {OV7725_REG_AEB, 0x63},
        {OV7725_REG_VPT, 0xA5},
        {OV7725_REG_HAECC1, 0x78},
        {OV7725_REG_HAECC2, 0x68},
        {0xA1, 0x03},              // Reserved register?
        {OV7725_REG_HAECC3, 0xDF}, // Histogram-based AEC/AGC setup
        {OV7725_REG_HAECC4, 0xDF},
        {OV7725_REG_HAECC5, 0xF0},
        {OV7725_REG_HAECC6, 0x90},
        {OV7725_REG_HAECC7, 0x94},
        {OV7725_REG_COM8, OV7725_COM8_FASTAEC | OV7725_COM8_AECSTEP |
                              OV7725_COM8_BANDING | OV7725_COM8_AGC |
                              OV7725_COM8_AEC},
        {OV7725_REG_COM5, 0x61},
        {OV7725_REG_COM6, 0x4B},
        {0x16, 0x02},            // Reserved register?
        {OV7725_REG_MVFP, 0x07}, // 0x07,
        {OV7725_REG_ADCCTR1, 0x02},
        {OV7725_REG_ADCCTR2, 0x91},
        {0x29, 0x07}, // Reserved register?
        {OV7725_REG_CHLF, 0x0B},
        {0x35, 0x0B}, // Reserved register?
        {OV7725_REG_ADC, 0x1D},
        {OV7725_REG_ACOM, 0x71},
        {OV7725_REG_OFON, 0x2A},
        {OV7725_REG_COM12, 0x78},
        {0x4D, 0x40}, // Reserved register?
        {0x4E, 0x20}, // Reserved register?
        {OV7725_REG_GFIX, 0x5D},
        {OV7725_REG_REG74, 0x19},
        {0x8D, 0x4F}, // Reserved register?
        {0x8E, 0x00}, // Reserved register?
        {0x8F, 0x00}, // Reserved register?
        {0x90, 0x00}, // Reserved register?
        {0x91, 0x00}, // Reserved register?
        {OV7725_REG_DM_LNL, 0x00},
        {0x96, 0x00}, // Reserved register?
        {0x9A, 0x80}, // Reserved register?
        {0xB0, 0x84}, // Reserved register?
        {OV7725_REG_ABLC1, 0x0C},
        {0xB2, 0x0E}, // Reserved register?
        {OV7725_REG_THL_ST, 0x82},
        {0xB8, 0x0A}, // Reserved register?
        {OV7725_REG_AWBC1, 0x14},
        {OV7725_REG_AWBC2, 0xF0},
        {OV7725_REG_AWBC3, 0x34},
        {OV7725_REG_AWBC4, 0x58},
        {OV7725_REG_AWBC5, 0x28},
        {OV7725_REG_AWBC6, 0x3A},
        {0x59, 0x88}, // Reserved register?
        {0x5A, 0x88}, // Reserved register?
        {0x5B, 0x44}, // Reserved register?
        {0x5C, 0x67}, // Reserved register?
        {0x5D, 0x49}, // Reserved register?
        {0x5E, 0x0E}, // Reserved register?
        {OV7725_REG_LCC3, 0x04},
        {OV7725_REG_LCC4, 0x20},
        {OV7725_REG_LCC5, 0x05},
        {OV7725_REG_LCC6, 0x04},
        {OV7725_REG_LCC7, 0x08},
        {OV7725_REG_AWBCTR3, 0x0A},
        {OV7725_REG_AWBCTR2, 0x55},
        {OV7725_REG_MTX1, 0x80},
        {OV7725_REG_MTX2, 0x80},
        {OV7725_REG_MTX3, 0x00},
        {OV7725_REG_MTX4, 0x22},
        {OV7725_REG_MTX5, 0x5E},
        {OV7725_REG_MTX6, 0x80}, // 0x40?
        {OV7725_REG_AWBCTR1, 0x11},
        {OV7725_REG_AWBCTR0, 0x9F}, // Or use 0x9E for advance AWB
        {OV7725_REG_BRIGHT, 0x00},
        {OV7725_REG_CONTRAS, 0x40},
        {OV7725_REG_CONTRAS_CENTER, 0x80}, // 0x40?
        {OV7725_REG_LAST + 1, 0x00},       // End-of-data marker
};

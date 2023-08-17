#ifndef __OV7725_HPP__
#define __OV7725_HPP__
#include <cstdint>
#include <stdint.h>

#define OV7725_REG_GAIN        int(0x00)                //< AGC gain bits 7:0 (9:8 in VREF)
#define OV7725_REG_BLUE        int(0x01)                //< AWB blue channel gain
#define OV7725_REG_RED         int(0x02)                //< AWB red channel gain
#define OV7725_REG_VREF        int(0x03)                //< Vert frame control bits
#define OV7725_REG_COM1        int(0x04)                //< Common control 1
#define OV7725_COM1_R656       int(0x40)                //< COM1 enable R656 format
#define OV7725_REG_BAVE        int(0x05)                //< U/B average level
#define OV7725_REG_GbAVE       int(0x06)                //< Y/Gb average level
#define OV7725_REG_AECHH       int(0x07)                //< Exposure value - AEC 15:10 bits
#define OV7725_REG_RAVE        int(0x08)                //< V/R average level
#define OV7725_REG_COM2        int(0x09)                //< Common control 2
#define OV7725_COM2_SSLEEP     int(0x10)                //< COM2 soft sleep mode
#define OV7725_REG_PID         int(0x0A)                //< Product ID MSB (read-only)
#define OV7725_REG_VER         int(0x0B)                //< Product ID LSB (read-only)
#define OV7725_REG_COM3        int(0x0C)                //< Common control 3
#define OV7725_COM3_SWAP       int(0x40)                //< COM3 output data MSB/LSB swap
#define OV7725_COM3_SCALEEN    int(0x08)                //< COM3 scale enable
#define OV7725_COM3_DCWEN      int(0x04)                //< COM3 DCW enable
#define OV7725_REG_COM4        int(0x0D)                //< Common control 4
#define OV7725_REG_COM5        int(0x0E)                //< Common control 5
#define OV7725_REG_COM6        int(0x0F)                //< Common control 6
#define OV7725_REG_AECH        int(0x10)                //< Exposure value 9:2
#define OV7725_REG_CLKRC       int(0x11)                //< Internal clock
#define OV7725_CLK_EXT         int(0x40)                //< CLKRC Use ext clock directly
#define OV7725_CLK_SCALE       int(0x3F)                //< CLKRC Int clock prescale mask
#define OV7725_REG_COM7        int(0x12)                //< Common control 7
#define OV7725_COM7_RESET      int(0x80)                //< COM7 SCCB register reset
#define OV7725_COM7_SIZE_MASK      int(0x38)               //< COM7 output size mask
#define OV7725_COM7_PIXEL_MASK     int(0x05)                //< COM7 output pixel format mask
#define OV7725_COM7_SIZE_VGA       int(0x00)                //< COM7 output size VGA
#define OV7725_COM7_SIZE_CIF       int(0x20)                //< COM7 output size CIF
#define OV7725_COM7_SIZE_QVGA      int(0x10)                //< COM7 output size QVGA
#define OV7725_COM7_SIZE_QCIF      int(0x08)                //< COM7 output size QCIF
#define OV7725_COM7_RGB            int(0x04)                //< COM7 pixel format RGB
#define OV7725_COM7_YUV            int(0x00)                //< COM7 pixel format YUV
#define OV7725_COM7_BAYER          int(0x01)                //< COM7 pixel format Bayer RAW
#define OV7725_COM7_PBAYER         int(0x05)                //< COM7 pixel fmt proc Bayer RAW
#define OV7725_COM7_COLORBAR       int(0x02)                //< COM7 color bar enable
#define OV7725_REG_COM8            int(0x13)                //< Common control 8
#define OV7725_COM8_FASTAEC        int(0x80)                //< COM8 Enable fast AGC/AEC algo,
#define OV7725_COM8_AECSTEP        int(0x40)                //< COM8 AEC step size unlimited
#define OV7725_COM8_BANDING        int(0x20)                //< COM8 Banding filter enable
#define OV7725_COM8_AGC            int(0x04)                //< COM8 AGC (auto gain) enable
#define OV7725_COM8_AWB            int(0x02)                //< COM8 AWB (auto white balance)
#define OV7725_COM8_AEC            int(0x01)                //< COM8 AEC (auto exposure) enable
#define OV7725_REG_COM9            int(0x14)                //< Common control 9 - max AGC value
#define OV7725_REG_COM10           int(0x15)                //< Common control 10
#define OV7725_COM10_HSYNC         int(0x40)                //< COM10 HREF changes to HSYNC
#define OV7725_COM10_PCLK_HB       int(0x20)                //< COM10 Suppress PCLK on hblank
#define OV7725_COM10_HREF_REV      int(0x08)                //< COM10 HREF reverse
#define OV7725_COM10_VS_EDGE       int(0x04)                //< COM10 VSYNC chg on PCLK rising
#define OV7725_COM10_VS_NEG        int(0x02)                //< COM10 VSYNC negative
#define OV7725_COM10_HS_NEG        int(0x01)                //< COM10 HSYNC negative
#define OV7725_REG_HSTART          int(0x17)                //< Horiz frame start high bits
#define OV7725_REG_HSTOP           int(0x18)                //< Horiz frame end high bits
#define OV7725_REG_VSTART          int(0x19)                //< Vert frame start high bits
#define OV7725_REG_VSTOP           int(0x1A)                //< Vert frame end high bits
#define OV7725_REG_PSHFT           int(0x1B)                //< Pixel delay select
#define OV7725_REG_MIDH            int(0x1C)                //< Manufacturer ID high byte
#define OV7725_REG_MIDL            int(0x1D)                //< Manufacturer ID low byte
#define OV7725_REG_MVFP            int(0x1E)                //< Mirror / vert-flip enable
#define OV7725_MVFP_MIRROR         int(0x20)                //< MVFP Mirror image
#define OV7725_MVFP_VFLIP          int(0x10)                //< MVFP Vertical flip
#define OV7725_REG_LAEC            int(0x1F)                //< Reserved
#define OV7725_REG_ADCCTR0         int(0x20)                //< ADC control
#define OV7725_REG_ADCCTR1         int(0x21)                //< Reserved
#define OV7725_REG_ADCCTR2         int(0x22)                //< Reserved
#define OV7725_REG_ADCCTR3         int(0x23)                //< Reserved
#define OV7725_REG_AEW             int(0x24)                //< AGC/AEC upper limit
#define OV7725_REG_AEB             int(0x25)                //< AGC/AEC lower limit
#define OV7725_REG_VPT             int(0x26)                //< AGC/AEC fast mode op region
#define OV7725_REG_BBIAS           int(0x27)                //< B channel signal output bias
#define OV7725_REG_GbBIAS          int(0x28)                //< Gb channel signal output bias
#define OV7725_REG_EXHCH           int(0x2A)                //< Dummy pixel insert MSB
#define OV7725_REG_EXHCL           int(0x2B)                //< Dummy pixel insert LSB
#define OV7725_REG_RBIAS           int(0x2C)                //< R channel signal output bias
#define OV7725_REG_ADVFL           int(0x2D)                //< Insert dummy lines MSB
#define OV7725_REG_ADVFH           int(0x2E)                //< Insert dummy lines LSB
#define OV7725_REG_YAVE            int(0x2F)                //< Y/G channel average value
#define OV7725_REG_HSYST           int(0x30)                //< HSYNC rising edge delay
#define OV7725_REG_HSYEN           int(0x31)                //< HSYNC falling edge delay
#define OV7725_REG_HREF            int(0x32)                //< HREF control
#define OV7725_REG_CHLF            int(0x33)                //< Array current control
#define OV7725_REG_ARBLM           int(0x34)                //< Array ref control - reserved
#define OV7725_REG_ADC             int(0x37)                //< ADC control - reserved
#define OV7725_REG_ACOM            int(0x38)                //< ADC & analog common - reserved
#define OV7725_REG_OFON            int(0x39)                //< ADC offset control - reserved
#define OV7725_REG_TSLB            int(0x3A)                //< Line buffer test option
#define OV7725_TSLB_NEG            int(0x20)                //< TSLB Negative image enable
#define OV7725_TSLB_YLAST          int(0x04)                //< TSLB UYVY or VYUY, see COM13
#define OV7725_TSLB_AOW            int(0x01)                //< TSLB Auto output window
#define OV7725_REG_COM11           int(0x3B)                //< Common control 11
#define OV7725_COM11_NIGHT         int(0x80)                //< COM11 Night mode
#define OV7725_COM11_NMFR          int(0x60)                //< COM11 Night mode frame rate mask
#define OV7725_COM11_HZAUTO        int(0x10)                //< COM11 Auto detect 50/60 Hz
#define OV7725_COM11_BAND          int(0x08)                //< COM11 Banding filter val select
#define OV7725_COM11_EXP           int(0x02)                //< COM11 Exposure timing control
#define OV7725_REG_COM12           int(0x3C)                //< Common control 12
#define OV7725_COM12_HREF          int(0x80)                //< COM12 Always has HREF
#define OV7725_REG_COM13           int(0x3D)                //< Common control 13
#define OV7725_COM13_GAMMA         int(0x80)                //< COM13 Gamma enable
#define OV7725_COM13_UVSAT         int(0x40)                //< COM13 UV saturation auto adj
#define OV7725_COM13_UVSWAP        int(0x01)                //< COM13 UV swap, use w TSLB[3]
#define OV7725_REG_COM14           int(0x3E)                //< Common control 14
#define OV7725_COM14_DCWEN         int(0x10)                //< COM14 DCW & scaling PCLK enable
#define OV7725_REG_EDGE            int(0x3F)                //< Edge enhancement adjustment
#define OV7725_REG_COM15           int(0x40)                //< Common control 15
#define OV7725_COM15_RMASK         int(0xC0)                //< COM15 Output range mask
#define OV7725_COM15_R10F0         int(0x00)                //< COM15 Output range 10 to F0
#define OV7725_COM15_R01FE         int(0x80)                //< COM15              01 to FE
#define OV7725_COM15_R00FF         int(0xC0)                //< COM15              00 to FF
#define OV7725_COM15_RGBMASK       int(0x30)                //< COM15 RGB 555/565 option mask
#define OV7725_COM15_RGB           int(0x00)                //< COM15 Normal RGB out
#define OV7725_COM15_RGB565        int(0x10)                //< COM15 RGB 565 output
#define OV7725_COM15_RGB555        int(0x30)                //< COM15 RGB 555 output
#define OV7725_REG_COM16           int(0x41)                //< Common control 16
#define OV7725_COM16_AWBGAIN       int(0x08)                //< COM16 AWB gain enable
#define OV7725_REG_COM17           int(0x42)                //< Common control 17
#define OV7725_COM17_AECWIN        int(0xC0)                //< COM17 AEC window must match COM4
#define OV7725_COM17_CBAR          int(0x08)                //< COM17 DSP Color bar enable
#define OV7725_REG_AWBC1           int(0x43)               //< Reserved
#define OV7725_REG_AWBC2           int(0x44)                //< Reserved
#define OV7725_REG_AWBC3           int(0x45)                //< Reserved
#define OV7725_REG_AWBC4           int(0x46)                //< Reserved
#define OV7725_REG_AWBC5           int(0x47)                //< Reserved
#define OV7725_REG_AWBC6           int(0x48)                //< Reserved
#define OV7725_REG_REG4B           int(0x4B)                //< UV average enable
#define OV7725_REG_DNSTH           int(0x4C)                //< De-noise strength
#define OV7725_REG_MTX1            int(0x4F)                //< Matrix coefficient 1
#define OV7725_REG_MTX2            int(0x50)                //< Matrix coefficient 2
#define OV7725_REG_MTX3            int(0x51)                //< Matrix coefficient 3
#define OV7725_REG_MTX4            int(0x52)                //< Matrix coefficient 4
#define OV7725_REG_MTX5            int(0x53)                //< Matrix coefficient 5
#define OV7725_REG_MTX6            int(0x54)                //< Matrix coefficient 6
#define OV7725_REG_BRIGHT          int(0x55)                //< Brightness control
#define OV7725_REG_CONTRAS         int(0x56)                //< Contrast control
#define OV7725_REG_CONTRAS_CENTER  int(0x57)                //< Contrast center
#define OV7725_REG_MTXS         int(0x58)                //< Matrix coefficient sign
#define OV7725_REG_LCC1         int(0x62)                //< Lens correction option 1
#define OV7725_REG_LCC2         int(0x63)                //< Lens correction option 2
#define OV7725_REG_LCC3         int(0x64)                //< Lens correction option 3
#define OV7725_REG_LCC4         int(0x65)                //< Lens correction option 4
#define OV7725_REG_LCC5         int(0x66)                //< Lens correction option 5
#define OV7725_REG_MANU         int(0x67)                //< Manual U value
#define OV7725_REG_MANV         int(0x68)                //< Manual V value
#define OV7725_REG_GFIX         int(0x69)                //< Fix gain control
#define OV7725_REG_GGAIN        int(0x6A)                //< G channel AWB gain
#define OV7725_REG_DBLV         int(0x6B)                //< PLL & regulator control
#define OV7725_REG_AWBCTR3      int(0x6C)                //< AWB control 3
#define OV7725_REG_AWBCTR2      int(0x6D)                //< AWB control 2
#define OV7725_REG_AWBCTR1      int(0x6E)                //< AWB control 1
#define OV7725_REG_AWBCTR0      int(0x6F)                //< AWB control 0
#define OV7725_REG_SCALING_XSC         int(0x70)            //< Test pattern X scaling
#define OV7725_REG_SCALING_YSC         int(0x71)            //< Test pattern Y scaling
#define OV7725_REG_SCALING_DCWCTR      int(0x72)            //< DCW control
#define OV7725_REG_SCALING_PCLK_DIV    int(0x73)            //< DSP scale control clock divide
#define OV7725_REG_REG74        int(0x74)                //< Digital gain control
#define OV7725_REG_REG76        int(0x76)                //< Pixel correction
#define OV7725_REG_SLOP         int(0x7A)                //< Gamma curve highest seg slope
#define OV7725_REG_GAM_BASE     int(0x7B)                //< Gamma register base (1 of 15)
#define OV7725_GAM_LEN          int(15)                //< Number of gamma registers
#define OV7725_R76_BLKPCOR      int(0x80)                //< REG76 black pixel corr enable
#define OV7725_R76_WHTPCOR      int(0x40)                //< REG76 white pixel corr enable
#define OV7725_REG_RGB444       int(0x8C)                //< RGB 444 control
#define OV7725_R444_ENABLE      int(0x02)                //< RGB444 enable
#define OV7725_R444_RGBX        int(0x01)                //< RGB444 word format
#define OV7725_REG_DM_LNL       int(0x92)                //< Dummy line LSB
#define OV7725_REG_LCC6         int(0x94)                //< Lens correction option 6
#define OV7725_REG_LCC7         int(0x95)                //< Lens correction option 7
#define OV7725_REG_HAECC1       int(0x9F)                //< Histogram-based AEC/AGC ctrl 1
#define OV7725_REG_HAECC2       int(0xA0)                //< Histogram-based AEC/AGC ctrl 2
#define OV7725_REG_SCALING_PCLK_DELAY int(0xA2)       //< Scaling pixel clock delay
#define OV7725_REG_BD50MAX   int(0xA5)               //< 50 Hz banding step limit
#define OV7725_REG_HAECC3    int(0xA6)               //< Histogram-based AEC/AGC ctrl 3
#define OV7725_REG_HAECC4    int(0xA7)               //< Histogram-based AEC/AGC ctrl 4
#define OV7725_REG_HAECC5    int(0xA8)               //< Histogram-based AEC/AGC ctrl 5
#define OV7725_REG_HAECC6    int(0xA9)               //< Histogram-based AEC/AGC ctrl 6
#define OV7725_REG_HAECC7    int(0xAA)               //< Histogram-based AEC/AGC ctrl 7
#define OV7725_REG_BD60MAX   int(0xAB)               //< 60 Hz banding step limit
#define OV7725_REG_ABLC1     int(0xB1)               //< ABLC enable
#define OV7725_REG_THL_ST    int(0xB3)               //< ABLC target
#define OV7725_REG_SATCTR    int(0xC9)               //< Saturation control
#define OV7725_REG_LAST      OV7725_REG_SATCTR   //< Maximum register address
#endif

#include <Arduino.h>
#include <i2c.hpp>
#include <adapter.hpp>

using namespace OV7725;
namespace cam_prop = OV7725;

namespace camera {

    template<class I2C, int RRST, int WRST, int RCK, int WR, int D0, int D1, int D2, int D3, int D4, int D6, int D7>
    class Camera {
        private:
            I2C& _i2c;
        public:
            Camera(I2C& i2c):_i2c(i2c) {}

            void writeRegister(const unsigned char data[][2]) {
                for(int i = 0; data[i][0] != 0xff; i++) {
                    _i2c.writeRegister(i2c::ADDR, data[i][0], data[i][1]);
                }
            }

            void frameControl(int hStart, int hStop, int vStart, int vStop) {
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_HSTART, hStart >> 3);
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_HSTOP,  hStop >> 3);
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_HREF, ((hStop & 0b111) << 3) | (hStart & 0b111));
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_VSTART, vStart >> 2);
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_VSTOP, vStop >> 2);
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_VREF, ((vStop & 0b11) << 2) | (vStart & 0b11));
            }

            /**
              *  param: s range(-2, 2)
            **/
            void saturation(int s) {
                //color matrix values
                _i2c.writeRegister(i2c::ADDR, 0x4f, 0x80 + 0x20 * s);
                _i2c.writeRegister(i2c::ADDR, 0x50, 0x80 + 0x20 * s);
                _i2c.writeRegister(i2c::ADDR, 0x51, 0x00);
                _i2c.writeRegister(i2c::ADDR, 0x52, 0x22 + (0x11 * s) / 2);
                _i2c.writeRegister(i2c::ADDR, 0x53, 0x5e + (0x2f * s) / 2);
                _i2c.writeRegister(i2c::ADDR, 0x54, 0x80 + 0x20 * s);
                _i2c.writeRegister(i2c::ADDR, 0x58, 0x9e);  //matrix signs
            }

            void QQVGA() {
                //160 x 120 (1/4)
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_COM14, 0x1a);
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_SCALING_DCWCTR, 0x22);
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_SCALING_PCLK_DIV, 0xf2);
            }

            void QQQVGA() {
                //80 x 60 (1/8)
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_COM14, 0x1b);
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_SCALING_DCWCTR, 0x33);
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_SCALING_PCLK_DIV, 0xf3);
            }

            void QQVGARGB565() {
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_COM7, 0b10000000);  //all registers default
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_CLKRC, 0b10000000); //double clock
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_COM11, 0b1000 | 0b10); //enable auto 50/60Hz detect + exposure timing can be less...
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_TSLB, 0b100); //sequence UYVY
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_COM7, 0b100); //RGB
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_COM15, 0b11000000 | 0b010000); //RGB565
                frameControl(192, 48, 8, 488); //no clue why horizontal needs such strange values, vertical works ok
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_COM10, 0x02); //VSYNC negative
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_COM3, 0x04);  //DCW enable
                _i2c.writeRegister(i2c::ADDR, cam_prop::REG_MVFP, 0x2b);  //mirror flip
                QQVGA();
                _i2c.writeRegister(i2c::ADDR, 0xb0, 0x84);// no clue what this is but it's most important for colors
                saturation(0);
                _i2c.writeRegister(i2c::ADDR, 0x13, 0xe7); //AWB on
                _i2c.writeRegister(i2c::ADDR, 0x6f, 0x9f); // Simple AWB
            }

            void QQVGAYUV() {
               _i2c.writeRegister(i2c::ADDR, cam_prop::REG_COM7, 0b10000000);  //all registers default
               _i2c.writeRegister(i2c::ADDR, cam_prop::REG_CLKRC, 0b10000000); //double clock
               _i2c.writeRegister(i2c::ADDR, cam_prop::REG_COM11, 0b1000 | 0b10); //enable auto 50/60Hz detect + exposure timing can be less...
               _i2c.writeRegister(i2c::ADDR, cam_prop::REG_TSLB, 0b100); //sequence UYVY
               frameControl(192, 48, 8, 488);
               _i2c.writeRegister(i2c::ADDR, cam_prop::REG_COM10, 0x02); //VSYNC negative
               _i2c.writeRegister(i2c::ADDR, cam_prop::REG_COM3, 0x04);  //DCW enable
               _i2c.writeRegister(i2c::ADDR, cam_prop::REG_MVFP, 0x2b);  //mirror flip
               QQVGA();
               _i2c.writeRegister(i2c::ADDR, 0xb0, 0x84);// no clue what this is but it's most important for colors
               _i2c.writeRegister(i2c::ADDR, 0x13, 0xe7); //AWB on
               _i2c.writeRegister(i2c::ADDR, 0x6f, 0x9f); // Simple AWB
        }

        void RGBRaw()
        {
            const unsigned char RGBBayerRAW[][2] = {
                    {0x12, 0x80},
                    {0x11, 0x80}, 
                    //{0x11, 0x01},
                    {0x3a, 0x04},
                    {0x12, 0x01},
                    {0x17, 0x12},
                    {0x18, 0x00},
                    {0x32, 0xb6},
                    {0x19, 0x02},
                    {0x1a, 0x7a},
                    {0x03, 0x00},
                    {0x0c, 0x00},
                    {0x3e, 0x00},
                    {0x70, 0x3a},
                    {0x71, 0x35},
                    {0x72, 0x11},
                    {0x73, 0xf0},
                    {0xa2, 0x02},
                    {0x13, 0xe0},
                    {0x00, 0x00},
                    {0x10, 0x00},
                    {0x0d, 0x40},
                    {0x14, 0x38},
                    {0xa5, 0x07},
                    {0xab, 0x08},
                    {0x24, 0x95},
                    {0x25, 0x33},
                    {0x26, 0xe3},
                    {0x9f, 0x78},
                    {0xa0, 0x68},
                    {0xa1, 0x0b},
                    {0xa6, 0xd8},
                    {0xa7, 0xd8},
                    {0xa8, 0xf0},
                    {0xa9, 0x90},
                    {0xaa, 0x94},
                    {0x13, 0xe5},
                    {0x0e, 0x61},
                    {0x0f, 0x4b},
                    {0x16, 0x02},
                    {0x21, 0x02},
                    {0x22, 0x91},
                    {0x29, 0x07},
                    {0x33, 0x03},
                    {0x35, 0x0b},
                    {0x37, 0x1c},
                    {0x38, 0x71},
                    {0x3c, 0x78},
                    {0x3d, 0x08},
                    {0x41, 0x3a},
                    {0x4d, 0x40},
                    {0x4e, 0x20},
                    {0x69, 0x55},
                    {0x6b, 0x4a},
                    {0x74, 0x19},
                    {0x76, 0x61},
                    {0x8d, 0x4f},
                    {0x8e, 0x00},
                    {0x8f, 0x00},
                    {0x90, 0x00},
                    {0x91, 0x00},
                    {0x96, 0x00},
                    {0x9a, 0x80},
                    {0xb0, 0x8c},
                    {0xb1, 0x0c},
                    {0xb2, 0x0e},
                    {0xb3, 0x82},
                    {0xb8, 0x0a},
                    {0x43, 0x14},
                    {0x44, 0xf0},
                    {0x45, 0x34},
                    {0x46, 0x58},
                    {0x47, 0x28},
                    {0x48, 0x3a},
                    {0x59, 0x88},
                    {0x5a, 0x88},
                    {0x5b, 0x44},
                    {0x5c, 0x67},
                    {0x5d, 0x49},
                    {0x5e, 0x0e},
                    {0x6c, 0x0a},
                    {0x6d, 0x55},
                    {0x6e, 0x11},
                    {0x6f, 0x9f},
                    {0x6a, 0x40},
                    {0x01, 0x40},
                    {0x02, 0x40},
                    {0x13, 0xe7},
                    {0x34, 0x11},
                    {0x92, 0x66},
                    {0x3b, 0x0a},
                    {0xa4, 0x88},
                    {0x96, 0x00},
                    {0x97, 0x30},
                    {0x98, 0x20},
                    {0x99, 0x20},
                    {0x9a, 0x84},
                    {0x9b, 0x29},
                    {0x9c, 0x03},
                    {0x9d, 0x4c},
                    {0x9e, 0x3f},
                    {0x78, 0x04},
                    {0x79, 0x01},
                    {0xc8, 0xf0},
                    {0x79, 0x0f},
                    {0xc8, 0x20},
                    {0x79, 0x10},
                    {0xc8, 0x7e},
                    {0x79, 0x0b},
                    {0xc8, 0x01},
                    {0x79, 0x0c},
                    {0xc8, 0x07},
                    {0x79, 0x0d},
                    {0xc8, 0x20},
                    {0x79, 0x09},
                    {0xc8, 0x80},
                    {0x79, 0x02},
                    {0xc8, 0xc0},
                    {0x79, 0x03},
                    {0xc8, 0x40},
                    {0x79, 0x05},
                    {0xc8, 0x30},
                    {0x79, 0x26},
                    {0xff, 0xff}};
                writeRegisters(RGBBayerRAW);
        }

    };

};

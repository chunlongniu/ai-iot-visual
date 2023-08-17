#ifndef __I2C_HPP__
#define __I2C_HPP__
#include "esp32-hal-gpio.h"
#include <Arduino.h>

#define DELAY_SECOND_TIME uint32_t(1)

namespace i2c {
    const int ADDR = 0x42; 

    template<int SDA, int SCL>
    class I2C {

        private:
            void DELAY() {
                delayMicroseconds(DELAY_SECOND_TIME);
            }

            void SCLLOW() {
                pinMode(SCL, OUTPUT);
                digitalWrite(SCL, 0);
            }

            void SCLHIGH() {
                pinMode(SCL, INPUT_PULLUP);
                digitalWrite(SCL, 1);
            }

            void CLOCK() {
                DELAY();
                SCLHIGH();
                DELAY();
                DELAY();
                SCLLOW();
                DELAY();
            }

            void SDALOW() {
                pinMode(SDA, OUTPUT);
                digitalWrite(SDA, 0);
            }

            void SDAHIGH() {
               pinMode(SDA, OUTPUT);
               digitalWrite(SDA, 1);
            }

            void SDAPULLUP() {
                pinMode(SDA, INPUT_PULLUP);
            }

            void pushByte(unsigned char b) {
                for(char i = 0; i < 8; i++) {
                    if ( b & 0x80 )
                        SDAHIGH();
                    else
                        SDALOW();
                    b <<= 1;
                    CLOCK();
                }
            }

            bool getAck() {
                SDAPULLUP();
                DELAY();
                SCLHIGH();
                DELAY();
                int r = digitalRead(SDA);
                SDALOW();
                DELAY();
                SCLLOW();
                DELAY();
                return r == 0;
            }

            void start() {
                SDAPULLUP();
                DELAY();
                SCLHIGH();
                DELAY();
                SDALOW();
                DELAY();
                SCLLOW();
                DELAY();
            }

            void end() {
                SCLHIGH();
                DELAY();
                SDAPULLUP();
                DELAY();
            }

        public:

            void init() {
                pinMode(SDA, INPUT_PULLUP);
                pinMode(SCL, INPUT_PULLUP);
                digitalWrite(SDA, 0);
                digitalWrite(SCL, 0);
            }

            bool writeRegister(unsigned char addr, unsigned char reg, unsigned char  data) {
                start();
                pushByte(addr);
                if (!getAck()) {
                    end();
                    return false;
                }
                pushByte(reg);
                if(!getAck()) {
                    end();
                    return false;
                }
                pushByte(data);
                if (!getAck(data)) {
                    end();
                    return false;
                }
                end();
                return true;
            }
    };
}
#endif

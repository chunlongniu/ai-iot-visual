#include <Arduino.h>
#include <OV7725/ov7725.hpp>
#include <OV7725/i2c.hpp>

using namespace i2c;

namespace camera {

    template<class I2C, int RRST, int WRST, int RCK, int WR, int D0, int D1, int D2, int D3, int D4, int D6, int D7>
    class Camera {
        private:
            I2C _i2c;
        public:
            Camera(I2C& i2c):_i2c(i2c) {}

            void QQVGA() {
                //160 x 120 (1/4)
                
                // _i2c.writeRegister(i2c::ADDR, );
            }
    };

};

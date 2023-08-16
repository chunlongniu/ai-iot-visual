#ifndef __DMA_BUFFER_HPP_
#define __DMA_BUFFER_HPP_
#include <cstdint>
#include <esp32/rom/lldesc.h>
#include <iostream>

class DMABuffer {
    public:
        lldesc_t description;
        unsigned char* buffer;

        DMABuffer(const int bytes) {
            buffer                   = new unsigned char[bytes]();
            description.length       = bytes;
            description.size         = description.length;
            description.owner        = 1;
            description.sosf         = 1;
            description.buf          = (uint8_t*)buffer;
            description.offset       = 0;
            description.empty        = 0;
            description.eof          = 1;
            description.qe.stqe_next = 0;
        }

        void next(DMABuffer* next) {
            description.qe.stqe_next = &(next->description);
        };

        const int count() {
            return description.length/4;
        };

        ~DMABuffer() {
            if (!buffer) 
                delete buffer;
        }
};

#endif

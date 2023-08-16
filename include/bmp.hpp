#ifndef __BMP_HPP__
#define __BMP_HPP__

namespace bmp {
    static void setChar(void *buffer, int pos, char ch) {
        ((char*)buffer)[pos] = ch;
    }

    static void setLong(void* buffer, int pos, long l) {
        ((long*)buffer)[pos] = l;
    }

    static void setShort(void* buffer, int pos, short s) {
        *((short*)buffer+pos) = s;
    }
}

#endif

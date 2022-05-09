#ifndef _BASE_CONV_H
#include <string>
#include <cstdio>
#include <iostream>

using std::string;
class BaseConv {
 public:
    BaseConv(int64_t data, int base) : data{data}, base{base} {}

    void reset(int64_t data, int base) {
        this->data = data;
        this->base = base;
    }

    /**
     * @brief convert data of base to data of radix
     *
     * @param radix
     *
     * @return 
     */
    string get(int radix) {
        std::stringstream in;
        // convert radix to 10
        in << std::dec << data;
        int64_t middle = 0;
        in >> middle;
        std::stringstream out;
        switch (radix) {
            case 2:
                return BaseConv::convertDec2Bin(middle);
                break;
            case 8:
                out << std::oct << middle;
                break;
            case 10:
                out << std::dec << middle;
                break;
            case 16:
                out << std::hex << middle;
                break;
        }
        return out.str();
    }

    /**
     * @brief convert data (>0) to string of bin
     *
     * @param data int64_t (>0)
     *
     * @return string of bin
     */
    static string convertDec2Bin(int64_t data) {
        std::stringstream convertor;
        bool get_MSB = false;
        for (int i = sizeof(data) * 8 - 1; i >= 0; i--) {
            if (data & (1L << i)) {
                convertor << "1";
                get_MSB = true;
            } else {
                if (get_MSB) convertor << "0";
            }
        }

        if (!get_MSB) return "0";
        return convertor.str();
    }

    static int64_t convertBin2Dec(string bin) {
        int index = 0;
        int64_t ret = 0;
        for (int i = bin.length() - 1; i >= 0; i--) {
            if (bin[i] == '1') {
                ret += powl(2, index);
            }
            index++;
        }
        return ret;
    }

    static string convertHex2Ascii(int64_t hex) {
        std::stringstream out;
        for (int i = 0; i < 4; i++) {
            uint8_t byte = (hex & (0xFF));
            if (byte != '\0') out << byte;
            hex >>= 8;
        }
        return out.str();
    }

    /**
     * @brief convert each char in ascii to hex
     *
     * @param ascii
     *
     * @return hex string (in lower case)
     */
    static string convertAsciiToHex(string ascii) {
        std::stringstream out;
        for (auto &x : ascii) {
            out << std::hex << (int) x;
        }
        return out.str();
    }

 private:
    int64_t data;
    int base;
};

#endif // _BASE_CONV_H

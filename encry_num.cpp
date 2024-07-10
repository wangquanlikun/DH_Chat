#include "encry_num.h"
#include "windowui.h"
#include <vector>
#include <cstdint>
#include <map>

Encry_num::Encry_num() {
    reset_num();
}

void Encry_num::reset_num() {
    this->p = 2;
    this->g = 2;
    this->private_key = 1;
    this->public_key = 0;
    this->shared_key = 0;
}

unsigned long long Encry_num::mod_operation(unsigned long long A, unsigned long long n, unsigned long long p) const {
    unsigned long long result = 1;
    A = A % p;

    while (n > 0) {
        if (n % 2 == 1) {
            result = (result * A) % p;
        }
        n = n >> 1;
        A = (A * A) % p;
    }

    return result;
}

bool Encry_num::is_qualified_p(const unsigned long long temp_p) const {
    if (temp_p <= 1) {
        return false;
    }
    if (temp_p <= 3) {
        return true;
    }
    if (temp_p % 2 == 0 || temp_p % 3 == 0) {
        return false;
    }

    for (unsigned long long i = 5; i * i <= temp_p; i += 6) {
        if (temp_p % i == 0 || temp_p % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

bool Encry_num::set_p(unsigned long long temp_p) {
    if(is_qualified_p(temp_p)){
        this->p = temp_p;
        return true;
    }
    else
        return false;
}

void Encry_num::set_public_key() {
    this->public_key = mod_operation(this->g, this->private_key, this->p);
}

void Encry_num::set_share_key(unsigned long long others_pubkey){
    this->shared_key = mod_operation(others_pubkey, this->private_key, this->p);
}

QString Encry_num::encry(int mode, QString input){
    QString output;
    if(mode == NUM_MODE){
        output += QString::number(NUM_MODE);
        int num = input.toInt();
        uint32_t out32 = encry_32((uint32_t)num);
        output += QString::asprintf("%08X", out32);
    }
    else if(mode == TEXT_MODE){
        output += QString::number(TEXT_MODE);
        std::vector<uint32_t> unicodeVector;
        for (QChar ch : input) {
            unicodeVector.push_back(ch.unicode());
        }
        int utfvec_len = (int)unicodeVector.size();
        for(int i = 0; i < utfvec_len; i++){
            uint32_t out32 = encry_32(unicodeVector[i]);
            output += QString::asprintf("%08X", out32);
        }
    }
    return output;
}

QString Encry_num::decry(QString input) {
    std::map<QChar, int> Hex_Char2int;
    Hex_Char2int['0'] = 0x0, Hex_Char2int['1'] = 0x1, Hex_Char2int['2'] = 0x2, Hex_Char2int['3'] = 0x3, Hex_Char2int['4'] = 0x4, Hex_Char2int['5'] = 0x5, Hex_Char2int['6'] = 0x6, Hex_Char2int['7'] = 0x7;
    Hex_Char2int['8'] = 0x8, Hex_Char2int['9'] = 0x9, Hex_Char2int['A'] = 0xA, Hex_Char2int['B'] = 0xB, Hex_Char2int['C'] = 0xC, Hex_Char2int['D'] = 0xD, Hex_Char2int['E'] = 0xE, Hex_Char2int['F'] = 0xF;
    QString output;
    QChar textmode = input[0];
    if(textmode == QString::number(NUM_MODE)[0]){
        uint32_t out32 = 0;
        QByteArray byteArray = input.toUtf8();
        for (int j = 1; j <= 8; j++) {
            out32 = (out32 << 4) + Hex_Char2int[byteArray[j]];
        }
        output += QString::number((int)decry_32(out32));
    }
    else if(textmode == QString::number(TEXT_MODE)[0]){
        std::vector<uint32_t> out32;
        QByteArray byteArray = input.toUtf8();
        for (int i = 1; i < byteArray.size(); i += 8) {
            if (i + 8 <= byteArray.size()) {
                uint32_t value = 0;
                for (int j = 0; j < 8; ++j) {
                    value = (value << 4) + Hex_Char2int[byteArray[j + i]];
                }
                out32.push_back(value);
            }
        }
        int out32_len = (int)out32.size();
        uint32_t unicode;
        for(int i = 0; i < out32_len; i++){
            unicode = decry_32(out32[i]);
            output += QChar(unicode);
        }
    }
    return output;
}

uint32_t Encry_num::encry_32(uint32_t input) {
    // FNV-1a 哈希算法生成32位密钥，防止原密钥可能过短的问题
    const uint32_t fnv_prime = 0x01000193;
    uint32_t new_key = 0x811C9DC5;

    unsigned char* key_bytes = reinterpret_cast<unsigned char*>(&shared_key);
    for (size_t i = 0; i < sizeof(shared_key); ++i) {
        new_key ^= key_bytes[i];
        new_key *= fnv_prime;
    }

    return input ^ new_key;
}

uint32_t Encry_num::decry_32(uint32_t input) {
    const uint32_t fnv_prime = 0x01000193;
    uint32_t new_key = 0x811C9DC5;

    unsigned char* key_bytes = reinterpret_cast<unsigned char*>(&shared_key);
    for (size_t i = 0; i < sizeof(shared_key); ++i) {
        new_key ^= key_bytes[i];
        new_key *= fnv_prime;
    }

    return input ^ new_key;
}

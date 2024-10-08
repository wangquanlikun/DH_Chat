#ifndef ENCRY_NUM_H
#define ENCRY_NUM_H

#include <QString>
#include <QChar>
#include <cstdint>

class Encry_num
{
public:
    Encry_num();
    void reset_num();

    unsigned long long p; //大的素数
    unsigned long long g; //生成元
    unsigned long long private_key;
    unsigned long long public_key;
    unsigned long long shared_key;

    bool set_p(unsigned long long temp_p);
    void set_public_key();
    void set_share_key(unsigned long long others_pubkey);

    QString encry(int mode, QString input);
    QString decry(QString input);

private:
    unsigned long long mod_operation(unsigned long long A, unsigned long long n, unsigned long long p) const; // A^n mod p 快速幂运算
    bool is_qualified_p(const unsigned long long temp_p) const; //检验输入的p是否为质数
    uint32_t encry_32(uint32_t input);
    uint32_t decry_32(uint32_t input);
};

#endif // ENCRY_NUM_H

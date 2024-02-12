
#pragma once

#include <QString>

struct Resistance
{
    uint64_t value;
    char const* display_text;
    const char* jlc_part_number;

    operator double() const 
    {
        return value;        
    };

    Resistance(const Resistance&) = default;
    Resistance(uint64_t value) : value(value), display_text(0), jlc_part_number(0){}
    Resistance(uint64_t value, const char* d, const char* jlc) : value(value), display_text(d), jlc_part_number(jlc){}

    operator QString() const
    {
        if (display_text)
            return QString::fromUtf8(display_text);
        if (value<1000)
            return QString("%1Ω").arg(value);
        if (value<1000000)
            return QString("%1kΩ").arg(value/1000);
        if (value<1000000000)
            return QString("%1MΩ").arg(value/1000000);
        return QString("%1MΩ").arg(value/1000000);
    }
};

struct R_lib
{
    enum R_type {
        good_R, // 1%
        regular_R, // 5%
        free_R, // jlc free R
    } current_type;

    R_lib(int _type) : current_type((R_type)_type)
    {}

    Resistance operator()(int idx) const;

    int get_R_counts() const;
};

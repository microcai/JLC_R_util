
#pragma once

#include <QString>

struct Resistance
{
    int value;
    char const* display_text;
    const char* jlc_part_number;

    operator double() const 
    {
        return value;        
    };

    Resistance(const Resistance&) = default;
    Resistance(int value) : value(value), display_text(0), jlc_part_number(0){}
    Resistance(int value, const char* d, const char* jlc) : value(value), display_text(d), jlc_part_number(jlc){}

    operator QString() const
    {
        if (display_text)
            return QString::fromUtf8(display_text);
        if (value<1000)
            return QString("%1").arg(value);
        return QString("%1k").arg(value/1000);
    }
};

struct R_lib
{
    enum R_type {
        regular_R, // 5%
        good_R, // 1%
        free_R, // jlc free R
    } current_type;

    R_lib(int _type) : current_type((R_type)_type)
    {}

    Resistance operator()(int idx) const;
};

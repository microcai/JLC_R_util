
#pragma once

#include <QString>

struct Resistance
{
    double value;
    char const* display_text;
    const char* jlc_part_number;

    operator double() const 
    {
        return value;        
    };

    Resistance(const Resistance&) = default;
    Resistance(double value) : value(value), display_text(0), jlc_part_number(0){}
    Resistance(double value, const char* d, const char* jlc) : value(value), display_text(d), jlc_part_number(jlc){}

    operator QString() const
    {
        if (display_text)
            return QString::fromUtf8(display_text);
        if (value<1000)
            return QString("%1Ω").arg(value);
        if (value<1000000)
        {
            if (value / 1000 * 1000 == value)
                return QString("%1kΩ").arg(value/1000);
            else
                return QString("%1kΩ").arg(value/1000.0f);
        }
        if (value<1000000000)
        {
            if (value / 1000000 * 1000000 == value)
                return QString("%1kΩ").arg(value/1000);
            else
                return QString("%1MΩ").arg(value/1000000);
        }
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

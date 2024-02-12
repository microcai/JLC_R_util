
#pragma once

#include <utility>

// Z walker generator
struct Zgen
{
    int x, y;
    Zgen() : x(0), y(0) {}

    int condition() const
    {
        if ( (x > 0) && (y > 0))
        {
            // in middle.
            if ((x + y) & 1)
            {
                return 1;
            }
            else return 0;
        }
        else if ( x == 0 && y > 0)
        {
            if ( y &1)
                return 2;
            else
                return 0;
        }
        else if ( y == 0 && x > 0)
        {
            if ( x &1)
                return 1;
            else
                return 3;
        }
        else if (x == 0 && y == 0)
        {
            return 3;
        }
        return 4;
    }

    std::pair<int, int> operator()()
    {
        std::pair<int, int> ret = {x, y};

        switch (condition())
        {
            case 0:
                x ++;
                y --;
                break;
            case 1:
                x --;
                y ++;
                break;
            case 2:
                y ++;
                x = 0;
                break;
            case 3:
                x ++;
                y = 0;
                break;
        }

        return ret;
    }

};


#pragma once

#include <utility>
#include <stdexcept>

// Z walker generator
struct Zgen
{
    int x, y;
    int max_x, max_y;

    Zgen() : x(0), y(0), max_x(-1), max_y(-1) {}

    void set_size(int x, int y)
    {
        max_x = x;
        max_y = y;
    }

    enum direction {
        dir_right,
        dir_leftdown,
        dir_down,
        dir_right_up,
        dir_stop,
    };

    direction prefered_dir = dir_leftdown;

    int condition()
    {
        if ( (x > 0) && (y > 0))
        {
            if ((x >= max_x) && (y >= max_y))
            {
                return dir_stop;
            }
            else if ( x >= max_x)
            {
                if (prefered_dir == dir_right_up)
                {
                    prefered_dir = dir_leftdown;
                    return dir_down;
                }
            }
            else if ( y >= max_x)
            {
                if (prefered_dir == dir_leftdown)
                {
                    prefered_dir = dir_right_up;
                    return dir_right;
                }
            }

            return prefered_dir;
        }
        else if ( x == 0 && y > 0)
        {
            if (prefered_dir == dir_leftdown)
            {
                prefered_dir = dir_right_up;
                if ( y == max_y)
                {                   
                    return dir_right;
                }
                return dir_down;
            }
            return prefered_dir;
        }
        else if ( y == 0 && x > 0)
        {
            if (prefered_dir == dir_right_up)
            {
                prefered_dir = dir_leftdown;
                if ( x == max_y)
                {                   
                    return dir_down;
                }
                return dir_right;
            }
            return prefered_dir;
        }
        else if (x == 0 && y == 0)
        {
            return dir_right;
        }
        return dir_right;
    }

    std::pair<int, int> operator()()
    {
        std::pair<int, int> ret = {x, y};

        switch (condition())
        {
            case dir_right_up:
                x ++;
                y --;
                break;
            case dir_leftdown:
                x --;
                y ++;
                break;
            case dir_down:
                y ++;
                // x = 0;
                break;
            case dir_right:
                x ++;
                // y = 0;
                break;
            case dir_stop:
                if ((x == 9999999) && (y == 9999999))
                {
                    throw std::out_of_range("eof");
                }
                x = y = 9999999;
                break;
        }

        return ret;
    }

};

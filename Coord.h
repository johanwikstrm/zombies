#ifndef COORD_H
#define COORD_H	

#include <stdint.h>

class Coord {

    private:
        uint32_t x;
        uint32_t y;
    
    public:
        Coord();
        Coord(uint32_t x, uint32_t y);
        Coord(const Coord& c);
        ~Coord();
        uint32_t getX();
        uint32_t getY();
        void setX(uint32_t x);
        void setY(uint32_t y);
        uint32_t manhattanDist(Coord c1, Coord c2, uint32_t width, uint32_t height);
        Coord& operator=(const Coord& c);
};

#endif

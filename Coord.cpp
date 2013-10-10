using namespace std;

#include "Coord.h"

int abs(int a){
    if (a < 0)
        return -1*a;
    else
        return a;
}

Coord::Coord() {
    this->x = 0;
    this->y = 0;
}

Coord::Coord(uint32_t x, uint32_t y) {
    this->x = x;
    this->y = y;
}

Coord::Coord(const Coord& c) {
    this->x = c.x;
    this->y = c.y;
}

Coord::~Coord() {
}

uint32_t Coord::getX() {
    return x;
}

uint32_t Coord::getY() {
    return y;
}

void Coord::setX(uint32_t x) {
    this->x = x;
}

void Coord::setY(uint32_t y) {
    this->y = y;
}

uint32_t Coord::manhattanDist(Coord c1, Coord c2, uint32_t width, uint32_t height){
    uint32_t dx,dy;
    dx = abs(c1.x - c2.x);
    dy = abs(c1.y - c2.y);
    if (dx > width / 2)
        dx = width - dx;
    if (dy > height/2)
        dy = height - dy;
    return dx + dy;
}

Coord& Coord::operator=(const Coord& c) {
    this->x = c.x;
    this->y = c.y;
    return *this;
}

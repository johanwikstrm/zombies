#ifndef COORD_H
#define COORD_H	

#include <stdint.h>

/**
 * @file Coord.h
 * @author Johan Wikstrom and Anael Bonneton
 */

class Coord {

    private:
        uint32_t x;     /**< the abscissa */
        uint32_t y;     /**< the ordinate */
    
    public:

         /**
         * @brief Default constructor : (0,0)
         */
        Coord();

         /**
         * @brief       Constructor with parameters
         *
         * @param   x       the abscissa       
         * @param   y       the ordinate   
         */
        Coord(uint32_t x, uint32_t y);

         /**
         * @brief       Copy constructor
         *
         * @param       c   the coordinates we want to copy
         */
        Coord(const Coord& c);

         /**
         * @brief       Destructor
         */
        ~Coord();

        /**
         * @return  the abscissa
         */
        uint32_t getX();

        /**
         * @return  the ordinate
         */
        uint32_t getY();
        
        /**
         * @param   the new abscissa
         */
        void setX(uint32_t x);
        
        /**
         * @param   the new ordinate
         */
        void setY(uint32_t y);

        // TODO
        uint32_t manhattanDist(Coord c1, Coord c2, uint32_t width, uint32_t height);
        
        /**
         * @brief       Overloading the = operator (assignment)
         *
         * @param c     the coordinates for the comparaison 
         */
        Coord& operator=(const Coord& c);
};

#endif

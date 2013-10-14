#ifndef CELL_H
#define CELL_H

#include <stdint.h>

/**
 * @file Cell.h
 * @author Johan Wikstrom and Anael Bonneton
 */

class Cell {
    
    private:
        uint8_t k;          /**< the kind of the cell (HUMAN, INFECTED, ZOMBIE, or EMPTY)*/
        uint8_t sex;    
        bool mFlag;          /**< hasMoved flag */

    public:

        /**
         * @brief       Constructor with parameter
         *
         * @param       kind    kind of the cell
         */
        Cell(uint32_t kind, uint32_t sex = 0);
        
        /**
         * @brief       Copy constructor 
         * @param       c   the cell we want to copy
         */
        Cell(const Cell& c);

        /**
         * @brief   Destructor
         */
        ~Cell();

        /**
         * @return      the kind of the cell
         */
        uint32_t getKind();

        /**
         * @param       the new kind of the cell
         */
        void setKind(uint32_t);
        
        uint32_t getSex();

        void setSex(uint32_t);

        /**
         * @return      the move flag of the cell
         */
        bool getMoveFlag();

        /**
         * @param       the new moveFlag of the cell
         */
        void setMoveFlag(bool);

};

#endif

#ifndef CELL_H
#define CELL_H

#include <stdint.h>

/**
 * @file Cell.h
 * @author Johan Wikstrom and Anael Bonneton
 */

class Cell {
    
    private:
        uint8_t k;           /**< the kind of the cell (HUMAN, INFECTED, ZOMBIE, or EMPTY)*/
        uint8_t sex;         /**< the sex of the cell */   
        bool mFlag;          /**< hasMoved flag */

    public:

        /**
         * @brief       Constructor with parameter
         *
         * @param       kind    kind of the cell
         * @param       sex     sex of the cell
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
         * @return      the sex of the cell
         */ 
        uint32_t getSex();
        
        /**
         * @return      the move flag of the cell
         */
        bool getMoveFlag();
        
        /**
         * @param       the new kind of the cell
         */
        void setKind(uint32_t);
       
        /**
         * @param sex   the new sex of the cell
         */
        void setSex(uint32_t);


        /**
         * @param       the new moveFlag of the cell
         */
        void setMoveFlag(bool);

};

#endif

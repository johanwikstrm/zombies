#ifndef _ARRAY_H
#define _ARRAY_H

/**
 * @file Array.h
 * @author Johan Wikstrom and Anael Bonneton
 */

#include "Cell.h"
#include <stdint.h>
#include <iostream>

class Array
{

    protected :
        uint32_t size;          /**< the array size */
        Cell** array;           /**< the array containing the data */

    public :

        /**
         * @brief Default constructor
         */
        Array();

        /**
         * @brief       Constructor with parameters
         *
         * @param       s       size of the array 
         * @param       kind    kind of the initial cells (HUMAN, INFECTED, ZOMBIE, or EMPTY)
         *                      by default EMPTY 
         */
        explicit Array(uint32_t s, uint32_t kind = 0);

        /**
         * @brief       Copy constructor
         *
         * @param       P   the array to copy
         */
        Array(const Array& P);

        /**
         * @brief       Destructor
         */
        ~Array();

        /**
         * @return      the array size 
         */
        uint32_t getSize() const;

        /**
         * @brief       Overloading the () operator to access the elements of the array (read only) 
         *             
         * @param       i       the index of the element
         *
         * @return      array[i]     
         */
        Cell* operator()(uint32_t i) const;

        /**
         * @brief       Overloading the () operator to access the elements of the array (read and write) 
         *
         * @param       i       the index of the element
         *
         * @return      array[i]     
         */
        Cell*& operator()(uint32_t i) ;

        /**
         * @brief       Overloading the = operator (assignment) 
         */
        Array& operator=(const Array& P);

        /**
         * @brief       Overloading the == operator (equality)
         *
         * @param       P       the array to compare to
         *
         * @return      true iff the arrays are equal
         */
        bool operator==(const Array& P) const;

        /**
         * @brief       Swap the array with the array in argument
         *
         * @param       the array for the swaping
         *
         */
        void swap(Array& P);

};

#endif

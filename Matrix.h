#ifndef _MATRIX_H
#define _MATRIX_H

/**
 * @file Matrix.h
 * @author  Johan Wikstrom and Anael Bonneton
 */

#include <stdint.h>
#include <pthread.h>
#include "Array.h"
#include "Coord.h"

class Matrix : public Array
{

    private :

        uint32_t height;            /**< matrix height */
        uint32_t width;             /**< matrix width */
        uint32_t *counts;           /**< the statistics : number of HUMAN, INFECTED, ZOMBIE and EMPTY */

        uint32_t kind(Cell* ptr);

    public :

        /**
         * @brief   Default constructor
         *          Create a matrix with a null dimension
         */
        Matrix();        

        /**
         * @brief       Constructor with parameters       
         *
         * @param       height
         * @param       width
         */
        Matrix(uint32_t height, uint32_t width);

        /**
         * @brief       Copy constructor
         *
         * @param       M       the matrix to copy
         */
        Matrix(const Matrix& M);


        /**
         * @brief       Destructor
         */
        ~Matrix();

        /**
         * @return      the matrix height
         */
        uint32_t getHeight() const;

        /**
         * @return      the matrix width
         */
        uint32_t getWidth() const;


        /**
         * @brief   
         *
         * @param   kind    the kind (HUMAN, INFECTED, ZOMBIE, or EMPTY)
         * @return  the number of kind persons
         */
        uint32_t getCount(uint32_t kind) const;

        /**
         * @brief       Print the elements of the matrix 
         */
        void print() const;
        void printMoveFlags() const;

        /**
         *  @brief      Change the kind of the cell at the position (x, y)
         *
         *  @param  x   the abscissa
         *  @param  y   the ordinate
         *  @param  k   the new kind of the cell at the position (x, y)
         */
        void set(uint32_t x, uint32_t y, uint32_t k, uint32_t sex = 0);

        /**
         * @brief       Move a person
         *
         * @param   oldX        the abscissa of origin
         * @param   oldY        the ordinate of origin
         * @param   newX        the abscissa of destination
         * @param   newY        the ordinate of destination
         *
         */
        void move(uint32_t oldX, uint32_t oldY, uint32_t newX, uint32_t newY);

        /**
         * @brief       The person (HUMAN or INFECTED) get infected
         *
         * @param       the abscissa of the person
         * @param       the ordinate of the person
         */
        void getInfected(uint32_t x, uint32_t y);

        /**
         * @brief       Overloading the () operator to access the elements of the matrix (read only) 
         *             
         * @param       x       the abscissa of the element
         * @param       y       the ordinate of the element
         *
         * @return      matrix(x, y)     
         */
        Cell* operator()(uint32_t x, uint32_t y) const;

        /**
         * @brief       Overloading the () operator to access the elements of the matrix (read and write) 
         *             
         * @param       x       the abscissa of the element
         * @param       y       the ordinate of the element
         *
         * @return      matrix(x, y)     
         */
        Cell*& operator()(uint32_t x, uint32_t y);

        /**
         * @brief       Overloading the () operator to access the elements of the matrix (read only) 
         *             
         * @param       c       the coordiantes of the element we want to access
         *
         * @return      matrix(c.x, c.y)     
         */
        Cell* operator()(Coord c) const;

        /**
         * @brief       Overloading the () operator to access the elements of the matrix (read and write) 
         *             
         * @param       c       the coordiantes of the element we want to access
         *
         * @return      matrix(c.x, c.y)     
         */
        Cell*& operator()(Coord c);

        /**
         * @brief       Overloading the == operator (equality)
         *
         * @param       M       the matrix to compare to
         *
         * @return      true iff the matrices are equal
         */
        bool operator==(const Matrix& M);


        Matrix& operator=(const Matrix& M);
        
        /**
         * @brief       Swap the matrix with the matrix in argument
         *
         * @param       M       the matrix for the swaping
         *
         */
        void swap(Matrix& M);

        /**
         * @brief   Extract a column from the matrix
         *
         * @param col   the number of the column we want to extract
         *
         * @return  an array containing the column number col of the matrix
         */
        Array* extractColumn(uint32_t col);

        /**
         * @brief   Extract a row from the matrix
         *
         * @param r     the number of the row we want to extract
         *
         * @return  an array containing the row number r of the matrix
         */
        Array* extractRow(uint32_t r);

        // TODO
        Array** toSend();

        // TODO
        void insert(Array*[4]);
        
        int insertColumnWithCollisions(Array * toInsert,uint32_t col);

        // Returns the number of collisions
        int insertRowWithCollisions(Array * toInsert,uint32_t row);
    
        /*  
            offset=0   offset=1
            X X X X X  E X E X E 
            X E E E X  X X X X X
            X E E E X  X X X X X
            X X X X X  E X E X E
        */
        Array** toSend(int offset);

        /*  offset=0   offset=1
            X X X X X  E X E X E 
            X E E E X  X X X X X
            X E E E X  X X X X X
            X X X X X  E X E X E
        */
        int insertWithCollisions(Array** toInsert,int offset);



};
#endif

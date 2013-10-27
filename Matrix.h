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
        uint32_t* counts;           /**< the statistics : number of HUMAN, INFECTED, ZOMBIE and EMPTY */
        // partialCounts is necessary for efficientcy issues
        // without this matrix, when a thread wants to update counts, we need to lock/unlock
        // and it is very time-consumming
        // It is less consumming to update counts at the end of each iteration thanks to the local
        // statistics computed by each thread
        int32_t** partialCounts;    /**< the local statistics computed by each thread */
        bool mpiEnabled;            /**< True iff the program is using MPI */
        uint32_t kind(Cell* ptr);   /**< return the kind of a cell */

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
         * @param       mpiEnabled  whether the programm is using MPI or not
         */
        Matrix(uint32_t height, uint32_t width, bool mpiEnabled = false);

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
         * @brief   return the number persons of a certain type  
         *
         * @param   kind    the kind (HUMAN, INFECTED, ZOMBIE, or EMPTY)
         * @return  the number of kind persons
         */
        uint32_t getCount(uint32_t kind) const;

        /**
         * @brief   Update the global statistics thanks to the partial
         *          statistics computed by each thread locally
         */
        void computeGlobalStatistics();

        /**
         * @brief       Print the elements of the matrix 
         */
        void print() const;

        /**
         *  @brief      Print all the move flags of the matrix
         */
        void printMoveFlags() const;

        /**
         *  @brief      Change the kind of the cell at the position (x, y)
         *              and update the statistics
         *              if numThread == NULL, then the global statistics are updated
         *              otherwise, the local statistics of the thread (*numThread) are updated
         *
         *  @param  x           the abscissa
         *  @param  y           the ordinate
         *  @param  k           the new kind of the cell at the position (x, y)
         *  @paam   *numThread  pointer to the number of the current thread
         *  @param  sex         the nex sex of the cell at the position (x, y)
         */
        void set(uint32_t x, uint32_t y, uint32_t k, uint32_t* numThread, uint32_t sex = 0);

        /**
         * @brief       Move a person
         *
         * @param   oldX        the abscissa of origin
         * @param   oldY        the ordinate of origin
         * @param   newX        the abscissa of destination
         * @param   newY        the ordinate of destination
         *
         */
        void move(uint32_t oldX, uint32_t oldY, uint32_t newX, uint32_t newY, uint32_t* numThread);

        /**
         * @brief       The person (HUMAN or INFECTED) get infected
         *              If numThread != NULL the global statistics are updated
         *              otherwise, the local statistics of the thread (*numThread)
         *              are updated
         *
         * @param  x            the abscissa of the person
         * @param  y            the ordinate of the person
         * @paam   *numThread   pointer to the number of the current thread
         */
        void getInfected(uint32_t x, uint32_t y, uint32_t* numThread);

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
        Array* extractColumn(uint32_t col,bool moveFlag);

        /**
         * @brief   Extract a row from the matrix
         *
         * @param r     the number of the row we want to extract
         *
         * @return  an array containing the row number r of the matrix
         */
        Array* extractRow(uint32_t r,bool moveFlag);

        // TODO
        void insert(Array*[4]);
        
        int insertColumnWithCollisions(Array * toInsert,uint32_t col, bool overwrite);

        // Returns the number of collisions
        int insertRowWithCollisions(Array * toInsert,uint32_t row, bool overwrite);
    
        /*  
            offset=0   offset=1
            X X X X X  E X E X E 
            X E E E X  X X X X X
            X E E E X  X X X X X
            X X X X X  E X E X E
        */
        Array** toSend(int offset,bool moveFlag);

        /*  offset=0   offset=1
            X X X X X  E X E X E 
            X E E E X  X X X X X
            X E E E X  X X X X X
            X X X X X  E X E X E
        */
        int insertWithCollisions(Array** toInsert,int offset);
};
#endif

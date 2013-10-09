#ifndef _DMATRIX_H
#define _DMATRIX_H

/**
 * @file Dmatrix.h
 * @author 
 */

#include <stdint.h>
#include "Darray.h"
#include "Coord.h"

#define numCellKinds 4

class Dmatrix : public Darray
{

    private :

        uint32_t height;            /**< number of rows */
        uint32_t width;             /**< number of columns */
        uint32_t *counts;
        //Cell* dummy;
        
    int kind(Cell* ptr);

    public :
        
        /**
         * @brief       Constructor with parameters       
         *
         * @param       width
         * @param       heigh
         * @param       init            Optional parameter, the value with which 
         *                              the matrix is initialize 
         *                              By default, init = 0
         */
        Dmatrix(uint32_t height, uint32_t width);

        /**
         * @brief       Constructeur par copie 
         *
         * @param       M       la matrice a copier
         */
        Dmatrix(const Dmatrix& M);

        Dmatrix();        

        /**
         * @brief       Destructor
         */
        ~Dmatrix();

        uint32_t getHeight() const;

        uint32_t getWidth() const;
       
        uint32_t getCount(int kind) const;


        /**
         * @brief       Print the element of the matrix 
         */
        void print() const;

        void set(uint32_t x, uint32_t y, int k);

        void move(uint32_t oldX, uint32_t oldY, uint32_t newX, uint32_t newY);

        void getInfected(uint32_t x, uint32_t y);

        /**
         * @brief       Accesseur a un element de la matrice (en lecture uniquement) 
         *             
         * @param       i       ligne de la matrice a laquelle on accede
         *                      (les lignes de la matrice etant numerotes de 1 a nb_lignes)
         * @param       j       colonne de la matrice a laquelle on accede
         *                      (les colonnes de la matrice etant numerotes de 1 a nb_colonnes)
         *
         * @return      element (i,j) de la matrice
         */
        Cell* operator()(uint32_t x, uint32_t y) const;

        Cell*& operator()(uint32_t x, uint32_t y);
        
        Cell* operator()(Coord c) const;

        Cell*& operator()(Coord c);
        
        /**
         * @brief       Teste l egalite de deux matrices
         *
         * @param       M       matrice avec laquelle on teste l egalite
         *
         * @return      true si les deux matrices sont identiques,
         *              false sinon
         */
        bool operator==(const Dmatrix& M);

        Dmatrix& operator=(const Dmatrix& M);

        void swap(Dmatrix& P);

        Darray* extractColumn(uint32_t col);

        int insertColumnWithCollisions(Darray * toInsert,uint32_t col);

        int insertRowWithCollisions(Darray * toInsert,uint32_t row);
        
        Darray* extractRow(uint32_t r);

        Darray** toSend(int offset);

        int insertWithCollisions(Darray** toInsert, int offset);


};
#endif

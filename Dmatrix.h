#ifndef _DMATRIX_H
#define _DMATRIX_H

/**
 * @file Dmatrix.h
 * @author 
 */

#include <stdint.h>
#include "Darray.h"

#define numCellKinds 4

class Dmatrix : public Darray
{

    private :

        uint32_t height;            /**< number of rows */
        uint32_t width;             /**< number of columns */
        uint32_t *counts;
        Cell* dummy;
        
    int kind(Cell* ptr);

    public :
        Cell*& operator()(uint32_t i, uint32_t j);
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

        void set(uint32_t i, uint32_t j, Cell* newValue);

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
        Cell* operator()(uint32_t i, uint32_t j) const;

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

        Darray extractColumn(uint32_t col);
        
        Darray extractRow(uint32_t r);


};
#endif

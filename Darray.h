#ifndef _DARRAY_H
#define _DARRAY_H

/**
 * @file Darray.h
 * @author
 */

#include "Cell.h"
#include <stdint.h>
#include <iostream>

class Darray
{
    protected :
    uint32_t size;        /**< the array size */
    Cell **array;           /**< the array containing the data */

    public :

        /**
         * @brief Default constructor
         */
        Darray();

        /**
         * @brief       Constructeur avec parametres
         *
         * @param       t       la taille du vecteur a creer 
         * @param       init    argument optionnel qui specifie la valeur 
         *                      a utiliser pour initialiser les elements du vecteur
         *                      (par defaut init=0)
         */
        explicit Darray(uint32_t s,Cell* init = 0);

        /**
         * @brief       Constructeur par copie 
         *              Le vecteur cree sera proprietaire de ses donnees si et seulement si le vecteur P l est
         * @param       P       le vecteur a copier
         */
        Darray(const Darray& P);

        /**
         * @brief       Destructor
         */
        ~Darray();

        /**
         * @brief       Accesseur de la taille du vecteur
         *
         * @return      taille du vecteur
         */
        uint32_t getSize() const;

        /**
         * @brief       Affiche les elements du vecteur
         */
        void print() const;

        /**
         * @brief       Accesseur au vecteur (en lecture uniquement) 
         *             
         * @param       i       indice du vecteur auquel on accede
         *                      (les indices du vecteur etant numerotes de 1 a taille)
         *
         * @return      le ieme element du vecteur      
         */
       Cell* operator()(uint32_t i) const;

        /**
         * @brief       Accesseur au vecteur : (en lecture et en ecriture))
         *
         * @param       i       indice du vecteur auquel on accede
         *                      (les indices du vecteur etant numerotes de 1 a taille)
         *              
         * @return      une reference sur le ieme element du vecteur          
         */
        Cell*& operator()(uint32_t i) ;

        Darray& operator=(const Darray& P);

        /**
         * @brief       Teste l egalite du vecteur avec un autre
         *
         * @param       P       vecteur de comparaison
         *
         * @return      true si et seulement si les vecteurs sont egaux
         */
        bool operator==(const Darray& P) const;

        void swap(Darray& P);

};

/**
 * @brief       Surcharge de l operateur de flux <<
 *
 * @param       out
 * @param       P
 *
 * @return           
 */
std::ostream& operator<<(std::ostream& out, const Darray& P);

/**
 * @brief       Surcharge de l operateur de flux >>
 *              Si le fichier contient plus de donnees que la taille du vecteur
 *              seules les taille premieres valeurs du fichier seront affectees 
 *              le vecteur
 *              Si le fichier contient moins de donnees que la taille du vecteur
 *              on affecte simplement les donnees du fichier (les autres valeurs 
 *              du vecteur resteront inchangees)
 *
 * @param       in
 * @param       P
 *
 * @return      
 */
std::istream& operator>>(std::istream& in, Darray& P);

#endif

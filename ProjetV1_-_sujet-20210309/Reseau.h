#ifndef __RESEAU_H__
#define __RESEAU_H__
#include "Chaine.h"
#include <stdlib.h>
#include <stdio.h>
typedef struct noeud Noeud;

/* Liste chainee de noeuds (pour la liste des noeuds du reseau ET les listes des voisins de chaque noeud) */
typedef struct cellnoeud {
    Noeud *nd;               /* Pointeur vers le noeud stock\'e */
    struct cellnoeud *suiv;         /* Cellule suivante dans la liste */
} CellNoeud;

/* Noeud du reseau */
struct noeud{
   int num;                        /* Numero du noeud */
   double x, y;                    /* Coordonnees du noeud*/
   CellNoeud *voisins;             /* Liste des voisins du noeud */
};

/* Liste chainee de commodites */
typedef struct cellCommodite {
    Noeud *extrA, *extrB;       /* Noeuds aux extremites de la commodite */
    struct cellCommodite *suiv;     /* Cellule suivante dans la liste */
} CellCommodite;

/* Un reseau */
typedef struct {
    int nbNoeuds;                   /* Nombre de noeuds du reseau */
    int gamma;                      /* Nombre maximal de fibres par cable */
    CellNoeud *noeuds;              /* Liste des noeuds du reseau */
    CellCommodite *commodites;      /* Liste des commodites a relier */
} Reseau;

/* Liste de liaisons */
typedef struct _liste_Liaisons{
    int *tab;                       /*tableau des numeros des extremites de la liaison*/
    struct _liste_Liaisons *suiv;   /*liaison suivante*/
} liste_L;

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y);
Reseau* reconstitueReseauListe(Chaines *C);
void ecrireReseau(Reseau *R, FILE *f);
int nbLiaisons(Reseau *R);
int nbCommodites(Reseau *R);
void afficheReseauSVGL2(Reseau *R, char* nomInstance);
void liberer_noeud(Noeud *nd);
void liberer_liste_noeuds(CellNoeud *lNd);
void liberer_liste_commodites(CellCommodite *l);
void liberer_reseau(Reseau *r);


#endif


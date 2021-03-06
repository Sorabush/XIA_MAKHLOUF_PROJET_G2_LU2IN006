#include "ArbreQuat.h"
#include "Chaine.h"
#include "Reseau.h"

//On cherche ici les maximums et minimums de chaque coordonnée en initialisant a la premiere valeur
void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax,double *ymax){
    Chaines* tmpC=C;
    if(tmpC==NULL){
        return;
    }
    
    CellChaine* tmpch=tmpC->chaines;
    if(tmpch==NULL){
        return;
    }
    //printf("Passee chaine\n");
    *xmin=tmpch->points->x;
    *xmax=tmpch->points->x;
    *ymin=tmpch->points->y;
    *ymax=tmpch->points->y;
    //printf("Passee chaine\n");
    CellPoint* tmpP=NULL;
    //printf("ALLO\n");

    while(tmpch!=NULL){
        tmpP=tmpch->points;
        while(tmpP!=NULL){
            if(tmpP->x<(*xmin)){
                //printf("ici les vals : tmpP->x = %lf et xmin= %lf\n",tmpP->x,*xmin);
                *xmin=(tmpP->x);
            }else if(tmpP->x>(*xmax)){
                //printf("ici l\n"); 
                *xmax=(tmpP->x);
            }
            if(tmpP->y<(*ymin)){
                *ymin=(tmpP->y);
            }else if(tmpP->y>(*ymax)){
                *ymax=(tmpP->y);
            }
            tmpP=tmpP->suiv;
            
        }
        //printf("chaque valeur est : xmax = %lf, xmin = %lf, ymax=%lf et ymin=%lf\n",*xmax,*xmin,*ymax,*ymin);
        tmpch=tmpch->suiv;
        
    }
    //printf("OK\n");
    //printf("au final chaque valeur est : xmax = %lf, xmin = %lf, ymax=%lf et ymin=%lf\n",*xmax,*xmin,*ymax,*ymin);
    //printf("OK\n");
}

//On alloue l'espace necessaire pour stocker un arbre mais il faudra liberer cet espace a la fin
ArbreQuat * creerArbreQuat(double xc, double yc, double coteX,double coteY){
    ArbreQuat* res=malloc(sizeof(ArbreQuat));
    res->xc=xc;
    res->yc=yc;
    res->coteX=coteX;
    res->coteY=coteY;
    res->noeud=NULL;
    res->so=NULL;
    res->se=NULL;
    res->no=NULL;
    res->ne=NULL;
    //printf("Passee l68\n");
    return res;
}

//Insere un noeud dans l'arbre, le noeud sera inserer lorsqu'une feuille vide sera trouvee
void insererNoeudArbre(Noeud * n,ArbreQuat** a, ArbreQuat* parent){
    if(n==NULL){
        return;
    }
    if(*a==NULL){
        //printf("Passee l76\n");
        double newcoteX=(parent->coteX)/2;
        double newcoteY=(parent->coteY)/2;
        double newxc;
        double newyc;
        //Dans l'ordre des tests so->se->no->ne
        
        //Version suivant l'enonce
        ArbreQuat* ajout;
        if(n->x<parent->xc && n->y<parent->yc){
            newxc=parent->xc/2;
            newyc=parent->yc/2;
            ajout=creerArbreQuat(newxc,newyc,newcoteX,newcoteY);
            ajout->noeud=n;
            //printf("Passee l87\n");
        }else if(n->x>=parent->xc && n->y<parent->yc){
            newxc=parent->xc+parent->xc/2;
            newyc=parent->yc/2;
            ajout=creerArbreQuat(newxc,newyc,newcoteX,newcoteY);
            ajout->noeud=n;
            //printf("Passee l91\n");
        }else if(n->x<parent->xc && n->y>=parent->yc){
            newxc=parent->xc/2;
            newyc=parent->yc+parent->yc/2;
            ajout=creerArbreQuat(newxc,newyc,newcoteX,newcoteY);
            ajout->noeud=n;
            //printf("Passee l95\n");
        }else if(n->x>=parent->xc && n->y>=parent->yc){
            newxc=parent->xc+parent->xc/2;
            newyc=parent->yc+parent->yc/2;
            ajout=creerArbreQuat(newxc,newyc,newcoteX,newcoteY);
            ajout->noeud=n;
        }else{
            printf("Probleme\n");
        }
        (*a)=ajout;
    }else if((*a)->noeud!=NULL){
        //On oublie pas de mettre a jour parent
        Noeud * ancienN=(*a)->noeud;
        (*a)->noeud=NULL;
        insererNoeudArbre(n,a,parent);
        insererNoeudArbre(ancienN,a,parent);


    }else if(*a!=NULL && ((*a)->noeud==NULL)){

        //En partant de l'enonce
        if(n->x<(*a)->xc && n->y<(*a)->yc){
            insererNoeudArbre(n,&((*a)->so),*a);
        }else if(n->x>=(*a)->xc && n->y<(*a)->yc){
            insererNoeudArbre(n,&((*a)->se),*a);
        }else if(n->x<(*a)->xc && n->y>=(*a)->yc){
            insererNoeudArbre(n,&((*a)->no),*a);
        }else if(n->x>=(*a)->xc && n->y>=(*a)->yc){
            insererNoeudArbre(n,&((*a)->ne),*a);
        }else{
            printf("Probleme \n");
        }
        //printf("Passee l239\n");
    }
}

//Recherche un noeud du reseau a partir d'un arbre si le noeud existe il sera renvoye sinon on cree le noeud et on le renvoie en allouant l'espace pour le noeud
Noeud* rechercheCreeNoeudArbre(Reseau* R,ArbreQuat** a,ArbreQuat* parent,double x,double y){
    if(*a==NULL){
        //printf("Passee l139\n");
        Noeud *ajout=malloc(sizeof(Noeud));
        ajout->voisins=NULL;
        ajout->num=(++R->nbNoeuds);
        ajout->x=x;
        ajout->y=y;
        
        CellNoeud* ajoutcell=malloc(sizeof(CellNoeud));
        ajoutcell->nd=ajout;
        if(R->noeuds!=NULL){
            ajoutcell->suiv=R->noeuds;
        }else{
            ajoutcell->suiv=NULL;
        }
        R->noeuds=ajoutcell;
        insererNoeudArbre(ajout,a,parent);
        
        //printf("Passee l146\n");
        return ajout;
    
    }else if((*a)->noeud!=NULL){
        if((*a)->noeud->x==x && (*a)->noeud->y==y){
            //printf("Passee l149\n");
            return (*a)->noeud;
        }
        //printf("Passee l154\n");
        Noeud *ajout=malloc(sizeof(Noeud));
        ajout->voisins=NULL;
        ajout->num=(++R->nbNoeuds);
        ajout->x=x;
        ajout->y=y;
        //printf("Passee l161\n");
        
        CellNoeud* ajoutcell=malloc(sizeof(CellNoeud));
        ajoutcell->nd=ajout;
        if(R->noeuds!=NULL){
            ajoutcell->suiv=R->noeuds;
        }else{
            ajoutcell->suiv=NULL;
        }
        R->noeuds=ajoutcell;
        
        //printf("Passee l197\n");
        insererNoeudArbre(ajout,a,parent);
        
        //printf("Passee l199\n");
        return ajout;
    }else if(*a!=NULL && ((*a)->noeud==NULL)){
        //printf("Passee l164\n");
        Noeud *ajout=NULL;
        
        //En partant de l'enonce
        if(x<(*a)->xc && y<(*a)->yc){
            ajout=rechercheCreeNoeudArbre(R,&((*a)->so),*a,x,y);
        }else if(x>=(*a)->xc && y<(*a)->yc){
            ajout=rechercheCreeNoeudArbre(R,&((*a)->se),*a,x,y);
        }else if(x<(*a)->xc && y>=(*a)->yc){
            ajout=rechercheCreeNoeudArbre(R,&((*a)->ne),*a,x,y);
        }else if(x>=(*a)->xc && y>=(*a)->yc){
            ajout=rechercheCreeNoeudArbre(R,&((*a)->no),*a,x,y);
        }else{
            //ne doit pas passer ici
            printf("Que se passe t il\n");
            return ajout;
        }

        return ajout;
        //printf("Passee l179\n");
        
    }else{
        printf("Que se passe t il 2\n");
        return NULL;
    }
}


//On reconstitue le reseau, on doit ajouter les voisins et les commodites en plus
Reseau* reconstitueReseauArbre(Chaines* C){
    
    Reseau *res=malloc(sizeof(Reseau));
    res->nbNoeuds=0;
    res->gamma=C->gamma;
    res->noeuds=NULL;
    res->commodites=NULL;

    double xmax,xmin,ymax,ymin;
    xmax=0;xmin=0;ymax=0;ymin=0;
    //printf("Passee\n");
    chaineCoordMinMax(C,&xmin,&ymin,&xmax,&ymax);
    //printf("Passee\n");
    //printf("Passee et valeur : %lf\n",xmin);
    //double coteX=sqrt(((xmax)-(xmin))*((xmax)-(xmin)));
    //double coteY=sqrt(((ymax)-(ymin))*((ymax)-(ymin)));
    /*double coteX=sqrt(((*xmax)-(*xmin))*((*xmax)-(*xmin)));
    double coteY=sqrt(((*ymax)-(*ymin))*((*ymax)-(*ymin)));*/
    ArbreQuat* abQ=creerArbreQuat(((xmax)+(xmin))/2,((ymax)+(ymin))/2,xmax-xmin,ymax-ymin);
    ArbreQuat* tmpab=abQ;
    ArbreQuat* parent=abQ;


    CellChaine *tmpC=C->chaines;
    while(tmpC){
        CellPoint *prec=tmpC->points;
        Noeud * ajoutp1=rechercheCreeNoeudArbre(res,&tmpab,parent,prec->x,prec->y);
        //printf("val de recup %lf et %lf\n",ajoutp1->x,ajoutp1->y);
        CellPoint *tmpP=NULL;
        Noeud * premiern=ajoutp1;
        if(prec!=NULL){
            tmpP=prec->suiv;
        }
        Noeud *derniern=NULL;
        //printf("Passee l217\n");
        while(tmpP!=NULL){
            //On ajoute le precedent aux voisins du noeud actuel
            Noeud* recup=rechercheCreeNoeudArbre(res,&tmpab,parent,tmpP->x,tmpP->y);
            
            int tmpr=0;
            CellNoeud *tmprecup=recup->voisins;
            while(tmprecup!=NULL){
                if(tmprecup->nd==ajoutp1){
                    tmpr=1;
                    break;
                    printf("OK\n");
                }
                tmprecup=tmprecup->suiv;
                
            }
            if(tmpr==0){
                CellNoeud *vois=malloc(sizeof(CellNoeud));
                vois->nd=ajoutp1;
                vois->suiv=recup->voisins;
                recup->voisins=vois;
            }

            CellNoeud * tmp1=ajoutp1->voisins;
            int trouve=0;
            while(tmp1!=NULL){
                if(tmp1->nd==recup){
                    trouve=1;
                    break;
                }
                tmp1=tmp1->suiv;
                
            }
            //printf("Passee l238\n");

            if(trouve==0){
                CellNoeud *voisduprec=malloc(sizeof(CellNoeud));
                voisduprec->nd=recup;
                voisduprec->suiv=ajoutp1->voisins;
                ajoutp1->voisins=voisduprec;
                //printf("Passee l245\n");
            }
            //printf("Passee l247\n");

            ajoutp1=recup;
            if(tmpP->suiv==NULL){
                derniern=recup;
            }
            tmpP=tmpP->suiv;
        }

        if(premiern!=NULL && derniern!=NULL){
            CellCommodite* commo=malloc(sizeof(CellCommodite));
            commo->extrA=premiern;
            commo->extrB=derniern;
            if(res->commodites==NULL){
                commo->suiv=NULL;
            }else{
                commo->suiv=res->commodites;
            }
            res->commodites=commo;
        }
        tmpC=tmpC->suiv;
    }
    detruire_arbre(parent);
    //printf("Reseau construit avec succes !!!!\n");
    
    return res;
}

//Libere l'espace attribue aux arbres
void detruire_arbre(ArbreQuat *a){
    if(a!=NULL){
        detruire_arbre(a->so);
        detruire_arbre(a->se);
        detruire_arbre(a->no);
        detruire_arbre(a->ne);
        free(a);
    }
}

//Fonction d'affichage fournie
void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}
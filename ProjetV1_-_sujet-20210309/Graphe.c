#include "Graphe.h"
#include "Struct_File.h"
#include "Struct_Liste.h"

void *afficheErreur(){
    printf("Erreur d'allocation memoire\n");
    return NULL;
}

// 7.1 Fonction de creation de graphe
Graphe *creerGraphe(Reseau *r){
    
    // Allocation memoire
    
    Graphe *g=(Graphe *)malloc(sizeof(Graphe));
    if(g==NULL){
        afficheErreur();
    }
    // Initialisation des differents champs

    g->nbsom=r->nbNoeuds;
    g->gamma=r->gamma;
    g->nbcommod=nbCommodites(r);
    g->T_som=(Sommet **)malloc(sizeof(Sommet *)*g->nbsom);
    if(g->T_som==NULL){
        free(g);
        afficheErreur();
    }
    for(int i = 0 ; i < g->nbsom ; i++){
        g->T_som[i]=(Sommet *)malloc(sizeof(Sommet));
        if(g->T_som[i]==NULL){
            for(int j = 0 ; j < i ; j++){
                free(g->T_som[j]);
            }    
            free(g->T_som);
            free(g);
            afficheErreur();
        }
        g->T_som[i]->num=0;
        g->T_som[i]->x=0;
        g->T_som[i]->y=0;
        g->T_som[i]->L_voisin=NULL;
    }

    g->T_commod=(Commod *)malloc(sizeof(Commod)*g->nbcommod);
    if(g->T_commod==NULL){
        for(int i = 0 ; i < g->nbsom ;i++)
            free(g->T_som[i]);
        free(g->T_som);
        free(g);
    }
    CellNoeud *listeNd=r->noeuds; // Liste des noeuds du reseau
    Sommet **tabSommets=g->T_som; 
    int trouve=0;
    for(int i = 0 ; i < g->nbsom ; i++){
        tabSommets[i]->num=listeNd->nd->num;
        tabSommets[i]->x=listeNd->nd->x;
        tabSommets[i]->y=listeNd->nd->y;
        CellNoeud *voisins=listeNd->nd->voisins; //Liste des voisins du noeud courant
        while(voisins){
            Cellule_arete *lv=tabSommets[i]->L_voisin; // Liste des aretes du sommet courant
            while(lv){
                if((lv->a->u==tabSommets[i]->num && lv->a->v==voisins->nd->num) || 
                (lv->a->u==voisins->nd->num && lv->a->v==tabSommets[i]->num)){
                    trouve=1;
                }
                lv=lv->suiv;
            }
            if(trouve==0){
                Cellule_arete *newCellArrete=(Cellule_arete *)malloc(sizeof(Cellule_arete));
                if(newCellArrete==NULL){
                    afficheErreur();
                }
                newCellArrete->a=NULL;
                newCellArrete->suiv=NULL;
                newCellArrete->a=(Arete *)malloc(sizeof(Arete));
                if(newCellArrete->a==NULL){
                    afficheErreur();
                }
                newCellArrete->a->u=tabSommets[i]->num;
                newCellArrete->a->v=voisins->nd->num;
                newCellArrete->suiv=tabSommets[i]->L_voisin;
                tabSommets[i]->L_voisin=newCellArrete;
                // Maj de la liste des aretes du voisin courant du sommet courant
                Sommet **tabSommets2=g->T_som;
                for( int j = 0 ; j < g->nbsom ; j++){
                    if(tabSommets2[j]->num==newCellArrete->a->v && tabSommets2[j]->num!=tabSommets[i]->num){
                        Cellule_arete *newCellArrete2=(Cellule_arete*)malloc(sizeof(Cellule_arete));
                        if(newCellArrete2==NULL){
                            afficheErreur();
                        }
                        newCellArrete2->suiv=NULL;
                        newCellArrete2->a=NULL;
                        newCellArrete2->a=newCellArrete->a;
                        newCellArrete2->suiv=tabSommets2[j]->L_voisin;
                        tabSommets2[j]->L_voisin=newCellArrete2;
                    }
                }
            }
            trouve=0;
            voisins=voisins->suiv;
        }   
        listeNd=listeNd->suiv;
    }
    CellCommodite *listeC=r->commodites; // Liste des commodites du reseau
    for(int i = 0 ; i < g->nbcommod ; i++){
        g->T_commod[i].e1=listeC->extrA->num;
        g->T_commod[i].e2=listeC->extrB->num;
        listeC=listeC->suiv;
    }
    return g;
}

// 7.2 Fonction de calcul du nombre minimal d'aretes entre deux sommets dans une chaine

int plus_petit_nombre_aretes(Graphe *g, int u,int v){
    int *D=malloc((g->nbsom)*sizeof(int));
    
    if(D==NULL){
        printf("Erreur d'allocation memoire\n");
        return -1;
    }   

    int *visit=(int*)malloc(g->nbsom*sizeof(int));
    
    if(visit==NULL){
        printf("Erreur d'allocation memoire\n");
        free(D);
        return -1;
    }
    
    for(int i = 0 ; i < g->nbsom ; i++){
        visit[i]=0;
        D[i]=0;
    }
    visit[u]=1;
    S_file *f=(S_file*)malloc(sizeof(S_file));
    if(f==NULL){
        printf("Erreur d'allocation memoire\n");
        return -1;
    }
    Init_file(f);
    enfile(f,u);
    while(! (estFileVide(f))){
        int recup=defile(f);
        //printf("je passe\n");
        Cellule_arete *v_cour=g->T_som[u]->L_voisin;
        //printf("%d: %d\n",v_cour->a->u,v_cour->a->v);
        //printf("je passe2\n");
        while(v_cour){
            int d;
            if(v_cour->a->u==recup){
                d=v_cour->a->v;
            }
            else{
                d=v_cour->a->u;
            }
            if(visit[d]==0){
                visit[d]=1;
                D[d]=D[recup]+1;
                enfile(f,d);
            
            }
            v_cour=v_cour->suiv;
        }
    }
    int res=D[v];
    //printf("%d\n",res);
    free(visit);
    free(f);
    free(D);
    return res;
}

//7.3  Fonction de calcul d'un tableau d’entiers contenant à l'indice i le numero du pere du sommet de numero i

int* pere(Graphe* g, int u, int s){
    //tableau des distances de r vers les autres sommets
    int* D=(int*)malloc(g->nbsom*sizeof(int));

    //tableau des peres des sommets
    int* P=(int*)malloc(g->nbsom*sizeof(int));
    int* visit=(int*)malloc(g->nbsom*sizeof(int));
    for(int i=0;i<g->nbsom;i++){
        visit[i]=0;
        D[i]=0;
        P[i]=-1;
        }

    P[u]=0;
    visit[u]=1;
    S_file* S=(S_file*)malloc(sizeof(S_file));
    Init_file(S);
    enfile(S,u);

    int v=-1;
    while ((!(estFileVide(S))) && (v!=s)){
        int d=defile(S);
        Cellule_arete* cour=g->T_som[d]->L_voisin;
        while((cour!=NULL) && (v!=s)){
            if(cour->a->u==d){
                v=cour->a->v;
            }
            else{
            v=cour->a->u;
            }

            if(visit[v]==0){
                visit[v]=1;
                D[v]=D[d]+1;
                P[v]=d;
                enfile(S,v);
            }
            cour=cour->suiv;
        }
    }
    free(visit);
    free(S);
    free(D);
    return P;
}

ListeEntier chemin(Graphe* g, int r, int s){
    int* P=pere(g, r, s);
    ListeEntier l;
    Init_Liste(&l);
    ajoute_en_tete(&l, s);
    int a=s;
    while(a!=r){
        ajoute_en_tete(&l, P[a]);
        a=P[a];
    }
    return l;
}

// Fonctions de liberation memoire

void liberer_sommets(Sommet **T_som, int nbsom){
    for(int i = 0 ; i < nbsom ; i++){
        if(T_som[i]!=NULL){
        while(T_som[i]->L_voisin){
                Cellule_arete *ca=T_som[i]->L_voisin->suiv;           
                //liberer_arete(T_som[i]->L_voisin);
                liberer_arete(T_som[i]->L_voisin);
                T_som[i]->L_voisin=ca;
            }
            free(T_som[i]);
            T_som[i]=NULL;
        }
    }
    free(T_som);
}
void liberer_arete(Cellule_arete *CA){
    if(CA==NULL)
        return;
    if(CA->a->u==-1 && CA->a->v==-1)
        free(CA->a);
    else{
        CA->a->u=-1;
        CA->a->v=-1;
    }
    free(CA);
}
void liberer_graphe(Graphe *g){
    //printf("passe\n");
    liberer_sommets(g->T_som, g->nbsom);
    //printf("passe\n");
    free(g->T_commod);
    free(g);
}

// 7.4 Fonction de reoganisation d'un reseau

int reorganiseReseau(Reseau* r){
    //creation du graphe 
    Graphe* g=creerGraphe(r);

    int nbsom=g->nbsom;
    int nbcommod=g->nbcommod;
    ListeEntier* L=(ListeEntier*)malloc(nbcommod*sizeof(ListeEntier));
    
    if(L==NULL){
        afficheErreur();
    }

    Commod c;
    int i;
    int j;
    //calcule la plus courte chaine pour chaque commodite
    for(i=0;i<nbcommod;i++){
        c=g->T_commod[i];
        L[i]=chemin(g, c.e1, c.e2);
    }

    int** mat=(int**)malloc(nbsom*sizeof(int*));

    if(mat==NULL){
        afficheErreur();
    }
    for(i=0;i<nbsom;i++){
        mat[i]=(int*)malloc(nbsom*sizeof(int));
        if(mat[i]==NULL)
            afficheErreur();
    }
    for(i=0;i<nbsom;i++){
        for(j=0;j<nbsom;j++){
            mat[i][j]=0;
        }
    }

    ListeEntier l;
    int u=L[0]->u;
    int v;
    for(i=1;i<nbcommod;i++){
        l=L[i];
        while(l!=NULL){
            v=l->u;
            mat[u][v]++;
            mat[v][u]++;
            u=v;
            l=l->suiv;
        }
    }

    int reorganise=1;
    for(i=0;i<nbsom;i++){
        for(j=0;j<nbsom;j++){
            if(mat[i][j]>=g->gamma){
                reorganise=0;
                break;
            }
        }
    }

    for(i=0;i<nbcommod;i++){
        free(L[i]);
    }
    free(L);
    for(i=0;i<nbsom;i++){
        free(mat[i]);
    }
    free(mat);
    liberer_graphe(g);
    return reorganise;
}
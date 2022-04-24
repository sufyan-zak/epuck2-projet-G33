#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// #include <noeud.h>

#define NB_NODES 7 

struct Noeud {
	int tab_liens[10];		// if the number is != no_link, it is a link. The number corresponds to the index in the matrix tn
	float access;
	int in;					// Boolean value, either 0 (false) or 1 (true)
	unsigned int parent;
	int uid;

	// spatial position
	int x;
	int y;
};
const int no_link = -1;


int main() {

	// SI J'ENLEVE LES INITIATLISATIONS QUI SUIVENT CA PART EN CACAHUETE ! SUREMENT EN LIEN AVEC LA NON-INITIALISATION de tn à la ligne 32
	int i, j = 0;
	int uid, x, y = 0;
	int link = 0;
	
	struct Noeud *tn[NB_NODES];
	for (int i = 0; i < NB_NODES; ++i){
		tn[i] = malloc(sizeof *tn[i]);}
	
	FILE *fptr;
	fptr = fopen("city.txt", "r");

	if (fptr == NULL){
		printf("Error opening file!\n");
		exit(1);
	}
	
//	while(!feof(fptr)){					// leads to an error when reaching the end of the file
	while(i < NB_NODES) {
		j = 0;
		fscanf(fptr, "%d %d %d", &uid, &x, &y);
		tn[i]->uid = uid;
		tn[i]->x = x;
		tn[i]->y = y;
		_Bool stay = 1;
		while(stay) {
			fscanf(fptr, "%d", &link);
			tn[i]->tab_liens[j] = link;
			if(link == no_link)		stay = 0;	
			++j;
		}
		i++;
	}

	fclose(fptr);

	
	for(int i = 0; i <  NB_NODES ; ++i) {
		printf("uid of nd : %d\n", tn[i]->uid); 
		printf("x of nd : %d\n", tn[i]->x);
		printf("y of nd : %d\n", tn[i]->y);
		for(int j = 0 ; tn[i]->tab_liens[j] != no_link ; ++j) {
			printf("link of nd : %d\n", tn[i]->tab_liens[j]);
		}
	}		
	
	return 0;
}

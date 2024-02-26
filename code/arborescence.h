#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

struct arbre;

typedef struct arbre arbre;

arbre* creer_arbre(arbre* previous, int type, char name[], char absolute_path[]);

void ajouter_suivants(arbre* noeud);

struct arbre {
	arbre* previous;
	arbre** next_dirs;
	arbre** next_files;
	int type;
	int n_dirs;
	int n_files;
	char* absolute_path;
	char* name;
};
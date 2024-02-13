#include "arborescence.h"

/*
Constructor of arbre
Is the only function that needs to be called, performs the push of the sons directories and files
Parameters :
  previous (arbre*) : Previous arbre (usually NULL)
  type (int) : type of the node (0 is file, 1 is dir)
  name (char[]) : name of the current file or dir
  absolute_path (char[]) : absolute_path of the current file or dir
Returns :
  head (arbre*) : current node constructed from parameters
*/
arbre* creer_arbre(arbre* previous, int type, char name[], char absolute_path[]) {
	// Allocation of the return node
	arbre* head = (arbre*) malloc(sizeof(arbre));
	// Value attribution for arbre attributes
	head->previous = previous;
	head->name = (char*) malloc((strlen(name)+1)*sizeof(char));
	strcpy(head->name, name);
	head->absolute_path = (char*) malloc((strlen(absolute_path)+1)*sizeof(char));
	strcpy(head->absolute_path, absolute_path);
	if (type == 0) {
		// Value attribution for files
		head->next_dirs = NULL;
		head->next_files = NULL;
		head->n_dirs = 0;
		head->n_files = 0;
		head->type = type;
	} else {
		// Value attribution for dirs
		head->type = 1;
		// Call for sons dirs and files construction
		ajouter_suivants(head);
	}
	return head;
}

/*
Returns if the path represents a file or a dir
Parameters :
  path (char[]) : absolute_path of the current file or dir
Returns :
  bool (int) : 0 for file, 1 for dir, -1 if error
*/
int isDirectory(const char* path) {
    struct stat statBuffer;
    if (stat(path, &statBuffer) == 0) {
        return S_ISDIR(statBuffer.st_mode);
    }
    return -1;
}

/*
Creates all sons nodes of the node in parameter and push them in noeud->next_dirs and noeud->next_files
Performs a 'ls' on the absolute path of the node in parameter
Parameters :
  noeud (arbre*) : current node created from 'creer_arbre' function
Returns :
  void
*/
void ajouter_suivants(arbre* noeud) {
	// 'if' prevents doing instructions if parameter node is a file
	if (noeud->type == 1) {
		// 'ls' of noeud->absolute_path
		char command[100];
		snprintf(command, sizeof(command), "ls %s", noeud->absolute_path);

		FILE* lsOutput = popen(command, "r");
		if (lsOutput == NULL) {
	        perror("Error executing ls");
	        return;
	    }

	    char** outputLines = NULL;
	    int lineCount = 0;
	    // Storage of 'ls' output
	    char buffer[128];
	    while (fgets(buffer, sizeof(buffer), lsOutput) != NULL) {
	        // Remove the newline character at the end
	        buffer[strcspn(buffer, "\n")] = '\0';

	        // Allocate memory for the new line
	        char* line = strdup(buffer);

	        // Resize the array of strings
	        outputLines = realloc(outputLines, (lineCount + 1) * sizeof(char*));
	        if (outputLines == NULL) {
	            perror("Memory allocation error");
	            exit(1);
	        }

	        // Add the new line to the array
	        outputLines[lineCount] = line;
	        lineCount++;
	    }
	    // Closing of the sys pipe
	    pclose(lsOutput);

	    // Calculation of number of dirs/files
	    int taille_dirs = 0;
	    int taille_files = 0;
	    for (int i=0; i<lineCount; i++) {
			char* absolute_path = (char*) malloc((strlen(noeud->absolute_path)+strlen(outputLines[i])+2)*sizeof(char));
			strcpy(absolute_path, noeud->absolute_path);
			strcat(absolute_path, "/");
			strcat(absolute_path, outputLines[i]);
			if (isDirectory(absolute_path)) {
				taille_dirs++;
			} else {
				taille_files++;
			}
	    }

	    // Creation of next_dirs/next_files
	    arbre** nexts_dirs = (arbre**) malloc((taille_dirs)*sizeof(arbre*));
	    arbre** nexts_files = (arbre**) malloc((taille_files)*sizeof(arbre*));
	    int j = 0;
	    int k = 0;
	    for (int i=0; i<lineCount; i++) {
	    	// Allocation of the current node
	    	arbre* next = (arbre*) malloc(sizeof(arbre));
	    	// Value attribution for arbre attributes
	    	next->previous = noeud;
			next->name = (char*) malloc((strlen(outputLines[i])+1)*sizeof(char));
			strcpy(next->name, outputLines[i]);
			next->absolute_path = (char*) malloc((strlen(noeud->absolute_path)+strlen(outputLines[i])+2)*sizeof(char));
			strcpy(next->absolute_path, noeud->absolute_path);
			strcat(next->absolute_path, "/");
			strcat(next->absolute_path, outputLines[i]);
			next->type = isDirectory(next->absolute_path);
			next->next_dirs = NULL;
			next->next_files = NULL;
			next->n_dirs = 0;
			next->n_files = 0;
			if (next->type == 0) {
				nexts_files[j] = next;
				j++;
			} else {
				nexts_dirs[k] = next;
				k++;
			}
	    }
	    // Value attribution for arbre attributes
	    noeud->n_dirs = taille_dirs;
	    noeud->n_files = taille_files;
	    noeud->next_dirs = nexts_dirs;
	    noeud->next_files = nexts_files;
	    return;
	} else {
		return;
	}
}
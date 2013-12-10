#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node{
	char* name;
	struct node *nextSib;
	struct node *firstChild;
	char type; /* 'd' == directory; 'f' == file */
}NODE;

typedef struct tree{
	NODE* root;
	int depth;
}TREE;

TREE* tree;

int index_of_first_slash(char* str){
	char tmp = *str;
	int i = 0;

	while(tmp != '\0'){
		if(tmp == '/') return i;
		i++;
		tmp = *(str+i);

	}
	return -1; /* '/' not found in str */
}

TREE* create_tree(){
	
	TREE* res = (TREE*)malloc(sizeof(TREE));
	NODE* root = (NODE*)malloc(sizeof(NODE));

	if(res == NULL)        printf("error allocating tree\n");
	else if(root == NULL)  printf("error allocating root\n");
	else{
		/* root of every tree is "/" */
		root->name = "/";
		root->nextSib = NULL;
		root->firstChild = NULL;
		root->type = 'd';
		
		res->root = root;
		res->depth = 1;
	}

	return res;
}

void add_node(TREE* tree, char* location){
	int slashIndex, location_length;
	char *dir, *tmp;
	NODE *parent, *child;
	int pathLength = 0;

	/* allocate memory for new_node */
	NODE* new_node = (NODE*)malloc(sizeof(NODE));
	if(new_node == NULL){
		printf("error allocating node\n");
		return;
	}

	location_length = strlen(location);
	dir = (char*)malloc(sizeof(char)*location_length);
	tmp = (char*)malloc(sizeof(char)*location_length);

	if(location[location_length-1] == '/'){
		new_node->type = 'd';
		/*remove last '/' */
		strncpy(tmp, location, location_length-1);
	}else{
		new_node->type = 'f';
		strcpy(tmp, location);
	}

	/* initialize new_node */
	new_node->nextSib = NULL;
	new_node->firstChild = NULL;

	if(tree->root == NULL){  /*tree is empty*/
		tree->root = new_node;
		tree->depth = 1;
	}
	else{
		slashIndex = index_of_first_slash(location);
		if(slashIndex != -1){
			parent = tree->root;
			while(slashIndex != -1 && slashIndex != (strlen(tmp) - 1)){
			/*divide string in 2 pieces --> dir=before '/'; tmp=after '/'; discard '/'*/
				strncpy(dir, tmp, slashIndex);
				strncpy(tmp, tmp + slashIndex + 1, location_length - slashIndex);

				if(strcmp(dir, "") == 0){
				/* make root the parent */
					parent = tree->root;
					pathLength = 1;
				}else if(strcmp(dir, parent->name) != 0){
				/* loop through children of tmpNode to find dir */
					if(parent->firstChild != NULL){
						parent = parent->firstChild;
						pathLength++;
					}
					while(strcmp(parent->name, dir) != 0){   
						parent = parent->nextSib;
						if(parent == NULL){
							printf("Error parent %s could not be found in tree. Error on path: %s\n", dir, location);
							return;
						}
					}
				}
				slashIndex = index_of_first_slash(tmp);
			}
			/*DIR == PARENT OF TMP       TMP == NAME OF NEW NODE*/
			/* add new_node to the list of children of parent */
			new_node->name = tmp;
			if(parent->firstChild == NULL){ /*special case: parent has no children */
				parent->firstChild = new_node;
				pathLength++;
			}else{
				child = parent->firstChild;
				while(child->nextSib != NULL){ /*find last child node in the linked list*/
					if(strcmp(tmp, child->name) == 0){
						printf("Error: this element already exists\tNode name = %s\n", child->name);
						return;
					}
					child = child->nextSib;
				}
				child->nextSib = new_node;
				pathLength++;
			}
		}
	}
	if(pathLength > tree->depth) tree->depth = pathLength;

}


void deparse_recursive(TREE* tree, NODE* root, FILE* out){
	NODE* tmp;
	char* str;
	str = (char*)malloc(sizeof(char)*1000);
	

	strcat(str, "<");
	strcat(str, root->name);
	strcat(str, ">");

	if(root->type == 'f'){
	//handle file contents
	}
	
	if(root!=tree->root){
		fprintf(out, "%s\n", str);
		strcpy(str, "");	
	}	
	
	tmp = root->firstChild;
	while(tmp!=NULL){
		deparse_recursive(tree, tmp, out);
		tmp = tmp->nextSib;
	}
	strcat(str, "</");
	strcat(str, root->name);
	strcat(str, ">");
	if(root!=tree->root){
	fprintf(out, "%s\n", str);
	strcpy(str, "");
	}
	
	free(str);

}

void deparse(TREE* tree){
	FILE *file = fopen("parsedexample.xml", "w");
	fclose(file);
	fopen("parsedexample.xml", "a");
	deparse_recursive(tree, tree->root, file);
	fclose(file);
}

int main(int argc, char *argv[])
{
	int length;

	tree = create_tree();

	static const char filename[] = "path.txt";
	FILE *file = fopen (filename, "r");
	if(file != NULL) {
		char line [128]; 
		while (fgets(line, sizeof line, file) != NULL){ 
			//printf("%s",line); 
			length = strlen(line);
			if(line[length-1] == '\n')
   				line[length-1] = 0;
			//change to mkdir and then add node in mkdir 
			add_node(tree, line);
		}
		fclose (file);
	}else{
		perror (filename); 
	}

	deparse(tree);
}

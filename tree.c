#include <stdlib.h>
#include <stdio.h>
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

/* given a string, return the index of the first instance of the '/' char */
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

/* given a tree and a path, return the node pointer associated with that path 
 * return null if the path does not exist
 */
NODE* get_node(TREE* tree, char* path){
	char *dir, *tmp;
	int pathLength, idOfSlash1;
	NODE* curNode;

	if(tree == NULL){
		printf("tree is empty\n");
		return NULL;
	}else if(tree->root == NULL){
		printf("tree is not initialized\n");
		return NULL;
	}

	pathLength = strlen(path);

	dir = (char*)malloc(sizeof(char) * pathLength);
	tmp = (char*)malloc(sizeof(char) * pathLength);
	strcpy(tmp, path);

	curNode = tree->root;

	if(tmp[0] == '/'){ //take care of leading slash
		strcpy(dir, "/");
		strcpy(tmp, tmp+1);
	}

	while(strcmp(tmp, "") != 0){
		if(curNode == NULL){
			printf("ERROR: path DNE");
			return NULL;
		}
		while(strcmp(curNode->name, dir) != 0){
			curNode = curNode->nextSib;
			if(curNode == NULL){
				printf("ERROR: path DNE");
				return NULL;
			}
		}
		idOfSlash1 = index_of_first_slash(tmp);
		strncpy(dir, tmp, idOfSlash1);
		strncpy(tmp, tmp + idOfSlash1 + 1, pathLength - idOfSlash1);
		curNode = curNode->firstChild;
	}
	if(strcmp(curNode->name, dir) == 0){
		return curNode;
	}
	else{
		printf("ERROR: path DNE");
		return NULL;
	}
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

/* given a path and a tree, create a new node, 
 * and add it to the tree in correct place (rep'd by location)
 */
 /*******TODO: ERROR CHECK - IF PATH TO LOCATION DNE -> PRINT ERROR AND RETURN*******/
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

/* given a tree and a path, find the node this path represents 
 * and print all of its children
 */
void printAllChildren(TREE* tree, char* path){
	NODE* dir;
	NODE* child;

	dir = get_node(tree, path);
	if(dir == NULL){
		printf("ERROR: Path does not exist\n");
		return;
	}

	child = dir->firstChild;
	if(child == NULL){
		printf("%s is empty!\n", dir->name);
	}else{
		while(child != NULL){
			printf("%s\n", child->name);
			child = child->nextSib;
		}
	}
}

/* helper function to print tree recursively */
/*TODO: consider changing to iterative function*/
void print_tree_recursive(TREE* tree, NODE* root, char* path){
	NODE* tmp;
	char* path2;

	path2 = (char*)malloc(sizeof(char)*500);

	//strcat(path, "/");
	strcat(path, root->name);
	if(root->type == 'd' && strcmp(root->name, "/") != 0)		strcat(path, "/");
	
	printf("Path of %s = %s\n", root->name, path);

	tmp = root->firstChild;
	while(tmp != NULL){
		strcpy(path2,path);
		print_tree_recursive(tree,tmp,path2);
		tmp = tmp->nextSib;
	}

	free(path2);
}

/* given a tree: print the path for each node in the tree */
void print_tree(TREE* tree){
	char* path;

	path = (char*)malloc(sizeof(char) * 500);

	print_tree_recursive(tree, tree->root, path);
	free(path);
}

void destroy_tree_recurssive(TREE* tree, NODE* node){
	//base case: leaf & last sib
	if(node->firstChild == NULL && node->nextSib == NULL){
		free(node);
		return;
	}else if(node->nextSib == NULL){ //recurssive case 1: last Sib ONLY -> remove node and all children
		destroy_tree_recurssive(tree, node->firstChild);
		free(node);
		return;
	}else if(node->firstChild == NULL){ //recurssive case 3: leaf ONLY -> remove node and all sibs
		destroy_tree_recurssive(tree, node->nextSib);
		free(node);
		return;
	}else{ //recurssive case 3: node has sibs and cildren -> remove node & all sibs & all children
		destroy_tree_recurssive(tree, node->nextSib);
		destroy_tree_recurssive(tree, node->firstChild);
		free(node);
		return;
	}
}

void destroy_tree(TREE* tree){
	destroy_tree_recurssive(tree,tree->root);
}

void big_tree_test(){
	//level 0
	TREE* tree = create_tree();

	if(tree != NULL){
		//level 1
		add_node(tree,"/dir1/");
		add_node(tree,"/dir2/");
		add_node(tree,"/dir3/");
		//level 2
		add_node(tree,"/dir1/dir11/");
		add_node(tree,"/dir1/file12");
		add_node(tree,"/dir1/file13");
		add_node(tree,"/dir2/file21");
		add_node(tree,"/dir2/dir22/");
		add_node(tree,"/dir3/file31");
		//level 3
		add_node(tree,"/dir1/dir11/dir111/");
		add_node(tree,"/dir1/dir11/dir112/");
		add_node(tree,"/dir1/dir11/dir113/");
		add_node(tree,"/dir2/dir22/dir221/");
		add_node(tree,"/dir2/dir22/file222");
		//level 4
		add_node(tree,"/dir1/dir11/dir111/dir1111/");
		add_node(tree,"/dir1/dir11/dir111/dir1112/");
		add_node(tree,"/dir1/dir11/dir111/dir1113/");
		add_node(tree,"/dir1/dir11/dir112/file1121");
		add_node(tree,"/dir1/dir11/dir112/dir1122/");
		add_node(tree,"/dir1/dir11/dir113/file1131");
		add_node(tree,"/dir2/dir22/dir221/dir2211/");
		//level 5
		add_node(tree,"/dir1/dir11/dir111/dir1112/file11121");
		add_node(tree,"/dir1/dir11/dir111/dir1113/file11131");
		add_node(tree,"/dir1/dir11/dir111/dir1113/file11132");
		add_node(tree,"/dir1/dir11/dir112/dir1122/file11221");
		add_node(tree,"/dir2/dir22/dir221/dir2211/dir22111/");
		//level 6
		add_node(tree,"/dir2/dir22/dir221/dir2211/dir22111/dir221111/");        
		//level7
		add_node(tree,"/dir2/dir22/dir221/dir2211/dir22111/dir221111/file2211111");    
	}

	print_tree(tree);
	printAllChildren(tree, "/dir1/dir11/");
	destroy_tree(tree);
}

void little_tree_test(){
	//level 0
	TREE* tree = create_tree();

	if(tree != NULL){
		//level 1
		add_node(tree,"/dir1/");
		add_node(tree,"/dir2/");
		add_node(tree,"/dir3/");
		//level 2
		add_node(tree,"/dir1/dir11/");
		add_node(tree,"/dir1/file12");
		add_node(tree,"/dir1/file13");
		add_node(tree,"/dir2/file21");
		add_node(tree,"/dir2/dir22/");
		add_node(tree,"/dir3/file31");  
		//level 3
		add_node(tree,"/dir1/dir11/dir111/");
		add_node(tree,"/dir1/dir11/dir112/");
		add_node(tree,"/dir1/dir11/dir113/");
		//level 4
		add_node(tree,"/dir1/dir11/dir111/dir1111/");
		add_node(tree,"/dir1/dir11/dir111/dir1112/");
	}

	//  printf("%s\n", tree->root->name);
	printf("num levels = %d\n", tree->depth);

	//  printf("%s\n", tree->root->firstChild->name);
	//print_tree(tree);
	destroy_tree(tree);
}

int main(){
	//big_tree_test();
	//little_tree_test();
	TREE* tree;
	int length;

	tree = create_tree();

	static const char filename[] = "path.txt";
	FILE *file = fopen (filename, "r");
	if(file != NULL) {
		char line [128]; /* or other suitable maximum line size */
		while (fgets(line, sizeof line, file) != NULL){ /* read a line */
			//printf("%s",line); /* write the line */
			length = strlen(line);
			if(line[length-1] == '\n')
   			line[length-1] = 0;
			add_node(tree, line);
		}
		fclose ( file );
	}else{
		perror ( filename ); /* why didn't the file open? */
	}

   print_tree(tree);
   //fclose(file);
	
	// return 0;
}




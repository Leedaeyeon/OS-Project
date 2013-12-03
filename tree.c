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
		if(slashIndex == -1){
			/*no '/' in location --> not a directory*/
		}else{
			parent = tree->root;
			//    while(strlen(tmp) != 0){
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
							printf("Error parent %s could not be found in tree\n", dir);
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
					child = child->nextSib;
				}
				child->nextSib = new_node;
				pathLength++;
			}
		}
	}
	if(pathLength > tree->depth) tree->depth = pathLength;

}

void printLevel(TREE* tree, int level){
	NODE *node1, *node2;
	int i;
	char* path;

	path = (char*)malloc(sizeof(char)*1000);
	
	if(level < 0 || level > (tree->depth - 1)){
		printf("Level entered is invalid\n");
		return;
	}

	/*************************STRCAT DOES NOT WORK THIS WAY**********************************
	**************************MUST CHANGE TO SYNTAX: strcat(char* dest, char* src)**********/

	if(level == 0){
		//print root
	//	printf("root:\n%s\n", tree->root->name);
	}else if(level == 1){
		node1 = tree->root->firstChild;
		while(node1 != NULL){
			if(node1->type == 'f')			printf("/%s\n", node1->name);
			else if(node1->type == 'd')	printf("/%s/\n", node1->name);
			node1 = node1->nextSib;			
		}
	}else{
		/* make node1 = first node in (level - 1) */
		node1 = tree->root;
		for(i=0; i<(level-1); i++){
			while(node1->firstChild == NULL){
				node1 = node1->nextSib;
				if(node1 == NULL){
					printf("\nERROR\n");
					return;
				}
			}
			strcat(path, "/");
			strcat(path, node1->name);
			node1 = node1->firstChild;
		}
		printf("\n");
		/* loop through level-1 */
		while(node1 != NULL){
		//	printf("sub-directories of %s:\n", node1->name);
			/* loop through children of node1 */
			node2 = node1->firstChild;
			while(node2 != NULL){
			//print node 2
		//		printf("%s\n", node2->name);
				if(node2->type == 'f')			printf("%s%s/%s\n", path, node1->name, node2->name);
				else if(node2->type == 'd')	printf("%s%s/%s/\n", path, node1->name, node2->name);
				node2 = node2->nextSib;
			}
			node1 = node1->nextSib;
		}
	}
}

void print_tree(TREE* tree){
	int i;

	// for(i=0; i < tree->depth; i++){
	for(i=0; i < tree->depth; i++){
	//	printf("--------LEVEL %d--------\n",i);
		printLevel(tree,i);
	}
}

print_tree2_recursive(TREE* tree, NODE* root, char* path){
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
		print_tree2_recursive(tree,tmp,path2);
		tmp = tmp->nextSib;
	}
}

void print_tree2(TREE* tree){
	char* path;

	path = (char*)malloc(sizeof(char) * 500);

	print_tree2_recursive(tree, tree->root, path);
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

	//  printf("%s\n", tree->root->name);
	printf("num levels = %d\n", tree->depth);

	//  printf("%s\n", tree->root->firstChild->name);
	print_tree2(tree);
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
	print_tree2(tree);
}

int main(){
	big_tree_test();
	//little_tree_test();
	

	return 0;
}




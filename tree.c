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

  /* allocate memory for new_node */
  NODE* new_node = (NODE*)malloc(sizeof(NODE));
  if(new_node == NULL){
    printf("error allocating node\n");
    return;
  }

  /* initialize new_node */
  new_node->nextSib = NULL;
  new_node->firstChild = NULL;

  if(tree->root == NULL)  /*tree is empty*/
    tree->root = new_node;
  else{
    slashIndex = index_of_first_slash(location);
    if(slashIndex == -1){
      /*no '/' in location --> not a directory*/
    }else{
      location_length = strlen(location);
      dir = (char*)malloc(sizeof(char)*location_length);
      tmp = (char*)malloc(sizeof(char)*location_length);
      strcpy(tmp, location);

      parent = tree->root;
      //    while(strlen(tmp) != 0){
      while(slashIndex != -1 && slashIndex != (strlen(tmp) - 1)){
	/*divide string in 2 pieces --> dir=before '/'; tmp=after '/'; discard '/'*/
	strncpy(dir, tmp, slashIndex);
	strncpy(tmp, tmp + slashIndex + 1, location_length - slashIndex);

	if(strcmp(dir, "") == 0){
	  /* make root the parent */
	  parent = tree->root;
	}else if(strcmp(dir, parent->name) != 0){
	  /* loop through children of tmpNode to find dir */
	  if(parent->firstChild != NULL)  parent = parent->firstChild;
	  while(parent->nextSib != NULL){   
	    parent = parent->nextSib;
	  }
	}
	slashIndex = index_of_first_slash(tmp);
      }
      printf("parent = %s\n", parent->name);
      /*DIR == PARENT OF TMP       TMP == NAME OF NEW NODE*/
      /* add new_node to the list of children of parent */
      new_node->name = tmp;
      if(parent->firstChild == NULL){ /*special case: parent has no children */
	parent->firstChild = new_node;
      }else{
	child = parent->firstChild;
	while(child->nextSib != NULL){ /*find last child node in the linked list*/
	  child = child->nextSib;
	}
	child->nextSib = new_node;
      }
    }
  }

}

int main(){
  TREE* tree = create_tree();

  if(tree != NULL){
    add_node(tree,"/dir1/");
    printf("---------------------\n");
    add_node(tree,"/dir1/dir2/");
    add_node(tree,"/dir1/dir2/dir3/");
  }

  printf("%s\n", tree->root->name);

  //  printf("%s\n", tree->root->firstChild->name);


  return 0;
}

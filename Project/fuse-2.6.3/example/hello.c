/*
    FUSE: Filesystem in Userspace
    Copyright (C) 2001-2006  Miklos Szeredi <miklos@szeredi.hu>

    This program can be distributed under the terms of the GNU GPL.
    See the file COPYING.

    gcc -Wall `pkg-config fuse --cflags --libs` hello.c -o hello
*/

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

typedef struct node{
	char* name;
	struct node *nextSib;
	struct node *firstChild;
	char type; /* 'd' == directory; 'f' == file */
	char* fileContents;
}NODE;

typedef struct tree{
	NODE* root;
	int depth;
}TREE;

static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";
static const char *test_path = "/test123";
static const char *test_sd_path = "/subdir1";
static const char *teststr = "Test me!\n";

static const char *file_path[1000];
static const char *file_str[1000];

NODE* mynode;
TREE* mytree;

TREE* create_tree(){
	
	TREE* res1 = (TREE*)malloc(sizeof(TREE));
	NODE* root = (NODE*)malloc(sizeof(NODE));

	if(res1 == NULL)        printf("error allocating tree\n");
	else if(root == NULL)  printf("error allocating root\n");
	else{
		/* root of every tree is "/" */
		root->name = "/";
		root->nextSib = NULL;
		root->firstChild = NULL;
		root->type = 'd';
		
		res1->root = root;
		res1->depth = 1;
	}

	return res1;
}


static int hello_mkdir(const char *path, mode_t mode)
{
	printf("hello_mkdir\n");
	printf("mkdir path: %s\n", path);

    int res;
    res = mkdir(path, mode);

	if (res == -1){
		//File exists
		//printf("mkdir file exists\n");
        	return -errno;
	}
	if(res ==0){
	/* if file does not exist call add node here to add to the node with path as arg*/
	
	}

    return 0;
}

static int hello_rmdir(const char *path){
	printf("hello_rmdir\n");

	int res;

   	 res = rmdir(path);
   	 if (res == -1){
		return -errno;
	}

	    return 0;
}

static int hello_getattr(const char *path, struct stat *stbuf)
{
	printf("hello_getattr\n");
	//printf("path: %s\n", path);

    int res=0;

    memset(stbuf, 0, sizeof(struct stat));
    if(strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }
    else if(strcmp(path, hello_path) == 0) {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(hello_str);
    }
   /* else if(path==NULL) {  //THIS STILL NEEDS TO BE FIXED SO IT WILL STILL CALL MKDIR FUNCTION 
        res = -ENOENT;
	}*/ 
    else{
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 1;
	//printf("getattr path: %s\n", path);	
    }	


    return res;
}

static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{

mytree = create_tree;
//NODE* fpath = ftree->root;

printf("hello_readdir\n");
printf("path readdir %s\n", path);
    (void) offset;
    (void) fi;

	if(strcmp(path, "/") != 0){
		//printf("what am i?, path: %s\n", path);
		 filler(buf, ".", NULL, 0);
	   	filler(buf, "..", NULL, 0);
		filler(buf, test_sd_path + 1, NULL, 0);	
	} 
        /* if get_node ==NULL
	return -ENOENT;
		*/
	else{
	    filler(buf, ".", NULL, 0);
	    filler(buf, "..", NULL, 0);
	    filler(buf, hello_path + 1, NULL, 0);
	    filler(buf, test_path + 1, NULL, 0);
	    filler(buf, test_sd_path + 1, NULL, 0);
	}


	/* add a loop here so that if path is not null add to filler with path as second argument +1 to remove the slash */

    return 0;
}

static int hello_open(const char *path, struct fuse_file_info *fi)
{
	printf("hello_open\n");
    if((strcmp(path, hello_path) != 0)){
        return -ENOENT;
    }

    if((fi->flags & 3) != O_RDONLY){
        return -EACCES;
	}

    return 0;
}

static int hello_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
	printf("hello_read\n");
    size_t len;
    (void) fi;
    if((strcmp(path, hello_path) != 0))
        return -ENOENT;

    if((strcmp(path, hello_path) == 0)){
    len = strlen(hello_str);
    if (offset < len) {
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, hello_str + offset, size);
    } else
        size = 0;
    }

    return size;
}

static struct fuse_operations hello_oper = {
    .mkdir	= hello_mkdir,
    .rmdir	= hello_rmdir,
    .getattr	= hello_getattr,
    .readdir	= hello_readdir,
    .open	= hello_open,
    .read	= hello_read,
};

int main(int argc, char *argv[])
{
    return fuse_main(argc, argv, &hello_oper, NULL);
}

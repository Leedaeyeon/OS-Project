#include   <stdio.h>
#include   <string.h>
#include   <stdlib.h>
#include   <libxml/xmlmemory.h>
#include   <libxml/parser.h>

int
main(int argc, char **argv) {
        char *docname;
        if (argc <= 1) {
                printf("Usage: %s docname\n", argv[0]);
                return(0);
        }
        docname = argv[1];
        //parseDoc (docname);
        return (1);
}

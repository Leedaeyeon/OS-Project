#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

/*function will go here
	read in path as is and level name
	strcmp cur-name and level name
	look for children
	if children add to path
	look for sib
	

*/

void parseXML(xmlDocPtr doc, xmlNodePtr cur, xmlNodePtr tmp, xmlChar path){
	cur = cur->xmlChildrenNode;
	

}


static void
parseDoc(char *docname){
	xmlDocPtr doc;
	xmlNodePtr cur;
	xmlChar *nextdir;
	doc = xmlParseFile(docname);

	if(doc==NULL){
		fprintf(stderr, "Document not parsed successfully. \n");
		return;
	}

	cur = xmlDocGetRootElement(doc);
	
	if(cur == NULL){
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return;
	}

	if(xmlStrcmp(cur->name, (const xmlChar*)"story")){
		fprintf(stderr, "document of the wrong type, root node != story");
		xmlFreeDoc(doc);
		return;
	}


	//call function here
	/*if((!xmlStrcmp(cur->name, (const xmlChar*)"story"))){
				

		//nextdir = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		

		printf("%s\n", cur->xmlChildrenNode->next->name);
		printf("%s\n", cur->xmlChildrenNode->next->xmlChildrenNode->next->name);
		printf("%s\n", cur->xmlChildrenNode->next->next->next->name);
		printf("%s\n", cur->xmlChildrenNode->next->next->next->xmlChildrenNode->next->name);
	} */

	cur = cur->xmlChildrenNode;
	while(cur !=NULL){
		if((!xmlStrcmp(cur->name, (const xmlChar *)"dir1"))){
			parseXml(doc, cur,)		
		}
		cur = cur->next;
	}
	
	xmlFreeDoc(doc);
	return;
}

int 
main(int argc, char **argv){
	char *docname;
	if(argc<=1){
	printf("Usage: %s docname\n", argv[0]);
	return(0);
	}
	docname = argv[1];
	parseDoc(docname);
	return (1);

}

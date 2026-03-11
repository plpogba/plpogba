#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>

#define BUFFERSIZE 4096
#define LINESIZE 1024

typedef struct node* LINK;
typedef struct node{
	LINK left;
	char element[LINESIZE];
	LINK right;
}NODE;

LINK createNode(char*);
void deleteNode(LINK, LINK);
void insertNode(LINK,LINK);
void oops(char *, char *);
void printList(LINK);
LINK makeList(char* fname);
void writeFile(char*, LINK);
void freeNode(LINK);
void appendList(LINK, LINK);

int main(int ac , char * av[]){
	int in_fd, n_chars,l=0,k=0;
	LINK head, cur;
	if (ac > 4){
		fprintf(stderr, "usage: %s mode\n", *av);
		exit(1);
	}
	head = makeList("historyfile.txt");
	if (ac == 1)
		printList(head);
	else if (strcmp(av[1],"-a") == 0){
		LINK head2;
		if (ac != 3){
			printf("Usage: myhistory -a filename\n");
			exit(1);
		}
		head2 = makeList(av[2]);
		appendList(head,head2);
		writeFile("historyfile.txt",head);
	}
	else if (strcmp(av[1],"-d") == 0){
		int deln = atoi(av[2]);
		cur = head;
		for (int i = 0; i<deln; i++){
			cur = cur->right;
			if (cur == head){
				printf("cannot delete %d\n",deln);
				exit(1);
			}
		}
		deleteNode(head, cur);
		writeFile("historyfile.txt", head);
	}

	freeNode(head);
	return 0;
	
}

void appendList(LINK head1, LINK head2){
	LINK h1, h2;
	h1 = head1->left;
	h2 = head2->left;
	h1->right = head2->right;
	h1->right->left = h1;
	h2->right = head1;
	head1->left = h2;

	free(head2);
}
void writeFile(char* fname, LINK head){
	int out_fd, n_chars;
	LINK cur;
	
	if ((out_fd = open(fname, O_WRONLY | O_TRUNC)) == -1){
		printf("cannot open %s\n",fname);
		exit(1);
	}
	cur = head->right;
	while (cur != head){
		write(out_fd, cur->element,strlen(cur->element));
		cur = cur->right;
			
	}
	if(close(out_fd) == -1){
		printf("cannot close %s\n",fname);
		exit(1);
	}

}
LINK makeList(char* fname){
	int in_fd, n_chars, l = 0, k= 0;
	char buf[BUFFERSIZE], line[LINESIZE];
	LINK new, cur;
	LINK head = createNode("head");
	cur = head;
	if ((in_fd = open(fname, O_RDONLY)) == -1){
		printf("cannot open %s\n",fname);
                exit(1);
        }
	while ((n_chars = read(in_fd , buf, BUFFERSIZE)>0)){
        	while (buf[l] != '\0'){
                        line[k++] = buf[l];
                        if (buf[l] == '\n'){
                                if (strlen(line) > 1){
                                        new = createNode(line);
                                        insertNode(cur,new);
                                        cur = new;
                                }
                                k = 0;
                                memset(line,'\0',LINESIZE);
                        }
                        l++;
                }
                l=0;
        }
	if (close(in_fd) == -1){
		printf("error closing file: %s\n", fname);
		exit(1);
	}
	memset(buf,'\0',BUFFERSIZE);
	memset(line,'\0',LINESIZE);
	return head;

}

void printList(LINK head){
	int i =1;
	LINK nextnode = head->right;
	while (nextnode != head){
		printf("%5d %s",i++,nextnode->element);
		nextnode = nextnode->right;
	}
}
void oops(char* s1, char* s2){	
	fprintf(stderr, "Error: %s ", s1);
	perror(s2);
	exit(1);
}

LINK createNode(char* name){
	LINK cur = (NODE*)malloc(sizeof(NODE));
	strcpy(cur->element, name);
	cur->left = cur;
	cur->right = cur;
	return cur;
}

void insertNode(LINK node, LINK newnode){
	newnode->left = node;
	newnode->right = node->right;
	node->right->left = newnode;
	node->right = newnode;
}

void deleteNode(LINK node, LINK deleted){
	if (node == deleted)
		printf("not permitted\n");
	else{
		deleted->left->right = deleted->right;
		deleted->right->left = deleted->left;
		free(deleted);
	}
}

void freeNode(LINK head){
	LINK cur, nextnode;
	cur = head;
	cur->left->right = NULL;
	while(cur){
		nextnode = cur->right;
		free(cur);
		cur = nextnode;
	}
}

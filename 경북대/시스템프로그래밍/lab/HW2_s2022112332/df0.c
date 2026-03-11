#include <stdio.h>
#include <stdlib.h>
#include <sys/statfs.h>
#include <sys/types.h>
#include <string.h>

void printB(char, struct statfs* buff);

int main(int argc, char* argv[]){
	if(argc == 1){
		printf("Usage: df . option\n");
		exit(1);
	}

	struct statfs* buff;
	if(!(buff = (struct statfs*)malloc(sizeof(struct statfs)))){
		perror("Failed to allocate memory");
	}
	if (statfs(argv[1], buff) < 0){
		perror("statvfs() failed");
	}
	if(argc == 2 || strcmp("-k",argv[2]) == 0 || strcmp("-K",argv[2]) == 0)
		printB('K',buff);

	else if(strcmp("-m",argv[2]) == 0 || strcmp("-M",argv[2]) == 0)
		printB('M',buff);

	else if(strcmp("-g",argv[2]) == 0 || strcmp("-G", argv[2]) == 0)
		printB('G', buff);

	else if(strcmp("-b",argv[2]) == 0 || strcmp("-B", argv[2]) == 0)
		printB('B',buff);
	
	else
		printf("wrong option\n");
	free(buff);
	return 0;	
}

void printB(char S, struct statfs* buff){
	char* strU = "Used";
       	char* strA = "Available";
        char* strM = "MyUse%";
	char strB[20];
	long blocks = buff->f_blocks*buff->f_bsize, used = (buff->f_blocks - buff->f_bfree)*buff->f_bsize
		, avail = buff->f_bavail*buff->f_bsize;
	double per;
	
	if (blocks != 0)
		per = used * 100.0 / blocks;
	else
		per = -1;

	switch(S){
		case 'G':
			strcpy(strB,"1G-blocks");
			printf("%12s%13s%13s%9s\n",strB,strU,strA,strM);
			blocks /= 1024*1024*1024;
			avail /= 1024 * 1024*1024;
			printf("%12d%12.1lfG%12dG%8.2lf%%\n",(int)blocks, (double)used/(1024*1024*1024), (int)avail, per);
			break;
		case 'M':
			strcpy(strB,"1M-blocks");
			printf("%12s%13s%13s%9s\n",strB,strU,strA,strM);
                        blocks /= 1024*1024;
                        avail /= 1024*1024;
			used /= 1024*1024;
                        printf("%12d%12dM%12dM%8.2lf%%\n",(int)blocks, (int)used, (int)avail, per);
			break;
		case 'B':
			strcpy(strB,"1B-blocks");
			printf("%15s%15s%15s%9s\n",strB,strU,strA,strM);
                        printf("%15ld%15ld%15ld%8.2lf%%\n",blocks, used, avail, per);
			break;
		case 'K':
			strcpy(strB,"1k-blocks");
			printf("%12s%12s%12s%9s\n",strB,strU,strA,strM);
                        printf("%12ld%12ld%12ld%8.2lf%%\n",blocks/1024, used/1025, avail/1024, per);
			break;
	}
	
}



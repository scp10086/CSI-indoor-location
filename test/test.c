#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(){
	
	FILE* file; 
	char ss[100];
	char argv[] = "./ss/";
	char process[20];
	int count = 0;
	char end[] = ".dat";
	char num[10];
	int len;
	sprintf(num,"%d",count);
	strcpy(process,argv);
	len =strlen(process);
	printf("%s %d",process,len);
	
	strcat(process,num);
	printf("%s\n",process);
	strcat(process,end);
	printf("%s\n",process);
	file = fopen(process,"w+");
	fputs("aa",file);
	fclose(file);
	
	printf("GKD \n");
	scanf("%s",ss);
	int def = atoi(ss);
	printf("\nGKD%d\n",def);
	printf("\n");
	if(def == 1)
{
	memset(process, 0, sizeof(process)); 
	memset(num, 0, sizeof(num)); 
	printf("%s\n",process);
	count++;
	strcpy(process,argv);
	sprintf(num,"%d",count);
	strcat(process,num);
	strcat(process,end);
	printf("%s\n",process);
	file = fopen(process,"w+");
	fputs("aa",file);
	fclose(file);
}

}

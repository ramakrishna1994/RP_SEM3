#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
int main(void)
{
 FILE* fp;
 char* command;
 char* line;

 line = malloc(200 * sizeof(char));
 command = malloc(200 * sizeof(char));
 strcpy(command,"strace -p 23760 -o dirlist23760.txt ");

 fp = popen(command,"a+");
 

 return 0;
}
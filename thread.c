/* thread1.c */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <mysql/mysql.h>
#define N 65535
int sum = 0;

void* fun (void *val) 
{
    int *n = (int *)val;
  
    char cmd[100] = {};
    sprintf(cmd,"strace -p %d -o logs/dirlist_%d.txt",*n,*n);
    
    
    printf("done\n");

    FILE *fp;
    printf("%s\n",cmd);
    fp = popen(cmd, "w");   
    
    return NULL;
}

int main (int argc, char* argv[]) 
{
    int data, errcode, i;
     pthread_t t [N];
     MYSQL *conn;
     MYSQL_RES *res,*res1;
     MYSQL_ROW row;
     char *server = "localhost";
     char *user = "root";
     char *password = "root"; /* set me first */
     char *database = "iot";
     conn = mysql_init(NULL);
     /* Connect to database */
     if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

while(1){
    /* send SQL query */
    if (mysql_query(conn, "select * from running where status=1;")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    res1 = mysql_store_result(conn);
    res = res1;
    mysql_free_result(res1);
   
   
    while ((row = mysql_fetch_row(res)) != NULL)
    {
              
         int pid = atoi(row[0]);
         printf("%d \n", pid);
         char q1[1024] = {};
        sprintf(q1,"update running set status=0 where pid=%d;",pid);
        
        if (mysql_query(conn, q1)) {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
        }
        if (pthread_create (&t[i], NULL, fun, (void*)&pid))
         {
            perror ("Error creating thread\n");
           exit(1);
        }
        
    }

    
    
    
    
}
    mysql_close(conn);
   
    exit(0);
}

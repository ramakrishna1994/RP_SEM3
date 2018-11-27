        /* This file is licensed under the GPL v2 (http://www.gnu.org/licenses/gpl2.txt) (some parts was originally borrowed from proc events example)

        pmon.c

        code highlighted with GNU source-highlight 3.1
        */

        #define _XOPEN_SOURCE 700
        #include <sys/socket.h>
        #include <linux/netlink.h>
        #include <linux/connector.h>
        #include <linux/cn_proc.h>
        #include <signal.h>
        #include <errno.h>
        #include <stdbool.h>
        #include <unistd.h>
        #include <string.h>
        #include <stdlib.h>
        #include <stdio.h>
        #include <pthread.h>
        #include <mysql/mysql.h>
        #define N 65535


        void* fun (void *val) 
        {
            int *n = (int *)val;
            
            char str[10];
            sprintf(str, "%d", *n);

            char * new_str ;
            if((new_str = malloc(strlen("strace -p ")+strlen(str)+strlen(" -o dirlist")+strlen(str)+strlen(".txt")+1)) != NULL){
                new_str[0] = '\0';   // ensures the memory is an empty string
                strcat(new_str,"strace -p ");
                strcat(new_str,str);
                strcat(new_str," -o dirlist");
                strcat(new_str,str);
                strcat(new_str,".txt");
            } else {
                printf("malloc failed!\n");
                // exit?
            }
            


            FILE *fp;
            char buf[1024];

            //fp = popen("strace -p 12365 -o vamshi.txt", "w");
            printf("%s\n",new_str);
            fp = popen(new_str, "w");   
            if (fp == NULL) {
                /* handle error */
            printf("done\n");
            return NULL;
            }

            


            return NULL;
        }


        /*
        * connect to netlink
        * returns netlink socket, or -1 on error
        */
        static int nl_connect()
        {
        int rc;
        int nl_sock;
        struct sockaddr_nl sa_nl;

        nl_sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
        if (nl_sock == -1) {
            perror("socket");
            return -1;
        }

        sa_nl.nl_family = AF_NETLINK;
        sa_nl.nl_groups = CN_IDX_PROC;
        sa_nl.nl_pid = getpid();

        rc = bind(nl_sock, (struct sockaddr *)&sa_nl, sizeof(sa_nl));
        if (rc == -1) {
            perror("bind");
            close(nl_sock);
            return -1;
        }

        return nl_sock;
        }

        /*
        * subscribe on proc events (process notifications)
        */
        static int set_proc_ev_listen(int nl_sock, bool enable)
        {
        int rc;
        struct __attribute__ ((aligned(NLMSG_ALIGNTO))) {
            struct nlmsghdr nl_hdr;
            struct __attribute__ ((__packed__)) {
            struct cn_msg cn_msg;
            enum proc_cn_mcast_op cn_mcast;
            };
        } nlcn_msg;

        memset(&nlcn_msg, 0, sizeof(nlcn_msg));
        nlcn_msg.nl_hdr.nlmsg_len = sizeof(nlcn_msg);
        nlcn_msg.nl_hdr.nlmsg_pid = getpid();
        nlcn_msg.nl_hdr.nlmsg_type = NLMSG_DONE;

        nlcn_msg.cn_msg.id.idx = CN_IDX_PROC;
        nlcn_msg.cn_msg.id.val = CN_VAL_PROC;
        nlcn_msg.cn_msg.len = sizeof(enum proc_cn_mcast_op);

        nlcn_msg.cn_mcast = enable ? PROC_CN_MCAST_LISTEN : PROC_CN_MCAST_IGNORE;

        rc = send(nl_sock, &nlcn_msg, sizeof(nlcn_msg), 0);
        if (rc == -1) {
            perror("netlink send");
            return -1;
        }

        return 0;
        }


        void dbops(MYSQL *conn,int pid)
        {
                MYSQL_RES *res;
                MYSQL_ROW row;
                char q[1024] = {};
                sprintf(q,"select count(*) from running where pid=%d;",pid);
                res = mysql_use_result(conn);
                while ((row = mysql_fetch_row(res)) != NULL)
                    printf("%s \n", row[0]);
                /*
                if(atoi(row[0]) == 0)
                {
                    char q1[1024] = {};
                    sprintf(q1,"INSERT INTO running VALUES(%d,1)",pid);
                    
                    if (mysql_query(conn, q1)) {
                        fprintf(stderr, "%s\n", mysql_error(conn));
                        exit(1);
                    }
                }
                else
                {
                    char q2[1024] = {};
                    sprintf(q2,"update running set status=1 where pid=%d;",pid);
                    
                    if (mysql_query(conn, q2)) {
                        fprintf(stderr, "%s\n", mysql_error(conn));
                        exit(1);
                    }   
                }
                */
                
        }

        /*
        * handle a single process event
        */
        static volatile bool need_exit = false;
        static int handle_proc_ev(int nl_sock)
        {
             MYSQL *conn;
             MYSQL_RES *res;
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
        pthread_t t [N] = {-1};
        int rc;
        struct __attribute__ ((aligned(NLMSG_ALIGNTO))) {
            struct nlmsghdr nl_hdr;
            struct __attribute__ ((__packed__)) {
            struct cn_msg cn_msg;
            struct proc_event proc_ev;
            };
        } nlcn_msg;

        while (!need_exit) {
            rc = recv(nl_sock, &nlcn_msg, sizeof(nlcn_msg), 0);
            if (rc == 0) {
            /* shutdown? */
            return 0;
            } else if (rc == -1) {
            if (errno == EINTR) continue;
            perror("netlink recv");
            return -1;
            }
            switch (nlcn_msg.proc_ev.what) {
            case PROC_EVENT_NONE:
                printf("set mcast listen ok\n");
                break;
            case PROC_EVENT_FORK:
                printf("fork: parent tid=%d pid=%d -> child tid=%d pid=%d\n",
                    nlcn_msg.proc_ev.event_data.fork.parent_pid,
                    nlcn_msg.proc_ev.event_data.fork.parent_tgid,
                    nlcn_msg.proc_ev.event_data.fork.child_pid,
                    nlcn_msg.proc_ev.event_data.fork.child_tgid);
                int p =  nlcn_msg.proc_ev.event_data.fork.parent_tgid;
                int c = nlcn_msg.proc_ev.event_data.fork.child_tgid;
                //dbops(conn,p);
                char q1[100] = {};
                sprintf(q1,"INSERT IGNORE INTO running VALUES(%d,1)",p);
                
                if (mysql_query(conn, q1)) {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    exit(1);
                }
                char q2[100] = {};
                sprintf(q2,"INSERT IGNORE INTO running VALUES(%d,1)",c);
                
                if (mysql_query(conn, q2)) {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    exit(1);
                }

                break;
                /*
            case PROC_EVENT_EXEC:
                printf("exec: tid=%d pid=%d\n",
                    nlcn_msg.proc_ev.event_data.exec.process_pid,
                    nlcn_msg.proc_ev.event_data.exec.process_tgid);
                break;
            case PROC_EVENT_UID:
                printf("uid change: tid=%d pid=%d from %d to %d\n",
                    nlcn_msg.proc_ev.event_data.id.process_pid,
                    nlcn_msg.proc_ev.event_data.id.process_tgid,
                    nlcn_msg.proc_ev.event_data.id.r.ruid,
                    nlcn_msg.proc_ev.event_data.id.e.euid);
                break;
            case PROC_EVENT_GID:
                printf("gid change: tid=%d pid=%d from %d to %d\n",
                    nlcn_msg.proc_ev.event_data.id.process_pid,
                    nlcn_msg.proc_ev.event_data.id.process_tgid,
                    nlcn_msg.proc_ev.event_data.id.r.rgid,
                    nlcn_msg.proc_ev.event_data.id.e.egid);
                break;
                */
            case PROC_EVENT_EXIT:
                printf("exit: tid=%d pid=%d exit_code=%d\n",
                    nlcn_msg.proc_ev.event_data.exit.process_pid,
                    nlcn_msg.proc_ev.event_data.exit.process_tgid,
                    nlcn_msg.proc_ev.event_data.exit.exit_code);
                t[nlcn_msg.proc_ev.event_data.exit.process_tgid] = -1;
                break;
                char q3[100] = {};
                int epid = nlcn_msg.proc_ev.event_data.exit.process_pid;
                sprintf(q3,"delete from running where pid=%d;",epid);
                
                if (mysql_query(conn, q3)) {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    exit(1);
                }
            default:
                printf("unhandled proc event\n");
                break;
            }
        }

        return 0;
        }

        static void on_sigint(int unused)
        {
        need_exit = true;
        }

        int main(int argc, const char *argv[])
        {
        int nl_sock;
        int rc = EXIT_SUCCESS;

        signal(SIGINT, &on_sigint);
        siginterrupt(SIGINT, true);

        nl_sock = nl_connect();
        if (nl_sock == -1)
            exit(EXIT_FAILURE);

        rc = set_proc_ev_listen(nl_sock, true);
        if (rc == -1) {
            rc = EXIT_FAILURE;
            goto out;
        }

        rc = handle_proc_ev(nl_sock);
        if (rc == -1) {
            rc = EXIT_FAILURE;
            goto out;
        }

            set_proc_ev_listen(nl_sock, false);

        out:
        close(nl_sock);
        exit(rc);
        }

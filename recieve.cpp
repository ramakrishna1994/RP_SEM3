#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
	int sockfd,portno=54235;
	char mip[32] = "";
	char buf[256];
	struct sockaddr_in mcast;
	struct ip_mreq mreq;
	printf("Enter Multicast IP address to listen to : ");
	scanf("%s",mip);
	printf("Enter Port No :");
	scanf("%d",&portno);

	sockfd = socket(AF_INET,SOCK_DGRAM,0);

	if(sockfd < 0)
	{
		perror("Error in Socket Connection ");
		exit(1);
	}
	printf("coming\n");
	memset(&mcast,'\0',sizeof(mcast));
	mcast.sin_family = AF_INET;
	mcast.sin_addr.s_addr = inet_addr(INADDR_ANY);
	mcast.sin_port = htons(portno);
	unsigned int addrlen = sizeof(mcast);
	mreq.imr_multiaddr.s_addr = inet_addr(mip);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	printf("coming\n");

	
	if (bind(sockfd, (struct sockaddr *) &mcast, sizeof(mcast)) < 0) {
		perror("bind");
		exit(1);
	}

	/*
	 * Registering ourselves with the multicast group with any interface and
	 * IP_ADD_MEMBERSHIP flag.
	 */
	if(setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq))<0)
	{
		perror("Setting Socket Options failed ");
		exit(1);
	}
	while(1)
	{
		printf("Data : ");
		if(recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&mcast,&addrlen) < 0)
		{
			perror("Error in receiving data from multicast IP");
			close(sockfd);
			exit(1);
		}
		puts(buf);
	}
	return 0;
}

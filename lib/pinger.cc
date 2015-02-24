// pinger.cc
#include "pinger.h"

#define	DEFDATALEN	(64 - ICMP_MINLEN) // default data length
#define	MAXIPLEN	60
#define	MAXICMPLEN	76
#define	MAXPACKET	(65536 - 60 - ICMP_MINLEN) // max packet size

int Pinger::ping(string target)
{
    int s, i, cc, packlen, datalen = DEFDATALEN;
	struct hostent *hp;
	struct sockaddr_in to, from;
	//struct protoent	*proto;
	struct ip *ip;
	u_char *packet;
	int outpack[MAXPACKET];
	char hnamebuf[MAXHOSTNAMELEN];
	string hostname;
	struct icmp *icp;
	int ret, fromlen, hlen;
	fd_set rfds;
	struct timeval tv;
	int retval;
	struct timeval start, end;
	int end_t = 0;
	bool cont = true;

	to.sin_family = AF_INET;

	// try to convert as dotted decimal address, else if that fails assume it's a hostname
	to.sin_addr.s_addr = inet_addr(target.c_str());
	if (to.sin_addr.s_addr != (u_int)-1)
		hostname = target;
	else
	{
		hp = gethostbyname(target.c_str());
		if (!hp)
		{
			cerr << "unknown host "<< target << endl;
			return -3;
		}
		to.sin_family = hp->h_addrtype;
		bcopy(hp->h_addr, (caddr_t)&to.sin_addr, hp->h_length);
		strncpy(hnamebuf, hp->h_name, sizeof(hnamebuf) - 1);
		hostname = hnamebuf;
	}

	packlen = datalen + MAXIPLEN + MAXICMPLEN;
	if((packet = (u_char *)malloc((u_int)packlen)) == NULL)
	{
		cerr << "malloc error\n";
		return -2;
	}

    // Used SOCK_DGRAM instead of SOCK_RAW. Raw sockets only available for superuser
	if((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP)) < 0)
	{
		perror("sockets");
		return -1;
	}

	icp = (struct icmp *)outpack;
	icp->icmp_type = ICMP_ECHO;
	icp->icmp_code = 0;
	icp->icmp_cksum = 0;
	icp->icmp_seq = 12345;	 // must be here
	icp->icmp_id = getpid(); // must be here. Converted automatically to u_int16_t

	cc = datalen + ICMP_MINLEN;
	icp->icmp_cksum = in_cksum((unsigned short *) icp, cc);

	gettimeofday(&start, NULL);

	i = sendto(s, (char *)outpack, cc, 0, (struct sockaddr*)&to, (socklen_t)sizeof(struct sockaddr_in));
	if (i < 0 || i != cc)
	{
		if(i < 0)
			perror("sendto error");
		//cout << "wrote " << hostname << " " <<  cc << " chars, ret= " << i << endl;
	}

	// Watch stdin (fd 0) to see when it has input.
	FD_ZERO(&rfds);
	FD_SET(s, &rfds);

	// Wait up to one seconds.
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	while(cont)
	{
		retval = select(s+1, &rfds, NULL, NULL, &tv);

		if (retval == -1)
		{
			perror("select()");
			end_t = -1;
			break;
		}
		else if (retval)
		{
			fromlen = sizeof(sockaddr_in);
			if ( (ret = recvfrom(s, (char *)packet, packlen, 0,(struct sockaddr *)&from, (socklen_t*)&fromlen)) < 0)
			{
				perror("recvfrom error");
				end_t = -1;
				break;
			}

			// Check the IP header
			ip = (struct ip *)((char*)packet);
			hlen = sizeof( struct ip );
			if (ret < (hlen + ICMP_MINLEN))
			{
				cerr << "packet too short (" << ret  << " bytes) from " << hostname << endl;;
				end_t = -1;
				break;
			}

			// ICMP part
			icp = (struct icmp *)(packet + hlen);

			// Echo reply
			if(icp->icmp_type == ICMP_ECHOREPLY)
			{

				if (icp->icmp_seq != 12345)
				{
					// cout << "received sequence # " << icp->icmp_seq << endl;
					continue;
				}
				if (icp->icmp_id != (u_int16_t) getpid())
				{
					// cout << "received id " << icp->icmp_id << " " << (u_int16_t) getpid() << endl;
					continue;
				}
				cont = false;
			}
			else
			{
				// cout << "Recv: not an echo reply" << endl;
				continue;
			}

			gettimeofday(&end, NULL);

			// Return in microseconds
			end_t = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);

			// Avoid 0 returns (they're treated as errors)
			if(end_t < 1)
				end_t = 1;

			break;
		}
		else
		{
			// cout << "No data within " << tv.tv_sec << " seconds" << endl;
			break;
		}
	}

	// Close the socket before exit. Otherwise it's going to crash at some point
	close(s);

	// Free the mallocs
	free(packet);

	return end_t;
}

// Copyright (c) 1989, 1993. The Regents of the University of California.
// https://www.inf.pucrs.br/~benso/redes601/2004_2/in_cksum.c
uint16_t Pinger::in_cksum(uint16_t *addr, unsigned len)
{
  register int sum = 0;
  u_short answer = 0;
  register u_short *w = addr;
  register int nleft = len;

  /*
   * Our algorithm is simple, using a 32 bit accumulator (sum), we add
   * sequential 16 bit words to it, and at the end, fold back all the
   * carry bits from the top 16 bits into the lower 16 bits.
   */
  while (nleft > 1)  {
		  sum += *w++;
		  nleft -= 2;
  }

  /* mop up an odd byte, if necessary */
  if (nleft == 1) {
		  *(u_char *)(&answer) = *(u_char *)w ;
		  sum += answer;
  }

  /* add back carry outs from top 16 bits to low 16 bits */
  sum = (sum >> 16) + (sum & 0xffff);     /* add hi 16 to low 16 */
  sum += (sum >> 16);                     /* add carry */
  answer = ~sum;                          /* truncate to 16 bits */

  return(answer);
}
/*
 *
 * Copyright (c) 2001
 *	Politecnico di Torino.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * For bug report and other information please visit Tstat site:
 * http://tstat.polito.it
 *
 * Tstat is deeply based on TCPTRACE. The authors would like to thank
 * Shawn Ostermann for the development of TCPTRACE.
 *
*/

/* Log file format:
 * V2
 * - request and response on the same line
 * - values not available are substituted by zeroes
 * The meaning of the fields 9-X depend on the protocol used
 * +-------+-------------------------------------------------------------+-----------+
 * | Field | Description                                                 | Protocol  |
 * +-------+-------------------------------------------------------------+-----------+
 * |    01 | L4 Protocol: 1 = TCP, 2 = UDP                               | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * | Request
 * +-------+--------------------------------------------------------------------------
 * | Field | Description
 * +-------+-------------------------------------------------------------+-----------+
 * |    02 | Protocol: 3 = RTP, 4 = RTCP                                 | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    03 | Source IP address                                           | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    04 | Source port number                                          | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    05 | Internal source? (computed using the -N netmask_file)       | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    06 | Number of packets Tstat has seen belonging to the flow      | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    07 | Inter Packet Gap (IPG) [ms]                                 | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    08 | Jitter (average)                                            | All       |
 * |       |  - if RTP, computed by Tstat as in RFC3550 [ms]             |           |
 * |       |  - if RTCP, extracted from the RTCP header [codec           |           |
 * |       |    timestamps units]                                        |           |
 * |       |  - if TCP, computed using only data packets [ms]            |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    09 | Jitter (max)                                                | All       |
 * |       |  - if RTP, computed by Tstat as in RFC3550 [ms]             |           |
 * |       |  - if RTCP, extracted from the RTCP header [codec           |           |
 * |       |    timestamps units]                                        |           |
 * |       |  - if TCP, computed using only data packets [ms]            |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    10 | Jitter (min)                                                | All       |
 * |       |  - if RTP, computed by Tstat as in RFC3550 [ms]             |           |
 * |       |  - if RTCP, extracted from the RTCP header [codec           |           |
 * |       |    timestamps units]                                        |           |
 * |       |  - if TCP, computed using only data packets [ms]            |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    11 | Time to live (TTL) (average)                                | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    12 | Time to live (TTL) (max)                                    | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    13 | Time to live (TTL) (min)                                    | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    14 | Start time (EPOCH format) [s]                               | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    15 | Duration [s]                                                | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    16 | Data transfered [bytes]                                     | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    17 | Average speed [bit/s]                                       | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    18 | SSRC                                                        | RTP, RTCP |
 * +-------+-------------------------------------------------------------+-----------+
 * |    19 | Lost packets                                                | RTP       |
 * |       |  - computed by Tstat using a window based algorithm         |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    20 | Out of sequence packets computed by Tstat                   | TCP,RTP   |
 * |       |  - computed by Tstat using a window based algorithm         |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    21 | Duplicate packets computed by Tstat                         | TCP,RTP   |
 * |       |  - if RTP, computed by Tstat using a window based algorithm |           |
 * |       |  - if TCP, computed as retrasmissions                       |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    22 | Late packets computed by Tstat                              | RTP       |
 * |       |  - computed by Tstat using a window based algorithm         |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    23 | RTP payload type                                            | RTP       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    24 | Bogus reset                                                 | RTP       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    25 | Cumulative packet loss:                                     | RTCP      |
 * |       |  - each lost packets increments this counter,               |           |
 * |       |    each duplicated packets decremnets it from RTCP          |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    26 | Fraction of lost packets                                    | RTCP      |
 * |       |  - extracted from the RTCP header [%]                       |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    27 | Associated RTP flow length [packets]                        | RTCP      |
 * +-------+-------------------------------------------------------------+-----------+
 * |    28 | Associated RTP flow length [bytes]                          | RTCP      |
 * +-------+-------------------------------------------------------------+-----------+
 * |    29 | Round Trip Time (RTT) (average) [ms]                        | TCP, RTCP |
 * +-------+-------------------------------------------------------------+-----------+
 * |    30 | Round Trip Time (RTT) (max) [ms]                            | TCP, RTCP |
 * +-------+-------------------------------------------------------------+-----------+
 * |    31 | Round Trip Time (RTT) (min) [ms]                            | TCP, RTCP |
 * +-------+-------------------------------------------------------------+-----------+
 * |    32 | Round Trip Time (RTT) (samples) [ms]                        | TCP, RTCP |
 * +-------+-------------------------------------------------------------+-----------+
 * |    33 | Truncated RTCP header                                       | RTCP      |
 * +-------+-------------------------------------------------------------+-----------+
 * |    34 | First HTTP packet (EPOCH) [s]                               | TCP       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    35 | First RTSP packet (EPOCH) [s]                               | TCP       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    36 | First RTP packet (EPOCH) [s]                                | TCP       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    37 | First ICY packet (EPOCH) [s]                                | TCP       |
 * +-------+-------------------------------------------------------------+-----------+
 * | Response
 * +-------+-------------------------------------------------------------+-----------+
 * |    38 | Protocol: 3 = RTP, 4 = RTCP                                 | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    39 | Destination IP address                                      | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    40 | Destination port number                                     | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    41 | Internal destination? (computed using the -N netmask_file)  | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    42 | Number of packets Tstat has seen belonging to the flow      | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    43 | Inter Packet Gap (IPG) [ms]                                 | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    44 | Jitter (average)                                            | All       |
 * |       |  - if RTP, computed by Tstat as in RFC3550 [ms]             |           |
 * |       |  - if RTCP, extracted from the RTCP header [codec           |           |
 * |       |    timestamps units]                                        |           |
 * |       |  - if TCP, computed using only data packets [ms]            |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    45 | Jitter (max)                                                | All       |
 * |       |  - if RTP, computed by Tstat as in RFC3550 [ms]             |           |
 * |       |  - if RTCP, extracted from the RTCP header [codec           |           |
 * |       |    timestamps units]                                        |           |
 * |       |  - if TCP, computed using only data packets [ms]            |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    46 | Jitter (min)                                                | All       |
 * |       |  - if RTP, computed by Tstat as in RFC3550 [ms]             |           |
 * |       |  - if RTCP, extracted from the RTCP header [codec           |           |
 * |       |    timestamps units]                                        |           |
 * |       |  - if TCP, computed using only data packets [ms]            |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    47 | Time to live (TTL) (average)                                | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    48 | Time to live (TTL) (max)                                    | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    49 | Time to live (TTL) (min)                                    | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    50 | Start time (EPOCH format) [s]                               | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    51 | Duration [s]                                                | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    52 | Data transfered [bytes]                                     | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    53 | Average speed [bit/s]                                       | All       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    54 | SSRC                                                        | UDP       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    55 | Lost packets                                                | RTP       |
 * |       |  - computed by Tstat using a window based algorithm         |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    56 | Out of sequence packets computed by Tstat                   | TCP,RTP   |
 * |       |  - computed by Tstat using a window based algorithm         |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    57 | Duplicate packets computed by Tstat                         | TCP,RTP   |
 * |       |  - if RTP,computed by Tstat using a window based algorithm  |           |
 * |       |  - if TCP,computed as retrasmissions                        |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    58 | Late packets computed by Tstat                              | RTP       |
 * |       |  - computed by Tstat using a window based algorithm         |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    59 | RTP payload type                                            | RTP       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    60 | Bogus reset                                                 | RTP       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    61 | Cumulative packet loss:                                     | RTCP      |
 * |       |  - each lost packets increments this counter,               |           |
 * |       |    each duplicated packets decremnets it from RTCP          |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    62 | Fraction of lost packets                                    | RTCP      |
 * |       |  - extracted from the RTCP header [%]                       |           |
 * +-------+-------------------------------------------------------------+-----------+
 * |    63 | Associated RTP flow length [packets]                        | RTCP      |
 * +-------+-------------------------------------------------------------+-----------+
 * |    64 | Associated RTP flow length [bytes]                          | RTCP      |
 * +-------+-------------------------------------------------------------+-----------+
 * |    65 | Round Trip Time (RTT) (average) [ms]                        | TCP, RTCP |
 * +-------+-------------------------------------------------------------+-----------+
 * |    66 | Round Trip Time (RTT) (max) [ms]                            | TCP, RTCP |
 * +-------+-------------------------------------------------------------+-----------+
 * |    67 | Round Trip Time (RTT) (min) [ms]                            | TCP, RTCP |
 * +-------+-------------------------------------------------------------+-----------+
 * |    68 | Round Trip Time (RTT) (samples) [ms]                        | TCP, RTCP |
 * +-------+-------------------------------------------------------------+-----------+
 * |    69 | Truncated RTCP header                                       | RTCP      |
 * +-------+-------------------------------------------------------------+-----------+
 * |    70 | First HTTP packet (EPOCH) [s]                               | TCP       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    71 | First RTSP packet (EPOCH) [s]                               | TCP       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    72 | First RTP packet (EPOCH) [s]                                | TCP       |
 * +-------+-------------------------------------------------------------+-----------+
 * |    73 | First ICY packet (EPOCH) [s]                                | TCP       |
 * +-------+-------------------------------------------------------------+-----------+
 */

#include "tstat.h"

#define get_u8(X,O)   (*(tt_uint8  *)(X + O))
#define get_u16(X,O)  (*(tt_uint16 *)(X + O))
#define get_u32(X,O)  (*(tt_uint32 *)(X + O))

extern unsigned long int f_RTP_count;
extern unsigned long int f_RTCP_count;
extern FILE *fp_rtp_logc;

#define RTP_DEBUG_LEVEL 1
#define RTP_DEBUG (debug>=RTP_DEBUG_LEVEL)
extern int debug;

#define IN_SEQ 0
#define LOST 1
#define LATE 2			/* consierare come reord */
#define NOT_IN_SEQ 3		/* come reord */
#define DUP 4

#ifndef RTP_SSRC_MAX_SINGLETON
#define RTP_SSRC_MAX_SINGLETON 10
#endif
#ifndef RTP_SSRC_SINGLETON_THRESHOLD
#define RTP_SSRC_SINGLETON_THRESHOLD 1
#endif


void rtp_plus_check (ucb *thisdir, struct rtphdr *prtp, int dir, struct ip *pip, struct udphdr *pudp, void *plast);
void rtp_plus_stat (ucb *thisdir, struct rtphdr *prtp, int dir, struct ip *pip, struct udphdr *pudp, void *plast);

rtp  *rtp_locate_ssrc (rtp *rtp_list, uint32_t ssrc, int *length, int *singletons);
rtcp *rtcp_locate_ssrc (rtcp *rtcp_list, uint32_t ssrc, int *length, int *singletons);
int  rtp_valid_pt (u_int8_t pt);
void rtp_init_pt_counter (rtp * f_rtp);
void rtcp_init_pt_counter (rtcp * f_rtcp);
void rtp_update_pt_counter (rtp* f_rtp, u_int8_t pt);
void rtcp_update_pt_counter (rtcp* f_rtcp, u_int8_t pt);
char *rtp_pt_lists (rtp *f_rtp);
char *rtcp_pt_lists (rtcp *f_rtcp);
rtp  *new_rtp_subflow (u_int32_t ssrc, u_int8_t pt, u_int32_t ts, u_int16_t seq, u_int32_t data_bytes);
rtcp *new_rtcp_subflow (u_int32_t ssrc, u_int8_t pt, u_long initial_bytes);
void rtcp_init_stats (rtcp *f_rtcp, char *payload_ptr, u_int8_t pt, unsigned char rc, void *plast, struct sudp_pair *pup, int dir);

void update_rtp_conn_histo (ucb * thisdir, int dir);
void update_rtcp_conn_histo (ucb * thisdir, int dir);
void update_conn_log_v3(udp_pair *flow);

/* Functions to manage the rtp/rtcp record list */

rtp *rtp_locate_ssrc(rtp *rtp_list,uint32_t ssrc, int *length, int *singletons)
{
   int iterator;
   int ssrc_singletons;
   rtp *item;
   if (rtp_list==NULL)
    {
      *length = 0;
      *singletons = 0;
      return(NULL);
    }

   item = rtp_list;
   iterator = 1;
   ssrc_singletons=0;
   while (item!=NULL)
    {
      if (item->ssrc == ssrc)
       {
	*length = iterator;
   *singletons = ssrc_singletons;
	return(item);
       }
      else
       {
	 iterator++;
     if (item->pnum<=RTP_SSRC_SINGLETON_THRESHOLD)
         ssrc_singletons++;
	 item = item->next;
       }
    }
   *length = iterator;
   *singletons = ssrc_singletons;
   return(NULL);
}

rtcp *rtcp_locate_ssrc(rtcp *rtcp_list,uint32_t ssrc, int *length, int *singletons)
{
   int iterator;
   int ssrc_singletons;
   rtcp *item;
   if (rtcp_list==NULL)
    {
      *length = 0;
      *singletons = 0;
      return(NULL);
    }
   item = rtcp_list;
   iterator = 1;
   ssrc_singletons = 0;
   while (item!=NULL)
    {
      if (item->ssrc == ssrc)
       {
	*length = iterator;
   *singletons = ssrc_singletons;
	return(item);
       }
      else
       {
	 iterator++;
     if (item->pnum<=RTP_SSRC_SINGLETON_THRESHOLD)
         ssrc_singletons++;
	 item = item->next;
       }
    }
   *length = iterator;
   *singletons = ssrc_singletons;
   return(NULL);
  
}

/* Check RTP valide PT */
int rtp_valid_pt(u_int8_t pt)
{
  /* Currently valid/assigned RTP Payload Types */
  if ( pt == 0  ||
     ( pt >= 3  && pt <= 18) ||
       pt == 25 ||
       pt == 26 ||
       pt == 28 ||
     ( pt >= 31 && pt <= 34) ||
     ( pt >= 96 && pt <= 127)
    )
    return 1;
  else
    return 0;
}

/* Classify a packet according to RFC7983 */
int RFC7983_classify(void * hdr)
{
  uint8_t first_byte = ((uint8_t*)hdr)[0];
  if ( first_byte <= 3)
    return RFC7983_STUN;    
  else if ( first_byte >= 16 && first_byte <= 19 )
    return RFC7983_ZRTP;
  else if ( first_byte >= 20 && first_byte <= 63 )
    return RFC7983_DTLS;
  else if ( first_byte >= 64 && first_byte <= 79 )
    return RFC7983_TURN;
  else if ( first_byte >= 128 && first_byte <= 191)
    {
      /* Distinguish RTP and RTCP (see RFC 5761)*/
      struct rtphdr *prtp = (struct rtphdr *) hdr;
      u_int8_t pt_rtcp = prtp->pt + (prtp->m << 7);
      if (prtp->v == VALID_VERSION && rtp_valid_pt(prtp->pt))
        {
          return RFC7983_RTP;
        }
      else if (prtp->v == VALID_VERSION && pt_rtcp >= RTCP_MIN_PT && pt_rtcp <= RTCP_MAX_PT) 
        {
          return RFC7983_RTCP;
        }
    }
  else if (first_byte == 0xff && (((uint8_t*)hdr)[1])==0x10 )
    return RFC7983_MTURN;
  else
    {
      return RFC7983_UNK;
    }
  return RFC7983_UNK;
}

void rtp_init_pt_counter(rtp * f_rtp)
{
  int i;
  for (i=0;i<MAX_COUNT_RTP_PT;i++)
   {
     f_rtp->pt_id[i]=255;    /* Set to invalid bogus value */
     f_rtp->pt_counter[i]=0;
   }
  return;
}

void rtcp_init_pt_counter(rtcp * f_rtcp)
{
  int i;
  for (i=0;i<MAX_COUNT_RTP_PT;i++)
   {
     f_rtcp->pt_id[i]=0;    /* Set to invalid bogus value */
     f_rtcp->pt_counter[i]=0;
   }
  return;
}

void rtp_update_pt_counter(rtp* f_rtp, u_int8_t pt)
{
  int i;
  for (i=0;i<MAX_COUNT_RTP_PT;i++)
  {
    if (f_rtp->pt_id[i]!=255 && f_rtp->pt_id[i] == pt )
     {
       f_rtp->pt_counter[i]++;
       return;
     }
    else if (f_rtp->pt_id[i]==255)
    {
       f_rtp->pt_id[i] = pt;
       f_rtp->pt_counter[i] = 1;
       return;
    }
  }
  /* If I'm not already returned, I already had MAX_COUNT_RTP_PT different PT, and I ignore this */
 
  fprintf (fp_stderr, "Excess RTP PT ignored !!!\n");
  return;
}

void rtcp_update_pt_counter(rtcp* f_rtcp, u_int8_t pt)
{
  int i;
  for (i=0;i<MAX_COUNT_RTP_PT;i++)
  {
    if (f_rtcp->pt_id[i]!=0 && f_rtcp->pt_id[i] == pt )
     {
       f_rtcp->pt_counter[i]++;
       return;
     }
    else if (f_rtcp->pt_id[i]==0)
    {
       f_rtcp->pt_id[i] = pt;
       f_rtcp->pt_counter[i] = 1;
       return;
    }
  }
  /* If I'm not already returned, I already had MAX_COUNT_RTP_PT different PT, and I ignore this */
 
  fprintf (fp_stderr, "Excess RTCP PT ignored !!!\n");
  return;
}

char *rtp_pt_lists(rtp *f_rtp)
{
  static char buffer[128];
  char pt_string[64];
  char count_string[64];
  char tmp[20];
  int i;
  int comma;
  
  strcpy(pt_string,"");
  strcpy(count_string,"");
  comma = 0;
  for (i=0;i<MAX_COUNT_RTP_PT;i++)
  {
    if (f_rtp->pt_id[i]!=255)
    {
      sprintf(tmp,"%s%d",(comma==1?",":""),f_rtp->pt_id[i]);
      strcat(pt_string,tmp);
      sprintf(tmp,"%s%d",(comma==1?",":""),f_rtp->pt_counter[i]);
      strcat(count_string,tmp);
      comma = 1;
    }
  }
  if (comma==0)
   {
     strcpy(buffer,"- -");
   }
  else
   {
     sprintf(buffer,"%s %s",pt_string,count_string);
   }
  return(buffer); 
}

char *rtcp_pt_lists(rtcp *f_rtcp)
{
  static char buffer[128];
  char pt_string[64];
  char count_string[64];
  char tmp[20];
  int i;
  int comma;
  
  strcpy(pt_string,"");
  strcpy(count_string,"");
  comma = 0;
  for (i=0;i<MAX_COUNT_RTP_PT;i++)
  {
    if (f_rtcp->pt_id[i]!=0)
    {
      sprintf(tmp,"%s%d",(comma==1?",":""),f_rtcp->pt_id[i]);
      strcat(pt_string,tmp);
      sprintf(tmp,"%s%d",(comma==1?",":""),f_rtcp->pt_counter[i]);
      strcat(count_string,tmp);
      comma = 1;
    }
  }
  if (comma==0)
   {
     strcpy(buffer,"- -");
   }
  else
   {
     sprintf(buffer,"%s %s",pt_string,count_string);
   }
  return(buffer); 
}

rtp *new_rtp_subflow(u_int32_t ssrc, u_int8_t pt, u_int32_t ts, u_int16_t seq, u_int32_t data_bytes)
{
  rtp *f_rtp;
  
  f_rtp = malloc(sizeof(struct rtp));

  memset (f_rtp, 0, sizeof (struct rtp));

  f_rtp->initial_seqno = seq;
  f_rtp->largest_seqno = seq;
  f_rtp->packets_win[0] = seq;
  f_rtp->pnum = 1;		/* is the first packet of the RTP flow */
  f_rtp->ssrc = ssrc;
  f_rtp->first_time = current_time;
  f_rtp->last_time = current_time;
  f_rtp->jitter_min = MAXFLOAT;
  f_rtp->av_jitter_min[0] = MAXFLOAT;
  f_rtp->av_jitter_min[1] = MAXFLOAT;
  f_rtp->first_ts = ts;
  f_rtp->largest_ts = ts;
  f_rtp->bogus_reset_during_flow = FALSE;
  f_rtp->data_bytes = data_bytes;
  f_rtp->max_payload_bytes = data_bytes;
  f_rtp->min_payload_bytes = data_bytes;
  f_rtp->squared_data_bytes = data_bytes * data_bytes;
  rtp_init_pt_counter(f_rtp);
  rtp_update_pt_counter(f_rtp,pt);
  f_rtp->next = NULL;

  return f_rtp;
}

rtcp *new_rtcp_subflow(u_int32_t ssrc, u_int8_t pt, u_long initial_bytes)
{
  rtcp *f_rtcp;

  f_rtcp = malloc(sizeof(struct rtcp));
  
  memset (f_rtcp, 0, sizeof (struct rtcp));

  f_rtcp->pnum = 1;
  f_rtcp->initial_data_bytes = initial_bytes;
  f_rtcp->first_time = current_time;
  f_rtcp->last_time = current_time;
  f_rtcp->ssrc = ssrc;
  f_rtcp->rtcp_data_bytes = 0;

  rtcp_init_pt_counter(f_rtcp);
  rtcp_update_pt_counter(f_rtcp,pt);
  f_rtcp->next = NULL;

  return f_rtcp;
}

void rtcp_init_stats(rtcp *f_rtcp, char *payload_ptr,u_int8_t pt,unsigned char rc, void *plast, struct sudp_pair *pup, int dir)
{
  ucb *otherdir;
  char *pdecode;
  struct rtcp_SR *SR;
  struct rtcp_RR *RR;
  
  pdecode = payload_ptr;
      
  pdecode += 8;		// skip common header

  switch (pt)
   {
     case RTCP_PT_SR:		/* Sender Report */
       if ((void *) (pdecode + sizeof (struct rtcp_SR)) > plast)
	{
	  f_rtcp->rtcp_header_error |= E_TRUNCATED;
	  if(warn_printtrunc)
	    {
	      fprintf (fp_stderr, "Warning: RTCP packet is truncated!\n");
	      break;
	    }
        }
          SR = (struct rtcp_SR *) pdecode;
	  f_rtcp->last_SR = current_time;
	  f_rtcp->last_SR_id = swap32 (SR->ntp_ts >> 16);
	  f_rtcp->tx_p = swap32 (SR->tx_p);
	  f_rtcp->tx_b = swap32 (SR->tx_b);
	  if (rc == 0)
	    break;
	  pdecode += 20;	// skip sender report
	case RTCP_PT_RR:
          if (rc == 0)
            break;
	  /* Receiver Report */
	  if (rc > 1)
	    {
	      f_rtcp->rtcp_header_error |= E_TOOMANYRR;
	      if (debug > 1)
		fprintf (fp_stderr,
			 "Warning: Tstat is able to manage RTCP with only 1 Receiver Report!\n");
	      break;
	    }
	  if ((void *) (pdecode + sizeof (struct rtcp_RR)) > plast)
	  {
	    f_rtcp->rtcp_header_error |= E_TRUNCATED;
	    if(warn_printtrunc)
	    {
	      if (debug > 1)
		fprintf (fp_stderr, "Warning: RTCP packet is truncated!\n");
	      break;
	    }
	  }
	  RR = (struct rtcp_RR *) pdecode;
	  /* jitter */
	  f_rtcp->jitter_sum = f_rtcp->jitter_max = f_rtcp->jitter_min = swap32 (RR->jitter);	/* TODO: conversion */
	  f_rtcp->jitter_samples++;
	  /* c_lost */
//           printf("RTCP SSRC 0x%x PT %d C %s %s",pts,pt,HostName(pup->addr_pair.a_address),ServiceName(pup->addr_pair.a_port));
//           printf(" S %s %s - ",HostName(pup->addr_pair.b_address),ServiceName(pup->addr_pair.b_port));

//	  printf("RR Record %02x %02x %02x %02x\n",(unsigned char)*(pdecode),(unsigned char)*(pdecode+1),
//						   (unsigned char)*(pdecode+2),(unsigned char)*(pdecode+3));
	  f_rtcp->c_lost = swap24 (RR->c_lost);
	  f_rtcp->f_lost = (unsigned char) RR->f_lost;
	  f_rtcp->f_lost_sum += (unsigned char) RR->f_lost;
	  /* RTT */
	  if (dir == C2S)
	    otherdir = &pup->s2c;
	  else
	    otherdir = &pup->c2s;

          if ( ( otherdir->type == RTP_PLUS || otherdir->type == FIRST_RTP_PLUS )
	       && otherdir->flow_ptr.rtcp_ptr!=NULL )	/* must be bidirectional RTCP */
           /* The other direction is RTC_PLUS and I've seen at least one RTCP packet */
	   {
	     u_int32_t RR_lst = swap32 (RR->lsr);
	     if (RR_lst != 0)	/* must be different from 0   */
	      {
	        /* Should locate the correct RTCP subflow in the opposite direction */
	        rtcp *matched_record;
	        matched_record = otherdir->flow_ptr.rtcp_ptr;
	        while (matched_record!=NULL)
	         {
	           if ( RR_lst == matched_record->last_SR_id ) /* do we have the correct SR ? */
		    {
		      double rtt;
		      rtt =
		        elapsed (matched_record->last_SR,current_time) / 1000.0 -
		              swap32 (RR->dlsr) * 1000.0 / 65536.0;
		      if (rtt >= 0)
		       {
		          if (f_rtcp->rtt_min > rtt || f_rtcp->rtt_sum == 0)
		             f_rtcp->rtt_min = rtt;
		          if (f_rtcp->rtt_max < rtt || f_rtcp->rtt_sum == 0)
		             f_rtcp->rtt_max = rtt;
		          f_rtcp->rtt_sum += rtt;
		          f_rtcp->rtt_samples++;
		       }
		      break;
		    }
		   matched_record = matched_record->next;
	         }
	      }
	   }
	  break;
        default: /* Any other RTCP_PT_XXX type */
	  break;
	}
  
  return;
}

int rtp_extensions_size(struct rtphdr *prtp)
{
  int extension_size = 0;
  char *base = (char *)prtp;
  
  if (prtp->x == 0)
    return 0;
  
//  printf("We have an RTP extension!!! ");

  extension_size = 4;
  base += 12 + prtp->cc * 4;
  
  if (get_u8(base,0)==0xbe && get_u8(base,1)==0xde)
   {
     // RFC 5285  1-byte extensions
//     printf(" %d bytes\n",4*ntohs(get_u16(base,2)));
     extension_size += 4*ntohs(get_u16(base,2));
     
#ifdef NAVIGATE_EXTENSIONS
     // experimental code to navigate the RTP extensions
     {
       int ext_ID;
       int ext_len;
       int ext_num;
       int ext_size = 4*ntohs(get_u16(base,2));
       // printf("Dimensione estensioni: %d - ",ext_size);

       int ext_offset = 0;
       int j;
       while (ext_offset < ext_size)
       {
           ext_ID = (get_u8(base,4+ext_offset) & 0x00f0 ) >> 4;
           ext_len = (get_u8(base,4+ext_offset) & 0x000f ) ;
           if (ext_ID==0 && ext_len == 0)
            {
                ext_offset += 1;
          //      printf("PAD - ");
            }
           else if (ext_ID<15)
           {
             // printf("ID: %d LEN: %d - ",ext_ID,ext_len+1);
             printf("(%d,",ext_ID);
             for (j=0;j<=ext_len;j++)
              { 
                printf("%02x",get_u8(base,4+ext_offset+1+j));
              }
              printf(") ");
             ext_offset += ext_len+1+1;
           }
           else 
           {
            //   printf("ID: %d ",ext_ID);
               break;
           }
       } 
       printf("\n");
    }
#endif

   }
  else if (get_u8(base,0)==0x10 && (get_u8(base,1) & 0xf0 ) == 0 )
   {
     // RFC 5285  2-byte extensions
 //    printf(" %d bytes - 2-bytes extensions\n",4*ntohs(get_u16(base,2)));
     extension_size += 4*ntohs(get_u16(base,2));
   }
  else
   {
     if (debug>1)
       fprintf (fp_stderr,"Warning: RTP Unknown extension format!\n");
   } 
  return extension_size;
}

/******** function used to find the RTP packet starting point **********/

struct rtphdr *
getrtp (struct udphdr *pudp, int tproto, void *prtp, void *plast)
{
  void *theheader;

  theheader = ((char *) pudp + 8);
  if ((u_long) theheader + (sizeof (struct rtphdr)) - 1 > (u_long) plast)
    {
      prtp = (void *) NULL;	// part of the header is missing       
    }
  else
    {
      prtp = (void *) theheader;
    }

  return (struct rtphdr *) prtp;
}



void
rtp_flow_stat (struct ip *pip, void *pproto, int tproto, void *pdir, int dir,
	       void *hdr, void *plast)
{
  struct udphdr *pudp;
  struct rtphdr *prtp;
  ucb *thisdir;

  if (tproto == PROTOCOL_TCP)
    {
      fprintf (fp_stderr, "Aaargh !? RTP over TCP !!!\n");
      exit (1);
    }

  prtp = (struct rtphdr *) hdr;
  pudp = (struct udphdr *) pproto;
  thisdir = (ucb *) pdir;

  switch (thisdir->type)
    {
    case UDP_UNKNOWN:
      {
        if ( (thisdir->flow_ptr.rtcp_ptr==NULL) && (thisdir->flow_ptr.rtp_ptr==NULL) )
	      init_rtp (thisdir, dir, pudp, prtp, plast);
	    break; 
      }
    case UDP_DTLS:
      {
        thisdir->multiplexed_protocols |= RFC7983_DTLS;
        if ( (thisdir->flow_ptr.rtcp_ptr==NULL) && (thisdir->flow_ptr.rtp_ptr==NULL) )
	      init_rtp (thisdir, dir, pudp, prtp, plast);
	    break;
      }
//    This could be enabled if MTURN and RTP could mix -MMM-  
//    case UDP_MTURN:
//      {
//        thisdir->multiplexed_protocols |= RFC7983_MTURN;
//        if ( (thisdir->flow_ptr.rtcp_ptr==NULL) && (thisdir->flow_ptr.rtp_ptr==NULL) )
//	      init_rtp (thisdir, dir, pudp, prtp, plast);
//	    break;
//      }
    case FIRST_RTP_PLUS:
      {
	    /* we got either a RTP or a RTCP packet ... double check until we have
	       confirmation for either protocol */
	    rtp_plus_check (thisdir, prtp, dir, pip, pudp, plast);
	    break;
      }
    case RTP_PLUS:
      {
	/* We have a full RTP or RTCP state in either direction. Now manage
	 * the cross-cases.
	 * It should be quite similar to rtp_plus_check
	 * Let's put it in a function
	 */
	
	rtp_plus_stat(thisdir, prtp, dir, pip, pudp, plast);
	break;
      }
    case SKYPE_E2E:
    case SKYPE_OUT:
    case SKYPE_SIG:
    default:
	break;
      
    }
}

/**** function used to initialize the parameters of the flows ****/

void
init_rtp (ucb * thisdir, int dir, struct udphdr *pudp, struct rtphdr *prtp,
	  void *plast)
{
  struct rtp *f_rtp;
  struct rtcp *f_rtcp;
  struct sudp_pair *pup;
  u_int8_t pt_rtcp;
  pup = thisdir->pup;
  int rfc7983_packet_type;
  int packet_payload;
  
  u_int32_t pssrc = swap32 (prtp->ssrc);
  u_int32_t pts = swap32 (prtp->ts);
  u_int16_t pseq = swap16 (prtp->seqno);
  
  /* this may be RTP or RTCP...
     version must be 2
     pt must be RTP valid
     udp port must be odd and greater than 1024
     then it is a RTCP flow... otherwise it may bw a RTP flow
     Note: the RTP and RTCP headers are similar but not identical
   */
  /* RTCP */
  pt_rtcp = prtp->pt + (prtp->m << 7);
  
  rfc7983_packet_type = RFC7983_classify ((void*)prtp);
  
  if ( rfc7983_packet_type == RFC7983_RTCP && 
      (pup->addr_pair.a_port > 1024) && 
      (pup->addr_pair.b_port > 1024))
    {
      unsigned char rc = prtp->cc + (prtp->x << 4);
      char *pdecode = (char *) prtp;

      if (thisdir->flow_ptr.rtcp_ptr!=NULL)
       {
	  fprintf (fp_stderr, "Warning: RTCP You are not supposed to be here without a NULL pointer!\n");

	 /* Release any previously allocated information. rtcp_prt might be a list. */
         while (thisdir->flow_ptr.rtcp_ptr!=NULL)
          {
             rtcp *this = thisdir->flow_ptr.rtcp_ptr;
             thisdir->flow_ptr.rtcp_ptr = thisdir->flow_ptr.rtcp_ptr->next;
             free(this);
          }
       }
      
      f_rtcp = new_rtcp_subflow(pts, pt_rtcp, thisdir->data_bytes - ntohs (pudp->uh_ulen));
      // f_rtcp->rtcp_data_bytes += getpayloadlength(pip,plast) - 8 ;
      f_rtcp->rtcp_data_bytes += ntohs (pudp->uh_ulen) - 8;
      //printf("First RTCP %d\n",f_rtcp->rtcp_data_bytes);
      
      thisdir->flow_ptr.rtcp_ptr = f_rtcp;
      f_rtcp->next = NULL;
      
      // printf("RTCP SSRC 0x%x PT %d C %s",pts,pt_rtcp,HostName(pup->addr_pair.a_address));
      // printf(" S %s\n",HostName(pup->addr_pair.b_address));

      rtcp_init_stats(f_rtcp,pdecode,pt_rtcp,rc,plast,pup,dir);
      
      thisdir->type = FIRST_RTP_PLUS;
    }
  /* RTP */
  else if (rfc7983_packet_type == RFC7983_RTP&& 
           (pup->addr_pair.a_port > 1024) && 
           (pup->addr_pair.b_port > 1024))
    {
    
      if (thisdir->flow_ptr.rtp_ptr!=NULL)
       {
	  fprintf (fp_stderr, "Warning: RTP You are not supposed to be here without a NULL pointer!\n");
	  
	 /* Release any previously allocated information. rtp_prt might be a list. */
         while (thisdir->flow_ptr.rtp_ptr!=NULL)
          {
             rtp *this = thisdir->flow_ptr.rtp_ptr;
             thisdir->flow_ptr.rtp_ptr = thisdir->flow_ptr.rtp_ptr->next;
             free(this);
          }
       }
      
      packet_payload = ntohs (pudp->uh_ulen) - 8 - 12 - prtp->cc * 4 - rtp_extensions_size(prtp);
      f_rtp = new_rtp_subflow(pssrc,prtp->pt,pts,pseq, packet_payload );
      
      thisdir->flow_ptr.rtp_ptr = f_rtp;
      f_rtp->next = NULL;

      thisdir->type = FIRST_RTP_PLUS;
    }

}



/* function used to control if the current analyzed packet is an RTP or an RTCP packet */
void
rtp_plus_check (ucb * thisdir, struct rtphdr *prtp, int dir, struct ip *pip, struct udphdr *pudp, void *plast)
{
   int rfc7983_packet_type;
   int packet_payload;
   int ssrc_list_length;
   int ssrc_singletons_count;
   struct rtp *f_rtp = NULL;
   struct rtcp *f_rtcp = NULL;
   u_int8_t pt_rtcp;
   pt_rtcp = prtp->pt + (prtp->m << 7);

   u_int32_t pssrc = swap32 (prtp->ssrc);
   u_int32_t pts = swap32 (prtp->ts);
   u_int16_t pseq = swap16 (prtp->seqno);
   
   rfc7983_packet_type = RFC7983_classify ((void*)prtp);
   
   switch(rfc7983_packet_type)
    {
     case RFC7983_RTP:
       // Check RTP stuff
       
       /* locate previous SSRC record */
       f_rtp = rtp_locate_ssrc(thisdir->flow_ptr.rtp_ptr,pssrc,&ssrc_list_length,&ssrc_singletons_count); 

       // If found, update status, else, allocate a new one

       if (f_rtp!=NULL)
         {
          /* Located a flow with the same SSRC */
          if ( (f_rtp->initial_seqno + (u_int16_t) f_rtp->pnum == pseq))
            {
              f_RTP_count++;
              switch (in_out_loc(thisdir->pup->internal_src, thisdir->pup->internal_dst, dir))
               {
                 case OUT_FLOW:
                   add_histo (L7_UDP_num_out, L7_FLOW_RTP);
                   if ( (dir==C2S && thisdir->pup->cloud_dst) || (dir==S2C && thisdir->pup->cloud_src))
                    {
                      add_histo (L7_UDP_num_c_out, L7_FLOW_RTP);
                    }
                   else
                    {
                      add_histo (L7_UDP_num_nc_out, L7_FLOW_RTP);
                    }
                   break;
                 case IN_FLOW:
                   add_histo (L7_UDP_num_in, L7_FLOW_RTP);
                   if ( (dir==C2S && thisdir->pup->cloud_src) || (dir==S2C && thisdir->pup->cloud_dst))
                    {
                      add_histo (L7_UDP_num_c_in, L7_FLOW_RTP);
                    }
                   else
                    {
                      add_histo (L7_UDP_num_nc_in, L7_FLOW_RTP);
                    }
                   break;
                 case LOC_FLOW:
                   add_histo (L7_UDP_num_loc, L7_FLOW_RTP);
                   break;
               }
              rtp_stat (thisdir, f_rtp, prtp, dir, pip, plast);
	      
              // thisdir->type = RTP;
              thisdir->type = RTP_PLUS;
              thisdir->multiplexed_protocols |= RFC7983_RTP;
          
            }
              /* Flow located, status confirmed */
          else
            {
	     // Correct SSRC but wrong SeqNo?
	          if (debug > 1)
	            fprintf (fp_stderr, "Warning: Ignoring not matching starting RTP seqno\n");
            }
         }
       else if (ssrc_list_length <= MAX_COUNT_RTP_SSRC && ssrc_singletons_count <= RTP_SSRC_MAX_SINGLETON)
         { 
           rtp *f_rtp2;

           // printf ("Len = %d Single(%d) = %d\n",ssrc_list_length,RTP_SSRC_SINGLETON_THRESHOLD,ssrc_singletons_count);
	   packet_payload = ntohs (pudp->uh_ulen) - 8 - 12 - prtp->cc * 4 -rtp_extensions_size(prtp);
           f_rtp2 = new_rtp_subflow(pssrc,prtp->pt,pts,pseq,packet_payload);

           f_rtp2->next = thisdir->flow_ptr.rtp_ptr;
           thisdir->flow_ptr.rtp_ptr = f_rtp2;
      
           thisdir->multiplexed_protocols |= RFC7983_RTP;
           // Do not change the type, since we still do not have a full RTP confirmation
         }
       else if (ssrc_list_length <= MAX_COUNT_RTP_SSRC && ssrc_singletons_count > RTP_SSRC_MAX_SINGLETON)
         {
	   // Too many singletons . This is probably a false positive. 
	   fprintf (fp_stderr, "Warning: RTP More than %d SSRC with less than %d packets associated to the flow!\n",RTP_SSRC_MAX_SINGLETON, RTP_SSRC_SINGLETON_THRESHOLD+1);
	   thisdir->type = UDP_UNKNOWN;
         }
       else
         {
	   // The SSRC list is too long. This is probably a false positive. Forget this flow
	   fprintf (fp_stderr, "Warning: RTP Too many SSRC associated to the flow!\n");
	   thisdir->type = UDP_UNKNOWN;
         }
       break;

     case RFC7983_RTCP:
       // Check RTCP stuff
      /* locate previous SSRC record */
       f_rtcp = rtcp_locate_ssrc(thisdir->flow_ptr.rtcp_ptr,pts,&ssrc_list_length,&ssrc_singletons_count); 

       if (f_rtcp!=NULL)
        {
	 if ( f_rtcp->pnum == 1 )
           {
             f_RTCP_count++;
             switch (in_out_loc(thisdir->pup->internal_src, thisdir->pup->internal_dst,dir))
              {
                case OUT_FLOW:
                  add_histo (L7_UDP_num_out, L7_FLOW_RTCP);add_histo (L7_UDP_num_out, L7_FLOW_RTCP);
	          if ( (dir==C2S && thisdir->pup->cloud_dst) || (dir==S2C && thisdir->pup->cloud_src))
	           {
            	     add_histo (L7_UDP_num_c_out, L7_FLOW_RTCP);
	           }
	          else
	           {
            	    add_histo (L7_UDP_num_nc_out, L7_FLOW_RTCP);
	           }
                break;
              case IN_FLOW:
                 add_histo (L7_UDP_num_in, L7_FLOW_RTCP);
	         if ( (dir==C2S && thisdir->pup->cloud_src) || (dir==S2C && thisdir->pup->cloud_dst))
	          {
            	    add_histo (L7_UDP_num_c_in, L7_FLOW_RTCP);
	          }
	        else
	          {
            	    add_histo (L7_UDP_num_nc_in, L7_FLOW_RTCP);
	          }
                break;
              case LOC_FLOW:
                add_histo (L7_UDP_num_loc, L7_FLOW_RTCP);
                break;
             }
            rtcp_stat (thisdir, f_rtcp, prtp, dir, pip, plast);
     //      thisdir->type = RTCP;
            thisdir->type = RTP_PLUS;
            thisdir->multiplexed_protocols |= RFC7983_RTCP;
           }	 
	 else
	   {
	     // Found but counter != 1
	     // Correct SSRC but wrong SeqNo?
	     if (debug > 1)
	       fprintf (fp_stderr, "Warning: Ignoring RTCP packet not matching the sequence\n");
	     // Probably impossible due to the state machine workings
	   }   
	  
       }
       else if (ssrc_list_length <= MAX_COUNT_RTP_SSRC && ssrc_singletons_count <= RTP_SSRC_MAX_SINGLETON)
        {
          rtcp *f_rtcp2;
          char *pdecode = (char *) prtp;
          unsigned char rc = prtp->cc + (prtp->x << 4);
 
          //  printf ("Len = %d Single(%d) = %d\n",ssrc_list_length,RTP_SSRC_SINGLETON_THRESHOLD,ssrc_singletons_count);
         f_rtcp2 = new_rtcp_subflow(pts, pt_rtcp, thisdir->data_bytes - ntohs (pudp->uh_ulen));
      // f_rtcp->rtcp_data_bytes += getpayloadlength(pip,plast) - 8 ;
      f_rtcp2->rtcp_data_bytes += ntohs (pudp->uh_ulen) - 8;
      //printf("First RTCP %d\n",f_rtcp2->rtcp_data_bytes);

          f_rtcp2->next = thisdir->flow_ptr.rtcp_ptr;
          thisdir->flow_ptr.rtcp_ptr = f_rtcp2;
      
          rtcp_init_stats(f_rtcp2,pdecode,pt_rtcp,rc,plast,thisdir->pup,dir);
      
          thisdir->multiplexed_protocols |= RFC7983_RTCP;
           // Do not change the type, since we still do not have a full RTP confirmation
        }
       else if (ssrc_list_length <= MAX_COUNT_RTP_SSRC && ssrc_singletons_count > RTP_SSRC_MAX_SINGLETON)
         {
	   // Too many singletons . This is probably a false positive. 
	   fprintf (fp_stderr, "Warning: RTP More than %d SSRC with less than %d packets associated to the flow!\n",RTP_SSRC_MAX_SINGLETON, RTP_SSRC_SINGLETON_THRESHOLD+1);
	   thisdir->type = UDP_UNKNOWN;
         }
       else
         {
	   // The SSRC list is too long. This is probably a false positive. Forget this flow
	   fprintf (fp_stderr, "Warning: RTCP Too many SSRC associated to the flow!\n");
	   thisdir->type = UDP_UNKNOWN;
         }
       break;

     case RFC7983_UNK:
        // reset status
        /* No minimal matching. No subflow and flow status reset */
	/* At the next iteration, this might trigger a warning (Pointer not NULL), 
	 * and the release of the existing rtp_plus data structures
	 */
       thisdir->type = UDP_UNKNOWN;
       break;

     case RFC7983_MTURN:
         // Verify that the MTURN format stands (0xff10 + length + 64bit id)
         // https://patents.google.com/patent/US20160380789
         if (ntohs (pudp->uh_ulen) == (ntohs(get_u16((char*)prtp,2)) + 12) )
             // Ignore allowed non RTP packet
           thisdir->multiplexed_protocols |= rfc7983_packet_type;
         else
             // MTURN check failed. Convert to UDP_UNKNOWN
           thisdir->type = UDP_UNKNOWN; 
         break;
     default:
        // ignore and set flag
        /* Ignored allowed non RTP packet */
       thisdir->multiplexed_protocols |= rfc7983_packet_type;
       break;
   }
 
}

void rtp_plus_stat (ucb * thisdir, struct rtphdr *prtp, int dir, struct ip *pip, struct udphdr *pudp,void *plast)
{
   int rfc7983_packet_type;
   int packet_payload;
   int ssrc_list_length;
   int ssrc_singletons_count;
   struct rtp *f_rtp = NULL;
   struct rtcp *f_rtcp = NULL;
   
   u_int8_t pt_rtcp;
   pt_rtcp = prtp->pt + (prtp->m << 7);
   
   u_int32_t pssrc = swap32 (prtp->ssrc);
   u_int32_t pts = swap32 (prtp->ts);
   u_int16_t pseq = swap16 (prtp->seqno);
   
   rfc7983_packet_type = RFC7983_classify ((void*)prtp);
   
   switch(rfc7983_packet_type)
   {
     case RFC7983_RTP:
       // Check RTP stuff
       if  ( (thisdir->pup->addr_pair.a_port > 1024) && 
             (thisdir->pup->addr_pair.b_port > 1024) )
        {
          /* locate previous SSRC record */
         f_rtp = rtp_locate_ssrc(thisdir->flow_ptr.rtp_ptr,pssrc,&ssrc_list_length,&ssrc_singletons_count);
         if (f_rtp!=NULL)
          {
           packet_payload = getpayloadlength(pip,plast) - 8 - 12 - prtp->cc * 4 - rtp_extensions_size(prtp);
           if (packet_payload >= 0 )
             {
               rtp_stat (thisdir, f_rtp, prtp, dir, pip, plast);
               thisdir->multiplexed_protocols |= RFC7983_RTP;
             }
           else
             {
               thisdir->type = UDP_UNKNOWN; 
               if (debug > 1)
	             fprintf (fp_stderr, "Warning: RTP packet with negative payload length\n");
             }
          }
         else if (ssrc_list_length <= MAX_COUNT_RTP_SSRC && ssrc_singletons_count <= RTP_SSRC_MAX_SINGLETON)
          {
            // Allocate the new subflow
            rtp *f_rtp2;
  
         //  printf ("Len = %d Single(%d) = %d\n",ssrc_list_length,RTP_SSRC_SINGLETON_THRESHOLD,ssrc_singletons_count);
	    packet_payload = ntohs (pudp->uh_ulen) - 8 - 12 - prtp->cc * 4 - rtp_extensions_size(prtp);
            f_rtp2 = new_rtp_subflow(pssrc,prtp->pt,pts,pseq, packet_payload );

            f_rtp2->next = thisdir->flow_ptr.rtp_ptr;
            thisdir->flow_ptr.rtp_ptr = f_rtp2;
          }
       else if (ssrc_list_length <= MAX_COUNT_RTP_SSRC && ssrc_singletons_count > RTP_SSRC_MAX_SINGLETON)
         {
	   // Too many singletons . This is probably a false positive. 
	   fprintf (fp_stderr, "Warning: RTP More than %d SSRC with less than %d packets associated to the flow!\n",RTP_SSRC_MAX_SINGLETON, RTP_SSRC_SINGLETON_THRESHOLD+1);
	   thisdir->type = UDP_UNKNOWN;
         }
         else
          {
	    // SSRC list became too long. Ignore.
	   fprintf (fp_stderr, "Warning: RTP Too many SSRC associated to the flow!\n");
            thisdir->type = UDP_UNKNOWN; 
          }
        }
       else
        {
	  // Ports are wrong. It should have not been RTP_PLUS from the beginning
          thisdir->type = UDP_UNKNOWN; 
        }
       break;
       
     case RFC7983_RTCP:
       // Check RTCP stuff
       /* locate previous SSRC record */
       if  ( (thisdir->pup->addr_pair.a_port > 1024) && 
             (thisdir->pup->addr_pair.b_port > 1024) )
        {
	  f_rtcp = rtcp_locate_ssrc(thisdir->flow_ptr.rtcp_ptr,pts,&ssrc_list_length,&ssrc_singletons_count);
          if (f_rtcp!=NULL)
	   { 
             thisdir->multiplexed_protocols |= RFC7983_RTCP;
	     rtcp_stat (thisdir, f_rtcp, prtp, dir, pip, plast);
	   }
	  else if (ssrc_list_length <= MAX_COUNT_RTP_SSRC && ssrc_singletons_count <= RTP_SSRC_MAX_SINGLETON)
	   {
	     // Allocate a new subflow
             rtcp *f_rtcp2;
             char *pdecode = (char *) prtp;
             unsigned char rc = prtp->cc + (prtp->x << 4);
 
        //   printf ("Len = %d Single(%d) = %d\n",ssrc_list_length,RTP_SSRC_SINGLETON_THRESHOLD,ssrc_singletons_count);
             f_rtcp2 = new_rtcp_subflow(pts, pt_rtcp, thisdir->data_bytes - ntohs (pudp->uh_ulen));
      // f_rtcp->rtcp_data_bytes += getpayloadlength(pip,plast) - 8 ;
      f_rtcp2->rtcp_data_bytes += ntohs (pudp->uh_ulen) - 8;
      //printf("First RTCP %d\n",f_rtcp2->rtcp_data_bytes);

             f_rtcp2->next = thisdir->flow_ptr.rtcp_ptr;
             thisdir->flow_ptr.rtcp_ptr = f_rtcp2;
      
             rtcp_init_stats(f_rtcp2,pdecode,pt_rtcp,rc,plast,thisdir->pup,dir);
      
             thisdir->multiplexed_protocols |= RFC7983_RTCP;
	   }
       else if (ssrc_list_length <= MAX_COUNT_RTP_SSRC && ssrc_singletons_count > RTP_SSRC_MAX_SINGLETON)
         {
	   // Too many singletons . This is probably a false positive. 
	   fprintf (fp_stderr, "Warning: RTP More than %d SSRC with less than %d packets associated to the flow!\n",RTP_SSRC_MAX_SINGLETON, RTP_SSRC_SINGLETON_THRESHOLD+1);
	   thisdir->type = UDP_UNKNOWN;
         }
         else
          {
	   // The SSRC list is too long. This is probably a false positive. Forget this flow
	   fprintf (fp_stderr, "Warning: RTCP Too many SSRC associated to the flow!\n");
            thisdir->type = UDP_UNKNOWN; 
        }
        }
       else
        {
	  // Ports are wrong. It should have not been RTP_PLUS from the beginning
          thisdir->type = UDP_UNKNOWN; 
        }
       break;

     case RFC7983_UNK:
        // reset status
        /* No minimal matching. No subflow and flow status reset */
       thisdir->type = UDP_UNKNOWN;
       break;

     case RFC7983_MTURN:
         // Verify that the MTURN format stands (0xff10 + length + 64bit id)
         // https://patents.google.com/patent/US20160380789
         if (ntohs (pudp->uh_ulen) == (ntohs(get_u16((char*)prtp,2)) + 12) )
             // Ignore allowed non RTP packet
           thisdir->multiplexed_protocols |= rfc7983_packet_type;
         else
             // MTURN check failed. Convert to UDP_UNKNOWN
           thisdir->type = UDP_UNKNOWN; 
         break;
       
     default:
       // ignore and set flag
       /* Ignored allowed non RTP packet */
       thisdir->multiplexed_protocols |= rfc7983_packet_type;
       break;
   }
  
}


/******* Statistics about RTP flows ********/

void
rtp_stat (ucb * thisdir, struct rtp *f_rtp, struct rtphdr *prtp, int dir,
	  struct ip *pip, void *plast)
{
  struct sudp_pair *pup;
  int i, freq, index, delta_seq, delta_seq_win;
  double period;
  double delta_t, t, ts, transit, d;
#ifdef LOG_OOO
  int seg_type = IN_SEQ;
  extern FILE *fp_dup_ooo_log;
#endif

//  u_int32_t pssrc = swap32 (prtp->ssrc);
  u_int32_t pts = swap32 (prtp->ts);
  u_int16_t pseq = swap16 (prtp->seqno);

  pup = thisdir->pup;
  f_rtp->pnum++;

  freq = det_freq (prtp);
  if (freq != 0)
    period = 1000. / freq;
  else
    period = 1000.;

  /* topix */
  f_rtp->pt = prtp->pt;
  /* end topix */
  rtp_update_pt_counter(f_rtp,prtp->pt);


  /* TOPIX compute data length */
  /* IP_len - IP_header - UDP_header - RTP_(fixed header+optional header) */
  /*    ntohs (pip->ip_len) - pip->ip_hl * 4 - 8 - 12 - prtp->cc * 4; */
  int packet_payload = 
      getpayloadlength(pip,plast) - 8 - 12 - prtp->cc * 4; /* Should be OK also for IPv6 */
//  f_rtp->data_bytes +=
//      getpayloadlength(pip,plast) - 8 - 12 - prtp->cc * 4; /* Should be OK also for IPv6 */

  packet_payload -= rtp_extensions_size(prtp);
  
  if (packet_payload<0)
   {
     printf("RTP Packet with negative payload. Possible mismatch.\n");
     packet_payload = 0;
   }
  f_rtp->data_bytes += packet_payload;
  f_rtp->max_payload_bytes = max(f_rtp->max_payload_bytes,packet_payload);
  f_rtp->min_payload_bytes = min(f_rtp->min_payload_bytes,packet_payload);
  f_rtp->squared_data_bytes += packet_payload*packet_payload;

/** management of the window used for oos, duplicate, late or lost packets **/

  if ((u_int16_t) (pseq - f_rtp->initial_seqno) >= RTP_WIN)
    f_rtp->w = TRUE;		/* got data for at least 1 window */
  index = ((u_int16_t) (pseq - f_rtp->initial_seqno) % RTP_WIN);
  delta_seq_win = (u_int16_t) (pseq - f_rtp->packets_win[index]);

  /* if there is a sudden jump in the sequence number 
     it happens that some bogus implementation of rtp encoders 
     change the sequence numbering at random (CISCO...)
     So try to detect them */
  if (delta_seq_win > RTP_WIN * 5
      && (u_int32_t) (pts - f_rtp->largest_ts) < 0xffff0000
      && f_rtp->w == TRUE)
    {
      f_rtp->w = FALSE;		/* window not valid */
      f_rtp->largest_seqno = pseq;
      f_rtp->initial_seqno = pseq - 1;
      f_rtp->first_ts = pts - (f_rtp->largest_ts - f_rtp->first_ts);	/* THEY SEEMS TO CHANGE THE TS as well */
      f_rtp->largest_ts = pts;
      f_rtp->bogus_reset_during_flow = TRUE;

      for (i = 0; i < RTP_WIN; i++)	/* reset of the window at the beginning of the flow */
	f_rtp->packets_win[i] = pseq - 1;

      index = ((u_int16_t) (pseq - f_rtp->initial_seqno) % RTP_WIN);	/* equal to 1 */
      delta_seq_win = (u_int16_t) (pseq - f_rtp->packets_win[index]);	/* idem */
    }

  /* delta_t evaluation if packets are in sequence */
  if ((u_int16_t) (f_rtp->largest_seqno + 1) == pseq)
    {
      /* delta_t in milliseconds */
      delta_t = elapsed (f_rtp->last_time, current_time);
      delta_t = delta_t / 1000;
      f_rtp->sum_delta_t += delta_t;
      f_rtp->n_delta_t++;


      /* jitter evaluation in milliseconds */

      if (freq != 0)
	{
	  /* alignment of the scales */

	  t = elapsed (f_rtp->first_time, current_time);
	  t = t / 1000.0;
	  ts = (((double) pts - (double) f_rtp->first_ts) * period);

	  /* jitter is computed as in the definition from RFC 3550 */
	  transit = t - ts;
	  d = transit - f_rtp->transit;
	  f_rtp->transit = transit;
	  if (d < 0)
	    d = -d;
	  f_rtp->jitter += (1. / 16.) * (d - f_rtp->jitter);
	  if (f_rtp->jitter_max < f_rtp->jitter)
	    f_rtp->jitter_max = f_rtp->jitter;
	  if (f_rtp->jitter_min > f_rtp->jitter)
	    f_rtp->jitter_min = f_rtp->jitter;
	}
	
      if (freq!=0 || (f_rtp->pt>=96 && f_rtp->pt<=127))
        {
	  // We are using a dynamic payload type.
	  // It's either audio or video. Let's compute 2 jitters, for the two reference
	  // rates 8000 and 90000
	  // We do it anyway also for others valid PT, as a comparison

	  double audio_period,video_period;
	  
	  audio_period = 1000. / 8000.;
	  video_period = 1000. / 90000.;
	  
	  /* Audio 8kHz */
	  /* alignment of the scales */

	  t = elapsed (f_rtp->first_time, current_time);
	  t = t / 1000.0;
	  ts = (((double) pts - (double) f_rtp->first_ts) * audio_period);

	  /* jitter is computed as in the definition from RFC 3550 */
	  transit = t - ts;
	  d = transit - f_rtp->av_transit[0];
	  f_rtp->av_transit[0] = transit;
	  if (d < 0)
	    d = -d;
	  f_rtp->av_jitter[0] += (1. / 16.) * (d - f_rtp->av_jitter[0]);
	  if (f_rtp->av_jitter_max[0] < f_rtp->av_jitter[0])
	    f_rtp->av_jitter_max[0] = f_rtp->av_jitter[0];
	  if (f_rtp->av_jitter_min[0] > f_rtp->av_jitter[0])
	    f_rtp->av_jitter_min[0] = f_rtp->av_jitter[0];
	  

	  /* Video 90 kHz */
	  
	  t = elapsed (f_rtp->first_time, current_time);
	  t = t / 1000.0;
	  ts = (((double) pts - (double) f_rtp->first_ts) * video_period);

	  /* jitter is computed as in the definition from RFC 3550 */
	  transit = t - ts;
	  d = transit - f_rtp->av_transit[1];
	  f_rtp->av_transit[1] = transit;
	  if (d < 0)
	    d = -d;
	  f_rtp->av_jitter[1] += (1. / 16.) * (d - f_rtp->av_jitter[1]);
	  if (f_rtp->av_jitter_max[1] < f_rtp->av_jitter[1])
	    f_rtp->av_jitter_max[1] = f_rtp->av_jitter[1];
	  if (f_rtp->av_jitter_min[1] > f_rtp->av_jitter[1])
	    f_rtp->av_jitter_min[1] = f_rtp->av_jitter[1];
	  
        }

      /* end jitter evaluation */
    }



  /* look at the largest ts */
  if ((u_int32_t) (pts - f_rtp->largest_ts) < OVERFLOW_TH)
    /* control of the overflow!! */
    f_rtp->largest_ts = pts;

  if (f_rtp->w == FALSE)	/* first time using the window */
    {

      if (delta_seq_win == 0)
	{
#ifdef LOG_OOO
	  seg_type = DUP;
#endif
	  f_rtp->n_dup++;
	}

      f_rtp->packets_win[index] = pseq;

      /* if the sequence number is not equal to largest_seqno+1 is 
         considered an out of sequence */
      if ((u_int16_t) (f_rtp->largest_seqno + 1) != pseq)
	{
#ifdef LOG_OOO
	  seg_type = NOT_IN_SEQ;
#endif
	  f_rtp->n_out_of_sequence++;

	  if (pup->internal_src && !pup->internal_dst)
	    {
	      if (dir == C2S)
		{
		  add_histo (mm_oos_p_out, 1);
		}
	      else
		{
		  add_histo (mm_oos_p_in, 1);
		}

	    }
	  else if (!pup->internal_src && pup->internal_dst)
	    {
	      if (dir == C2S)
		{
		  add_histo (mm_oos_p_in, 1);
		}

	      else
		{
		  add_histo (mm_oos_p_out, 1);
		}

	    }
#ifndef LOG_UNKNOWN
	  else if (pup->internal_src && pup->internal_dst)
#else
          else
#endif
	    {
	      add_histo (mm_oos_p_loc, 1);
	    }

	  if (((u_int16_t) (pseq - f_rtp->largest_seqno) > OVERFLOW_TH) &&
	      (delta_seq_win != 0))
	    {
	      double delay;
	      float byte_period;

	      /* evaluate the delay of the oos segment */
//	      byte_period = (ntohs (pip->ip_len) - ntohs (pip->ip_hl) - 8	/* udp header len */
//			     - (12 + ntohs (prtp->cc) * 4)	/* rtp header len */
//		) * period;
              /* Possible bug above, *4 missing in ip_hl  */
	      byte_period = (getpayloadlength(pip,plast) - 8	/* udp header len */
			     - (12 + ntohs (prtp->cc) * 4 + rtp_extensions_size(prtp) )	/* rtp header len */
		) * period;
	      delay =
		//((u_int16_t) (f_rtp->largest_seqno - pseq) * byte_period) +
		((double) (f_rtp->largest_seqno - pseq) * byte_period) +
		(((double) elapsed (f_rtp->last_time, current_time)) /
		 1000.0);
	      if (pup->internal_src && !pup->internal_dst)
		{
		  if (dir == C2S)
		    {
		      add_histo (mm_reord_p_n_out, 1);
		      add_histo (mm_reord_delay_out, delay);
		    }
		  else
		    {
		      add_histo (mm_reord_p_n_in, 1);
		      add_histo (mm_reord_delay_in, delay);
		    }
		}
	      else if (!pup->internal_src && pup->internal_dst)
		{
		  if (dir == C2S)
		    {
		      add_histo (mm_reord_p_n_in, 1);
		      add_histo (mm_reord_delay_in, delay);
		    }
		  else
		    {
		      add_histo (mm_reord_p_n_out, 1);
		      add_histo (mm_reord_delay_out, delay);
		    }
		}
#ifndef LOG_UNKNOWN
	      else if (pup->internal_src && pup->internal_dst)
#else
              else
#endif
		{
		  add_histo (mm_reord_p_n_loc, 1);
		  add_histo (mm_reord_delay_loc, delay);
		}
	    }
	}

      if ((u_int16_t) (pseq - f_rtp->largest_seqno) < OVERFLOW_TH)
	/* control of the overflow!! */
	f_rtp->largest_seqno = pseq;
    }
  else				/* got the window data filled up */
    {
      delta_seq = (u_int16_t) (pseq - f_rtp->largest_seqno);

      /* This packet is late than it doesn't belong to the present window */
      if (delta_seq_win >= OVERFLOW_TH)
	{
#ifdef LOG_OOO
	  seg_type = LATE;
#endif
	  f_rtp->n_late++;
	}
      else if (delta_seq_win == 0)
	{
#ifdef LOG_OOO
	  seg_type = DUP;
#endif
	  f_rtp->n_dup++;
	}
      else if (delta_seq_win > RTP_WIN)
	{
#ifdef LOG_OOO
	  seg_type = LOST;
#endif
	  f_rtp->n_lost++;
	  f_rtp->burst++;
	  f_rtp->packets_win[index] = pseq;
	}
      else if (delta_seq_win == RTP_WIN)
	{
	  /* in sequence */
	  /* Statistics of the burst and reset of the burst length */
	  if (pup->internal_src && !pup->internal_dst)
	    {
	      if (dir == C2S)
		{
		  add_histo (mm_burst_loss_out, f_rtp->burst);
		}
	      else
		{
		  add_histo (mm_burst_loss_in, f_rtp->burst);
		}
	    }
	  else if (!pup->internal_src && pup->internal_dst)
	    {
	      if (dir == C2S)
		{
		  add_histo (mm_burst_loss_in, f_rtp->burst);
		}
	      else
		{
		  add_histo (mm_burst_loss_out, f_rtp->burst);
		}
	    }
#ifndef LOG_UNKNOWN
	  else if (pup->internal_src && pup->internal_dst)
#else
          else
#endif
	    {
	      add_histo (mm_burst_loss_loc, f_rtp->burst);
	    }

	  f_rtp->burst = 0;
	  f_rtp->packets_win[index] = pseq;
	}

      /* if the sequence number is not equal to largest_seqno+1 is considered an
         out of sequence */
      if ((u_int16_t) (f_rtp->largest_seqno + 1) != pseq)
	{
#ifdef LOG_OOO
	  if (seg_type == IN_SEQ)
	    {
	      seg_type = NOT_IN_SEQ;
	    }
#endif
	  f_rtp->n_out_of_sequence++;

	  if (pup->internal_src && !pup->internal_dst)
	    {
	      if (dir == C2S)
		{
		  add_histo (mm_oos_p_out, 1);
		}
	      else
		{
		  add_histo (mm_oos_p_in, 1);
		}

	    }
	  else if (!pup->internal_src && pup->internal_dst)
	    {
	      if (dir == C2S)
		{
		  add_histo (mm_oos_p_in, 1);
		}

	      else
		{
		  add_histo (mm_oos_p_out, 1);
		}
	    }
#ifndef LOG_UNKNOWN
	  else if (pup->internal_src && pup->internal_dst)
#else
          else
#endif
	    {
	      add_histo (mm_oos_p_loc, 1);
	    }

	  if (((u_int16_t) (pseq - f_rtp->largest_seqno) > OVERFLOW_TH) &&
	      (delta_seq_win != 0))
	    {

	      double delay;
	      float byte_period;

//	      byte_period = (ntohs (pip->ip_len) - ntohs (pip->ip_hl) - 8	/* udp header len */
//			     - (12 + ntohs (prtp->cc) * 4)	/* rtp header len */
//		) * period;
              /* Possible bug above, *4 missing in ip_hl  */
	      byte_period = (getpayloadlength(pip,plast) - 8	/* udp header len */
			     - (12 + ntohs (prtp->cc) * 4 + rtp_extensions_size(prtp))	/* rtp header len */
		) * period;
	      delay =
		//((u_int16_t) (f_rtp->largest_seqno - pseq) * byte_period) +
		((double) (f_rtp->largest_seqno - pseq) * byte_period) +
		(((double) elapsed (f_rtp->last_time, current_time)) /
		 1000.0);
	      if (pup->internal_src && !pup->internal_dst)
		{
		  if (dir == C2S)
		    {
		      add_histo (mm_reord_p_n_out, 1);
		      add_histo (mm_reord_delay_out, delay);
		    }
		  else
		    {
		      add_histo (mm_reord_p_n_in, 1);
		      add_histo (mm_reord_delay_in, delay);
		    }
		}
	      else if (!pup->internal_src && pup->internal_dst)
		{
		  if (dir == C2S)
		    {
		      add_histo (mm_reord_p_n_in, 1);
		      add_histo (mm_reord_delay_in, delay);
		    }
		  else
		    {
		      add_histo (mm_reord_p_n_out, 1);
		      add_histo (mm_reord_delay_out, delay);
		    }
		}
#ifndef LOG_UNKNOWN
	      else if (pup->internal_src && pup->internal_dst)
#else
              else
#endif
		{
		  add_histo (mm_reord_p_n_loc, 1);
		  add_histo (mm_reord_delay_loc, delay);
		}
	    }
	}

      if ((u_int16_t) (pseq - f_rtp->largest_seqno) < OVERFLOW_TH)
	/* control of the overflow!! */
	f_rtp->largest_seqno = pseq;
    }


#ifdef LOG_OOO
  if (seg_type != IN_SEQ)
    {
      wfprintf (fp_dup_ooo_log, "R: %f ",
	       (elapsed (first_packet, current_time) / 1000.0));

      if (dir == C2S)
	{
	  if (pup->crypto_src==FALSE)
	     wfprintf (fp_dup_ooo_log, "%d %d %s %s",
		   PROTOCOL_UDP,
		   RTP_PROTOCOL,
		   HostName (pup->addr_pair.a_address),
		   ServiceName (pup->addr_pair.a_port));
          else
	     wfprintf (fp_dup_ooo_log, "%d %d %s %s",
		   PROTOCOL_UDP,
		   RTP_PROTOCOL,
		   HostNameEncrypted (pup->addr_pair.a_address),
		   ServiceName (pup->addr_pair.a_port));
	  if (pup->crypto_dst==FALSE)
	     wfprintf (fp_dup_ooo_log,
		   " %s %s",
		   HostName (pup->addr_pair.b_address),
		   ServiceName (pup->addr_pair.b_port));
	  else
	     wfprintf (fp_dup_ooo_log,
		   " %s %s",
		   HostNameEncrypted (pup->addr_pair.b_address),
		   ServiceName (pup->addr_pair.b_port));
	}
      else
	{
	  if (pup->crypto_dst==FALSE)
	     wfprintf (fp_dup_ooo_log, "%d %d %s %s",
		   PROTOCOL_UDP,
		   RTP_PROTOCOL,
		   HostName (pup->addr_pair.b_address),
		   ServiceName (pup->addr_pair.b_port));
          else
	     wfprintf (fp_dup_ooo_log, "%d %d %s %s",
		   PROTOCOL_UDP,
		   RTP_PROTOCOL,
		   HostNameEncrypted (pup->addr_pair.b_address),
		   ServiceName (pup->addr_pair.b_port));
	  if (pup->crypto_src==FALSE)
	     wfprintf (fp_dup_ooo_log,
		   " %s %s",
		   HostName (pup->addr_pair.a_address),
		   ServiceName (pup->addr_pair.a_port));
	  else
	     wfprintf (fp_dup_ooo_log,
		   " %s %s",
		   HostNameEncrypted (pup->addr_pair.a_address),
		   ServiceName (pup->addr_pair.a_port));
	}
      wfprintf (fp_dup_ooo_log, " %d", seg_type);

      if ((pup->internal_src && !pup->internal_dst && dir == C2S) ||
	  (!pup->internal_src && pup->internal_dst && dir == S2C))
	wfprintf (fp_dup_ooo_log, " %d\n", 1) /* uscente */ ;

      if ((pup->internal_src && !pup->internal_dst && dir == S2C) ||
	  (!pup->internal_src && pup->internal_dst && dir == C2S))
	wfprintf (fp_dup_ooo_log, " %d\n", 0) /* entrante */ ;

#ifndef LOG_UNKNOWN
      if ((pup->internal_src && pup->internal_dst))
#endif
	wfprintf (fp_dup_ooo_log, " %d\n", 2) /* Locale */ ;

    }
#endif
  f_rtp->last_time = current_time;
}



/******* Statistics about RTCP flows ********/

void
rtcp_stat (struct ucb *thisdir, struct rtcp *f_rtcp, struct rtphdr *prtp, int dir, struct ip *pip, void *plast)
{


  struct sudp_pair *pup = thisdir->pup;
  struct ucb *otherdir;

  double delta_t;

  struct rtcp_SR *SR;
  struct rtcp_RR *RR;
  unsigned char rc = prtp->cc + (prtp->x << 4);
  char *pdecode = (char *) prtp;
  u_int8_t pt_rtcp = prtp->pt + (prtp->m << 7);
  u_int32_t jitter;
  double rtt = -1;
  int32_t delta_lost;
  double delta_sr = 0;
  u_int32_t delta_b;

  f_rtcp->pnum++;		/* rtcp packets number */
  /* delta_t calculus in  milliseconds */
  delta_t = elapsed (f_rtcp->last_time, current_time) / 1000.0;
  f_rtcp->sum_delta_t += delta_t;
  f_rtcp->rtcp_data_bytes += getpayloadlength(pip,plast) - 8;
  //printf("RTCP %d add %d\n",pt_rtcp,(getpayloadlength(pip,plast) - 8));

  rtcp_update_pt_counter(f_rtcp, pt_rtcp) ;

  /* RTCP decoding */
  pdecode += 8;
  switch (pt_rtcp)
    {
    case RTCP_PT_SR:			/* Sender Report */
      if ((void *) (pdecode + sizeof (struct rtcp_SR)) > plast)
      {
	f_rtcp->rtcp_header_error |= E_TRUNCATED;
	if(warn_printtrunc)
	{
	  fprintf (fp_stderr, "Warning: RTCP packet is truncated!\n");
	  break;
	}
      }
      SR = (struct rtcp_SR *) pdecode;
      if (f_rtcp->last_SR.tv_sec != 0 || f_rtcp->last_SR.tv_usec != 0)
	delta_sr = elapsed (f_rtcp->last_SR, current_time) / 1000.0;
      f_rtcp->last_SR = current_time;
      f_rtcp->last_SR_id = swap32 (SR->ntp_ts >> 16);
      f_rtcp->tx_p = swap32 (SR->tx_p);
      delta_b = (swap32 (SR->tx_b) - f_rtcp->tx_b) * 8;
      f_rtcp->tx_b = swap32 (SR->tx_b);
      /* histograms update */
      if (pup->internal_src && !pup->internal_dst)
	{
	  if (dir == C2S)
	    {
	      if (delta_sr != 0)
		{
		  add_histo (rtcp_mm_bt_out, delta_b / delta_sr);
		}
	    }
	  else
	    {
	      if (delta_sr != 0)
		{
		  add_histo (rtcp_mm_bt_in, delta_b / delta_sr);
		}
	    }
	}
      else if (!pup->internal_src && pup->internal_dst)
	{
	  if (dir == C2S)
	    {
	      if (delta_sr != 0)
		{
		  add_histo (rtcp_mm_bt_in, delta_b / delta_sr);
		}
	    }
	  else
	    {
	      if (delta_sr != 0)
		{
		  add_histo (rtcp_mm_bt_out, delta_b / delta_sr);
		}
	    }
	}
#ifndef LOG_UNKNOWN
      else if (pup->internal_src && pup->internal_dst)
#else
      else
#endif
	{
	  if (delta_sr != 0)
	    {
	      add_histo (rtcp_mm_bt_loc, delta_b / delta_sr);
	    }
	}

      if (rc == 0)
	break;
      pdecode += 20;
    case RTCP_PT_RR:			/* Receiver Report */
      if (rc == 0)
	break;

      if (rc > 1)
	{
	  f_rtcp->rtcp_header_error |= E_TOOMANYRR;
	  if (debug > 1)
	    fprintf (fp_stderr,
		     "Warning: Tstat is able to manage RTCP with only 1 Receiver Report!\n");
	  break;
	}
      if ((void *) (pdecode + sizeof (struct rtcp_RR)) > plast)
      {
	f_rtcp->rtcp_header_error |= E_TRUNCATED;
	if(warn_printtrunc)
	{
	  if (debug > 1)
	    fprintf (fp_stderr, "Warning: RTCP packet is truncated!\n");
	  break;
	}
      }
      RR = (struct rtcp_RR *) pdecode;
      /* jitter */
      jitter = swap32 (RR->jitter);
      f_rtcp->jitter_sum += jitter;
      f_rtcp->jitter_samples++;
      if (f_rtcp->jitter_max < jitter)
	f_rtcp->jitter_max = jitter;
      if (f_rtcp->jitter_min > jitter)
	f_rtcp->jitter_min = jitter;	/* TODO: conversion */
      /* c_lost */
      delta_lost = swap24 (RR->c_lost) - f_rtcp->c_lost;
      f_rtcp->c_lost = swap24 (RR->c_lost);
      f_rtcp->f_lost = (unsigned char) RR->f_lost;
      f_rtcp->f_lost_sum += (unsigned char) RR->f_lost;
      /* RTT */
      if (dir == C2S)
	otherdir = &pup->s2c;
      else
	otherdir = &pup->c2s;

      if ( ( otherdir->type == RTP_PLUS || otherdir->type == FIRST_RTP_PLUS )
	    && otherdir->flow_ptr.rtcp_ptr!=NULL )	/* must be bidirectional RTCP */
         /* The other direction is RTC_PLUS and I've seen at least one RTCP packet */
	 {
	   u_int32_t RR_lst = swap32 (RR->lsr);

	   if ( RR_lst != 0)	/* must be different from 0   */
	    {
	      /* Should locate the correct RTCP subflow in the opposite direction */
	      rtcp *matched_record;
	      matched_record = otherdir->flow_ptr.rtcp_ptr;
	      while (matched_record!=NULL)
	       {
	          if ( RR_lst == matched_record->last_SR_id ) /* do we have the correct SR ? */
		   {
		     rtt =
		        elapsed (matched_record->last_SR,current_time) / 1000.0 -
		              swap32 (RR->dlsr) * 1000.0 / 65536.0;
		     if (rtt >= 0)
		       {
		          if (f_rtcp->rtt_min > rtt || f_rtcp->rtt_sum == 0)
		             f_rtcp->rtt_min = rtt;
		          if (f_rtcp->rtt_max < rtt || f_rtcp->rtt_sum == 0)
		             f_rtcp->rtt_max = rtt;
		          f_rtcp->rtt_sum += rtt;
		          f_rtcp->rtt_samples++;
		       }
		     break;
		   }
		  matched_record = matched_record->next;
	       }
	    }
	 }
      /* histograms update */
      if (pup->internal_src && !pup->internal_dst)
	{
	  if (dir == C2S)
	    {
	      add_histo (rtcp_jitter_out, jitter);
	      add_histo (rtcp_f_lost_out, f_rtcp->f_lost / 256.0 * 100.0);
	      if (f_rtcp->jitter_samples >= 2)
		{		/* we alredy saw at least 2 RR */
		  if (delta_lost >= 0)
		    {
		      add_histo (rtcp_dup_out, 0);
		      add_histo (rtcp_lost_out, delta_lost);
		    }
		  else
		    {
		      add_histo (rtcp_dup_out, -delta_lost);
		      add_histo (rtcp_lost_out, 0);
		    }
		}
	      if (rtt >= 0)
		{
		  add_histo (rtcp_rtt_out, rtt);
		}
	    }
	  else
	    {
	      add_histo (rtcp_jitter_in, jitter);
	      add_histo (rtcp_f_lost_in, f_rtcp->f_lost / 256.0 * 100.0);
	      if (f_rtcp->jitter_samples >= 2)
		{		/* we alredy saw at least 2 RR */
		  if (delta_lost >= 0)
		    {
		      add_histo (rtcp_dup_in, 0);
		      add_histo (rtcp_lost_in, delta_lost);
		    }
		  else
		    {
		      add_histo (rtcp_dup_in, -delta_lost);
		      add_histo (rtcp_lost_in, 0);
		    }
		}
	      if (rtt >= 0)
		{
		  add_histo (rtcp_rtt_in, rtt);
		}
	    }
	}
      else if (!pup->internal_src && pup->internal_dst)
	{
	  if (dir == C2S)
	    {
	      add_histo (rtcp_jitter_in, jitter);
	      add_histo (rtcp_f_lost_in, f_rtcp->f_lost / 256.0 * 100.0);
	      if (f_rtcp->jitter_samples >= 2)
		{		/* we alredy saw at least 2 RR */
		  if (delta_lost >= 0)
		    {
		      add_histo (rtcp_dup_in, 0);
		      add_histo (rtcp_lost_in, delta_lost);
		    }
		  else
		    {
		      add_histo (rtcp_dup_in, -delta_lost);
		      add_histo (rtcp_lost_in, 0);
		    }
		}
	      if (rtt >= 0)
		{
		  add_histo (rtcp_rtt_in, rtt);
		}
	    }
	  else
	    {
	      add_histo (rtcp_jitter_out, jitter);
	      add_histo (rtcp_f_lost_out, f_rtcp->f_lost / 256.0 * 100.0);
	      if (f_rtcp->jitter_samples >= 2)
		{		/* we alredy saw at least 2 RR */
		  if (delta_lost >= 0)
		    {
		      add_histo (rtcp_dup_out, 0);
		      add_histo (rtcp_lost_out, delta_lost);
		    }
		  else
		    {
		      add_histo (rtcp_dup_out, -delta_lost);
		      add_histo (rtcp_lost_out, 0);
		    }
		}
	      if (rtt >= 0)
		{
		  add_histo (rtcp_rtt_out, rtt);
		}
	    }
	}
#ifndef LOG_UNKNOWN
      else if (pup->internal_src && pup->internal_dst)
#else
      else
#endif
	{
	  add_histo (rtcp_jitter_loc, jitter);
	  add_histo (rtcp_f_lost_loc, f_rtcp->f_lost / 256.0 * 100.0);
	  if (f_rtcp->jitter_samples >= 2)
	    {			/* we alredy saw at least 2 RR */
	      if (delta_lost >= 0)
		{
		  add_histo (rtcp_dup_loc, 0);
		  add_histo (rtcp_lost_loc, delta_lost);
		}
	      else
		{
		  add_histo (rtcp_dup_loc, -delta_lost);
		  add_histo (rtcp_lost_loc, 0);
		}
	    }
	  if (rtt >= 0)
	    {
	      add_histo (rtcp_rtt_loc, rtt);
	    }
	}
      break;
     default: /* Any other RTCP_PT_XXX type */
      break;
    }
  f_rtcp->last_time = current_time;
}



/******** function to update the RTP histograms *********/

/* this will be called by the plugin */
void
make_rtp_conn_stats (void * thisflow, int tproto)
{
  udp_pair *flow = (udp_pair *)thisflow;
  
  if (flow->c2s.type == RTP)
    {
      update_rtp_conn_histo (&(flow->c2s), C2S);
    }
  if (flow->s2c.type == RTP)
    {
      update_rtp_conn_histo (&(flow->s2c), S2C);
    }
  
  if (flow->c2s.type == RTCP)
    {
      update_rtcp_conn_histo (&(flow->c2s), C2S);
    }
  if (flow->s2c.type == RTCP)
    {
      update_rtcp_conn_histo (&(flow->s2c), S2C);
    }
  if (!LOG_IS_ENABLED(LOG_MM_COMPLETE) || fp_rtp_logc == NULL)
    return;

    update_conn_log_v3(flow);
}

void
update_conn_log_v3(udp_pair *flow)
{
  static char common_head[128],
              common_head2[128];
  
  if (flow->c2s.type != RTP_PLUS &&
      flow->s2c.type != RTP_PLUS )
	  return;

  /* Request C-->S */
  if (flow->crypto_src==FALSE)
     sprintf (common_head2, "%d %s %s %u %u %u",
	  flow->c2s.type,
	  HostName (flow->addr_pair.a_address),
	  ServiceName (flow->addr_pair.a_port),
	  flow->internal_src,
	  flow->crypto_src,
	  flow->c2s.multiplexed_protocols);
  else 
     sprintf (common_head2, "%d %s %s %u %u %u",
	  flow->c2s.type,
	  HostNameEncrypted (flow->addr_pair.a_address),
	  ServiceName (flow->addr_pair.a_port),
	  flow->internal_src,
	  flow->crypto_src,
	  flow->c2s.multiplexed_protocols);

  /* Answer C<--S */
  if (flow->crypto_dst==FALSE)
     sprintf (common_head, "%s %d %s %s %u %u %u",
	      common_head2,
	  flow->s2c.type,
	  HostName (flow->addr_pair.b_address),
	  ServiceName (flow->addr_pair.b_port),
	  flow->internal_dst,
	  flow->crypto_dst,
	  flow->s2c.multiplexed_protocols);
  else
     sprintf (common_head, "%s %d %s %s %u %u %u",
	      common_head2,
	  flow->s2c.type,
	  HostNameEncrypted (flow->addr_pair.b_address),
	  ServiceName (flow->addr_pair.b_port),
	  flow->internal_dst,
	  flow->crypto_dst,
	  flow->s2c.multiplexed_protocols);

  if (flow->c2s.flow_ptr.rtp_ptr!=NULL)
  {
     struct rtp *f_rtp;
     f_rtp = (flow->c2s.flow_ptr.rtp_ptr);
     double etime;

     while (f_rtp!=NULL)
     {
     etime = elapsed (f_rtp->first_time, f_rtp->last_time) / 1000.0;
     
   wfprintf (fp_rtp_logc, "R C %s",common_head);
     
     /* Stats */
     wfprintf (fp_rtp_logc, " %lu %g %g %g %g %g %u %u %f %f %llu %g 0x%x %ld %ld %ld %ld %s %d 0 0 0 0 0 0 0 0 0",
	      /* Common stats */
              f_rtp->pnum,
	      (f_rtp->sum_delta_t / f_rtp->n_delta_t),
	      f_rtp->jitter, 
	      f_rtp->jitter_max,
	      f_rtp->jitter_min == MAXFLOAT ? 0 : f_rtp->jitter_min,
	      (double) flow->c2s.ttl_tot / (double) flow->c2s.packets, 
	      flow->c2s.ttl_max,
	      flow->c2s.ttl_min, 
	      (double) f_rtp->first_time.tv_sec + (double) f_rtp->first_time.tv_usec / 1000000.0,
	      etime / 1000.0 /* [s] */,
	      f_rtp->data_bytes,
	      (double) f_rtp->data_bytes / (etime / 1000.0) * 8,
	      f_rtp->ssrc,
	      /* RTP only */
	      f_rtp->n_lost,
	      f_rtp->n_out_of_sequence,
	      f_rtp->n_dup,
	      f_rtp->n_late,
	      rtp_pt_lists(f_rtp),
	      f_rtp->bogus_reset_during_flow);
              /* RTCP only (zeroed out) */
	      /* f_rtcp->c_lost,
	      (float) f_rtcp->f_lost_sum * 100.0 / 256.0, 
	      f_rtcp->tx_p, 
	      f_rtcp->tx_b,
	      (float) f_rtcp->rtt_sum / (float) f_rtcp->rtt_samples,
	      f_rtcp->rtt_max, 
	      f_rtcp->rtt_min, 
	      f_rtcp->rtt_samples,
	      f_rtcp->rtcp_header_error); */

	      /* write stat to file */
       wfprintf (fp_rtp_logc, " %d %d %f %f",
		 f_rtp->max_payload_bytes,
		 f_rtp->min_payload_bytes,
		 f_rtp->data_bytes*1.0/f_rtp->pnum,
		 (f_rtp->pnum>1?sqrt(f_rtp->squared_data_bytes-(double)f_rtp->data_bytes*(double)f_rtp->data_bytes/(double)f_rtp->pnum)*1.0/(f_rtp->pnum-1):0.0));

       wfprintf (fp_rtp_logc, " %g %g %g %g %g %g",
		 f_rtp->av_jitter[0],
		 f_rtp->av_jitter_max[0],
		 f_rtp->av_jitter_min[0] == MAXFLOAT ? 0 : f_rtp->av_jitter_min[0],
		 f_rtp->av_jitter[1],
		 f_rtp->av_jitter_max[1],
		 f_rtp->av_jitter_min[1] == MAXFLOAT ? 0 : f_rtp->av_jitter_min[1]);

       wfprintf (fp_rtp_logc, "\n");
       f_rtp = f_rtp->next;
     }
  }

  if (flow->c2s.flow_ptr.rtcp_ptr!=NULL)
  {
     struct rtcp *f_rtcp;
     f_rtcp = (flow->c2s.flow_ptr.rtcp_ptr);
     double etime;
     uint64_t data_bytes;

          while (f_rtcp!=NULL)
     {

     etime = elapsed (f_rtcp->first_time, f_rtcp->last_time) / 1000.0;
//     data_bytes = flow->c2s.data_bytes - f_rtcp->initial_data_bytes - (f_rtcp->pnum << 3);
     data_bytes = f_rtcp->rtcp_data_bytes;

   wfprintf (fp_rtp_logc, "T C %s",common_head);
     
     wfprintf (fp_rtp_logc, " %lu %g %g %g %g %g %u %u %f %f %llu %g 0x%x 0 0 0 0 %s 0 %d %g %u %u %g %g %g %u %d",
	      f_rtcp->pnum, 
	      (float) f_rtcp->sum_delta_t / (float) f_rtcp->pnum, 
	      (float) f_rtcp->jitter_sum / (float) f_rtcp->jitter_samples,
	      f_rtcp->jitter_max,
	      f_rtcp->jitter_min,
	      (double) flow->c2s.ttl_tot / (double) flow->c2s.packets, 
	      flow->c2s.ttl_max,
	      flow->c2s.ttl_min,
	      (double) f_rtcp->first_time.tv_sec + (double) f_rtcp->first_time.tv_usec / 1000000.0,
	      etime,	/* [s] */
	      data_bytes,
	      (double) data_bytes / etime * 8,
              f_rtcp->ssrc,
	      /* RTP only (zeroed out)*/
	      /* f_rtp->n_lost,
	      f_rtp->n_out_of_sequence,
	      f_rtp->n_dup,
	      f_rtp->n_late, */
	      rtcp_pt_lists(f_rtcp),
	      /* f_rtp->bogus_reset_during_flow); */
	      /* RTCP only */
	      f_rtcp->c_lost,
	      (float) f_rtcp->f_lost_sum / (float) f_rtcp->jitter_samples * 100.0 / 256.0, 
	      f_rtcp->tx_p, 
	      f_rtcp->tx_b,
	      (float) f_rtcp->rtt_sum / (float) f_rtcp->rtt_samples,
	      f_rtcp->rtt_max, 
	      f_rtcp->rtt_min, 
	      f_rtcp->rtt_samples,
	      f_rtcp->rtcp_header_error);
     
       wfprintf (fp_rtp_logc, " 0 0 0.0 0.0");
       wfprintf (fp_rtp_logc, " 0.0 0.0 0.0 0.0 0.0 0.0");

       wfprintf (fp_rtp_logc, "\n");
       f_rtcp = f_rtcp->next;
     }
  }
  else
  {
     /* we miss the request */
     
     //wfprintf (fp_rtp_logc, "%s",common_head);
     //wfprintf (fp_rtp_logc, " C 0 0 0 0 0 0 0 0 0 0 0 0 0x0 0 0 0 0 - - 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
     //wfprintf (fp_rtp_logc, "\n");

  }
	

  if (flow->s2c.flow_ptr.rtp_ptr!=NULL)
  {
     struct rtp *f_rtp;
     f_rtp = (flow->s2c.flow_ptr.rtp_ptr);
     double etime;

     while (f_rtp!=NULL)
     {
     etime = elapsed (f_rtp->first_time, f_rtp->last_time) / 1000.0;
     
   wfprintf (fp_rtp_logc, "R S %s",common_head);
     
     /* Stats */
     wfprintf (fp_rtp_logc, " %lu %g %g %g %g %g %u %u %f %f %llu %g 0x%x %ld %ld %ld %ld %s %d 0 0 0 0 0 0 0 0 0",
	      /* Common stats */
              f_rtp->pnum,
	      (f_rtp->sum_delta_t / f_rtp->n_delta_t),
	      f_rtp->jitter, 
	      f_rtp->jitter_max,
	      f_rtp->jitter_min == MAXFLOAT ? 0 : f_rtp->jitter_min,
	      (double) flow->s2c.ttl_tot / (double) flow->s2c.packets, 
	      flow->s2c.ttl_max,
	      flow->s2c.ttl_min, 
	      (double) f_rtp->first_time.tv_sec + (double) f_rtp->first_time.tv_usec / 1000000.0,
	      etime / 1000.0 /* [s] */,
	      f_rtp->data_bytes,
	      (double) f_rtp->data_bytes / (etime / 1000.0) * 8,
	      f_rtp->ssrc,
	      /* RTP only */
	      f_rtp->n_lost,
	      f_rtp->n_out_of_sequence,
	      f_rtp->n_dup,
	      f_rtp->n_late,
	      rtp_pt_lists(f_rtp),
	      f_rtp->bogus_reset_during_flow);
              /* RTCP only (zeroed out) */
	      /* f_rtcp->c_lost,
	      (float) f_rtcp->f_lost_sum / (float) f_rtcp->jitter_samples * 100.0 / 256.0,    
	      f_rtcp->tx_p, 
	      f_rtcp->tx_b,
	      (float) f_rtcp->rtt_sum / (float) f_rtcp->rtt_samples,
	      f_rtcp->rtt_max, 
	      f_rtcp->rtt_min, 
	      f_rtcp->rtt_samples,
	      f_rtcp->rtcp_header_error); */

	      /* write stat to file */
       wfprintf (fp_rtp_logc, " %d %d %f %f",
		 f_rtp->max_payload_bytes,
		 f_rtp->min_payload_bytes,
		 f_rtp->data_bytes*1.0/f_rtp->pnum,
		 (f_rtp->pnum>1?sqrt( f_rtp->squared_data_bytes-(double)f_rtp->data_bytes*(double)f_rtp->data_bytes/(double)f_rtp->pnum)*1.0/(f_rtp->pnum-1):0.0));

       wfprintf (fp_rtp_logc, " %g %g %g %g %g %g",
		 f_rtp->av_jitter[0],
		 f_rtp->av_jitter_max[0],
		 f_rtp->av_jitter_min[0] == MAXFLOAT ? 0 : f_rtp->av_jitter_min[0],
		 f_rtp->av_jitter[1],
		 f_rtp->av_jitter_max[1],
		 f_rtp->av_jitter_min[1] == MAXFLOAT ? 0 : f_rtp->av_jitter_min[1]);

       wfprintf (fp_rtp_logc, "\n");
       f_rtp = f_rtp->next;
     }
  }

  if (flow->s2c.flow_ptr.rtcp_ptr!=NULL)
  {
     struct rtcp *f_rtcp;
     f_rtcp = (flow->s2c.flow_ptr.rtcp_ptr);
     double etime;
     uint64_t data_bytes;

     while (f_rtcp!=NULL)
     {

     etime = elapsed (f_rtcp->first_time, f_rtcp->last_time) / 1000.0;
//     data_bytes = flow->s2c.data_bytes - f_rtcp->initial_data_bytes - (f_rtcp->pnum << 3);
     data_bytes = f_rtcp->rtcp_data_bytes;
     
   wfprintf (fp_rtp_logc, "T S %s",common_head);
     
     wfprintf (fp_rtp_logc, " %lu %g %g %g %g %g %u %u %f %f %llu %g 0x%x 0 0 0 0 %s 0 %d %g %u %u %g %g %g %u %d",
	      f_rtcp->pnum, 
	      (float) f_rtcp->sum_delta_t / (float) f_rtcp->pnum, 
	      (float) f_rtcp->jitter_sum / (float) f_rtcp->jitter_samples,
	      f_rtcp->jitter_max,
	      f_rtcp->jitter_min,
	      (double) flow->s2c.ttl_tot / (double) flow->s2c.packets, 
	      flow->s2c.ttl_max,
	      flow->s2c.ttl_min,
	      (double) f_rtcp->first_time.tv_sec + (double) f_rtcp->first_time.tv_usec / 1000000.0,
	      etime,	/* [s] */
	      data_bytes,
	      (double) data_bytes / etime * 8,
              f_rtcp->ssrc,
	      /* RTP only (zeroed out)*/
	      /* f_rtp->n_lost,
	      f_rtp->n_out_of_sequence,
	      f_rtp->n_dup,
	      f_rtp->n_late, */
	      rtcp_pt_lists(f_rtcp),
	      /* f_rtp->bogus_reset_during_flow); */
	      /* RTCP only */
	      f_rtcp->c_lost,  
	      (float) f_rtcp->f_lost_sum / (float) f_rtcp->jitter_samples * 100.0 / 256.0, 
	      f_rtcp->tx_p, 
	      f_rtcp->tx_b,
	      (float) f_rtcp->rtt_sum / (float) f_rtcp->rtt_samples,
	      f_rtcp->rtt_max, 
	      f_rtcp->rtt_min, 
	      f_rtcp->rtt_samples,
	      f_rtcp->rtcp_header_error);
     
       wfprintf (fp_rtp_logc, " 0 0 0.0 0.0");
       wfprintf (fp_rtp_logc, " 0.0 0.0 0.0 0.0 0.0 0.0");
       wfprintf (fp_rtp_logc, "\n");
       f_rtcp = f_rtcp->next;
     }
  }
  else
  {
     //wfprintf (fp_rtp_logc, "%s",common_head);
     /* we miss the answer */
     //wfprintf (fp_rtp_logc, " S 0 0 0 0 0 0 0 0 0 0 0 0 0x0 0 0 0 0 - - 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
     //wfprintf (fp_rtp_logc, "\n");
  }
  /* write stat to file */
//  wfprintf (fp_rtp_logc, "\n");
}

void
update_rtp_conn_histo (ucb * thisdir, int dir)
{
  struct sudp_pair *pup;
  struct rtp *f_rtp;
  double etime;
  int max_index, index, min_val;
  int uni_multi = UNICAST;
#ifdef LOG_OOO
  int seg_type = IN_SEQ;
  extern FILE *fp_dup_ooo_log;
#endif
  extern unsigned int ip_obfuscate_mask;

  f_rtp = thisdir->flow_ptr.rtp_ptr;
  pup = thisdir->pup;

  /* Calculus of the RTP flow length in milliseconds */

  etime = elapsed (f_rtp->first_time, f_rtp->last_time) / 1000.0;

  /* multicast adx ? */
  if (pup->addr_pair.b_address.addr_vers == 4)
    {
      uint32_t ip_addr;
      if (ip_obfuscate_mask==0x00000000)
       {
	  if (dir == C2S)
	    ip_addr = ntohl (pup->addr_pair.b_address.un.ip4.s_addr);
	  else
	    ip_addr = ntohl (pup->addr_pair.a_address.un.ip4.s_addr);
       }
      else
       {
	  if (dir == C2S)
	  {
	    if (pup->internal_dst)
	      ip_addr = ntohl (pup->addr_pair.b_address.un.ip4.s_addr ^ ip_obfuscate_mask);
	    else
	      ip_addr = ntohl (pup->addr_pair.b_address.un.ip4.s_addr);
	  }
	  else
	  {
	    if (pup->internal_src)
	      ip_addr = ntohl (pup->addr_pair.a_address.un.ip4.s_addr ^ ip_obfuscate_mask);
	    else
	      ip_addr = ntohl (pup->addr_pair.a_address.un.ip4.s_addr);
	  }
       }
      if (ip_addr >= LB_MULTICAST && ip_addr < UB_MULTICAST)
	uni_multi = MULTICAST;
      else
	uni_multi = UNICAST;
    }
#ifdef SUPPORT_IPV6
  else
    {
      unsigned char multicast_byte;
      if (dir == C2S)
	{
	  if (pup->addr_pair.b_address.un.ip6.s6_addr[0] == 0xFF)
	    uni_multi = MULTICAST;
	  else
	    uni_multi = UNICAST;
	}
      else
	{
	  if (pup->addr_pair.a_address.un.ip6.s6_addr[0] == 0xFF)
	    uni_multi = MULTICAST;
	  else
	    uni_multi = UNICAST;
	}
    }
#endif
/* end multicast */

/* analysis of the last window before the closing of the flow */
/* maximum index reached in the window */
  max_index =
    ((u_int16_t) (f_rtp->largest_seqno - f_rtp->initial_seqno) % RTP_WIN);
/* value that must be contained in the first elemet of the window */
  min_val = (u_int16_t) (f_rtp->largest_seqno - max_index);
  for (index = 0; index <= max_index; index++)
    {
      if (f_rtp->packets_win[index] != (u_int16_t) (min_val + index))
	{
#ifdef LOG_OOO
	  seg_type = LOST;
#endif
	  f_rtp->n_lost++;
	  f_rtp->burst++;
	}
      else
	{
	  if (pup->internal_src && !pup->internal_dst)
	    {
	      if (dir == C2S)
		{
		  add_histo (mm_burst_loss_out, f_rtp->burst);
		}
	      else
		{
		  add_histo (mm_burst_loss_in, f_rtp->burst);
		}
	    }
	  else if (!pup->internal_src && pup->internal_dst)
	    {
	      if (dir == C2S)
		{
		  add_histo (mm_burst_loss_in, f_rtp->burst);
		}
	      else
		{
		  add_histo (mm_burst_loss_out, f_rtp->burst);
		}
	    }
#ifndef LOG_UNKNOWN
          else if (pup->internal_src && pup->internal_dst)
#else
          else
#endif
	    {
	      add_histo (mm_burst_loss_loc, f_rtp->burst);
	    }
	  f_rtp->burst = 0;
	}
    }

#ifdef LOG_OOO
  if (seg_type != IN_SEQ)
    {
      wfprintf (fp_dup_ooo_log, "R: %f ",
	       (elapsed (first_packet, current_time) / 1000.0));

      if (dir == C2S)
	{
	  if (pup->crypto_src==FALSE)
	     wfprintf (fp_dup_ooo_log, "%d %d %s %s",
		   PROTOCOL_UDP,
		   RTP_PROTOCOL,
		   HostName (pup->addr_pair.a_address),
		   ServiceName (pup->addr_pair.a_port));
	  else
	     wfprintf (fp_dup_ooo_log, "%d %d %s %s",
		   PROTOCOL_UDP,
		   RTP_PROTOCOL,
		   HostNameEncrypted (pup->addr_pair.a_address),
		   ServiceName (pup->addr_pair.a_port));
	  if (pup->crypto_dst==FALSE)
	     wfprintf (fp_dup_ooo_log,
		   " %s %s",
		   HostName (pup->addr_pair.b_address),
		   ServiceName (pup->addr_pair.b_port));
	  else
	     wfprintf (fp_dup_ooo_log,
		   " %s %s",
		   HostNameEncrypted (pup->addr_pair.b_address),
		   ServiceName (pup->addr_pair.b_port));
	}
      else
	{
	  if (pup->crypto_dst==FALSE)
	     wfprintf (fp_dup_ooo_log, "%d %d %s %s",
		   PROTOCOL_UDP,
		   RTP_PROTOCOL,
		   HostName (pup->addr_pair.b_address),
		   ServiceName (pup->addr_pair.b_port));
	  else
	     wfprintf (fp_dup_ooo_log, "%d %d %s %s",
		   PROTOCOL_UDP,
		   RTP_PROTOCOL,
		   HostNameEncrypted (pup->addr_pair.b_address),
		   ServiceName (pup->addr_pair.b_port));
	  if (pup->crypto_src==FALSE)
	     wfprintf (fp_dup_ooo_log,
		   " %s %s",
		   HostName (pup->addr_pair.a_address),
		   ServiceName (pup->addr_pair.a_port));
	  else
	     wfprintf (fp_dup_ooo_log,
		   " %s %s",
		   HostNameEncrypted (pup->addr_pair.a_address),
		   ServiceName (pup->addr_pair.a_port));
	}
      wfprintf (fp_dup_ooo_log, " %d %d %d\n",
	       seg_type, pup->internal_src, pup->internal_dst);

    }
#endif

  if (pup->internal_src && !pup->internal_dst)
    {

      if (dir == C2S)
	{
	  /* topix */
	  add_histo (mm_cl_b_out, f_rtp->data_bytes);
	  add_histo (mm_cl_b_s_out, f_rtp->data_bytes);

	  if (f_rtp->pnum >= BITRATE_MIN_PKTS)
	    {
	      add_histo (mm_avg_bitrate_out,
			 (f_rtp->data_bytes >> 7) / (etime / 1000.0));
	    }
	  add_histo (mm_type_out, RTP_PROTOCOL);
	  add_histo (mm_rtp_pt_out, f_rtp->pt);
	  add_histo (mm_uni_multi_out, uni_multi);
	  /* end topix */

	  add_histo (mm_cl_p_out, f_rtp->pnum);
	  add_histo (mm_cl_p_s_out, f_rtp->pnum);

	  add_histo (mm_avg_ipg_out,
		     10. * (f_rtp->sum_delta_t / f_rtp->n_delta_t));
	  add_histo (mm_avg_jitter_out, 10. * (f_rtp->jitter));
	  add_histo (mm_n_oos_out, f_rtp->n_out_of_sequence);
	  add_histo (mm_p_oos_out,
		     ((float) f_rtp->n_out_of_sequence /
		      (float) f_rtp->pnum) * 1000);
	  add_histo (mm_tot_time_out, etime / 1000);
	  //if (etime <= SHORT_MM_TOT_TIME)
	  add_histo (mm_tot_time_s_out, etime);
	  add_histo (mm_p_dup_out, (f_rtp->n_dup * 1000) / f_rtp->pnum);
	  add_histo (mm_p_lost_out, (f_rtp->n_lost * 1000) / f_rtp->pnum);
	  add_histo (mm_p_late_out, (f_rtp->n_late * 1000) / f_rtp->pnum);
	}
      else
	{
	  /* topix */
	  add_histo (mm_cl_b_in, f_rtp->data_bytes);
	  add_histo (mm_cl_b_s_in, f_rtp->data_bytes);

	  if (f_rtp->pnum >= BITRATE_MIN_PKTS)
	    {
	      add_histo (mm_avg_bitrate_in,
			 (f_rtp->data_bytes >> 7) / (etime / 1000.0));
	    }
	  add_histo (mm_type_in, RTP_PROTOCOL);
	  add_histo (mm_rtp_pt_in, f_rtp->pt);
	  add_histo (mm_uni_multi_in, uni_multi);
	  /* end topix */

	  add_histo (mm_cl_p_in, f_rtp->pnum);
	  add_histo (mm_cl_p_s_in, f_rtp->pnum);

	  add_histo (mm_avg_ipg_in,
		     10. * (f_rtp->sum_delta_t / f_rtp->n_delta_t));
	  add_histo (mm_avg_jitter_in, 10. * (f_rtp->jitter));
	  add_histo (mm_n_oos_in, f_rtp->n_out_of_sequence);
	  add_histo (mm_p_oos_in,
		     ((float) f_rtp->n_out_of_sequence /
		      (float) f_rtp->pnum) * 1000);
	  add_histo (mm_tot_time_in, etime / 1000);
	  //if (etime <= SHORT_MM_TOT_TIME)
	  add_histo (mm_tot_time_s_in, etime);
	  add_histo (mm_p_dup_in, (f_rtp->n_dup * 1000) / f_rtp->pnum);
	  add_histo (mm_p_lost_in, (f_rtp->n_lost * 1000) / f_rtp->pnum);
	  add_histo (mm_p_late_in, (f_rtp->n_late * 1000) / f_rtp->pnum);
	}
    }
  else if (!pup->internal_src && pup->internal_dst)
    {
      if (dir == C2S)
	{
	  /* topix */
	  add_histo (mm_cl_b_in, f_rtp->data_bytes);
	  add_histo (mm_cl_b_s_in, f_rtp->data_bytes);

	  if (f_rtp->pnum >= BITRATE_MIN_PKTS)
	    add_histo (mm_avg_bitrate_in,
		       (f_rtp->data_bytes >> 7) / (etime / 1000.0));
	  add_histo (mm_type_in, RTP_PROTOCOL);
	  add_histo (mm_rtp_pt_in, f_rtp->pt);
	  add_histo (mm_uni_multi_in, uni_multi);
	  /* end topix */
	  add_histo (mm_cl_p_in, f_rtp->pnum);
	  add_histo (mm_cl_p_s_in, f_rtp->pnum);

	  add_histo (mm_avg_ipg_in,
		     10. * (f_rtp->sum_delta_t / f_rtp->n_delta_t));
	  add_histo (mm_avg_jitter_in, 10. * (f_rtp->jitter));
	  add_histo (mm_n_oos_in, f_rtp->n_out_of_sequence);
	  add_histo (mm_p_oos_in,
		     ((float) f_rtp->n_out_of_sequence /
		      (float) f_rtp->pnum) * 1000);
	  add_histo (mm_tot_time_in, etime / 1000);
	  //if (etime <= SHORT_MM_TOT_TIME)
	  add_histo (mm_tot_time_s_in, etime);
	  add_histo (mm_p_dup_in, (f_rtp->n_dup * 1000) / f_rtp->pnum);
	  add_histo (mm_p_lost_in, (f_rtp->n_lost * 1000) / f_rtp->pnum);
	  add_histo (mm_p_late_in, (f_rtp->n_late * 1000) / f_rtp->pnum);
	}
      else
	{
	  /* topix */
	  add_histo (mm_cl_b_out, f_rtp->data_bytes);
	  add_histo (mm_cl_b_s_out, f_rtp->data_bytes);

	  if (f_rtp->pnum >= BITRATE_MIN_PKTS)
	    {
	      add_histo (mm_avg_bitrate_out,
			 (f_rtp->data_bytes >> 7) / (etime / 1000.0));
	    }
	  add_histo (mm_type_out, RTP_PROTOCOL);
	  add_histo (mm_rtp_pt_out, f_rtp->pt);
	  add_histo (mm_uni_multi_out, uni_multi);
	  /* end topix */
	  add_histo (mm_cl_p_out, f_rtp->pnum);
	  add_histo (mm_cl_p_s_out, f_rtp->pnum);

	  add_histo (mm_avg_ipg_out,
		     10. * (f_rtp->sum_delta_t / f_rtp->n_delta_t));
	  add_histo (mm_avg_jitter_out, 10. * (f_rtp->jitter));
	  add_histo (mm_n_oos_out, f_rtp->n_out_of_sequence);
	  add_histo (mm_p_oos_out,
		     ((float) f_rtp->n_out_of_sequence /
		      (float) f_rtp->pnum) * 1000);
	  add_histo (mm_tot_time_out, etime / 1000);
	  //if (etime <= SHORT_MM_TOT_TIME)
	  add_histo (mm_tot_time_s_out, etime);
	  add_histo (mm_p_dup_out, (f_rtp->n_dup * 1000) / f_rtp->pnum);
	  add_histo (mm_p_lost_out, (f_rtp->n_lost * 1000) / f_rtp->pnum);
	  add_histo (mm_p_late_out, (f_rtp->n_late * 1000) / f_rtp->pnum);

	}
    }
#ifndef LOG_UNKNOWN
  else if (pup->internal_src && pup->internal_dst)
#else
  else
#endif
    {
      /* topix */
      add_histo (mm_cl_b_loc, f_rtp->data_bytes);
      add_histo (mm_cl_b_s_loc, f_rtp->data_bytes);

      if (f_rtp->pnum >= BITRATE_MIN_PKTS)
	add_histo (mm_avg_bitrate_loc,
		   (f_rtp->data_bytes >> 7) / (etime / 1000.0));
      add_histo (mm_type_loc, RTP_PROTOCOL);
      add_histo (mm_rtp_pt_loc, f_rtp->pt);
      add_histo (mm_uni_multi_loc, uni_multi);
      /* end topix */
      add_histo (mm_cl_p_loc, f_rtp->pnum);
      add_histo (mm_cl_p_s_loc, f_rtp->pnum);

      add_histo (mm_avg_ipg_loc,
		 10. * (f_rtp->sum_delta_t / f_rtp->n_delta_t));
      add_histo (mm_avg_jitter_loc, 10. * (f_rtp->jitter));
      add_histo (mm_n_oos_loc, f_rtp->n_out_of_sequence);
      add_histo (mm_p_oos_loc,
		 ((float) f_rtp->n_out_of_sequence /
		  (float) f_rtp->pnum) * 1000);
      add_histo (mm_tot_time_loc, etime / 1000);
      //if (etime <= SHORT_MM_TOT_TIME)
      add_histo (mm_tot_time_s_loc, etime);
      add_histo (mm_p_dup_loc, (f_rtp->n_dup * 1000) / f_rtp->pnum);
      add_histo (mm_p_lost_loc, (f_rtp->n_lost * 1000) / f_rtp->pnum);
      add_histo (mm_p_late_loc, (f_rtp->n_late * 1000) / f_rtp->pnum);
    }
}

/******** function used to update the RTCP histograms *********/

void
update_rtcp_conn_histo (ucb * thisdir, int dir)
{
  struct sudp_pair *pup;
  struct rtcp *f_rtcp;
  double etime;
  uint64_t data_bytes;

  f_rtcp = thisdir->flow_ptr.rtcp_ptr;
  pup = thisdir->pup;

  etime = elapsed (f_rtcp->first_time, f_rtcp->last_time) / 1000000.0;

  data_bytes =
    thisdir->data_bytes - f_rtcp->initial_data_bytes - (f_rtcp->pnum << 3);

  if (pup->internal_src && !pup->internal_dst)
    {

      if (dir == C2S)
	{
	  add_histo (rtcp_t_lost_out, f_rtcp->c_lost);
	  add_histo (rtcp_bt_out, (double) data_bytes / etime * 8.0);
	  add_histo (rtcp_mm_cl_p_out, f_rtcp->tx_p);
	  add_histo (rtcp_mm_cl_b_out, f_rtcp->tx_b);
	  add_histo (rtcp_cl_p_out, f_rtcp->pnum);
	  add_histo (rtcp_cl_b_out, data_bytes);
	  add_histo (rtcp_avg_inter_out,
		     (float) f_rtcp->sum_delta_t / (float) f_rtcp->pnum);
	}
      else
	{
	  add_histo (rtcp_t_lost_in, f_rtcp->c_lost);
	  add_histo (rtcp_bt_in, (double) data_bytes / etime * 8.0);
	  add_histo (rtcp_mm_cl_p_in, f_rtcp->tx_p);
	  add_histo (rtcp_mm_cl_b_in, f_rtcp->tx_b);
	  add_histo (rtcp_cl_p_in, f_rtcp->pnum);
	  add_histo (rtcp_cl_b_in, data_bytes);
	  add_histo (rtcp_avg_inter_in,
		     (float) f_rtcp->sum_delta_t / (float) f_rtcp->pnum);
	}
    }
  else if (!pup->internal_src && pup->internal_dst)
    {
      if (dir == C2S)
	{
	  add_histo (rtcp_t_lost_in, f_rtcp->c_lost);
	  add_histo (rtcp_bt_in, (double) data_bytes / etime * 8.0);
	  add_histo (rtcp_mm_cl_p_in, f_rtcp->tx_p);
	  add_histo (rtcp_mm_cl_b_in, f_rtcp->tx_b);
	  add_histo (rtcp_cl_p_in, f_rtcp->pnum);
	  add_histo (rtcp_cl_b_in, data_bytes);
	  add_histo (rtcp_avg_inter_in,
		     (float) f_rtcp->sum_delta_t / (float) f_rtcp->pnum);

	}
      else
	{
	  add_histo (rtcp_t_lost_out, f_rtcp->c_lost);
	  add_histo (rtcp_bt_out, (double) data_bytes / etime * 8.0);
	  add_histo (rtcp_mm_cl_p_out, f_rtcp->tx_p);
	  add_histo (rtcp_mm_cl_b_out, f_rtcp->tx_b);
	  add_histo (rtcp_cl_p_out, f_rtcp->pnum);
	  add_histo (rtcp_cl_b_out, data_bytes);
	  add_histo (rtcp_avg_inter_out,
		     (float) f_rtcp->sum_delta_t / (float) f_rtcp->pnum);

	}
    }
#ifndef LOG_UNKNOWN
  else if (pup->internal_src && pup->internal_dst)
#else
  else
#endif
    {
      add_histo (rtcp_t_lost_loc, f_rtcp->c_lost);
      add_histo (rtcp_bt_loc, (double) data_bytes / etime * 8.0);
      add_histo (rtcp_mm_cl_p_loc, f_rtcp->tx_p);
      add_histo (rtcp_mm_cl_b_loc, f_rtcp->tx_b);
      add_histo (rtcp_cl_p_loc, f_rtcp->pnum);
      add_histo (rtcp_cl_b_loc, data_bytes);
      add_histo (rtcp_avg_inter_loc,
		 (float) f_rtcp->sum_delta_t / (float) f_rtcp->pnum);
    }
}

/* function used to interprete the 16 bit values whether in the BIG ENDIAN case
or in the  LITTLE ENDIAN one */

u_int16_t
swap16 (u_int16_t val)
{
#ifdef RTP_LITTLE_ENDIAN
  return ((val & 0xff00) >> 8) | ((val & 0x00ff) << 8);
#else
  return val;
#endif
}

/* function used to interprete the 24 bit values whether in the BIG ENDIAN case
or in the  LITTLE ENDIAN one */

int32_t
swap24 (u_int32_t val)
{
  u_int32_t ret;
#ifdef RTP_LITTLE_ENDIAN
  ret =
    ((val & 0x00ff0000) >> 16) |
    (val & 0x0000ff00) | ((val & 0x000000ff) << 16);
  if (ret > 0x007FFFFF)
    {
      return (ret - 0x01000000);
    }
  else
    return ret;
#else
  return val;
#endif /* ; */
}

/* function used to interprete the 32 bit values whether in the BIG ENDIAN case
or in the  LITTLE ENDIAN one */

u_int32_t
swap32 (u_int32_t val)
{
  return (ntohl (val));
#ifdef RTP_LITTLE_ENDIAN
  return
    ((val & 0xff000000) >> 24) |
    ((val & 0x00ff0000) >> 8) |
    ((val & 0x0000ff00) << 8) | ((val & 0x000000ff) << 24);
#else
  return val;
#endif
}


/** function used to determine the sampling frequence indicated by the 
'Payload Type' field **/

int
det_freq (struct rtphdr *prtp)
{
  switch (prtp->pt)
    {
    case RTP_PT_PCMU:
    case RTP_PT_CELP:
    case RTP_PT_G711:
    case RTP_PT_G723:
    case RTP_PT_DVI4_8000:
    case RTP_PT_GSM:
    case RTP_PT_LPC:
    case RTP_PT_PCMA:
    case RTP_PT_G722:
    case RTP_PT_QCELP:
    case RTP_PT_CN:
    case RTP_PT_G728:
    case RTP_PT_G729:
      {
	return (8000);
	break;
      }
    case RTP_PT_DVI4_11025:
      {
	return (11025);
	break;
      }
    case RTP_PT_DVI4_16000:
      {
	return (16000);
	break;
      }
    case RTP_PT_DVI4_22050:
      {
	return (22050);
	break;
      }
    case RTP_PT_L16_2:
    case RTP_PT_L16_1:
      {
	return (44100);
	break;
      }
    case RTP_PT_MPA:
    case RTP_PT_CELLB:
    case RTP_PT_JPEG:
    case RTP_PT_NV:
    case RTP_PT_H261:
    case RTP_PT_MPEG:
    case RTP_PT_MP2T:
      {
	return (90000);
	break;
      }
    default:
      {
	return (0);
	break;
      }
    }
}

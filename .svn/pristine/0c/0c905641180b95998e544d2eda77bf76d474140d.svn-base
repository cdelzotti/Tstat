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

#include <stdio.h>
#include "tstat.h"

/* 
 * All the global histogram structs are defined here,
 * while they are declared as extern in histo.h
 * This is to solve the issues raised by the new GCC 10 default behavior
 * for tentative definitions, that switched from -fcommon to -fno-common
 * 
 * This could have been a the top of histo.c, but it's probably more clean
 * to keep the long list here
 */

/*-------------------------------------------------------------*/
/* here are all the histo tracked by tstat                     */
/* struct double_histo_list *test;                             */
/* NOTE                                                         */
/*      variable name MUST be SHORTER (<) than 20               */
/*      char, otherwise RRD will complain without               */
/*      further explaining the error                            */
/*-------------------------------------------------------------*/


struct double_histo_list *ip_bitrate_in = NULL;
struct double_histo_list *ip_bitrate_out = NULL;
struct double_histo_list *ip_bitrate_loc = NULL;
struct double_histo_list *ip_bitrate_c_in = NULL;
struct double_histo_list *ip_bitrate_c_out = NULL;
struct double_histo_list *ip_bitrate_nc_in = NULL;
struct double_histo_list *ip_bitrate_nc_out = NULL;


struct double_histo_list *ip_protocol_in = NULL;
struct double_histo_list *ip_protocol_out = NULL;
struct double_histo_list *ip_protocol_loc = NULL;

struct double_histo_list *ip_len_out = NULL;
struct double_histo_list *ip_len_in = NULL;
struct double_histo_list *ip_len_loc = NULL;

struct double_histo_list *ip_ttl_out = NULL;
struct double_histo_list *ip_ttl_in = NULL;
struct double_histo_list *ip_ttl_loc = NULL;

struct double_histo_list *ip_tos_out = NULL;
struct double_histo_list *ip_tos_in = NULL;
struct double_histo_list *ip_tos_loc = NULL;

//IPv6 module
#ifdef SUPPORT_IPV6

struct double_histo_list *ip6_protocol_in = NULL;
struct double_histo_list *ip6_hop_limit_in = NULL;
struct double_histo_list *ip6_plen_in = NULL;

struct double_histo_list *ip6_protocol_out = NULL;
struct double_histo_list *ip6_hop_limit_out = NULL;
struct double_histo_list *ip6_plen_out = NULL;

struct double_histo_list *ip6_protocol_loc = NULL;
struct double_histo_list *ip6_hop_limit_loc = NULL;
struct double_histo_list *ip6_plen_loc = NULL;

//ICMPv6 module
struct double_histo_list *icmpv6_type_in = NULL;
struct double_histo_list *icmpv6_type_loc = NULL;
struct double_histo_list *icmpv6_type_out = NULL;
#endif
//IPv6 module


struct double_histo_list *tcp_port_src_in = NULL;
struct double_histo_list *tcp_port_src_out = NULL;
struct double_histo_list *tcp_port_src_loc = NULL;
struct double_histo_list *tcp_port_dst_in = NULL;
struct double_histo_list *tcp_port_dst_out = NULL;
struct double_histo_list *tcp_port_dst_loc = NULL;

struct double_histo_list *tcp_port_synsrc_in = NULL;
struct double_histo_list *tcp_port_synsrc_out = NULL;
struct double_histo_list *tcp_port_synsrc_loc = NULL;
struct double_histo_list *tcp_port_syndst_in = NULL;
struct double_histo_list *tcp_port_syndst_out = NULL;
struct double_histo_list *tcp_port_syndst_loc = NULL;

struct double_histo_list *tcp_bitrate_in = NULL;
struct double_histo_list *tcp_bitrate_out = NULL;
struct double_histo_list *tcp_bitrate_loc = NULL;
struct double_histo_list *tcp_bitrate_c_in = NULL;
struct double_histo_list *tcp_bitrate_c_out = NULL;
struct double_histo_list *tcp_bitrate_nc_in = NULL;
struct double_histo_list *tcp_bitrate_nc_out = NULL;

struct double_histo_list *tcp_opts_SACK = NULL;
struct double_histo_list *tcp_opts_WS = NULL;
struct double_histo_list *tcp_opts_TS = NULL;
struct double_histo_list *tcp_opts_MPTCP = NULL;

struct double_histo_list *tcp_mss_a = NULL;
struct double_histo_list *tcp_mss_b = NULL;
struct double_histo_list *tcp_mss_used = NULL;

struct double_histo_list *tcp_win_min = NULL;
struct double_histo_list *tcp_win_avg = NULL;
struct double_histo_list *tcp_win_max = NULL;

struct double_histo_list *tcp_cwnd = NULL;

struct double_histo_list *tcp_cl_b_l_out = NULL;
struct double_histo_list *tcp_cl_b_l_in = NULL;
struct double_histo_list *tcp_cl_b_l_loc = NULL;
struct double_histo_list *tcp_cl_b_s_out = NULL;
struct double_histo_list *tcp_cl_b_s_in = NULL;
struct double_histo_list *tcp_cl_b_s_loc = NULL;
struct double_histo_list *tcp_cl_p_out = NULL;
struct double_histo_list *tcp_cl_p_in = NULL;
struct double_histo_list *tcp_cl_p_loc = NULL;

struct double_histo_list *tcp_cl_b_l_c2s = NULL;
struct double_histo_list *tcp_cl_b_l_s2c = NULL;
struct double_histo_list *tcp_cl_b_s_c2s = NULL;
struct double_histo_list *tcp_cl_b_s_s2c = NULL;
struct double_histo_list *tcp_cl_p_c2s = NULL;
struct double_histo_list *tcp_cl_p_s2c = NULL;

struct double_histo_list *tcp_rtt_min_out = NULL;
struct double_histo_list *tcp_rtt_min_in = NULL;
struct double_histo_list *tcp_rtt_min_loc = NULL;
struct double_histo_list *tcp_rtt_min_c2s = NULL;
struct double_histo_list *tcp_rtt_min_s2c = NULL;

/* v.1.2.0 -----------------------------------*/

struct double_histo_list *tcp_rtt_avg_out = NULL;
struct double_histo_list *tcp_rtt_avg_in = NULL;
struct double_histo_list *tcp_rtt_avg_loc = NULL;
struct double_histo_list *tcp_rtt_avg_c2s = NULL;
struct double_histo_list *tcp_rtt_avg_s2c = NULL;

struct double_histo_list *tcp_rtt_c_avg_out = NULL;
struct double_histo_list *tcp_rtt_c_avg_in = NULL;
struct double_histo_list *tcp_rtt_nc_avg_out = NULL;
struct double_histo_list *tcp_rtt_nc_avg_in = NULL;

/* v.1.2.0 -----------------------------------*/

struct double_histo_list *tcp_rtt_max_out = NULL;
struct double_histo_list *tcp_rtt_max_in = NULL;
struct double_histo_list *tcp_rtt_max_loc = NULL;
struct double_histo_list *tcp_rtt_max_c2s = NULL;
struct double_histo_list *tcp_rtt_max_s2c = NULL;

struct double_histo_list *tcp_rtt_stdev_out = NULL;
struct double_histo_list *tcp_rtt_stdev_in = NULL;
struct double_histo_list *tcp_rtt_stdev_loc = NULL;
struct double_histo_list *tcp_rtt_stdev_c2s = NULL;
struct double_histo_list *tcp_rtt_stdev_s2c = NULL;

struct double_histo_list *tcp_rtt_cnt_out = NULL;
struct double_histo_list *tcp_rtt_cnt_in = NULL;
struct double_histo_list *tcp_rtt_cnt_loc = NULL;
struct double_histo_list *tcp_rtt_cnt_c2s = NULL;
struct double_histo_list *tcp_rtt_cnt_s2c = NULL;

struct double_histo_list *tcp_anomalies_c2s = NULL;
struct double_histo_list *tcp_anomalies_s2c = NULL;
struct double_histo_list *tcp_anomalies_out = NULL;
struct double_histo_list *tcp_anomalies_in = NULL;
struct double_histo_list *tcp_anomalies_loc = NULL;

struct double_histo_list *tcp_rtx_RTO_c2s = NULL;
struct double_histo_list *tcp_rtx_RTO_s2c = NULL;
struct double_histo_list *tcp_rtx_RTO_out = NULL;
struct double_histo_list *tcp_rtx_RTO_in = NULL;
struct double_histo_list *tcp_rtx_RTO_loc = NULL;

struct double_histo_list *tcp_rtx_FR_c2s = NULL;
struct double_histo_list *tcp_rtx_FR_s2c = NULL;
struct double_histo_list *tcp_rtx_FR_out = NULL;
struct double_histo_list *tcp_rtx_FR_in = NULL;
struct double_histo_list *tcp_rtx_FR_loc = NULL;

struct double_histo_list *tcp_reordering_c2s = NULL;
struct double_histo_list *tcp_reordering_s2c = NULL;
struct double_histo_list *tcp_reordering_out = NULL;
struct double_histo_list *tcp_reordering_in = NULL;
struct double_histo_list *tcp_reordering_loc = NULL;

struct double_histo_list *tcp_net_dup_c2s = NULL;
struct double_histo_list *tcp_net_dup_s2c = NULL;
struct double_histo_list *tcp_net_dup_out = NULL;
struct double_histo_list *tcp_net_dup_in = NULL;
struct double_histo_list *tcp_net_dup_loc = NULL;

struct double_histo_list *tcp_unknown_c2s = NULL;
struct double_histo_list *tcp_unknown_s2c = NULL;
struct double_histo_list *tcp_unknown_out = NULL;
struct double_histo_list *tcp_unknown_in = NULL;
struct double_histo_list *tcp_unknown_loc = NULL;

struct double_histo_list *tcp_flow_ctrl_c2s = NULL;
struct double_histo_list *tcp_flow_ctrl_s2c = NULL;
struct double_histo_list *tcp_flow_ctrl_out = NULL;
struct double_histo_list *tcp_flow_ctrl_in = NULL;
struct double_histo_list *tcp_flow_ctrl_loc = NULL;

struct double_histo_list *tcp_unnrtx_RTO_c2s = NULL;
struct double_histo_list *tcp_unnrtx_RTO_s2c = NULL;
struct double_histo_list *tcp_unnrtx_RTO_out = NULL;
struct double_histo_list *tcp_unnrtx_RTO_in = NULL;
struct double_histo_list *tcp_unnrtx_RTO_loc = NULL;

struct double_histo_list *tcp_unnrtx_FR_c2s = NULL;
struct double_histo_list *tcp_unnrtx_FR_s2c = NULL;
struct double_histo_list *tcp_unnrtx_FR_out = NULL;
struct double_histo_list *tcp_unnrtx_FR_in = NULL;
struct double_histo_list *tcp_unnrtx_FR_loc = NULL;


struct double_histo_list *tcp_tot_time = NULL;
struct double_histo_list *tcp_thru_c2s = NULL;
struct double_histo_list *tcp_thru_s2c = NULL;

struct double_histo_list *tcp_thru_lf_c2s = NULL;
struct double_histo_list *tcp_thru_lf_s2c = NULL;
struct double_histo_list *tcp_thru_lf_c_c2s = NULL;
struct double_histo_list *tcp_thru_lf_c_s2c = NULL;
struct double_histo_list *tcp_thru_lf_nc_c2s = NULL;
struct double_histo_list *tcp_thru_lf_nc_s2c = NULL;

struct double_histo_list *tcp_interrupted = NULL;

struct double_histo_list *L4_flow_number = NULL;


    /* udp histograms */

struct double_histo_list *udp_cl_p_out = NULL;
struct double_histo_list *udp_cl_p_in = NULL;
struct double_histo_list *udp_cl_p_loc = NULL;

struct double_histo_list *udp_cl_b_l_out = NULL;
struct double_histo_list *udp_cl_b_l_in = NULL;
struct double_histo_list *udp_cl_b_l_loc = NULL;
struct double_histo_list *udp_cl_b_s_out = NULL;
struct double_histo_list *udp_cl_b_s_in = NULL;
struct double_histo_list *udp_cl_b_s_loc = NULL;

struct double_histo_list *udp_tot_time = NULL;
struct double_histo_list *udp_port_dst_in = NULL;
struct double_histo_list *udp_port_dst_out = NULL;
struct double_histo_list *udp_port_dst_loc = NULL;
struct double_histo_list *udp_port_flow_dst = NULL;

struct double_histo_list *udp_bitrate_in = NULL;
struct double_histo_list *udp_bitrate_out = NULL;
struct double_histo_list *udp_bitrate_loc = NULL;
struct double_histo_list *udp_bitrate_c_in = NULL;
struct double_histo_list *udp_bitrate_c_out = NULL;
struct double_histo_list *udp_bitrate_nc_in = NULL;
struct double_histo_list *udp_bitrate_nc_out = NULL;

      /* stream histograms */

struct double_histo_list *mm_type_in = NULL;
struct double_histo_list *mm_type_out = NULL;
struct double_histo_list *mm_type_loc = NULL;

struct double_histo_list *mm_rtp_pt_in = NULL;
struct double_histo_list *mm_rtp_pt_out = NULL;
struct double_histo_list *mm_rtp_pt_loc = NULL;

struct double_histo_list *mm_uni_multi_in = NULL;
struct double_histo_list *mm_uni_multi_out = NULL;
struct double_histo_list *mm_uni_multi_loc = NULL;

struct double_histo_list *mm_tot_time_in = NULL;
struct double_histo_list *mm_tot_time_out = NULL;
struct double_histo_list *mm_tot_time_loc = NULL;

struct double_histo_list *mm_tot_time_s_in = NULL;
struct double_histo_list *mm_tot_time_s_out = NULL;
struct double_histo_list *mm_tot_time_s_loc = NULL;


struct double_histo_list *mm_cl_p_out = NULL;
struct double_histo_list *mm_cl_p_in = NULL;
struct double_histo_list *mm_cl_p_loc = NULL;

struct double_histo_list *mm_cl_b_out = NULL;
struct double_histo_list *mm_cl_b_in = NULL;
struct double_histo_list *mm_cl_b_loc = NULL;

struct double_histo_list *mm_cl_p_s_out = NULL;
struct double_histo_list *mm_cl_p_s_in = NULL;
struct double_histo_list *mm_cl_p_s_loc = NULL;

struct double_histo_list *mm_cl_b_s_out = NULL;
struct double_histo_list *mm_cl_b_s_in = NULL;
struct double_histo_list *mm_cl_b_s_loc = NULL;

struct double_histo_list *mm_avg_bitrate_in = NULL;
struct double_histo_list *mm_avg_bitrate_out = NULL;
struct double_histo_list *mm_avg_bitrate_loc = NULL;

struct double_histo_list *mm_avg_ipg_in = NULL;
struct double_histo_list *mm_avg_ipg_out = NULL;
struct double_histo_list *mm_avg_ipg_loc = NULL;

struct double_histo_list *mm_avg_jitter_in = NULL;
struct double_histo_list *mm_avg_jitter_out = NULL;
struct double_histo_list *mm_avg_jitter_loc = NULL;

struct double_histo_list *mm_n_oos_in = NULL;
struct double_histo_list *mm_n_oos_out = NULL;
struct double_histo_list *mm_n_oos_loc = NULL;
struct double_histo_list *mm_p_oos_in = NULL;
struct double_histo_list *mm_p_oos_out = NULL;
struct double_histo_list *mm_p_oos_loc = NULL;
struct double_histo_list *mm_p_dup_in = NULL;
struct double_histo_list *mm_p_dup_out = NULL;
struct double_histo_list *mm_p_dup_loc = NULL;
struct double_histo_list *mm_p_lost_in = NULL;
struct double_histo_list *mm_p_lost_out = NULL;
struct double_histo_list *mm_p_lost_loc = NULL;
struct double_histo_list *mm_p_late_in = NULL;
struct double_histo_list *mm_p_late_out = NULL;
struct double_histo_list *mm_p_late_loc = NULL;
struct double_histo_list *mm_burst_loss_in = NULL;
struct double_histo_list *mm_burst_loss_out = NULL;
struct double_histo_list *mm_burst_loss_loc = NULL;
struct double_histo_list *mm_reord_p_n_in = NULL;
struct double_histo_list *mm_reord_p_n_out = NULL;
struct double_histo_list *mm_reord_p_n_loc = NULL;
struct double_histo_list *mm_reord_delay_in = NULL;
struct double_histo_list *mm_reord_delay_out = NULL;
struct double_histo_list *mm_reord_delay_loc = NULL;
struct double_histo_list *mm_oos_p_in = NULL;
struct double_histo_list *mm_oos_p_out = NULL;
struct double_histo_list *mm_oos_p_loc = NULL;

      /* rtcp histograms */

struct double_histo_list *rtcp_cl_p_out = NULL;
struct double_histo_list *rtcp_cl_p_in = NULL;
struct double_histo_list *rtcp_cl_p_loc = NULL;
struct double_histo_list *rtcp_cl_b_out = NULL;
struct double_histo_list *rtcp_cl_b_in = NULL;
struct double_histo_list *rtcp_cl_b_loc = NULL;
struct double_histo_list *rtcp_avg_inter_in = NULL;
struct double_histo_list *rtcp_avg_inter_out = NULL;
struct double_histo_list *rtcp_avg_inter_loc = NULL;
struct double_histo_list *rtcp_bt_in = NULL;
struct double_histo_list *rtcp_bt_out = NULL;
struct double_histo_list *rtcp_bt_loc = NULL;
struct double_histo_list *rtcp_mm_bt_in = NULL;
struct double_histo_list *rtcp_mm_bt_out = NULL;
struct double_histo_list *rtcp_mm_bt_loc = NULL;
struct double_histo_list *rtcp_lost_in = NULL;
struct double_histo_list *rtcp_lost_out = NULL;
struct double_histo_list *rtcp_lost_loc = NULL;
struct double_histo_list *rtcp_dup_in = NULL;
struct double_histo_list *rtcp_dup_out = NULL;
struct double_histo_list *rtcp_dup_loc = NULL;
struct double_histo_list *rtcp_f_lost_in = NULL;
struct double_histo_list *rtcp_f_lost_out = NULL;
struct double_histo_list *rtcp_f_lost_loc = NULL;
struct double_histo_list *rtcp_t_lost_in = NULL;
struct double_histo_list *rtcp_t_lost_out = NULL;
struct double_histo_list *rtcp_t_lost_loc = NULL;
struct double_histo_list *rtcp_mm_cl_p_in = NULL;
struct double_histo_list *rtcp_mm_cl_p_out = NULL;
struct double_histo_list *rtcp_mm_cl_p_loc = NULL;
struct double_histo_list *rtcp_mm_cl_b_in = NULL;
struct double_histo_list *rtcp_mm_cl_b_out = NULL;
struct double_histo_list *rtcp_mm_cl_b_loc = NULL;
struct double_histo_list *rtcp_jitter_in = NULL;
struct double_histo_list *rtcp_jitter_out = NULL;
struct double_histo_list *rtcp_jitter_loc = NULL;
struct double_histo_list *rtcp_rtt_in = NULL;
struct double_histo_list *rtcp_rtt_out = NULL;
struct double_histo_list *rtcp_rtt_loc = NULL;

/* global bitrate */
struct double_histo_list *g_tcp_bitrate_in = NULL;
struct double_histo_list *g_tcp_bitrate_loc = NULL;
struct double_histo_list *g_tcp_bitrate_out = NULL;
struct double_histo_list *g_udp_bitrate_in = NULL;
struct double_histo_list *g_udp_bitrate_loc = NULL;
struct double_histo_list *g_udp_bitrate_out = NULL;
struct double_histo_list *g_icmp_bitrate_in = NULL;
struct double_histo_list *g_icmp_bitrate_loc = NULL;
struct double_histo_list *g_icmp_bitrate_out = NULL;
struct double_histo_list *g_other_bitrate_in = NULL;
struct double_histo_list *g_other_bitrate_loc = NULL;
struct double_histo_list *g_other_bitrate_out = NULL;

/* Application stat */

struct double_histo_list *L7_TCP_num_out = NULL;
struct double_histo_list *L7_TCP_num_in = NULL;
struct double_histo_list *L7_TCP_num_loc = NULL;
struct double_histo_list *L7_TCP_num_c_out = NULL;
struct double_histo_list *L7_TCP_num_c_in = NULL;
struct double_histo_list *L7_TCP_num_nc_out = NULL;
struct double_histo_list *L7_TCP_num_nc_in = NULL;

struct double_histo_list *L7_UDP_num_out = NULL;
struct double_histo_list *L7_UDP_num_in = NULL;
struct double_histo_list *L7_UDP_num_loc = NULL;
struct double_histo_list *L7_UDP_num_c_out = NULL;
struct double_histo_list *L7_UDP_num_c_in = NULL;
struct double_histo_list *L7_UDP_num_nc_out = NULL;
struct double_histo_list *L7_UDP_num_nc_in = NULL;

struct double_histo_list *L7_HTTP_num_out = NULL;
struct double_histo_list *L7_HTTP_num_in = NULL;
struct double_histo_list *L7_HTTP_num_loc = NULL;
struct double_histo_list *L7_HTTP_num_c_out = NULL;
struct double_histo_list *L7_HTTP_num_c_in = NULL;
struct double_histo_list *L7_HTTP_num_nc_out = NULL;
struct double_histo_list *L7_HTTP_num_nc_in = NULL;

struct double_histo_list *http_bitrate_out = NULL;
struct double_histo_list *http_bitrate_in = NULL;
struct double_histo_list *http_bitrate_loc = NULL;
struct double_histo_list *http_bitrate_c_out = NULL;
struct double_histo_list *http_bitrate_c_in = NULL;
struct double_histo_list *http_bitrate_nc_out = NULL;
struct double_histo_list *http_bitrate_nc_in = NULL;

struct double_histo_list *L7_WEB_num_out = NULL;
struct double_histo_list *L7_WEB_num_in = NULL;
struct double_histo_list *L7_WEB_num_loc = NULL;

struct double_histo_list *web_bitrate_out = NULL;
struct double_histo_list *web_bitrate_in = NULL;
struct double_histo_list *web_bitrate_loc = NULL;

struct double_histo_list *L7_TLS_num_out = NULL;
struct double_histo_list *L7_TLS_num_in = NULL;
struct double_histo_list *L7_TLS_num_loc = NULL;

struct double_histo_list *tls_bitrate_out = NULL;
struct double_histo_list *tls_bitrate_in = NULL;
struct double_histo_list *tls_bitrate_loc = NULL;

struct double_histo_list *L7_VIDEO_num_out = NULL;
struct double_histo_list *L7_VIDEO_num_in = NULL;
struct double_histo_list *L7_VIDEO_num_loc = NULL;
struct double_histo_list *L7_VIDEO_num_c_out = NULL;
struct double_histo_list *L7_VIDEO_num_c_in = NULL;
struct double_histo_list *L7_VIDEO_num_nc_out = NULL;
struct double_histo_list *L7_VIDEO_num_nc_in = NULL;

struct double_histo_list *video_rate_out = NULL;
struct double_histo_list *video_rate_in = NULL;
struct double_histo_list *video_rate_loc = NULL;
struct double_histo_list *video_rate_c_out = NULL;
struct double_histo_list *video_rate_c_in = NULL;
struct double_histo_list *video_rate_nc_out = NULL;
struct double_histo_list *video_rate_nc_in = NULL;

/* IPv4/IPv6 statistics */
struct double_histo_list *L3_protocol_out = NULL;
struct double_histo_list *L3_protocol_in = NULL;
struct double_histo_list *L3_protocol_loc = NULL;
struct double_histo_list *L3_bitrate_out = NULL;
struct double_histo_list *L3_bitrate_in = NULL;
struct double_histo_list *L3_bitrate_loc = NULL;

/* profiling */
struct double_histo_list *profile_cpu = NULL;
struct double_histo_list *profile_flows = NULL;
struct double_histo_list *profile_tcpdata = NULL;
win_stat ave_win_usr_cpu;
win_stat ave_win_sys_cpu;
double max_cpu;
win_stat active_flows_win_TCP;
win_stat active_flows_win_UDP;
win_stat missed_flows_win_TCP;
win_stat missed_flows_win_UDP;
double tcpdata_received_total;
double tcpdata_missed_total;

struct double_histo_list *profile_trash = NULL;

/* Chat stat */

struct double_histo_list *chat_flow_num = NULL;

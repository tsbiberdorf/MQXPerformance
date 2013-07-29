
/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 1989-2008 ARC International;
* All Rights Reserved
*
*************************************************************************** 
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: cgi.c$
* $Version : 3.8.2.0$
* $Date    : Sep-25-2012$
*
* Comments:
*
*   Example of shell using RTCS.
*
*END************************************************************************/

#include "httpd.h"
#include "cgi.h"
#include "httpServer.h"
 
static _mqx_int cgi_ipstat(HTTPD_SESSION_STRUCT *session);
static _mqx_int cgi_icmpstat(HTTPD_SESSION_STRUCT *session);
static _mqx_int cgi_udpstat(HTTPD_SESSION_STRUCT *session);
static _mqx_int cgi_tcpstat(HTTPD_SESSION_STRUCT *session);
static _mqx_int cgi_rtc_data(HTTPD_SESSION_STRUCT *session);

const HTTPD_CGI_LINK_STRUCT cgi_lnk_tbl[] = {
    { "ipstat",         cgi_ipstat},
    { "icmpstat",       cgi_icmpstat},
    { "udpstat",        cgi_udpstat},
    { "tcpstat",        cgi_tcpstat},
    { "rtcdata",        cgi_rtc_data},
	{ "toggleled1",		cgi_toggle_led1},
	{ "toggleled2",		cgi_toggle_led2},
	{ "toggleled3",		cgi_toggle_led3},
	{ "toggleled4",		cgi_toggle_led4},
	{ "ledControl",     cgi_ledcontrol_set},
	//Datalogger
	{ "adcloggerset",     cgi_adcdatalogger_set},
   { 0, 0 }    // DO NOT REMOVE - last item - end of table
};

static _mqx_int cgi_rtc_data(HTTPD_SESSION_STRUCT *session) {
    TIME_STRUCT time;
    int min, hour;
    
    _time_get(&time);
    
    min = time.SECONDS / 60;
    hour = min / 60;
    min %= 60;
    
    session->response.contenttype = CONTENT_TYPE_PLAIN;
    httpd_sendhdr(session, 0, 0);
    
    CGI_SEND_NUM(hour);
    CGI_SEND_NUM(min);
    CGI_SEND_NUM(time.SECONDS % 60);

    return session->request.content_len;
}

static _mqx_int cgi_ipstat(HTTPD_SESSION_STRUCT *session) {
	session->response.contenttype = CONTENT_TYPE_PLAIN;
	httpd_sendhdr(session, 0, 0);
	
#if RTCSCFG_ENABLE_IP_STATS
    {
    
    #if RTCSCFG_ENABLE_IP4
        IP_STATS_PTR ip = IP_stats();
        
        CGI_SEND_NUM(ip->COMMON.ST_RX_TOTAL);
        CGI_SEND_NUM(ip->ST_RX_DELIVERED);
        CGI_SEND_NUM(ip->ST_RX_FORWARDED);
        CGI_SEND_NUM(ip->COMMON.ST_RX_MISSED);
        CGI_SEND_NUM(ip->COMMON.ST_RX_ERRORS);
        CGI_SEND_NUM(ip->COMMON.ST_RX_DISCARDED);
        CGI_SEND_NUM(ip->ST_RX_HDR_ERRORS);
        CGI_SEND_NUM(ip->ST_RX_ADDR_ERRORS);
        CGI_SEND_NUM(ip->ST_RX_NO_PROTO);
        CGI_SEND_NUM(ip->ST_RX_FRAG_RECVD);
        CGI_SEND_NUM(ip->ST_RX_FRAG_REASMD);
        CGI_SEND_NUM(ip->ST_RX_FRAG_DISCARDED);
        CGI_SEND_NUM(ip->COMMON.ST_TX_TOTAL);
        CGI_SEND_NUM(ip->COMMON.ST_TX_MISSED);
        CGI_SEND_NUM(ip->COMMON.ST_TX_ERRORS);
        CGI_SEND_NUM(ip->COMMON.ST_TX_DISCARDED);
        CGI_SEND_NUM(ip->ST_TX_FRAG_FRAGD);
        CGI_SEND_NUM(ip->ST_TX_FRAG_SENT);
        CGI_SEND_NUM(ip->ST_TX_FRAG_DISCARDED);
    #endif
    
    }
#else
    int i;
    
    for (i = 0; i < 18; i++) {
        httpd_sendstr(session->sock, "unk\n");
    }
#endif

    return session->request.content_len;
}
   
static _mqx_int cgi_icmpstat(HTTPD_SESSION_STRUCT *session) {
	
	session->response.contenttype = CONTENT_TYPE_PLAIN;
	httpd_sendhdr(session, 0, 0);
	
#if RTCSCFG_ENABLE_ICMP_STATS
    {
    #if RTCSCFG_ENABLE_IP4    
        ICMP_STATS_PTR icmp = ICMP_stats();
    
        CGI_SEND_NUM(icmp->COMMON.ST_RX_TOTAL);
        CGI_SEND_NUM(icmp->COMMON.ST_RX_MISSED);
        CGI_SEND_NUM(icmp->COMMON.ST_RX_ERRORS);
        CGI_SEND_NUM(icmp->COMMON.ST_RX_DISCARDED);
        CGI_SEND_NUM(icmp->ST_RX_BAD_CHECKSUM + icmp->ST_RX_SMALL_DGRAM);
        CGI_SEND_NUM(icmp->ST_RX_BAD_CODE);
        CGI_SEND_NUM(icmp->ST_RX_RD_NOTGATE);
    
        CGI_SEND_NUM(icmp->COMMON.ST_TX_TOTAL);
        CGI_SEND_NUM(icmp->COMMON.ST_TX_MISSED);
        CGI_SEND_NUM(icmp->COMMON.ST_TX_ERRORS);
        CGI_SEND_NUM(icmp->COMMON.ST_TX_DISCARDED);
    
        CGI_SEND_NUM(icmp->ST_RX_DESTUNREACH);
        CGI_SEND_NUM(icmp->ST_RX_TIMEEXCEED);
        CGI_SEND_NUM(icmp->ST_RX_PARMPROB);
        CGI_SEND_NUM(icmp->ST_RX_SRCQUENCH);
        CGI_SEND_NUM(icmp->ST_RX_REDIRECT);
        CGI_SEND_NUM(icmp->ST_RX_ECHO_REQ);
        CGI_SEND_NUM(icmp->ST_RX_ECHO_REPLY);
        CGI_SEND_NUM(icmp->ST_RX_TIME_REQ);
        CGI_SEND_NUM(icmp->ST_RX_TIME_REPLY);
        CGI_SEND_NUM(icmp->ST_RX_INFO_REQ);
        CGI_SEND_NUM(icmp->ST_RX_INFO_REPLY);
        CGI_SEND_NUM(icmp->ST_RX_OTHER);
    
        CGI_SEND_NUM(icmp->ST_TX_DESTUNREACH);
        CGI_SEND_NUM(icmp->ST_TX_TIMEEXCEED);
        CGI_SEND_NUM(icmp->ST_TX_PARMPROB);
        CGI_SEND_NUM(icmp->ST_TX_SRCQUENCH);
        CGI_SEND_NUM(icmp->ST_TX_REDIRECT);
        CGI_SEND_NUM(icmp->ST_TX_ECHO_REQ);
        CGI_SEND_NUM(icmp->ST_TX_ECHO_REPLY);
        CGI_SEND_NUM(icmp->ST_TX_TIME_REQ);
        CGI_SEND_NUM(icmp->ST_TX_TIME_REPLY);
        CGI_SEND_NUM(icmp->ST_TX_INFO_REQ);
        CGI_SEND_NUM(icmp->ST_TX_INFO_REPLY);
        CGI_SEND_NUM(icmp->ST_TX_OTHER);
    #endif
    }
#else
    int i;
    
    for (i = 0; i < 34; i++) {
        httpd_sendstr(session->sock, "unk\n");
    }
#endif

    return session->request.content_len;
}
   
static _mqx_int cgi_udpstat(HTTPD_SESSION_STRUCT *session) {
	
	session->response.contenttype = CONTENT_TYPE_PLAIN;
	httpd_sendhdr(session, 0, 0);
	
#if RTCSCFG_ENABLE_UDP_STATS
    {
        UDP_STATS_PTR udp = UDP_stats();

        CGI_SEND_NUM(udp->COMMON.ST_RX_TOTAL);
        CGI_SEND_NUM(udp->COMMON.ST_RX_MISSED);
        CGI_SEND_NUM(udp->COMMON.ST_RX_ERRORS);
        CGI_SEND_NUM(udp->COMMON.ST_RX_DISCARDED);
        CGI_SEND_NUM(udp->ST_RX_BAD_PORT + udp->ST_RX_BAD_CHECKSUM + udp->ST_RX_SMALL_DGRAM + udp->ST_RX_SMALL_PKT);                             
        CGI_SEND_NUM(udp->ST_RX_NO_PORT);
    
        CGI_SEND_NUM(udp->COMMON.ST_TX_TOTAL);
        CGI_SEND_NUM(udp->COMMON.ST_TX_MISSED);
        CGI_SEND_NUM(udp->COMMON.ST_TX_ERRORS);
        CGI_SEND_NUM(udp->COMMON.ST_TX_DISCARDED);
    }
#else
    int i;
    
    for (i = 0; i < 9; i++) {
        httpd_sendstr(session->sock, "unk\n");
    }
#endif

    return session->request.content_len;
}
   
static _mqx_int cgi_tcpstat(HTTPD_SESSION_STRUCT *session) {
	
	session->response.contenttype = CONTENT_TYPE_PLAIN;
	httpd_sendhdr(session, 0, 0);
	
#if RTCSCFG_ENABLE_TCP_STATS
    {
        TCP_STATS_PTR        tcp  = TCP_stats();
    
        CGI_SEND_NUM(tcp->COMMON.ST_RX_TOTAL);
        CGI_SEND_NUM(tcp->COMMON.ST_RX_MISSED);
        CGI_SEND_NUM(tcp->COMMON.ST_RX_ERRORS);
        CGI_SEND_NUM(tcp->COMMON.ST_RX_DISCARDED);
        CGI_SEND_NUM(tcp->ST_RX_BAD_PORT + tcp->ST_RX_BAD_CHECKSUM + tcp->ST_RX_SMALL_HDR + tcp->ST_RX_SMALL_DGRAM + tcp->ST_RX_SMALL_PKT);
        CGI_SEND_NUM(tcp->ST_RX_BAD_ACK);
        CGI_SEND_NUM(tcp->ST_RX_BAD_DATA);
        CGI_SEND_NUM(tcp->ST_RX_LATE_DATA);
        CGI_SEND_NUM(tcp->ST_RX_DATA);
        CGI_SEND_NUM(tcp->ST_RX_DATA_DUP);
        CGI_SEND_NUM(tcp->ST_RX_ACK);
        CGI_SEND_NUM(tcp->ST_RX_ACK_DUP);
        CGI_SEND_NUM(tcp->ST_RX_RESET);
        CGI_SEND_NUM(tcp->ST_RX_PROBE);
        CGI_SEND_NUM(tcp->ST_RX_WINDOW);
    
        CGI_SEND_NUM(tcp->COMMON.ST_TX_TOTAL);
        CGI_SEND_NUM(tcp->COMMON.ST_TX_MISSED);
        CGI_SEND_NUM(tcp->COMMON.ST_TX_ERRORS);
        CGI_SEND_NUM(tcp->COMMON.ST_TX_DISCARDED);
        CGI_SEND_NUM(tcp->ST_TX_DATA);
        CGI_SEND_NUM(tcp->ST_TX_DATA_DUP);
        CGI_SEND_NUM(tcp->ST_TX_ACK);
        CGI_SEND_NUM(tcp->ST_TX_ACK_DELAYED);
        CGI_SEND_NUM(tcp->ST_TX_RESET);
        CGI_SEND_NUM(tcp->ST_TX_PROBE);
        CGI_SEND_NUM(tcp->ST_TX_WINDOW);
    
        CGI_SEND_NUM(tcp->ST_CONN_ACTIVE);
        CGI_SEND_NUM(tcp->ST_CONN_PASSIVE);
        CGI_SEND_NUM(tcp->ST_CONN_OPEN);
        CGI_SEND_NUM(tcp->ST_CONN_CLOSED);
        CGI_SEND_NUM(tcp->ST_CONN_RESET);
        CGI_SEND_NUM(tcp->ST_CONN_FAILED);
    }
#else
    int i;
    
    for (i = 0; i < 23; i++) {
        httpd_sendstr(session->sock, "unk\n");
    }
#endif
    
    return session->request.content_len;
}


uint_32 str2int(char_ptr string) 
{
   int_8 i;
   uint_8 len, mask = 0x30;
   uint_32 pow[] = { 1, 10, 100, 1000, 10000, 100000, 1000000};
   uint_32 result = 0;
   
   len = strlen(string);
   for(i=0; i<len; i++) 
   {
   	  result += (((char*)string)[i] & ~mask)*pow[strlen(string)-1-i];
   }
   return result;
}


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
* $FileName: config.h$
* $Version : 3.8.11.0$
* $Date    : Jun-14-2012$
*
* Comments:
*
*   Configurable information for the RTCS examples.
*
*END************************************************************************/

/* Disable the cache */
#define MON_TYPE   MQX_MONITOR_TYPE_BDM

/*
** Uncomment this line to enable Ethernet
*/
#define ENET_DEVICE     0

/*
** Define the Ethernet MAC address, IP address and IP network mask
** These are ignored if ENET_DEVICE is not #define'd
*/
#ifndef ENET_IPADDR
    #define ENET_IPADDR  IPADDR(192,168,1,202) 
#endif

#ifndef ENET_IPMASK
    #define ENET_IPMASK  IPADDR(255,255,255,0) 
#endif

/* Define TRAP receiver addresses here */
#define ENET_TRAP_ADDR1 IPADDR(192,168,1,182)
#define ENET_TRAP_ADDR2 IPADDR(192,168,1,182)
#define ENET_TRAP_ADDR3 IPADDR(192,168,1,182)
#define ENET_TRAP_ADDR4 IPADDR(192,168,1,182)
#define ENET_TRAP_ADDR5 IPADDR(192,168,1,182)

/*
** The target's Ethernet address
** The first three bytes should be 00 00 5E
** The last three bytes should match the last three bytes of
** your test IP address
*/
#undef  _IPBN       /* IP address byte number b */
#define _IPBN(b)    (((ENET_IPADDR) >> (b*8)) & 255)
#define ENET_ENETADDR   {0x00, 0x00, 0x5E, _IPBN(2), _IPBN(1), _IPBN(0)}

/*
** Uncomment this line to enable PPP
*/

//#define PPP_DEVICE      "ittya:"

/*
** Define PPP_DEVICE_DUN only when using PPP to communicate
** to Win9x Dial-Up Networking over a null-modem
** This is ignored if PPP_DEVICE is not #define'd
*/
#define PPP_DEVICE_DUN

/*
** Define the local and remote IP addresses for the PPP link
** These are ignored if PPP_DEVICE is not #define'd
*/
#define PPP_LOCADDR     IPADDR(192,168,1,203) 
#define PPP_PEERADDR    IPADDR(192,168,1,204) 

/*
** Define a default gateway
*/
#define GATE_ADDR        IPADDR(0,0,0,0) 

#ifndef ENET_IPGATEWAY
    #define ENET_IPGATEWAY  IPADDR(0,0,0,0) 
#endif

/*
** Define a server to be used by the example clients
*/
//#define SERVER_ADDR     0xC00002CC

/* code copied in from the http sample operation
 */
/* IP address macros */
#define A 192
#define B 168
#define C 1
#define D 202

#ifndef ENET_IPMASK
    #define ENET_IPMASK  IPADDR(255,255,255,0) 
#endif

#define DEMOCFG_USE_WIFI  0  /* USE WIFI Interface */

#if DEMOCFG_USE_WIFI
   
   #define DEMOCFG_SSID            "NGZG"
   //Possible Values managed or adhoc
   #define DEMOCFG_NW_MODE         "managed" 
   //Possible vales 
   // 1. "wep"
   // 2. "wpa"
   // 3. "wpa2"
   // 4. "none"
   #define DEMOCFG_SECURITY        "none"
   #define DEMOCFG_PASSPHRASE      NULL
   #define DEMOCFG_WEP_KEY         "ABCDE"
   //Possible values 1,2,3,4
   #define DEMOCFG_WEP_KEY_INDEX   1
   #define DEMOCFG_DEFAULT_DEVICE  1
#endif

#ifndef DEMOCFG_DEFAULT_DEVICE
   #define DEMOCFG_DEFAULT_DEVICE   BSP_DEFAULT_ENET_DEVICE
#endif


#if RTCSCFG_ENABLE_IP6   
    #if RTCSCFG_ENABLE_IP4   
        /* 
        **  HTTP_INET_AF is AF_INET+AF_INET for http support IPv4+IPv6.
        **  HTTP_INET_AF is AF_INET  for http support IPv4 only.
        **  HTTP_INET_AF is AF_INET6 for http support IPv6 only.
        */
        #define HTTP_INET_AF    AF_INET6+AF_INET 
    #else
        #define HTTP_INET_AF    AF_INET6        
    #endif
#else
        #define HTTP_INET_AF    AF_INET         
#endif
    
    //#define HTTP_INET_AF           AF_INET
    #define HTTP_SCOPE_ID          0 /* For any IF. */


/* EOF */

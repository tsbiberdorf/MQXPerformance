/*
 * httpServer.c
 *
 *  Created on: Jul 24, 2013
 *      Author: tsbiberdorf
 */

#include <mqx.h>
#include <bsp.h>
#include <rtcs.h>
#include <ipcfg.h>

#include "httpd.h"
#include "httpServer.h"
#include "tfs.h"
#include "cgi.h"
#include "enet_wifi.h"
#include "config.h"
#include "ioDriver.h"

#include <select.h>
#include <shell.h>
#include <sh_rtcs.h>



const HTTPD_ROOT_DIR_STRUCT root_dir[] = 
{
    { "", "tfs:" },
    { 0, 0 }
};


/*TASK*-----------------------------------------------------------------
*
* Function Name  : main_task
* Returned Value : void
* Comments       :
*
*END------------------------------------------------------------------*/
void http_task(uint_32 temp) 
{
    int_32            error;
    uint_32 ipByte;

    HTTPD_STRUCT                        *server[BSP_ENET_DEVICE_COUNT];
    extern const HTTPD_CGI_LINK_STRUCT  cgi_lnk_tbl[];
    extern const TFS_DIR_ENTRY          tfs_data[];
    HTTPD_PARAMS_STRUCT                 *params[BSP_ENET_DEVICE_COUNT];
    _enet_address                       address[BSP_ENET_DEVICE_COUNT];
    uint_32                             phy_addr[BSP_ENET_DEVICE_COUNT];
    uint_32                             ip_addr[BSP_ENET_DEVICE_COUNT];    
    uint_32                             i = 0;
    char*                               indexes[BSP_ENET_DEVICE_COUNT];
    uint_8 								n_devices = BSP_ENET_DEVICE_COUNT;
#if RTCSCFG_ENABLE_IP6      
    IPCFG6_GET_ADDR_DATA    data[RTCSCFG_IP6_IF_ADDRESSES_MAX];
    char prn_addr6[sizeof "ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"];
    uint_32 n = 0;
    uint_32 j = 0;
#endif

    _int_install_unexpected_isr();
    /*---------------- Initialize devices and variables ----------------------*/
    _RTCSPCB_init = 4;
    _RTCSPCB_grow = 2;
    _RTCSPCB_max = 8;
    
    _RTCSTASK_stacksize = 4500;
    
    InitializeIO();
    
    /*--------------------------- Init end -----------------------------------*/ 
    
    error = RTCS_create();
    if (error != RTCS_OK) 
    {
        printf("RTCS failed to initialize, error = 0x%X\n", error);
        _task_block();
    }
    _IP_forward = TRUE;
    
    for (i = 0; (i < n_devices) && (n_devices != 0); i++)
    {
        IPCFG_IP_ADDRESS_DATA	ip_data; 
        phy_addr[i] = i;
        ip_addr[i] = IPADDR(A,B,C+i,D);
        ENET_get_mac_address (phy_addr[i], ip_addr[i], address[i]);
        /* Init ENET device */
        error = ipcfg_init_device (phy_addr[i], address[i]);
        if (error != RTCS_OK) 
        {
            printf("IPCFG: Device n.%d init failed. Error = 0x%X\n", i, error);
            _task_set_error(MQX_OK);
            n_devices--;
            i--;
            continue;
        }
#if RTCSCFG_ENABLE_IP4
        ip_data.ip      = ip_addr[i];
        ip_data.mask    = ENET_IPMASK;
        ip_data.gateway = 0;
        /* Bind IPv4 address */
#if STATIC_IP_BINDING
        error = ipcfg_bind_staticip (phy_addr[i], &ip_data);
        if (error != RTCS_OK) 
        {
            printf("\nIPCFG: Failed to bind IP address. Error = 0x%X", error);
            _task_block();
        }
#else

        /* DHCP binding   
       #################################################################
         */

        do
        {
        	_time_delay (200);
        	error = ipcfg_bind_dhcp (BSP_DEFAULT_ENET_DEVICE, TRUE);
        	printf ("Error during DHCP bind 1 %08x!\n", error);
        } while (error == IPCFG_ERROR_BUSY);
        if (error == IPCFG_ERROR_OK)
        {
        	do
        	{
        		_time_delay (200);
        		error = ipcfg_poll_dhcp (BSP_DEFAULT_ENET_DEVICE, TRUE, &ip_data);
        		printf ("Error during DHCP poll %08x!\n", error);
        	}while (error == IPCFG_ERROR_BUSY);
        }
        if (error != IPCFG_ERROR_OK)
        {
        	printf ("Error during DHCP bind %08x!\n", error);
        }
        else
        {
        	printf ("DHCP bind successful \n");
        }

        ipcfg_get_ip(0, &ip_data);

        printf ("IP address = %x.\n",ip_data.ip); // send the IP address to the hyperterminal.
        printf ("IP address = ");
        ipByte = (ip_data.ip & 0xFF000000)>>24; // send the IP address to the hyperterminal.
        printf("%d.",ipByte);
        ipByte = (ip_data.ip & 0xFF0000)>>16; // send the IP address to the hyperterminal.
        printf("%d.",ipByte);
        ipByte = (ip_data.ip & 0xFF00)>>8; // send the IP address to the hyperterminal.
        printf("%d.",ipByte);
        ipByte = (ip_data.ip & 0xFF); // send the IP address to the hyperterminal.
        printf("%d\n\n",ipByte);
        /* DHCP binding end
        *********************************************************************
        */
#endif
        
        
        
        
        
        
#endif /* RTCSCFG_ENABLE_IP4 */
       
        indexes[i] = (char*) _mem_alloc(sizeof("\\index_x.html"));
    }  
    
 
#if DEBUG__MESSAGES 
    printf("Preparing http servers...\n");
#endif
    for (i = 0; i < n_devices; i++)
    {
        params[i] = httpd_default_params(NULL);
        params[i]->af = HTTP_INET_AF;  //IPv4+IPv6, set connection family from config.h
#if RTCSCFG_ENABLE_IP6   
        params[i]->if_scope_id = HTTP_SCOPE_ID;    // set interface number here. 0 is any .
#endif
        if (params[i])
        {
            sprintf(indexes[i], "\\index.html", i);
            params[i]->root_dir = (HTTPD_ROOT_DIR_STRUCT*)root_dir;
            params[i]->index_page = indexes[i];
            printf("Starting http server No.%d on IP", i);
             /*
            ** If there is only one interface listen on any IP address 
            ** so address can change in runtime (DHCP etc.)
            */
            #if RTCSCFG_ENABLE_IP4
            if ((i == 0) && (n_devices == 1))
            {
                params[i]->address = INADDR_ANY;
            }
            else
            {
                params[i]->address = ip_addr[i];
            }
            /* Print active ip addresses */
            printf(" %d.%d.%d.%d", IPBYTES(ip_addr[i]));
            #endif
            #if RTCSCFG_ENABLE_IP6 
            while(!ipcfg6_get_addr_info_n(i, n, &data[n]))
            {
                n++;
            }
            while(j < n)
            {
                if(inet_ntop(AF_INET6, &data[j++].ip_addr, prn_addr6, sizeof(prn_addr6)))
                {
                    /* Print active ip addresses */
                    #if RTCSCFG_ENABLE_IP4
                    printf("%s %s", (HTTP_INET_AF & AF_INET) ? " and" : "", prn_addr6);
                    #else
                    printf(" %s", prn_addr6);
                    #endif
                }
            }
            #endif
#if PSP_MQX_CPU_IS_MCF5223X
            params[i]->max_ses = 1;
#else
            params[i]->max_ses = _RTCS_socket_part_max - 1;
#endif
            server[i] = httpd_init(params[i]);
        }
        HTTPD_SET_PARAM_CGI_TBL(server[i], (HTTPD_CGI_LINK_STRUCT*)cgi_lnk_tbl);
#if HTTPD_SEPARATE_TASK || !HTTPDCFG_POLL_MODE
        printf("...");
        error = httpd_server_run(server[i]);
        if (error)
        {
            printf("[FAIL]\n");
        }
        else
        {
            printf("[OK]\n");
        }
#endif
    }

    /* user stuff come here */
#if HTTPD_SEPARATE_TASK || !HTTPDCFG_POLL_MODE      
//    _task_create(0, SHELL_TASK, 0);
//    _task_block();
#else
    printf("Servers polling started.\n")
    while (1)
    {
        for (i = 0; i < n_devices; i++)
        {
            httpd_server_poll(server[i], 1);
        }
        /* user stuff come here - only non blocking calls */
    }
#endif
}

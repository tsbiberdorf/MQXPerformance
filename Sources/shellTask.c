/*
 * shellTask.c
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

const SHELL_COMMAND_STRUCT Shell_commands[] = {
   /* RTCS commands */ 

   { "help",      Shell_help }, 
   { "ipconfig",  Shell_ipconfig },   
#if RTCSCFG_ENABLE_ICMP      
   { "ping",      Shell_ping },      
#endif
   { "?",         Shell_command_list },     
   { NULL,        NULL } 
};



#define SES_MAX     4

void shell_task(uint_32 datas)
{
    for (;;)  
    {
        /* Run the shell */
        Shell(Shell_commands, NULL);
        printf("Shell exited, restarting...\n");
    }
}

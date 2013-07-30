
//>>>>>>>>>>>>>>>>>>>>>>>>
//cgi-datalogger.c
//>>>>>>>>>>
//This file contains the CGI responder function for the Datalogger / Sampler configuration page.
//


#include <mqx.h>
#include <httpd.h>
#include <string.h>
#include "cgi.h"

//#include <ltctwrutils.h>
//#include <ltctwrboard.h>
//
//#include <logging.h>


extern uint_32 samplerprogresscount;


//CGI Function that configures the DataLogger Task with the appropriate parameters

_mqx_int cgi_adcdatalogger_set(HTTPD_SESSION_STRUCT *session) 
{
	uint_32  len = 0;
	char span[2], setting[2], temp[2], interval[10], numsamp[10], logname[16];
	uint_32 iLedState;
	
	char ledSelection[3] = "A0";
	char channame2498[3] = "B0";
    uint_8  i;
    
	
	boolean  bParams = FALSE; 
	char buffer[100];
	//uint_32  result;
	
	uint_8 spanindex, setindex, count;
	uint_8 mask = 0x41;
//	LTCTWR_PARAMS_PTR LTCTWR_Params_Ptr;
	
	pointer pt;
    _task_id id;
	

	//Get pointer and configure
//	LTCTWR_Params_Ptr = LTCTWR_GetParameters();

	

	//If we got a response of non-zero length
	
	if (session->request.content_len) {
    	len = session->request.content_len;
    	len = httpd_read(session, buffer, (int)((len > sizeof(buffer)) ? sizeof(buffer) : len));
    	buffer[len] = 0;
    	session->request.content_len -= len;
    	len = 0;
    	
    	//Check if all contents (except channel) are present
		if (httpd_get_varval(session, buffer, "int", interval, sizeof(interval)) &&
			httpd_get_varval(session, buffer, "destname", logname, sizeof(logname)) &&
			httpd_get_varval(session, buffer, "span", span, sizeof(span))
			) 
		{
			
			bParams =  TRUE;
			
			printf("Destname: %s\n",logname);
			printf("Sample interval:%d\n",str2int(interval) );
            spanindex = (*(char*)span) - mask;//Figure out the Span (1859)
            printf("Span Index: %d\n", spanindex);
            
            
            //Find the selected channels
            count = 0;
            iLedState = 0;
            for (i = 0; i<4; i++) 
            {
            	ledSelection[1] = (char)(i + 0x41);
            	if(httpd_get_varval(session, buffer, ledSelection, temp, sizeof(temp))) 
	            {
            		switch(ledSelection[1])
            		{
            		case 'A':
            			iLedState |= 0x01;
            			break;
            		case 'B':
            			iLedState |= 0x02;
            			break;
            		case 'C':
            			iLedState |= 0x04;
            			break;
            		case 'D':
            			iLedState |= 0x08;
            			break;
            		}
	            	printf("%s selected\n", ledSelection);
	            }
            }
            for (i = 0; i<4; i++) 
            {
            	SetOutput((1<<i), iLedState & (1<<i) );
            }

			
			
			count = 0;
            for (i = 0; i<4; i++) 
            {
            	channame2498[1] = (char)(i + 0x41);
            	if(httpd_get_varval(session, buffer, channame2498, temp, sizeof(temp))) 
	            {
	            	printf("%s selected\n", channame2498);
	            	count ++;
	            }
            }
            
     

			httpd_sendstr(session->sock, 
			"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">"
			"<html><head><title>MQX</title>"
			"<meta http-equiv=\"REFRESH\" content=\"0;url=adclogger.html\"></HEAD>"
			"<BODY></BODY></HTML>");   
			
  			//_task_create(0, 5, 0);  //5 is the logger task
			//LTC1859_LogCurrentState(); 
        	
        	
       		printf("Setup complete, unblocking sampler\n");
	  		id = _task_get_id_from_name("Sampler");  //Only good way to get the id
	 		pt = _task_get_td(id);
	 		_task_ready(pt);
    				
   		}
   	}
  
   
	if (!bParams) {
		httpd_sendstr(session->sock, "No parameters received.<br>\n");
	}
	
	
	return session->request.content_len;
}


_mqx_int cgi_sampler_progress(HTTPD_SESSION_STRUCT *session) {
//	CGI_SEND_NUM(samplerprogresscount);
//	CGI_SEND_NUM(samplerprogresscount);
//	CGI_SEND_NUM(samplerprogresscount);

	return session->request.content_len;
}/*
 * cgi-datalogger.c
 *
 *  Created on: Jul 29, 2013
 *      Author: tsbiberdorf
 */



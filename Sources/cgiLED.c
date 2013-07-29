/*
 * cgiLED.c
 *
 *  Created on: Jul 26, 2013
 *      Author: tsbiberdorf
 */

#include <mqx.h>
#include <httpd.h>
#include <string.h>
#include "cgi.h"

_mqx_int cgi_toggle_led1(HTTPD_SESSION_STRUCT *session)
{
	static boolean iLedState = FALSE;
	if(iLedState)
	{
		iLedState = FALSE;
	}
	else
	{
		iLedState = TRUE;
	}
	SetOutput(1, iLedState);
    return session->request.content_len;
}

_mqx_int cgi_toggle_led2(HTTPD_SESSION_STRUCT *session)
{
	static boolean iLedState = FALSE;
	if(iLedState)
	{
		iLedState = FALSE;
	}
	else
	{
		iLedState = TRUE;
	}
	SetOutput(2, iLedState);
    return session->request.content_len;
}

_mqx_int cgi_toggle_led3(HTTPD_SESSION_STRUCT *session)
{
	static boolean iLedState = FALSE;
	if(iLedState)
	{
		iLedState = FALSE;
	}
	else
	{
		iLedState = TRUE;
	}
	SetOutput(3, iLedState);
    return session->request.content_len;
}

_mqx_int cgi_toggle_led4(HTTPD_SESSION_STRUCT *session)
{
	static boolean iLedState = FALSE;
	if(iLedState)
	{
		iLedState = FALSE;
	}
	else
	{
		iLedState = TRUE;
	}
	SetOutput(4, iLedState);
    return session->request.content_len;
}



_mqx_int cgi_ledcontrol_set(HTTPD_SESSION_STRUCT *session)
{
	uint_32  len = 0;
	int i;
	char     LEDSelection[10] = "LEDA";
	boolean  bParams = FALSE; 
	char buffer[100];
	char temp[2];
	char interval[10];
	uint_32 inputValue;
	
	uint_8 chanindex, setindex;
	uint_8 mask = 0x41;

	if (session->request.content_len)
	{
    	len = httpd_read(session, buffer, (int)((len > sizeof(buffer)) ? sizeof(buffer) : len));
    	buffer[len] = 0;

    	if( httpd_get_varval(session, buffer, "int", interval, sizeof(interval)) )
    	{
        	inputValue = str2int(interval);
        	printf("text box input: %d\n",inputValue);
        	
    		for(i=0; i<4;i++)
    		{
    			LEDSelection[3] = (char)(i + 0x41);
    			if(httpd_get_varval(session, buffer, LEDSelection, temp, sizeof(temp))) 
    			{
    				printf("%s selected\n", LEDSelection);
    			}
    		}
    	}
    	
    	printf("received %d\n\n",session->request.content_len);
    	
    	session->request.content_len -= len;
    	len = 0;

		
		httpd_sendstr(session->sock, 
		"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">"
		"<html><head><title>MQX</title>"
		"<meta http-equiv=\"REFRESH\" content=\"0;url=led.html\"></HEAD>"
		"<BODY></BODY></HTML>"); 
	}
	return session->request.content_len;

}



/*
 * debug.h
 *  Sebastien Couret <sebastien.couret@elios-informatique.fr> November 2003
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEBUG_DHCP_H
#define _DEBUG_DHCP_H


#include <stdio.h>

#undef LOG_EMERG
#undef LOG_ALERT
#undef LOG_CRIT
#undef LOG_WARNING
#undef LOG_ERR
#undef LOG_INFO
#undef LOG_DEBUG
#undef LOG

//#define LOGACTIVE

#ifdef LOGACTIVE
#define LOG_EMERG   "EMER"
#define LOG_ALERT   "ALER"
#define LOG_CRIT    "crit"
#define LOG_WARNING "warn"
#define LOG_ERR "error"
#define LOG_INFO    "info"
#define LOG_DEBUG   "debug"
#undef LOG
#define LOG(level, function, str, args...) do { printf("[DHCPELIOS]\t{%12s}\t\t%s,\t\t", function, level); \
                printf(str, ## args); \
                printf("\n"); } while(0)
#else
#define LOG_EMERG   ""
#define LOG_ALERT   ""
#define LOG_CRIT    ""
#define LOG_WARNING ""
#define LOG_ERR     ""
#define LOG_INFO    ""
#define LOG_DEBUG   ""
#define LOG(fnction,level, str, args...) do {;} while(0)
#endif

#ifdef DEBUGGING
#undef DEBUG
#define DEBUG(level, str, args...) LOG(level, str, ## args)
#else
#undef DEBUG
#define DEBUG(level, str, args...) do {;} while(0)
#endif

#endif

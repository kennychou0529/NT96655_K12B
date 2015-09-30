/**
    Iperf module

    Network tool for measuring bandwidth.

    @file       Iperf.h
    @ingroup    mIIperf

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/
#ifndef _IPERF_H
#define _IPERF_H
/**
    @addtogroup mIIPERF
*/
//@{

__externC void Iperf_Open( int argc, char **argv );

__externC void Iperf_Close(void);

__externC void Iperf_RunClient( int argc, char **argv );


//@}
#endif

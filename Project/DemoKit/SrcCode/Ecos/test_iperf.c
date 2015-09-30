#include <cyg/infra/mainfunc.h>
#include "Iperf.h"

MAINFUNC_ENTRY(iperf,argc,argv)
{
    Iperf_Close();
    Iperf_Open(argc, argv);
    return 0;
}

MAINFUNC_ENTRY(iperfc,argc,argv)
{
    Iperf_RunClient(argc, argv);
    return 0;
}


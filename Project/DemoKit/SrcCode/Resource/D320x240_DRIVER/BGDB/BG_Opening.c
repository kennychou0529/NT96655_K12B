#include "BG_Images.h"

//¿ª»úlogo
#include "welcome.c"
#include "Goodbye.c"

//#include "decai_off.c"
//#include "decai_on.c"

int Logo_getBGOpening_size(void)
{
    return sizeof(g_ucBGOpening);
}

int Logo_getBGGoodbye_size(void)
{
    return sizeof(g_ucBGGoodbye);
}

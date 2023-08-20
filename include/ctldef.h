#ifndef CTLDEF_H
#define CTLDEF_H

#ifdef __cplusplus
    #define CTL_EXTC_OPEN extern "C" {
    #define CTL_EXTC_CLOSE }
#else
    #define CTL_EXTC_OPEN
    #define CTL_EXTC_CLOSE
#endif

#endif

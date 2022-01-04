#ifndef __JD_USER_CONFIG_H__
#define __JD_USER_CONFIG_H__

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef JD_LOG
#define JD_LOG JD_NOLOG
#endif

#define JD_CONFIG_STATUS 0

static uint32_t now;

#ifdef __cplusplus
}
#endif

#endif
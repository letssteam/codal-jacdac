
#include "jd_protocol.h"

/**
 * This function configures firmware for a given module.
 * It's typically the only function that changes between modules.
 */
void app_init_services(void)
{
    // Nothing to do
}

/**
 * This is an optionally implementable callback invoked in main.c.
 * It allows apps to perform meta-level configuration detection.
 * This is useful for devices that may need to dynamically detect
 * hardware changes at runtime (e.g. XAC module).
 */
void app_process(void)
{
    // Nothing to do
}

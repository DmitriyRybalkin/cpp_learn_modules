#include "lib.h"

#include "version.h"

int major_version()
{
    return VERSION_MAJOR;
}

int minor_version()
{
    return VERSION_MINOR;
}

int patch_version()
{
    return VERSION_PATCH;
}

const char *full_version()
{
    return VERSION_STRVERSION;
}

const char *application_name()
{
    return APPLICATION_NAME;
}
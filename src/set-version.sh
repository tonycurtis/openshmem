#!/bin/sh

rev="`svn info | grep '^Revision:'`"

cat >version.c <<EOT
/*
 * AUTOGENERATED, DO NOT EDIT DIRECTLY
 */

char *
shmem_version(void)
{
  return "Super Happy Fun OpenSHMEM, $rev";
}

#ifdef HAVE_PSHMEM_SUPPORT
_Pragma("weak pshmem_version=shmem_version")
#endif /* HAVE_PSHMEM_SUPPORT */
EOT

#if !defined(__EXT_ORACLE_H__)
#define __EXT_ORACLE_H__

#if defined (OCI_API)
#undef OCI_API
#endif

//Fix linked error
#define OCI_API __stdcall


#include <stdlib.h>
#include <ocilib.h>


#endif //end of #if !defined(__EXT_ORACLE_H__)

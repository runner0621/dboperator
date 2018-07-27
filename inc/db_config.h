#if !defined(__DB_CONFIG_H__)
#define __DB_CONFIG_H__

#include "public.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define DBC_LINE_LEN_MAX       2048

#define DBC_KEY_LEN_MAX        32

#define DBC_FILE_LEN_MAX       (1024*10)



int LoadDefaultConfig(void);

int DbcDefaultDriverSet(const char* value);

int DbcDefaultInstanceNameSet(const char* value);

int DbcDefaultDbUserNameSet(const char* value);

int DbcDefaultDbIpSet(const char* value);

int DbcDefaultDbPortSet(const char* value);

int DbcDefaultSearchModeSet(BOOL isCaseSensitive);

int DbCfgOciLibPathSet(const char* value);

int DbcDefaultDriverGet(const char* valueBuf, int valueBufLen);

int DbcDefaultInstanceNameGet(const char* valueBuf, int valueBufLen);

int DbcDefaultDbUserNameGet(const char* valueBuf, int valueBufLen);

int DbcDefaultDbIpGet(const char* valueBuf, int valueBufLen);

int DbcDefaultDbPortGet(const char* valueBuf, int valueBufLen);

int DbcDefaultSearchModeGet(BOOL* isCaseSensitive);

int DbCfgOciLibPathGet(const char* valueBuf, int valueBufLen);

#if defined(__cplusplus)
}
#endif

#endif //end of #if !defined(__DB_CONFIG_H__)

#if !defined(__DB_MANAGER_H__)
#define __DB_MANAGER_H__

#include "public.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define DB_NAME_LEN_MAX                   30

//#define DB_DEFAULT_PORT                 1521

#define HANDLER_INDEX_IS_VALID(hd_index)  ((hd_index>=0) && (hd_index<DbmGetSupportedDbCount()))

#define INVALID_HANDLER_INDEX             -1

#define DBOP_NAME_ORACLE                  "OCI"

#define DBOP_TABALE_SEPARATOR             ','

#define DBOP_SEARCH_MODE_CASE_SENSITIVE   0
#define DBOP_SEARCH_MODE_IGNORE_CASE      1

/* C Data Type mapping */
#define DBOP_CDT_INVALID                  -1
//short, int, long long, float, double
#define DBOP_CDT_NUMERIC                  (1<<1)
//OCI_Date *
#define DBOP_CDT_DATETIME                 (1<<3)
//otext *
#define DBOP_CDT_TEXT                     (1<<4)
//OCI_Long *
#define DBOP_CDT_LONG                     (1<<5)
//OCI_Statement *
#define DBOP_CDT_CURSOR                   (1<<6)
//OCI_Lob  *
#define DBOP_CDT_LOB                      (1<<7)
//OCI_File *
#define DBOP_CDT_FILE                     (1<<8)
//OCI_Timestamp *
#define DBOP_CDT_TIMESTAMP                (1<<9)
//OCI_Interval *
#define DBOP_CDT_INTERVAL                 (1<<10)
//void *
#define DBOP_CDT_RAW                      (1<<11)
//OCI_Object *
#define DBOP_CDT_OBJECT                   (1<<12)
//OCI_Coll *
#define DBOP_CDT_COLLECTION               (1<<13)
//OCI_Ref *
#define DBOP_CDT_REF                      (1<<14)
//boolean
#define DBOP_CDT_BOOLEAN                  (1<<15)


#define  DBOP_INSTANT_CLIENT_PATH   "D:/Program Files (x86)/Oracle/Instant Client/bin"


typedef struct {
    //TODO: interface description
    int (* Open)(const char* strServiceName,
                 const char* strUserName,
                 const char* strPassword);
    int (* Close)(void);
    int (* Execute)(char* str);
    int (* GetColumnType)(unsigned int columnIndex);
    const char* (* GetColumnName)(unsigned int columnIndex);
    int (* GetColumnCount)(void);
    BOOL (* FetchNext)(void);
    const char* (* GetString)(unsigned int columnIndex);
    int (* GetAllTableName)(char* buf_ptr, int buf_size);

} DbHandlerSt;

typedef struct {
    const char dbName[DB_NAME_LEN_MAX];
    const DbHandlerSt handler;
} DbInfoSt;

int DbmGetSupportedDbCount();

const char* DbmGetSupportedDbName(int handler_index);

const DbHandlerSt* DbmInit(const char* dbName);

int DbmDatabaseSearch(const DbHandlerSt* handlePtr,
        int dataType,
        char* searchStr,
        int searchMode,
        const char* searchResultSavePath);

#if defined(__cplusplus)
}
#endif

#endif //end of #if !defined(__DB_MANAGER_H__)

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


typedef struct {

    /**
     * @brief
     * Initialize the library.
     * Create a physical connection to an Oracle database server. Executing SQL statements
     *
     * @param strServiceName - Database service name, for example: "192.168.41.200:1521/orcl"
     * @param strUserName - User name for database login
     * @param strPassword - Password for database login
     *
     *  @return
     *  DBOP_SUCCESS on success otherwise DBOP_FAILURE
     */
    int (* Open)(const char* strServiceName,
                 const char* strUserName,
                 const char* strPassword);

    /**
     * @brief
     * Perform an immediate abort of any currently Oracle OCI call.
     * Clean up all resources allocated by the library.
     *
     *  @return
     *  DBOP_SUCCESS on success otherwise DBOP_FAILURE
     */
    int (* Close)(void);

    /**
     * @brief
     * Prepare and Execute a SQL statement or PL/SQL block.
     * Retrieve the resultset handle from an executed statement.
     *
     * @param strSql  - SQL order - PL/SQL block
     *
     *  @return
     *  DBOP_SUCCESS on success otherwise DBOP_FAILURE
     */
    int (* Execute)(char* str);

    /**
     * @brief
     * Get column type.
     *
     * @param columnIndex - column index, start with 1.
     *
     *  @return
     *  column type, refer to DBOP_CDT_xxx
     */
    int (* GetColumnType)(unsigned int columnIndex);

    /**
     * @brief
     * Get column name.
     *
     * @param columnIndex - column index, start with 1.
     *
     *  @return
     *  column name
     */
    const char* (* GetColumnName)(unsigned int columnIndex);

    /**
     * @brief
     * Get column count.
     *
     *  @return
     *  column count
     */
    int (* GetColumnCount)(void);

    /**
     * @brief
     * Fetch the next row of the resultset.
     *
     *  @return
     * TRUE on success otherwise FALSE if :
     * - Empty resultset
     * - Last row already fetched
     * - An error occurred
     */
    BOOL (* FetchNext)(void);

    /**
     * @brief
     * Return the current string value of the column at the given index in the resultset.
     *
     * @param columnIndex - column index, start with 1.
     *
     *  @return
     *  string value of the column at the given index in the resultset
     */
    const char* (* GetString)(unsigned int columnIndex);

    /**
     * @brief
     * Get all table name with database.
     *
     * @param buf_ptr  - The buffer to output all table list, seperating with ','
     * @param buf_size - Buffer size.
     *
     *  @return
     *  table count with database.
     */
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

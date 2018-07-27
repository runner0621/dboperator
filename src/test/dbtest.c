#include "plg_oracle.h"
#include "public.h"
#include "db_manager.h"
#include "db_config.h"
#include "utilities.h"

#if 1
#define  DBOP_SERVICE_NAME          "192.168.91.98:1521/orcl"
#define  DBOP_USER_NAME             "cgw_gapc"
#define  DBOP_PASSWORD              "kaile"
#else
#define  DBOP_SERVICE_NAME          "192.168.41.200:1521/orcl"
#define  DBOP_USER_NAME             "cgw"
#define  DBOP_PASSWORD              "changxin"
#endif

static int OciLibTst()
{
    clock_t start = 0;
    clock_t end = 0;
    OCI_Connection *cn = NULL;
    int ret = DBOP_FAILURE;

    do {
        start=clock();
        if (!OCI_Initialize(NULL,
                            DBOP_INSTANT_CLIENT_PATH,
                            OCI_ENV_DEFAULT|OCI_ENV_CONTEXT))
        {
            DBOP_LOG(DBOP_LOG_ERR, "OCI_Initialize return error!");
            ret = DBOP_FAILURE;
            break;
        }
        DBOP_LOG(DBOP_LOG_INFO, "OCI_Initialize successfully!");

        cn = OCI_ConnectionCreate(DBOP_SERVICE_NAME,
                                  DBOP_USER_NAME,
                                  DBOP_PASSWORD,
                                  OCI_SESSION_DEFAULT);

        OCI_Statement * st = OCI_StatementCreate(cn);

        OCI_ExecuteStmt(st, "select * from TB_DRUG");

        OCI_Resultset *rs = OCI_GetResultset(st);

        int rowcount = OCI_GetRowCount(rs);
        DBOP_LOG(DBOP_LOG_INFO, "OCI_GetRowCount:%d!", rowcount);

        int columCnt = OCI_GetColumnCount(rs);

        DBOP_LOG(DBOP_LOG_INFO, "OCI_GetColumnCount:%d!", columCnt);

        OCI_Column* columnPtr = NULL;
        const otext* columnName = NULL;
        unsigned int columnType = 0;

        for (int j=1; j<=columCnt; ++j)
        {
            columnPtr = OCI_GetColumn(rs, j);
            columnType = OCI_ColumnGetType(columnPtr);
            columnName = OCI_ColumnGetName(columnPtr);

            DBOP_LOG(DBOP_LOG_INFO, "Column No:%3d, column type:%2d, column name:%s", j, columnType, columnName);
        }

        for (int i=0; OCI_FetchNext(rs); i++)
        {
            for (int j=1; j<columCnt; ++j)
            {
                DBOP_LOG(DBOP_LOG_INFO, "Row: %d, column:%d, data:%s!", i+1, j, OCI_GetString(rs, j));
            }
        }
        rowcount = OCI_GetRowCount(rs);
        DBOP_LOG(DBOP_LOG_INFO, "OCI_GetRowCount:%d!", rowcount);

        OCI_Break(cn);
        OCI_Cleanup();
    } while (0);

    return ret;
}


static int PluginForOracleTst()
{
    int ret = DBOP_FAILURE;
    int columnType = -1;
    int columnCnt = -1;
    char* str = NULL;
    int rowCnt = 0;

    DBOP_LOG(DBOP_LOG_INFO, "Supported database Count:%d", DbmGetSupportedDbCount());
    for (int i=0; i<DbmGetSupportedDbCount(); ++i)
    {
        DBOP_LOG(DBOP_LOG_INFO, "Supported database name(index:%d):%s", i, DbmGetSupportedDbName(i));
    }

    const DbHandlerSt* handlePtr = DbmInit(DBOP_NAME_ORACLE);
    DBOP_CHECK(NULL != handlePtr);

    ret = handlePtr->Open(DBOP_SERVICE_NAME, DBOP_USER_NAME, DBOP_PASSWORD);
    DBOP_CHECK(DBOP_SUCCESS == ret);

    ret = handlePtr->Execute("select * from TB_DRUG");
    DBOP_CHECK(DBOP_SUCCESS == ret);

    columnCnt = handlePtr->GetColumnCount();
    DBOP_LOG(DBOP_LOG_INFO, "OCI_GetColumnCount:%d!", columnCnt);

    while(handlePtr->FetchNext())
    {
        ++rowCnt;

        for (int j=1; j<=columnCnt; ++j)
        {
            columnType = handlePtr->GetColumnType(j);
            str = handlePtr->GetString(j);
            DBOP_LOG(DBOP_LOG_INFO, "Column No:%3d, column type:%2d, data:%s", j, columnType, str);
        }
    }

    DBOP_LOG(DBOP_LOG_INFO, "Row count:%d", rowCnt);

    int buf_size = 20240;
    char* buf_ptr = malloc(buf_size);
    int tableCnt = handlePtr->GetAllTableName(buf_ptr, buf_size);
    DBOP_LOG(DBOP_LOG_INFO, "Table count:%d, table list:%s", tableCnt, buf_ptr);
    if (NULL != buf_ptr)
    {
        free(buf_ptr);
    }
    ret = handlePtr->Close();

    return ret;
}

static int SearchDatabaseTst()
{
    int ret = DBOP_FAILURE;

    const DbHandlerSt* handlePtr = DbmInit(DBOP_NAME_ORACLE);
    DBOP_CHECK(NULL != handlePtr);

    ret = handlePtr->Open(DBOP_SERVICE_NAME, DBOP_USER_NAME, DBOP_PASSWORD);
    DBOP_CHECK(DBOP_SUCCESS == ret);

    ret = DbmDatabaseSearch(handlePtr,
            DBOP_CDT_TEXT,
            "NAME",
            DBOP_SEARCH_MODE_IGNORE_CASE,
            "C:/Users/yucheng/Desktop/SearchResult.txt");
    ret = handlePtr->Close();

    return ret;
}

/*

int DbcDefaultDriverGet(const char* valueBuf, int valueBufLen);
int DbcDefaultInstanceNameGet(const char* valueBuf, int valueBufLen);
int DbcDefaultDbUserNameGet(const char* valueBuf, int valueBufLen);
int DbcDefaultDbIpGet(const char* valueBuf, int valueBufLen);
int DbcDefaultDbPortGet(const char* valueBuf, int valueBufLen);
 */
static int DbConfigTst(void)
{
    int ret = DBOP_FAILURE;
#if 0
    char buf[80];
    //getcwd(buf, sizeof(buf));
    //GetModuleFileName(NULL, buf, sizeof(buf));
    GetCurrentDirectory(sizeof(buf), buf);
    DBOP_LOG(DBOP_LOG_INFO, "current working directory: %s", buf);
    system("pause");
#endif

#if 0
    ret = DbcDefaultDriverSet("OCI");
    DBOP_CHECK(DBOP_SUCCESS == ret);

    ret = DbcDefaultInstanceNameSet("orcl");
    DBOP_CHECK(DBOP_SUCCESS == ret);

    ret = DbcDefaultDbUserNameSet("cgw");
    DBOP_CHECK(DBOP_SUCCESS == ret);

    ret = DbcDefaultDbIpSet("192.168.41.200");
    DBOP_CHECK(DBOP_SUCCESS == ret);

    ret = DbcDefaultDbPortSet("1521");
    DBOP_CHECK(DBOP_SUCCESS == ret);
#else
    int len = DBC_KEY_LEN_MAX;
    char value[DBC_KEY_LEN_MAX];

    ret =  DbcDefaultDriverGet(value, len);
    DBOP_LOG(DBOP_LOG_INFO, "DefaultDriver:%s", value);
    DBOP_CHECK(DBOP_SUCCESS == ret);

    ret =  DbcDefaultInstanceNameGet(value, len);
    DBOP_LOG(DBOP_LOG_INFO, "DefaultDbName:%s", value);
    DBOP_CHECK(DBOP_SUCCESS == ret);

    ret =  DbcDefaultDbUserNameGet(value, len);
    DBOP_LOG(DBOP_LOG_INFO, "DefaultDbUserName:%s", value);
    DBOP_CHECK(DBOP_SUCCESS == ret);

    ret =  DbcDefaultDbIpGet(value, len);
    DBOP_LOG(DBOP_LOG_INFO, "DefaultDbIp:%s", value);
    DBOP_CHECK(DBOP_SUCCESS == ret);

    ret =  DbcDefaultDbPortGet(value, len);
    DBOP_LOG(DBOP_LOG_INFO, "DefaultDbPort:%s", value);
    DBOP_CHECK(DBOP_SUCCESS == ret);
#endif
    BOOL isCaseSensitive = FALSE;
    DbcDefaultSearchModeGet(&isCaseSensitive);
    DBOP_LOG(DBOP_LOG_INFO, "DB search isCaseSensitive:%s", isCaseSensitive?"true":"false");

    DBOP_LOG(DBOP_LOG_INFO, "DB search set to ignore case");
    DbcDefaultSearchModeSet(FALSE);
    isCaseSensitive = TRUE;
    DbcDefaultSearchModeGet(&isCaseSensitive);
    DBOP_LOG(DBOP_LOG_INFO, "DB search isCaseSensitive:%s", isCaseSensitive?"true":"false");

    DBOP_LOG(DBOP_LOG_INFO, "DB search set to CaseSensitive");
    DbcDefaultSearchModeSet(TRUE);
    isCaseSensitive = FALSE;
    DbcDefaultSearchModeGet(&isCaseSensitive);
    DBOP_LOG(DBOP_LOG_INFO, "DB search isCaseSensitive:%s", isCaseSensitive?"true":"false");

    char libPathBuf[DBC_KEY_LEN_MAX*4];
    len = sizeof(libPathBuf);
    ret = DbCfgOciLibPathGet(libPathBuf, len);
    DBOP_CHECK(DBOP_SUCCESS == ret);
    DBOP_LOG(DBOP_LOG_INFO, "Db Cfg Oci Lib Path:%s", libPathBuf);

    return ret;
}



int DbConnectTest()
{
    int ret = DBOP_FAILURE;

    //ret = OciLibTst();
    //ret = PluginForOracleTst();
    //ret = SearchDatabaseTst();
    ret = DbConfigTst();

    DBOP_CHECK(DBOP_SUCCESS == ret);

    return ret;
}

#include "plg_oracle.h"
#include "public.h"
#include "db_manager.h"
#include "db_config.h"
#include <string.h>

static OCI_Connection* oracle_cn = NULL;
static OCI_Statement*  oracle_st = NULL;
static OCI_Resultset*  oracle_rs = NULL;

static BOOL oracleConnectIsOpen  = FALSE;

#define SQL_GET_ALL_ORACLE_TABLES   "SELECT table_name FROM user_tables"



int OracleOpen(const char* strServiceName,
           const char* strUserName,
           const char* strPassword)
{
    int ret = DBOP_SUCCESS;

    do {
        if (TRUE == oracleConnectIsOpen)
        {
            DBOP_LOG(DBOP_LOG_ERR, "OCI is open already, no need to open again!");
            ret = DBOP_SUCCESS;
            break;
        }

        //Get OCI lib path
        char libPathBuf[DBC_KEY_LEN_MAX*4];
        int len = sizeof(libPathBuf);
        ret = DbCfgOciLibPathGet(libPathBuf, len);
        DBOP_CHECK(DBOP_SUCCESS == ret);

        //Skip the first ", and override the last " with '\0'
        char* tmpStr = libPathBuf + 1;
        tmpStr = strstr(tmpStr, "\"");
        tmpStr[0] = '\0';

        if (!OCI_Initialize(NULL,
                            libPathBuf + 1,
                            OCI_ENV_DEFAULT|OCI_ENV_CONTEXT))
        {
            DBOP_LOG(DBOP_LOG_ERR, "OCI_Initialize return error!");
            ret = DBOP_FAILURE;
            break;
        }

        DBOP_LOG(DBOP_LOG_INFO, "OCI_Initialize successfully!");

        oracle_cn = OCI_ConnectionCreate(strServiceName,
                strUserName,
                strPassword,
                OCI_SESSION_DEFAULT);
        if(oracle_cn != NULL)
        {
            oracle_st = OCI_StatementCreate(oracle_cn);
        }

        //DBOP_CHECK(NULL != oracle_cn);
        //DBOP_CHECK(NULL != oracle_st);

        if ((oracle_cn==NULL) || (oracle_st==NULL))
        {
            DBOP_LOG(DBOP_LOG_ERR, "oracle_st:%x, oracle_cn:%x", oracle_cn, oracle_st);
            ret = DBOP_FAILURE;
            break;
        }

        oracleConnectIsOpen = TRUE;
        DBOP_LOG(DBOP_LOG_INFO, "OCI connection opened successfully!");

    } while(0);

    return ret;
}

int OracleClose(void)
{
    int ret = DBOP_FAILURE;

    do {
        if (FALSE == oracleConnectIsOpen)
        {
            DBOP_LOG(DBOP_LOG_ERR, "OCI is closed already, no need to close again!");
            ret = DBOP_SUCCESS;
            break;
        }

        if (FALSE == OCI_Break(oracle_cn))
        {
            break;
        }

        OCI_Cleanup();
        oracle_cn = NULL;
        oracle_st = NULL;
        oracle_rs = NULL;
        oracleConnectIsOpen = FALSE;
        ret = DBOP_SUCCESS;

        DBOP_LOG(DBOP_LOG_INFO, "OCI connection closed successfully!");
    } while (0);

    return ret;
}

int OracleSqlExecute(char* strSql)
{
    int ret = DBOP_FAILURE;

    DBOP_CHECK(TRUE == oracleConnectIsOpen);

    if (TRUE == OCI_ExecuteStmt(oracle_st, strSql))
    {
        oracle_rs = OCI_GetResultset(oracle_st);
        if (NULL != oracle_rs)
        {
            ret = DBOP_SUCCESS;
        }
    }

    return ret;
}

int OracleGetColumnType(unsigned int columnIndex)
{
    unsigned int type = -1;
    DBOP_CHECK(TRUE == oracleConnectIsOpen);

    OCI_Column* columnPtr = OCI_GetColumn(oracle_rs, columnIndex);
    DBOP_CHECK(NULL != columnPtr);

    type = OCI_ColumnGetType(columnPtr);
    DBOP_CHECK((type>=OCI_CDT_NUMERIC) && (type<=OCI_CDT_BOOLEAN));

    return (1 << type);
}

const char* OracleGetColumnName(unsigned int columnIndex)
{
    const char* columnName = NULL;
    DBOP_CHECK(TRUE == oracleConnectIsOpen);

    OCI_Column* columnPtr = OCI_GetColumn(oracle_rs, columnIndex);
    DBOP_CHECK(NULL != columnPtr);

    columnName = OCI_ColumnGetName(columnPtr);
    DBOP_CHECK(NULL != columnName);

    return columnName;
}

int OracleGetColumnCount(void)
{
    return (int)OCI_GetColumnCount(oracle_rs);
}

BOOL OracleFetchNext(void)
{
    DBOP_CHECK(TRUE == oracleConnectIsOpen);

    return OCI_FetchNext(oracle_rs);
}

const char* OracleGetString(unsigned int columnIndex)
{
    DBOP_CHECK(TRUE == oracleConnectIsOpen);

    return (const char* )OCI_GetString(oracle_rs, columnIndex);
}

int OracleGetAllTableName(char* buf_ptr, int buf_size)
{
    DBOP_CHECK(TRUE == oracleConnectIsOpen);
    DBOP_CHECK(NULL != buf_ptr);
    DBOP_CHECK(buf_size > 0);

    int tableCnt = 0;
    int len = 0;
    int ret = DBOP_FAILURE;

    OCI_Resultset* res = NULL;

    do {
        if (FALSE == OCI_ExecuteStmt(oracle_st, SQL_GET_ALL_ORACLE_TABLES))
        {
            break;
        }

        res = OCI_GetResultset(oracle_st);
        if (NULL == res)
        {
            break;
        }

        while (OCI_FetchNext(res))
        {
            if ((buf_size - len - 1) <= 0)
            {
                //No free buffer, exit
                break;
            }

            len += snprintf(buf_ptr + len,
                            buf_size - len - 1,
                            "%s%c",
                            OCI_GetString(res, 1),
                            DBOP_TABALE_SEPARATOR);
            ++tableCnt;
        }
        ret = DBOP_SUCCESS;

    } while (0); //End of do {} while (0)

    return ((DBOP_SUCCESS == ret) ? tableCnt : -1);
}


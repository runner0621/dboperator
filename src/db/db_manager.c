#include "db_manager.h"
#include "public.h"
#include "plg_oracle.h"
#include "utilities.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define    DBM_TABLES_BUF_SIZE             (20*1024)

#define    DBM_TABLE_WRITE_BUF_SIZE        (5*1024)

#define    DBM_SQL_CMD_BUF_SIZE            100

#define    DBM_TABLE_COLUMN_PRINT_LEN      "40"

#define    DBM_CLOD_DATA_IND               "-- CLOB data, don't display --"

typedef char* (* FunTypeStrSearch) (const char * str1, const char * str2);

static const DbInfoSt dbInfo[] = {
    //Added plugin for oracle begin
    {
        .dbName = DBOP_NAME_ORACLE,

        .handler =
            {
                .Open             = OracleOpen,
                .Close            = OracleClose,
                .Execute          = OracleSqlExecute,
                .GetColumnType    = OracleGetColumnType,
                .GetColumnName    = OracleGetColumnName,
                .GetColumnCount   = OracleGetColumnCount,
                .FetchNext        = OracleFetchNext,
                .GetString        = OracleGetString,
                .GetAllTableName  = OracleGetAllTableName,
            },
    },
    //Added plugin for oracle end

};

int DbmGetSupportedDbCount()
{
    return sizeof(dbInfo) / sizeof(DbInfoSt);
}

const char* DbmGetSupportedDbName(int handler_index)
{
    const char* dbName = NULL;
    BOOL index_is_valid = HANDLER_INDEX_IS_VALID(handler_index);

    DBOP_CHECK(index_is_valid);

    if(index_is_valid)
    {
        dbName = dbInfo[handler_index].dbName;
    }
    return dbName;
}

const DbHandlerSt* DbmInit(const char* dbName)
{
    const DbHandlerSt* handlerPtr = NULL;
    int dbCount = DbmGetSupportedDbCount();

    for (int i=0; i<dbCount; ++i)
    {
        if((dbName != NULL) &&
                (strcmp(dbName, DbmGetSupportedDbName(i)) == 0))
        {
            handlerPtr = &(dbInfo[i].handler);
            break;
        }
    }

    DBOP_CHECK(NULL != handlerPtr);

    return handlerPtr;
}



/*
    If there are more than one table within table list, return the pointer of the second table,
    otherwise return NULL.
 */

static char* GetNextDbTable(char* tableListStr)
{
    int i = 0;
    char* nextTableStr = NULL;
    if (NULL == tableListStr)
    {
        return NULL;
    }

    for (; '\0' != tableListStr[i]; ++i)
    {
        if (DBOP_TABALE_SEPARATOR == tableListStr[i])
        {
            break;
        }
    }

    if ('\0' != tableListStr[i])
    {
        tableListStr[i] = '\0';
        nextTableStr =  tableListStr + i +1;
    }

    return nextTableStr;
}

static void TableCurrentColumnOutput(const DbHandlerSt* handlePtr,
        const char* searchTableNameStr,
        BOOL printTableHead,
        FILE* fileHandler)
{
    int columnCnt = handlePtr->GetColumnCount();
    int i;
    int writeLen = 0;
    static char tabaleWriteBuf[DBM_TABLE_WRITE_BUF_SIZE+1];
    const char* tmpStr = NULL;

    if (TRUE == printTableHead)
    {
        writeLen += snprintf(tabaleWriteBuf + writeLen,
                DBM_TABLE_WRITE_BUF_SIZE - writeLen,
                "\n==================================================\n\t\t" \
                "Table name:%s\n==================================================\n",
                searchTableNameStr);

        for (i=1; i<=columnCnt; ++i)
        {
            writeLen += snprintf(tabaleWriteBuf + writeLen,
                    DBM_TABLE_WRITE_BUF_SIZE - writeLen,
                    "%-" DBM_TABLE_COLUMN_PRINT_LEN "s",
                    handlePtr->GetColumnName(i));

            //DBOP_LOG(DBOP_LOG_INFO, "%-" DBM_TABLE_COLUMN_PRINT_LEN "s", handlePtr->GetColumnName(i));
        }
        writeLen += snprintf(tabaleWriteBuf + writeLen, DBM_TABLE_WRITE_BUF_SIZE - writeLen, "\n");
    }

    for (i=1; i<=columnCnt; ++i)
    {
        tmpStr = (DBOP_CDT_LOB == handlePtr->GetColumnType(i)) ? DBM_CLOD_DATA_IND : handlePtr->GetString(i);
        writeLen += snprintf(tabaleWriteBuf + writeLen,
                DBM_TABLE_WRITE_BUF_SIZE - writeLen,
                "%-" DBM_TABLE_COLUMN_PRINT_LEN "s",
                tmpStr);
        //DBOP_LOG(DBOP_LOG_INFO, "%-" DBM_TABLE_COLUMN_PRINT_LEN "s", handlePtr->GetString(i));
    }
    writeLen += snprintf(tabaleWriteBuf + writeLen, DBM_TABLE_WRITE_BUF_SIZE - writeLen, "\n");

    //check the buf size if big enough to store tabale content.
#if defined(DBOP_DEBUG_ENABLE)
    DBOP_CHECK(writeLen < (DBM_TABLE_WRITE_BUF_SIZE));
#endif

    fwrite(tabaleWriteBuf, sizeof(char), writeLen, fileHandler);
}

static int DbmTableSearch(const DbHandlerSt* handlePtr,
        int dataType,
        const char* searchStr,
        const char* searchTableNameStr,
        int searchMode,
        FILE* fileHandler)
{
    int ret = DBOP_FAILURE;
    int columnCnt = 0;
    int columnType = DBOP_CDT_INVALID;
    char sqlCmdBuf[DBM_SQL_CMD_BUF_SIZE] = {0};
    BOOL printTableHead = TRUE;
    const char* dataStr = NULL;

    do {
        snprintf(sqlCmdBuf, DBM_SQL_CMD_BUF_SIZE - 1, "select * from %s", searchTableNameStr);
        //DBOP_LOG(DBOP_LOG_INFO, "sqlCmdBuf: %s", sqlCmdBuf);

        FunTypeStrSearch StrSearchFunPtr = NULL;
        if (DBOP_SEARCH_MODE_CASE_SENSITIVE == searchMode)
        {
            StrSearchFunPtr = strstr;
            DBOP_LOG(DBOP_LOG_INFO, "CASE_SENSITIVE, test:%d", StrSearchFunPtr("ghjj8890", "333"));
        } else {
            StrSearchFunPtr = UtilStristr;

        }

        ret = handlePtr->Execute(sqlCmdBuf);
        DBOP_CHECK(DBOP_SUCCESS == ret);

        columnCnt = handlePtr->GetColumnCount();
        while(handlePtr->FetchNext())
        {
            for (int j=1; j<=columnCnt; ++j)
            {
                columnType = handlePtr->GetColumnType(j);
                if (0 == (dataType & columnType))
                {
                    continue;
                }
                dataStr = handlePtr->GetString(j);
                //DBOP_LOG(DBOP_LOG_INFO, "Column No:%3d, column type:%2d, data:%s", j, columnType, dataStr);

                if (NULL == dataStr)
                {
                    continue;
                }

                if (StrSearchFunPtr(dataStr, searchStr) <= 0)
                {
                    continue;
                }

                TableCurrentColumnOutput(handlePtr, searchTableNameStr, printTableHead, fileHandler);
                printTableHead = FALSE;
            } //End of for (int j=1; j<=columnCnt; ++j)
        } //End of while(handlePtr->FetchNext())

        ret = DBOP_SUCCESS;
    } while (0); //End of do {} while (0)

    return DBOP_SUCCESS;
}

int DbmDatabaseSearch(const DbHandlerSt* handlePtr,
        int dataType,
        char* searchStr,
        int searchMode,
        const char* searchResultSavePath)
{
    char* tablesBufPtr = NULL;
    char* tableStr = NULL;
    char* tablelistNew = NULL;
    int tableCnt = 0;
    int ret = DBOP_FAILURE;
    FILE* fileHandler = NULL;

    DBOP_CHECK(NULL != handlePtr);
    DBOP_CHECK(NULL != searchStr);
    DBOP_CHECK(NULL != searchResultSavePath);

    do {
        if (NULL != searchResultSavePath)
        {
            fileHandler = fopen(searchResultSavePath, "w");
        }

        tablesBufPtr = malloc(DBM_TABLES_BUF_SIZE);
        //memset(tablesBufPtr, 0, DBM_TABLES_BUF_SIZE);

        tableCnt = handlePtr->GetAllTableName(tablesBufPtr, DBM_TABLES_BUF_SIZE);
#if defined(DBOP_DEBUG_ENABLE)
        //TODO
        //T_D284  11,159,660 rows
        strcpy(tablesBufPtr, "CHC_JG_GJZWKL,CHC_JG_GLBSL,CHC_JG_ID_CFL,CHC_JG_WYZD_CFL,CHC_JG_ZDCD,TB_DRUG,");
        //strcpy(tablesBufPtr, "CHC_JG_GJZWKL,CHC_JG_GLBSL,");
#endif
        DBOP_LOG(DBOP_LOG_INFO, "Table count:%d, table list:%s", tableCnt, tablesBufPtr);

        tablelistNew = GetNextDbTable(tablesBufPtr);
        tableStr = tablesBufPtr;
        while ((NULL != tableStr) && (strlen(tableStr) > 0))
        {
            ret  = DbmTableSearch(handlePtr,
                    dataType,
                    searchStr,
                    tableStr,
                    searchMode,
                    fileHandler);
            //Move to the next new table.
            tableStr = tablelistNew;
            tablelistNew = GetNextDbTable(tablelistNew);
        } // End of while ((NULL != tableStr) && (strlen(tableStr) >= 0))

        ret = DBOP_SUCCESS;
    } while (0); //End of do {} while (0)

    if (NULL != tablesBufPtr)
    {
        free(tablesBufPtr);
    }

    if (NULL != fileHandler)
    {
        fclose(fileHandler);
        fileHandler = NULL;
    }

    return ret;
}


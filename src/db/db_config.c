#include "db_manager.h"
#include "public.h"
#include "db_config.h"
#include "utilities.h"
#include <string.h>

#define    DBC_CONFIG_NAME                    "config.ini"

#define    DBC_KEY_DB_DRIVER                  "DefaultDatabaseDriver"
#define    DBC_KEY_DB_INSTANCE_NAME           "DefaultDatabaseInstanceName"
#define    DBC_KEY_DB_USER_NAME               "DefaultDatabaseUserName"
#define    DBC_KEY_DB_IP                      "DefaultDatabaseIP"
#define    DBC_KEY_DB_PORT                    "DefaultDatabasePort"
#define    DBC_KEY_SEARCH_CASE_SENSITIVE      "DefaultSearchCaseSensitive"
#define    DBC_KEY_OCI_LIB_PATH               "DbCfgOciLibPath"


#define    DBC_INIT_CHECK()                   \
do {                                          \
    if (TRUE == db_config_need_init) {        \
        LoadDefaultConfig();                  \
    }                                         \
} while (0)


static BOOL db_config_need_init = TRUE;

static char default_db_driver_name[DBC_KEY_LEN_MAX]        = {0};
static char default_db_name[DBC_KEY_LEN_MAX]               = {0};
static char default_db_user_name[DBC_KEY_LEN_MAX]          = {0};
static char default_db_ip[DBC_KEY_LEN_MAX]                 = {0};
static char default_db_port[DBC_KEY_LEN_MAX]               = {0};
static char default_search_case_sensitive[DBC_KEY_LEN_MAX] = {0};
static char db_cfg_oci_lib_path[DBC_KEY_LEN_MAX*4]         = {0};


static char *substr(char *linebuf,char *pKey);
static int DbcGetCfItem(const char *pFileName, char *pKey, char * pValue, int * pValueLen );
static int DbcSetCfItem(const char *pFileName, char *pKey, char *pValue);


int LoadDefaultConfig(void)
{
    int ret = DBOP_SUCCESS;
    int len = DBC_KEY_LEN_MAX;

    do {
        // TODO:The best choice: If config.ini is modified after the last time loading default config,
        // then load the default config again.
        if (FALSE == db_config_need_init)
        {
            break;
        }

        ret = DbcGetCfItem(DBC_CONFIG_NAME, DBC_KEY_DB_DRIVER, default_db_driver_name, &len);
        DBOP_CHECK(DBOP_SUCCESS == ret);

        ret = DbcGetCfItem(DBC_CONFIG_NAME, DBC_KEY_DB_INSTANCE_NAME, default_db_name, &len);
        DBOP_CHECK(DBOP_SUCCESS == ret);

        ret = DbcGetCfItem(DBC_CONFIG_NAME, DBC_KEY_DB_USER_NAME, default_db_user_name, &len);
        DBOP_CHECK(DBOP_SUCCESS == ret);

        ret = DbcGetCfItem(DBC_CONFIG_NAME, DBC_KEY_DB_IP, default_db_ip, &len);
        DBOP_CHECK(DBOP_SUCCESS == ret);

        ret = DbcGetCfItem(DBC_CONFIG_NAME, DBC_KEY_DB_PORT, default_db_port, &len);
        DBOP_CHECK(DBOP_SUCCESS == ret);

        ret = DbcGetCfItem(DBC_CONFIG_NAME, DBC_KEY_SEARCH_CASE_SENSITIVE, default_search_case_sensitive, &len);
        //DBOP_CHECK(DBOP_SUCCESS == ret);

        len = sizeof(db_cfg_oci_lib_path);
        ret = DbcGetCfItem(DBC_CONFIG_NAME, DBC_KEY_OCI_LIB_PATH, db_cfg_oci_lib_path, &len);
        DBOP_CHECK(DBOP_SUCCESS == ret);

        db_config_need_init = FALSE;
    } while (0);

    return ret;
}

int DbcDefaultDriverSet(const char* value)
{
    DBOP_CHECK(NULL != value);
    strncpy(default_db_driver_name, value, DBC_KEY_LEN_MAX);
    default_db_driver_name[DBC_KEY_LEN_MAX - 1] = '\0';

    int ret = DbcSetCfItem(DBC_CONFIG_NAME, DBC_KEY_DB_DRIVER, default_db_driver_name);
    return ret;
}

int DbcDefaultInstanceNameSet(const char* value)
{
    DBOP_CHECK(NULL != value);
    strncpy(default_db_name, value, DBC_KEY_LEN_MAX);
    default_db_name[DBC_KEY_LEN_MAX - 1] = '\0';

    int ret = DbcSetCfItem(DBC_CONFIG_NAME, DBC_KEY_DB_INSTANCE_NAME, default_db_name);
    return ret;
}

int DbcDefaultDbUserNameSet(const char* value)
{
    DBOP_CHECK(NULL != value);
    strncpy(default_db_user_name, value, DBC_KEY_LEN_MAX);
    default_db_user_name[DBC_KEY_LEN_MAX - 1] = '\0';

    int ret = DbcSetCfItem(DBC_CONFIG_NAME, DBC_KEY_DB_USER_NAME, default_db_user_name);
    return ret;
}

int DbcDefaultDbIpSet(const char* value)
{
    DBOP_CHECK(NULL != value);
    strncpy(default_db_ip, value, DBC_KEY_LEN_MAX);
    default_db_ip[DBC_KEY_LEN_MAX - 1] = '\0';

    int ret = DbcSetCfItem(DBC_CONFIG_NAME, DBC_KEY_DB_IP, default_db_ip);
    return ret;
}

int DbcDefaultDbPortSet(const char* value)
{
    DBOP_CHECK(NULL != value);
    strncpy(default_db_port, value, DBC_KEY_LEN_MAX);
    default_db_port[DBC_KEY_LEN_MAX - 1] = '\0';

    int ret = DbcSetCfItem(DBC_CONFIG_NAME, DBC_KEY_DB_PORT, default_db_port);
    return ret;
}

int DbcDefaultSearchModeSet(BOOL isCaseSensitive)
{
    strncpy(default_search_case_sensitive, (TRUE==isCaseSensitive)?"true":"false", DBC_KEY_LEN_MAX);

    int ret = DbcSetCfItem(DBC_CONFIG_NAME, DBC_KEY_SEARCH_CASE_SENSITIVE, default_search_case_sensitive);
    return DBOP_SUCCESS;
}

int DbCfgOciLibPathSet(const char* value)
{
    DBOP_CHECK(NULL != value);
    int size = sizeof(db_cfg_oci_lib_path);
    strncpy(db_cfg_oci_lib_path, value, size);
    db_cfg_oci_lib_path[size - 1] = '\0';

    int ret = DbcSetCfItem(DBC_CONFIG_NAME, DBC_KEY_OCI_LIB_PATH, db_cfg_oci_lib_path);
    return ret;
}

int DbcDefaultDriverGet(const char* valueBuf, int valueBufLen)
{
    DBC_INIT_CHECK();
    int ret = snprintf(valueBuf, valueBufLen, "%s", default_db_driver_name);

    return (ret>0) ? DBOP_SUCCESS : DBOP_FAILURE;
}

int DbcDefaultInstanceNameGet(const char* valueBuf, int valueBufLen)
{
    DBC_INIT_CHECK();
    int ret = snprintf(valueBuf, valueBufLen, "%s", default_db_name);

    return (ret>0) ? DBOP_SUCCESS : DBOP_FAILURE;
}

int DbcDefaultDbUserNameGet(const char* valueBuf, int valueBufLen)
{
    DBC_INIT_CHECK();
    int ret = snprintf(valueBuf, valueBufLen, "%s", default_db_user_name);

    return (ret>0) ? DBOP_SUCCESS : DBOP_FAILURE;
}

int DbcDefaultDbIpGet(const char* valueBuf, int valueBufLen)
{
    DBC_INIT_CHECK();
    int ret = snprintf(valueBuf, valueBufLen, "%s", default_db_ip);

    return (ret>0) ? DBOP_SUCCESS : DBOP_FAILURE;
}

int DbcDefaultDbPortGet(const char* valueBuf, int valueBufLen)
{
    DBC_INIT_CHECK();
    int ret = snprintf(valueBuf, valueBufLen, "%s", default_db_port);

    return (ret>0) ? DBOP_SUCCESS : DBOP_FAILURE;
}

int DbcDefaultSearchModeGet(BOOL* isCaseSensitive)
{
    DBC_INIT_CHECK();

    if (UtilStristr(default_search_case_sensitive, "false") > 0)
    {
        *isCaseSensitive = FALSE;
    } else {
        *isCaseSensitive = TRUE;
    }

    return DBOP_SUCCESS;
}

int DbCfgOciLibPathGet(const char* valueBuf, int valueBufLen)
{
    DBC_INIT_CHECK();
    int ret = snprintf(valueBuf, valueBufLen, "%s", db_cfg_oci_lib_path);

    return (ret>0) ? DBOP_SUCCESS : DBOP_FAILURE;
}

static char *substr(char *linebuf,char *pKey)
{
    char* pTmp = NULL;
    char* pRv = NULL;
    int lenKey = 0;
    int len = 0;

    do {
        if (linebuf==NULL || pKey==NULL)
        {
            DBOP_LOG(DBOP_LOG_ERR, "func parameter err: (str==NULL || substr==NULL)");
            break;
        }
        pRv = strstr(linebuf, pKey);
        if(pRv == NULL)
        {
            break;
        }

        pTmp = pRv;
        lenKey = strlen(pKey);
        while(*pTmp != '\0' && *pTmp != ' '&& *pTmp != '=' && *pTmp != '\n')
        {
            len++;
            pTmp++;
            if(len>lenKey)
            {
                break;
            }
        }
        if (lenKey != len)
        {
            pRv = NULL;
            break;
        }
    } while (0);

    return pRv;
}

/*
 * pFileName  -> in
 * pKey       -> in
 * pValue     -> in/out
 * pValueLen  -> out
 *
 * Read the file line by line
 * Parsing each row, if matching the keyword, parse the value.
 * Remove the space unless the space within the "xxxx yyyyy"
 */
static int DbcGetCfItem(const char *pFileName, char *pKey, char * pValue, int * pValueLen )
{
    int ret = DBOP_SUCCESS;
    FILE *fp = NULL;
    char linebuf[DBC_LINE_LEN_MAX];
    char *pTmp = NULL, *pBegin = NULL, *pEnd = NULL;

    do {
        if (pFileName==NULL||pKey==NULL||pValue==NULL||pValueLen==NULL)
        {
            ret = DBOP_FAILURE;
            DBOP_LOG(DBOP_LOG_ERR, "func GetCfItem paramter err!");
            break;
        }
        //open file in read only mode
        fp = fopen(pFileName,"r");
        if (fp==NULL)
        {
            ret = DBOP_FAILURE;
            DBOP_LOG(DBOP_LOG_ERR, "fopen file err %d (fp==NULL)",ret);
            break;
        }

        while(!feof(fp))
        {
            memset(linebuf,0,DBC_LINE_LEN_MAX);
            // Read one line at a time
            pTmp = fgets(linebuf, DBC_LINE_LEN_MAX, fp);
            if (pTmp==NULL)
            {
                break;
            }

            //If keyword don't within this line,
            //continue to parse the next line
            pTmp = substr(linebuf, pKey);
            if (pTmp==NULL)
            {
                continue;
            }

            // search '=', if '=' exists, the value is behind '='
            pTmp = strchr(linebuf, '=');
            if (pTmp==NULL)
            {
                continue;
            }
            //skip '=' to parse value
            pTmp=pTmp+1;

            //goto the start of value
            while (1)
            {
                if(*pTmp==' ')
                {
                    pTmp++;
                }
                else
                {
                    pBegin = pTmp;
                    if(*pBegin == '\n'||*pBegin=='\0')
                    {
                        ret = DBOP_FAILURE;
                        DBOP_LOG(DBOP_LOG_ERR, "The key %s don't have value",pKey);
                    }
                    break;
                }
            }

            BOOL isFilePathEnded = TRUE;
            //goto the end of value
            while (1)
            {
                if (*pTmp == '"')
                {
                    if (TRUE == isFilePathEnded)
                    {
                        //start to parse the file path, start with '"', end with '"'.
                        isFilePathEnded = FALSE;
                    } else {
                        //end to parse the file path.
                        isFilePathEnded = TRUE;
                    }
                }

                if(((*pTmp==' ') && isFilePathEnded)||*pTmp=='\n'||*pTmp=='\0')
                {
                    break;
                }
                else
                {
                    pTmp++;
                }
            }
            pEnd = pTmp;
            *pValueLen = pEnd-pBegin;

            memcpy(pValue,pBegin,*pValueLen);
            *(pValue+*pValueLen)='\0';
            break;
        }

        // don't find keywork
        if(pBegin==NULL)
        {
            DBOP_LOG(DBOP_LOG_ERR, "can't find the key:%s \n",pKey);
            ret = DBOP_FAILURE;
        }
    } while (0);

    if (fp!=NULL)
    {
        fclose(fp);
    }
    return ret;
}

/*
 * pFileName  -> in
 * pKey       -> in
 * pValue     -> in
 *
 * Read a line each time, check if the keyword exists.
 * If it exist, modify the value.
 * If it don't exist, insert new item(keyword=value) to the end of config file.
 */
static int DbcSetCfItem(const char *pFileName, char *pKey, char *pValue)
{
    int ret = DBOP_SUCCESS;
    int length = 0,iFlag = 0;
    FILE *fp = NULL;
    char filebuf[DBC_FILE_LEN_MAX] = {0};
    char linebuf[DBC_LINE_LEN_MAX];
    char *pTmp = NULL;

    do {
        if (pFileName==NULL||pKey==NULL||pValue==NULL)
        {
            ret = DBOP_FAILURE;
            DBOP_LOG(DBOP_LOG_ERR, "func WriteCfItem paramter err!");
            break;
        }
        fp = fopen(pFileName,"r+");
        if (fp==NULL)
        {
            ret = DBOP_FAILURE;
            DBOP_LOG(DBOP_LOG_ERR, "fopen file err:%d",ret);
            break;
        }
        fseek(fp, 0L, SEEK_END);
        //Get file length
        length = ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        if (length>DBC_FILE_LEN_MAX)
        {
            ret = DBOP_FAILURE;
            DBOP_LOG(DBOP_LOG_ERR, "file too long unspport:%d",ret);
            break;
        }
        while (!feof(fp))
        {
            //Read a line each time
            memset(linebuf, 0, sizeof(linebuf));
            pTmp = fgets(linebuf, DBC_LINE_LEN_MAX, fp);
            if(pTmp==NULL)
            {
                break;
            }

            pTmp = substr(linebuf, pKey);
            if(pTmp==NULL)
            {
                strcat(filebuf, linebuf);
                continue;
            }
            else
            {
                sprintf(linebuf,"%s=%s\n", pKey, pValue);
                strcat(filebuf, linebuf);
                //keyword exists
                iFlag = 1;
            }
        }
        //keyword don't exists, insert it.
        if (iFlag == 0)
        {
            fprintf(fp,"\n%s=%s", pKey, pValue);
        }
        //keyword exists
        else
        {
            if (fp!=NULL)
            {
                fclose(fp);
            }
            fp = fopen(pFileName,"w+");
            if (fp==NULL)
            {
                ret = DBOP_FAILURE;
                DBOP_LOG(DBOP_LOG_ERR, "fopen file err:%d",ret);
                break;
            }
            //write back to config file
            fputs(filebuf, fp);
        }
    } while (0);

    if (fp!=NULL)
    {
        fclose(fp);
    }
    return ret;
}

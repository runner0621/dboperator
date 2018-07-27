#if !defined(__DB_PLUGIN_ORACLE_H__)
#define __DB_PLUGIN_ORACLE_H__

#if defined (OCI_API)
#undef OCI_API
#endif
//Include header from ocilib
//Fix linked error
#define OCI_API __stdcall
#include <ocilib.h>

#if defined(__cplusplus)
extern "C" {
#endif

    int OracleOpen(const char* strServiceName,
               const char* strUserName,
               const char* strPassword);

    int OracleClose(void);

    int OracleSqlExecute(char* strSql);

    int OracleGetColumnType(unsigned int columnIndex);

    const char* OracleGetColumnName(unsigned int columnIndex);

    int OracleGetColumnCount(void);

    BOOL OracleFetchNext(void);

    const char* OracleGetString(unsigned int columnIndex);

    int OracleGetAllTableName(char* buf_ptr, int buf_size);

#if defined(__cplusplus)
}
#endif

#endif //end of #if !defined(__DB_PLUGIN_ORACLE_H__)

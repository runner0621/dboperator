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
    int OracleOpen(const char* strServiceName,
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
    int OracleClose(void);


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
    int OracleSqlExecute(char* strSql);

    /**
     * @brief
     * Get column type.
     *
     * @param columnIndex - column index, start with 1.
     *
     *  @return
     *  column type, refer to DBOP_CDT_xxx
     */
    int OracleGetColumnType(unsigned int columnIndex);

    /**
     * @brief
     * Get column name.
     *
     * @param columnIndex - column index, start with 1.
     *
     *  @return
     *  column name
     */
    const char* OracleGetColumnName(unsigned int columnIndex);

    /**
     * @brief
     * Get column count.
     *
     *  @return
     *  column count
     */
    int OracleGetColumnCount(void);

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
    BOOL OracleFetchNext(void);

    /**
     * @brief
     * Return the current string value of the column at the given index in the resultset.
     *
     * @param columnIndex - column index, start with 1.
     *
     *  @return
     *  string value of the column at the given index in the resultset
     */
    const char* OracleGetString(unsigned int columnIndex);

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
    int OracleGetAllTableName(char* buf_ptr, int buf_size);

#if defined(__cplusplus)
}
#endif

#endif //end of #if !defined(__DB_PLUGIN_ORACLE_H__)

#if !defined(__PUBLIC_H__)
#define __PUBLIC_H__

#include <assert.h>
#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif


//QT_QML_DEBUG will be define in debug version
#if defined (QT_QML_DEBUG)
//Enable debug
#define    DBOP_DEBUG_ENABLE
#endif

//Define return value by function
#define    DBOP_SUCCESS                0
#define    DBOP_FAILURE                -1

//Define basic data & type begin
#if !defined(NULL)
#define    NULL                        0
#endif

#if !defined(BOOL)
#define    BOOL                        int
#endif

#if !defined(FALSE)
#define    FALSE                       0
#endif

/*
 * Be careful with the usage of TRUE,
 * all non-zero value mean to TRUE.
 */
#if !defined(TRUE)
#define    TRUE                        1
#endif
//Define basic data & type end

//Define log level
#define    DBOP_LOG_ERR                0
#define    DBOP_LOG_INFO               1

//Definition for log print begin
#if defined(DBOP_DEBUG_ENABLE)
#define DBOP_LOG(level, format, ...)                                           \
    do {                                                                       \
        fprintf(stderr, "[%s][%s,%d] " format "\n",                            \
                (DBOP_LOG_ERR==(level))?"Error":"Info ",                       \
                __FILE__, __LINE__, ##__VA_ARGS__);                            \
    } while (0)

#else
#define DBOP_LOG(level, format, ...)                                           \
    do {                                                                       \
        fprintf(stderr, "[%s] " format "\n",                                   \
                (DBOP_LOG_ERR==(level))?"Error":"Info ",                       \
                ##__VA_ARGS__);                                                \
    } while (0)

#endif
//Definition for log print end

#define    DBOP_ASSERT(expression)     assert((expression))

//Definition for exception check begin
#if defined (DBOP_DEBUG_ENABLE)
#define    DBOP_CHECK(exp)                                                     \
    do {                                                                       \
        if (FALSE == (exp))                                                    \
        {                                                                      \
            DBOP_LOG(DBOP_LOG_ERR, "Check failed, invalid expression:" #exp);  \
            DBOP_ASSERT(exp);                                                  \
        }                                                                      \
    } while (0)

#else
#define    DBOP_CHECK(exp)                                                     \
do {                                                                           \
    if (FALSE == (exp))                                                        \
    {                                                                          \
        DBOP_LOG(DBOP_LOG_ERR, "Check failed, invalid expression:" #exp);      \
    }                                                                          \
} while (0)

#endif
//Definition for exception check end


#define    DBOP_CHECK_THEN_BREAK(exp)                                          \
    if (FALSE == (exp))                                                        \
    {                                                                          \
        DBOP_LOG(DBOP_LOG_ERR, "Check failed, invalid expression:" #exp);      \
        break;                                                                 \
    }



#if defined(__cplusplus)
}
#endif

#endif //end of #if !defined(__PUBLIC_H__)

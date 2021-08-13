/* 
 * File:   assert.h
 * Author: C16825
 *
 * Created on July 29, 2020, 4:39 PM
 */

#ifndef ASSERT_H
#define	ASSERT_H

#include "system/debug/sys_debug.h"

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef SYS_ASSERT
#define SYS_ASSERT(test,msg) \
    ((test) \
        ? (void)0 \
        : __assert_func( \
            __FILE__, \
            __LINE__, \
            __PRETTY_FUNCTION__, \
            #msg, \
            #test) \
    )
#endif //SYS_ASSERT

void user_assert_func( char * file, int line, const char * func, char * msg, char * expr); 

#ifdef	__cplusplus
}
#endif

#endif	/* ASSERT_H */


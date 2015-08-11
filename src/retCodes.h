#ifndef RETCODES_H
#define RETCODES_H

typedef enum memRetCodes {
    OK_MEM,			//!< No Error
    ERR_MEM_ALLOC,		//!< Problem reported by allocation routines
    ERR_MEM_NOSIZE,		//!< Needed size field missing/zero
    ERR_MEM_NULLPTR		//!< Required pointer had a NULL value
} memRetCodes_type;

typedef enum retCodes {
    OK = 0,			//!< No Error
    WARN,		     //!< Warning, non-fatal
    ERROR,		     //!< Error, non-fatal
    FATAL_ERROR		     //!< Fatal Error
} retCodes_type;
#endif				// RETCODES_H

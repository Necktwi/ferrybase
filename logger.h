/* 
 * File:   logger.h
 * Author: Gowtham
 *
 * Created on June 24, 2014, 2:25 PM
 */

#ifndef LOGGER_H
#define	LOGGER_H

/**
 * 
 */
enum _ff_log_type {
    FFL_ERR = 1 << 0,
    FFL_WARN = 1 << 1,
    FFL_NOTICE = 1 << 2,
    FFL_INFO = 1 << 3,
    FFL_DEBUG = 1 << 4,
    FFL_PARSER = 1 << 5,
    FFL_HEADER = 1 << 6,
    FFL_EXT = 1 << 7,
    FFL_CLIENT = 1 << 8,
    FFL_LATENCY = 1 << 9
};
extern int ff_log_type;
extern int ff_log_level;
int _ff_log(_ff_log_type t, const char* format, ...);
int _ff_log(_ff_log_type t, int l, const char* format, ...);
int _ff_log(_ff_log_type t, const char* func, const char* file_name, int line_no, const char* format, ...);

/* notice, warn and log are always compiled in */
#define ffl_notice(...) _ff_log(FFL_NOTICE, __VA_ARGS__)
#define ffl_warn(...) _ff_log(FFL_WARN, __VA_ARGS__)
#define ffl_err(...) _ff_log(FFL_ERR, __VA_ARGS__)

/*
 *  weaker logging can be deselected at configure time using --disable-debug
 *  that gets rid of the overhead of checking while keeping _warn and _err
 *  active
 */
#ifdef _DEBUG

#define ffl_info(...) _ff_log(FFL_INFO, __VA_ARGS__)
#define ffl_debug(level,...) ((level & ff_log_level)==level)?_ff_log(FFL_DEBUG, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__):0
#define ffl_parser(...) _ff_log(FFL_PARSER, __VA_ARGS__)
#define ffl_header(...)  _ff_log(FFL_HEADER, __VA_ARGS__)
#define ffl_ext(...)  _ff_log(FFL_EXT, __VA_ARGS__)
#define ffl_client(...) _ff_log(FFL_CLIENT, __VA_ARGS__)
#define ffl_latency(...) _ff_log(FFL_LATENCY, __VA_ARGS__)
//void log_hexdump(void *buf, size_t len);

#else /* no debug */

#define ffl_info(...)
#define ffl_debug(...)
#define ffl_parser(...)
#define ffl_header(...)
#define ffl_ext(...)
#define ffl_client(...)
#define ffl_latency(...)
//#define ffl_hexdump(a, b)

#endif



#endif	/* LOGGER_H */


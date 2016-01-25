#ifndef TRACE_H
#define TRACE_H

#include<stdio.h>

extern int isLogged;
#define TRACELOG(...)  track(__FUNCTION__,__LINE__,__VA_ARGS__)

#endif//TRACE_H

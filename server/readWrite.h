#ifndef _READWRITE_H_
#define _uREADWRITE_H_
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<errno.h>
#include <fstream>
#include <iostream>
#include <map>
int readn(int fd, void*buf, int n) ;
int writen(int fd, void* buf, int n) ;
void  getInfo(std::map<int,int>&ls) ;
#endif

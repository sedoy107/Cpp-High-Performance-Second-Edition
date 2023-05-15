#ifndef MY_LIB_H
#define MY_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

// Function prototype
void download_webpage(const char* url, void (*callback)(int, const char*));

#ifdef __cplusplus
}
#endif


#endif
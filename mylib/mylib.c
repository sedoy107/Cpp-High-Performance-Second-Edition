#include "mylib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void download_webpage(const char* url, void (*callback)(int, const char*)) {

    printf("Downloading webpage from %s\n", url);

    const char* response = "<html><body><h1>Webpage Content</h1></body></html>";

    callback(200, response);
}
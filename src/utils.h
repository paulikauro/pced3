#pragma once

#include <assert.h>
#include <stdlib.h>

_Noreturn void error_std(const char *message);

void *allocate(size_t size);

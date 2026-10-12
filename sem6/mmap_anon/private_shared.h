#pragma once

#include <stddef.h>

int
save_buf_to_file(char *buf, size_t size, char *file);

void
rand_buf(char *buf, size_t size);

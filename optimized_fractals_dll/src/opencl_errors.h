#pragma once
#include <CL/cl.h>

// Converts OpenCl error to error message
const char* cl_error_to_string(cl_int err);
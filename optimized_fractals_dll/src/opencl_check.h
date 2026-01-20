#pragma once
#include <stdexcept>
#include <sstream>
#include "opencl_errors.h"

#define CL_CHECK(expr)                                             \
    do {                                                           \
        cl_int _err = (expr);                                      \
        if (_err != CL_SUCCESS) {                                  \
            std::ostringstream _oss;                               \
            _oss << "OpenCL error: " << cl_error_to_string(_err)   \
                 << " (" << _err << ") at "                        \
                 << __FILE__ << ":" << __LINE__;                   \
            throw std::runtime_error(_oss.str());                  \
        }                                                          \
    } while (0)

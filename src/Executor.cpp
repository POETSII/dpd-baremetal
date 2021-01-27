// A class that executes something on a given volume

#include "Executor.hpp"

#ifndef _EXECUTOR_IMPL
#define _EXECUTOR_IMPL

    // Constructors and destructors
    template<class T>
    Executor<T>::Executor(const T volume_length, const unsigned cells_per_dimension) : volume(Volume<T>(volume_length, cells_per_dimension)) {
    }

#endif /*_EXECUTOR_IMPL */

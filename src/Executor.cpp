// A class that executes something on a given volume

#include "Executor.hpp"

#ifndef _EXECUTOR_IMPL
#define _EXECUTOR_IMPL

    // Constructors and destructors
    template<class T>
    Executor<T>::Executor(Volume<ptype> *volume) {
        this->volume = volume;
    };


#endif /*_EXECUTOR_IMPL */

// A class that executes something on a given volume

#ifndef _EXECUTOR_H
#define _EXECUTOR_H

#include "dpd.hpp"

template<class V>
class Executor {
    public:

    // Constructors and destructors
    Executor<V>(float volume_length, unsigned cells_per_dimension) : volume(volume_length, cells_per_dimension) { };

    ~Executor<V>() { };

    // The function which carries out the execution
    virtual void run() = 0;

    // Executes and returns a vector of that received result for testing purposes
    virtual void test(void *result) = 0;

    // Returns the volume
    V * get_volume() { return &volume; };

    protected:

    // The volume this executor works on
    V volume;

};

// #include "../src/Executor.cpp"

#endif /*_EXECUTOR_H */
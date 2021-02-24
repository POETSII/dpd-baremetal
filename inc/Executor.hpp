// A class that executes something on a given volume

#ifndef _EXECUTOR_H
#define _EXECUTOR_H

#include "Volume.hpp"
#include "dpd.hpp"

template<class T>
class Executor {
    public:

    // Constructors and destructors
    Executor<T>(T volume_length, unsigned cells_per_dimension);

    ~Executor<T>() { };

    // The function which carries out the execution
    virtual void run() = 0;

    // Executes and returns a vector of that received result for testing purposes
    virtual void test(void *result) = 0;

    // Returns the volume
    Volume<T> * get_volume();

    protected:

    // The volume this executor works on
    Volume<T> volume;

};

#include "../src/Executor.cpp"

#endif /*_EXECUTOR_H */

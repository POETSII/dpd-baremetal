// A class that executes something on a given volume

#ifndef _EXECUTOR_H
#define _EXECUTOR_H

#include "Volume.hpp"
#include "dpd.hpp"

class Executor {
    public:

    // Constructors and destructors
    Executor(Volume<ptype> *volume);

    ~Executor() {}

    // The function which carries out the execution
    virtual void run() = 0;

    // Executes and returns a vector of that received result for testing purposes
    virtual void test() = 0;

    protected:

    // The volume this executor works on
    Volume<ptype> *volume;

};

#endif /*_EXECUTOR_H */

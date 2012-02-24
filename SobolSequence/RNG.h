#if defined(_MSC_VER)
#pragma once
#endif

#ifndef PBRT_CORE_RNG_H
#define PBRT_CORE_RNG_H

// core/rng.h*
// Random Number Declarations
class RNG {
public: 
    RNG(unsigned __int32 seed = 5489UL) {
        mti = N+1; /* mti==N+1 means mt[N] is not initialized */
        Seed(seed);
    }

    void Seed(unsigned __int32 seed) const;
    float RandomFloat() const;
    unsigned long RandomUInt() const;

private:
    static const int N = 624;
    mutable unsigned long mt[N]; /* the array for the state vector  */
    mutable int mti;
};



#endif // PBRT_CORE_RNG_H

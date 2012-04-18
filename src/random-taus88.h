/* -*- Mode: C; c-basic-offset: 8 -*- */
/*
    A random number generator suitable for audio work. Originally from SC_RGen.h
    from SuperCollider, copyright (c) 2002 James McCartney.

    Adapted to C from C++ by Andy Wingo, copyright (c) 2004 Andy Wingo.

    Hash function from SuperCollider's Hash.h, same authors.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

//----------------------------------------------------------------------------//
// Ran088: L'Ecuyer's 1996 three-component Tausworthe generator "taus88"
//----------------------------------------------------------------------------//
// 
// Returns an integer random number uniformly distributed within [0,4294967295]
//
// The period length is approximately 2^88 (which is 3*10^26). 
// This generator is very fast and passes all standard statistical tests.
//
// Reference:
//   (1) P. L'Ecuyer, Maximally equidistributed combined Tausworthe generators,
//       Mathematics of Computation, 65, 203-213 (1996), see Figure 4.
//   (2) recommended in:
//       P. L'Ecuyer, Random number generation, chapter 4 of the
//       Handbook on Simulation, Ed. Jerry Banks, Wiley, 1997.
//
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// I chose this random number generator for the following reasons:
//		fast. 
//		easier and faster to seed than other high quality rng's such as Mersenne Twister.
//		the internal state is only 12 bytes.
//		the period is long enough for music/audio.
//		possible to code in altivec in future if needed.
// - James McCartney
//----------------------------------------------------------------------------//

#ifndef __RANDOM_TAUS88_H__
#define __RANDOM_TAUS88_H__

#include <endian.h>
#include <math.h>

static inline uint32_t hash_uint32 (uint32_t hash)
{
        // Thomas Wang's integer hash.
        // http://www.concentric.net/~Ttwang/tech/inthash.htm
        // a faster hash for integers. also very good.
        hash += ~(hash << 15);
        hash ^=   hash >> 10;
        hash +=   hash << 3;
        hash ^=   hash >> 6;
        hash += ~(hash << 11);
        hash ^=   hash >> 16;
        return hash;
}

typedef struct rt88_state
{
	uint32_t s1, s2, s3;		// random generator state
} rt88_state;

/* macros to put rgen state in registers */
#define GET_RANDOM(state) \
	uint32_t s1 = state.s1; \
	uint32_t s2 = state.s2; \
	uint32_t s3 = state.s3

#define PUT_RANDOM(state) \
	state.s1 = s1; \
	state.s2 = s2; \
	state.s3 = s3

static inline void rt88_init (rt88_state *state, uint32_t seed)
{	
        uint32_t s1, s2, s3;

	// humans tend to use small seeds - mess up the bits
	seed = hash_uint32 (seed);
	
	// initialize seeds using the given seed value taking care of
	// the requirements. The constants below are arbitrary otherwise
	s1 = 1243598713U ^ seed; if (s1 <  2) s1 = 1243598713U;
	s2 = 3093459404U ^ seed; if (s2 <  8) s2 = 3093459404U;
	s3 = 1821928721U ^ seed; if (s3 < 16) s3 = 1821928721U;	

        state->s1 = s1; state->s2 = s2; state->s3 = s3;
}

static inline uint32_t rt88_trand (rt88_state *state)
{
        uint32_t s1 = state->s1, s2 = state->s2, s3 = state->s3;
	// generate a random 32 bit number
	s1 = ((s1 &  -2) << 12) ^ (((s1 << 13) ^  s1) >> 19);
	s2 = ((s2 &  -8) <<  4) ^ (((s2 <<  2) ^  s2) >> 25);
	s3 = ((s3 & -16) << 17) ^ (((s3 <<  3) ^  s3) >> 11);
        state->s1 = s1; state->s2 = s2; state->s3 = s3;
	return s1 ^ s2 ^ s3;
}

static inline double rt88_drand (rt88_state *state)
{
	// return a double from 0.0 to 0.999...
#if BYTE_ORDER == BIG_ENDIAN
	union { struct { uint32_t hi, lo; } i; double f; } du;
#else
	union { struct { uint32_t lo, hi; } i; double f; } du;
#endif
	du.i.hi = 0x41300000; 
	du.i.lo = rt88_trand (state);
	return du.f - 1048576.;
}

static inline float rt88_frand (rt88_state *state)
{
	// return a float from 0.0 to 0.999...
	union { uint32_t i; float f; } u;		// union for floating point conversion of result
	u.i = 0x3F800000 | (rt88_trand(state) >> 9);
	return u.f - 1.f;
}

static inline float rt88_frand0 (rt88_state *state)
{
	// return a float from +1.0 to +1.999...
	union { uint32_t i; float f; } u;		// union for floating point conversion of result
	u.i = 0x3F800000 | (rt88_trand(state) >> 9);
	return u.f;
}

static inline float rt88_frand2 (rt88_state *state)
{
	// return a float from -1.0 to +0.999...
	union { uint32_t i; float f; } u;		// union for floating point conversion of result
	u.i = 0x40000000 | (rt88_trand (state) >> 9);
	return u.f - 3.f;
}

static inline float rt88_frand8 (rt88_state *state)
{
	// return a float from -1.0 to +0.999...
	union { uint32_t i; float f; } u;		// union for floating point conversion of result
	u.i = 0x3E800000 | (rt88_trand (state) >> 9);
	return u.f - 0.375f;
}

static inline float rt88_fcoin (rt88_state *state)
{
	// only return one of the two values -1.0 or +1.0
	union { uint32_t i; float f; } u;		// union for floating point conversion of result
	u.i = 0x3E000000 | (0x80000000 & rt88_trand (state));
	return u.f;
}

static inline int32_t rt88_irand (rt88_state *state, int32_t scale)
{
	// return an int from 0 to scale - 1
	return (int32_t)floor(scale * rt88_drand (state));
}

static inline int32_t rt88_irand2 (rt88_state *state, int32_t scale)
{
	// return a int from -scale to +scale
	return (int32_t)floor((2. * scale + 1.) * rt88_drand (state) - scale);
}

static inline int32_t rt88_ilinrand (rt88_state *state, int32_t scale)
{
	int32_t a, b;
        a = rt88_irand (state, scale);
        b = rt88_irand (state, scale);
	return a < b ? a : b; // min
}

static inline double rt88_linrand (rt88_state *state, double scale)
{
	double a, b;
        a = rt88_drand (state);
        b = rt88_drand (state);
	return (a < b ? a : b) * scale;
}

static inline int32_t rt88_ibilinrand (rt88_state *state, int32_t scale)
{
	int32_t a, b;
        a = rt88_irand (state, scale);
        b = rt88_irand (state, scale);
	return a - b;
}

static inline double rt88_bilinrand (rt88_state *state, double scale)
{
	double a, b;
        a = rt88_drand (state);
        b = rt88_drand (state);
	return (a - b) * scale;
}

static inline double rt88_exprandrng (rt88_state *state, double lo, double hi)
{
	return lo * exp(log(hi / lo) * rt88_drand (state));
}

static inline double rt88_exprand (rt88_state *state, double scale)
{
	double z;
	while ((z = rt88_drand (state)) == 0.0) {}
	return -log(z) * scale;
}

static inline double rt88_biexprand (rt88_state *state, double scale)
{
	double z;
	while ((z = rt88_drand2 (state, 1.)) == 0.0 || z == -1.0) {}
	if (z > 0.0) z = log(z);
	else z = -log(-z);
	return z * scale;
}

static inline double rt88_sum3rand (rt88_state *state, double scale)
{
	// larry polansky's poor man's gaussian generator
	return (rt88_drand (state) + rt88_drand (state) + rt88_drand(state) - 1.5)
                * 0.666666667 * scale;
}

// These functions are provided for speed in inner loops where the state
// variables are loaded into registers. Thus updating the instance variables can
// be postponed until the end of the loop.

static inline uint32_t rt88_trand_3 (uint32_t *s1, uint32_t *s2, uint32_t *s3)
{
	*s1 = ((*s1 &  -2) << 12) ^ (((*s1 << 13) ^  *s1) >> 19);
	*s2 = ((*s2 &  -8) <<  4) ^ (((*s2 <<  2) ^  *s2) >> 25);
	*s3 = ((*s3 & -16) << 17) ^ (((*s3 <<  3) ^  *s3) >> 11);
	return *s1 ^ *s2 ^ *s3;
}

static inline double rt88_drand_3 (uint32_t *s1, uint32_t *s2, uint32_t *s3)
{
	union { struct { uint32_t hi, lo; } i; double f; } u;
	u.i.hi = 0x41300000; 
	u.i.lo = rt88_trand_3 (s1,s2,s3);
	return u.f - 1048576.;
}

static inline float rt88_frand_3 (uint32_t *s1, uint32_t *s2, uint32_t *s3)
{
	// return a float from 0.0 to 0.999...
	union { uint32_t i; float f; } u;
	u.i = 0x3F800000 | (rt88_trand_3 (s1,s2,s3) >> 9);
	return u.f - 1.f;
}

static inline float rt88_frand0_3 (uint32_t *s1, uint32_t *s2, uint32_t *s3)
{
	// return a float from +1.0 to +1.999...
	union { uint32_t i; float f; } u;
	u.i = 0x3F800000 | (rt88_trand_3 (s1,s2,s3) >> 9);
	return u.f;
}

static inline float rt88_frand2_3 (uint32_t *s1, uint32_t *s2, uint32_t *s3)
{
	// return a float from -1.0 to +0.999...
	union { uint32_t i; float f; } u;
	u.i = 0x40000000 | (rt88_trand_3 (s1,s2,s3) >> 9);
	return u.f - 3.f;
}

static inline float rt88_frand8_3 (uint32_t *s1, uint32_t *s2, uint32_t *s3)
{
	// return a float from -0.125 to +0.124999...
	union { uint32_t i; float f; } u;
	u.i = 0x3E800000 | (rt88_trand_3 (s1,s2,s3) >> 9);
	return u.f - 0.375f;
}

static inline float rt88_fcoin_3 (uint32_t *s1, uint32_t *s2, uint32_t *s3)
{
	// only return one of the two values -1.0 or +1.0
	union { uint32_t i; float f; } u;
	u.i = 0x3F800000 | (0x80000000 & rt88_trand_3 (s1,s2,s3));
	return u.f;
}

#endif /* __RANDOM_TAUS88_H__ */

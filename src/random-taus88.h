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

//#include <endian.h>
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

#endif /* __RANDOM_TAUS88_H__ */

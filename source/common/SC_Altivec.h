/*
	SuperCollider real time audio synthesis system
    Copyright (c) 2003 James McCartney. All rights reserved.
	http://www.audiosynth.com

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

#ifndef _SC_Altivec_
#define _SC_Altivec_
#if __VEC__

//#define vload(x) (*vtempptr = (x), vec_splat(vtemp,0))
#define define_vtemp vfloat32 vtemp; float *vtempptr = (float*)&vtemp;
#define define_vones vfloat32 vones = vec_ctf(vec_splat_s32(1),0);
#define define_vzero vfloat32 vzero = (vfloat32)vec_splat_s32(0);
#define v0123 (vec_ctf(vec_unpackh(vec_unpackh((vector signed char)vec_lvsl(0,(int*)0))), 0))
#define v0123_4ths (vec_ctf(vec_unpackh(vec_unpackh((vector signed char)vec_lvsl(0,(int*)0))), 2))
#define vstart(x, vslope) (vec_madd(vslope, v0123_4ths, vload(x)))

#define vec_not(a) (vtemp = (a); vec_nor(vtemp, vtemp))
#define vec_cmplt(a, b) (vec_not(vec_cmpge(a, b)))
#define vec_cmple(a, b) (vec_not(vec_cmpgt(a, b)))
#define vec_mul(a, b) (vec_madd(a, b, vzero))
#define vec_2sComp(x) (vec_sub(vec_sub (x, x), x))

#define USEVEC (ft->mAltivecAvailable && !(BUFLENGTH & 3))

typedef union vec_union {
        int32		i[4];
        float32		f[4];
        vfloat32	v;
} vec_union;

inline vfloat32 vload( float f )
{
        vec_union temp;
        temp.f[0] = f;
        return vec_splat( vec_lde( 0, temp.f ), 0 );
}

// seed = ((seed & mask) << shift1) ^ (((seed << shift2) ^ seed) >> shift3);

#define define_trshifts \
	vuint32 trmask = ((vuint32)(0xFFFFFFFE,0xFFFFFFF8,0xFFFFFFF0,0)); \
	vuint32 trshift1 = ((vuint32)(12, 14,  7,  0)); \
	vuint32 trshift2 = ((vuint32)(13,  2,  3,  0)); \
	vuint32 trshift3 = ((vuint32)(19, 25, 11,  0));

inline vuint32 trands(vuint32 seed, vuint32 trmask, vuint32 shift1, vuint32 shift2, vuint32 shift3)
{
	return vec_xor(vec_sl(vec_and(seed, trmask),shift1), vec_sr(vec_xor(vec_sl(seed,shift2),seed),shift3));
}

#endif
#endif



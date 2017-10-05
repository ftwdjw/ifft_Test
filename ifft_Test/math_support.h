//
//  math_support.h
//  ifft_Test
//
//  Created by John Mac on 6/29/14.
//  Copyright (c) 2014 John Wetters. All rights reserved.
//

#ifndef ifft_Test_math_support_h
#define ifft_Test_math_support_h

/*****************************************************************************
 * math_support.h
 * all the included headers from math_support2.c
 *
 ******************************************************************************/
void gen_cmplx_input(float *s, int n);
void gen_cmplx_ramp(float *s, int n);
void gen_w_r2(float *w, int n);
void bit_rev(float *x, int n);
void divide(float* x, int n);
void DSPF_sp_cfftr2_dit(float* x, float* w, short n);
void DSPF_sp_icfftr2_dif(float* x, float* w, short n);



#endif

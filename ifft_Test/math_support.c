//
//  math_support.c
//  ifft_Test
//
//  Created by John Mac on 6/29/14.
//  Copyright (c) 2014 John Wetters. All rights reserved.
//

/*****************************************************************************
 This is a file that lists all the functions to perform an FFT and inverse FFT
 ******************************************************************************/


/*****************************************************************************
 * included files first
 ******************************************************************************/

#include <stdio.h>
#include <math.h>

/* generate a complex impulse*/
void gen_cmplx_input(float *s, int n)
{
    int i;
    for(i=0; i<n; i++)
	{
        if(i==0)
        {
            s[i]=(float) 1;
            s[i+1]=(float) 0;
        }
        else
        {
            s[i]=(float) 0;
            s[i+1]=(float) 0;
        }
	}
}

/* generate a complex ramp*/
void gen_cmplx_ramp(float *s, int n)
{
    int i,count=1;
    for(i=0; i<2*n; i=i+2)
	{
        s[i]=(float) count; //real
        s[i+1]=(float) 0; //imag
        count++;
	}
}
/* generate real and imaginary twiddle
 table of size n/2 complex numbers */
void gen_w_r2(float *w, int n)
{
    int i;
    float pi = 4.0*atan(1.0);
    float e = pi*2.0/n;
    for(i=0; i < ( n>>1 ); i++)
    {
        w[2*i] = cos(i*e);
        w[2*i+1] = sin(i*e);
    }
}

/*bit reverse twiddle factors*/
void bit_rev(float *x, int n)
{
    int i, j, k;
    float rtemp, itemp;
    j = 0;
    for(i=1; i < (n-1); i++)
    {
        k = n >> 1;
        while(k <= j)
        {
            j -= k;
            k >>= 1;
        }
        j += k;
        if(i < j)
        {
            rtemp = x[j*2];
            x[j*2] = x[i*2];
            x[i*2] = rtemp;
            itemp = x[j*2+1];
            x[j*2+1] = x[i*2+1];
            x[i*2+1] = itemp;
        }
    }
}

/*NORMALIZE*/
void divide(float* x, int n)
{
    int i;
    float inv = 1.0 / n;
    for(i=0; i < n; i++)
    {
        x[2*i] = inv * x[2*i];
        x[2*i+1] = inv * x[2*i+1];
    }
}

void DSPF_sp_cfftr2_dit(float* x, float* w, short n)
{
    short n2, ie, ia, i, j, k, m;
    float rtemp, itemp, c, s;
    n2 = n;
    ie = 1;
    for(k=n; k > 1; k >>= 1)
    {
        n2 >>= 1;
        ia = 0;
        for(j=0; j < ie; j++)
        {
            c = w[2*j];
            s = w[2*j+1];
            for(i=0; i < n2; i++)
            {
                m = ia + n2;
                rtemp = c * x[2*m] + s * x[2*m+1];
                itemp = c * x[2*m+1] - s * x[2*m];
                x[2*m] = x[2*ia] - rtemp;
                x[2*m+1] = x[2*ia+1] - itemp;
                x[2*ia] = x[2*ia] + rtemp;
                x[2*ia+1] = x[2*ia+1] + itemp;
                ia++;
            }
            ia += n2;
        }
        ie <<= 1;
    }
}

void DSPF_sp_icfftr2_dif(float* x, float* w, short n)
{
    short n2, ie, ia, i, j, k, m;
    float rtemp, itemp, c, s;
    n2 = 1;
    ie = n;
    for(k=n; k > 1; k >>= 1)
    {
        ie >>= 1;
        ia = 0;
        for(j=0; j < ie; j++)
        {
            c = w[2*j];
            s = w[2*j+1];
            for(i=0; i < n2; i++)
            {
                m = ia + n2;
                rtemp = x[2*ia] - x[2*m];
                x[2*ia] = x[2*ia] + x[2*m];
                itemp = x[2*ia+1] - x[2*m+1];
                x[2*ia+1] = x[2*ia+1] + x[2*m+1];
                x[2*m] = c*rtemp - s*itemp;
                x[2*m+1] = c*itemp + s*rtemp;
                ia++;
            }
            ia += n2;
        }
        n2 <<= 1;
    }
}

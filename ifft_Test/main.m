//
//  main.m
//  ifft_Test
//
//  Created by John Mac on 6/29/14.
//  Copyright (c) 2014 John Wetters. All rights reserved.
//

#import <Foundation/Foundation.h>
/*****************************************************************************
 This is a test file to test the single precision floating-point radix-2 complex
 FFT and Single-precision inverse, complex, radix-2, decimation-in-frequency IFFT
 An impulse is applied to the FFT. The IFFT of the FFT results in
 same sequence.
 ******************************************************************************/

/*****************************************************************************
 * included files first
 ******************************************************************************/
#include <math.h>
#include "math_support.h"
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************
 * Define constants
 ******************************************************************************/

#define N 32  //FFT size, must be a power a 2


/*Single-precision floating-point radix-2 FFT with complex input*/
/*DSP library function*/

int main(int argc, const char * argv[])
{
    float x[2*N];//x array is an interleaved complex array
    /*Input and output sequences (dim-n) (input/output) x has n
     complex numbers (2*n SP values). The real and imaginary
     values are interleaved in memory.
     
     ie X_real(0), X_imag(0),X_real(1), X_imag(1),......X_real(n), X_imag(n),
     
     The input is in bit-reversed
     order and output is in normal order.*/
    
    float w[N];
    /*w array is an interleaved FFT coeficients complex array
     FFT coefficients (dim-n/2) (input) w has n/2 complex
     numbers (n SP values). FFT coeficients must be in
     bit-reversed order. The real and imaginary values are
     interleaved in memory.*/
    
    float y[2*N];//array y is for a copy of x
    int i;
    FILE *fp;

    @autoreleasepool {
        
        // insert code here...
        NSLog(@"Hello, World!");
        gen_cmplx_ramp(x,N);  //generate inpulse input
        
        printf("complex impulse input array\n");
        for(i=0; i<2*N; i++)
            printf("s_cmplx[%d]=%f\n",i,x[i]);
        printf("\n");
        
        fp=fopen("c:/twiddle_coef.txt","w");
        
        gen_w_r2(w, N);  /*Generate bit reversed coefficient table*/
        
        //for(i=0; i<N; i++)
            //fprintf(fp,"%f,\n",w[i]);
        
        fclose(fp);
        
        bit_rev(w, N>>1);  /*Bit-reverse coefficient table*/
        /*N>>1 N shift right 1 divides N/2*/
        
        DSPF_sp_cfftr2_dit(x, w, N);
        /*input in normal order, output in*/
        /*order bit-reversed*/
        /*coefficient table in bit-reversed*/
        /*order*/
        
        for(i=0;i<2*N;i++)
            y[i]=x[i];//make a copy of x
        
        bit_rev( y, N);
        
        
        //Before bit reverse
        
        fp=fopen("c:/fft_real_before_rev.txt","w");
        //fprintf(fp,"FFT complex real output array before //reversal\n");
        //for(i=0; i<2*N-1; i=i+2)
            //fprintf(fp,"s[%d]=%f\n",i,x[i]);
        fclose(fp);
        printf("\n");
        
        fp=fopen("fft__imag_before_rev.txt","w");
        fprintf(fp,"FFT complex output array before reversal\n");
        for(i=1; i<2*N; i=i+2)
            fprintf(fp,"s[%d]=%f\n",i,x[i]);
        fclose(fp);
        printf("\n");
        
        fp=fopen("fft_re_out.txt","w");
        fprintf(fp,"FFT complex real output array after reversal\n");
        for(i=0; i<2*N-1; i=i+2)
            fprintf(fp,"re[%d]=,%f\n",i,y[i]);
        fclose(fp);
        
        fp=fopen("fft_im_out.txt","w");
        fprintf(fp,"FFT complex real output array after reversal\n");
        for(i=1; i<2*N; i=i+2)
            fprintf(fp,"im[%d]=,%f\n",i,y[i]);
        fclose(fp);
        
        fprintf(fp,"\n");
        fclose(fp);
        
        DSPF_sp_icfftr2_dif(x, w, N);
        // Inverse radix 2 FFT
        // input in bit-reversed order,
        // order output in normal
        // coefficient table in bit-reversed
        // order
        
        divide(x, N); // scale inverse FFT output
        
        printf("complex impulse output array\n");	
        for(i=0; i<2*N; i++)
        printf("s[%d]=%f\n",i,x[i]);
        
    }
    return 0;
}


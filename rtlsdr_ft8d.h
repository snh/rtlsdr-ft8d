/*
 * FreeBSD License
 * Copyright (c) 2016, Guenael
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#pragma once


#include <unistd.h>


#ifndef bool
	typedef uint32_t bool;
	#define true  1
	#define false 0
#endif


struct receiver_state {
    /* Variables used for stop conditions */
    bool exit_flag;
    bool decode_flag;

    /* Buffer used for sampling */
    float *iSamples;
    float *qSamples;

    /* Simple index */
    uint32_t iqIndex;
};


/* Option & config of the receiver */
struct receiver_options {
    uint32_t dialfreq;
    uint32_t realfreq;
    int32_t  gain;
    int32_t  autogain;
    int32_t  ppm;
    int32_t  shift;
    int32_t  upconverter;
    int32_t  directsampling;
    int32_t  maxloop;
    int32_t  device;
    char     date[7];
    char     uttime[5];
};


/* Option & config of decoder (Shared with the wsprd code) */
struct decoder_options {
    uint32_t freq;         // Dial frequency
    char     rcall[13];    // Callsign of the RX station
    char     rloc[7];      // Locator of the RX station
    char     date[7];      // Date & time of the processes samples
    char     uttime[5];    //  ''
    uint32_t quickmode;    // Decoder option & tweak
    uint32_t usehashtable; //  ''
    uint32_t npasses;      //  ''
    uint32_t subtraction;  //  ''
};


struct decoder_results {
    double   freq;
    float    sync;
    float    snr;
    float    dt;
    float    drift;
    int32_t  jitter;
    char     message[23];
    char     call[13];
    char     loc[7];
    char     pwr[3];
    uint32_t cycles;
};


static void rtlsdr_callback(unsigned char *samples, uint32_t samples_count, void *ctx);
static void *rtlsdr_rx(void *arg);
void postSpots(uint32_t n_results);
static void *decoder(void *arg);
double atofs(char *s);
int32_t parse_u64(char* s, uint64_t* const value);
void initSampleStorage();
void initDecoder_options();
void initrx_options();
void sigint_callback_handler(int signum);
void usage(void);
int32_t readfile(float *iSamples, float *qSamples, char *filename);
int32_t writefile(float *iSamples, float *qSamples, char *filename, uint32_t type, double freq);

// Cleanup todo
float hann_i(int i, int N);
void ft8_subsystem(float *idat, float *qdat, uint32_t npoints, struct decoder_options options, struct decoder_results *decodes, int32_t *n_results);
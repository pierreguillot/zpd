/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#ifndef cpd_dsp_h
#define cpd_dsp_h

#include "cpd_instance.h"

//! @defgroup dsp dsp
//! @brief The digital signal processing part of cpd.
//! @details This part manages the digital signal processing.

//! @addtogroup dsp
//! @{

//! @brief The type used for samples during digital signal processing.
typedef float cpd_sample;

//! @brief Prepares the digital signal processing for an instance.
//! @param instance The instance.
//! @param nins The number of inputs.
//! @param nouts The number of outputs.
//! @param samplerate The sample rate.
//! @param nsamples The number of samples per ticks.
CPD_EXTERN void cpd_instance_dsp_prepare(cpd_instance* instance, const int nins, const int nouts, const int samplerate, const int nsamples);

//! @brief Performs the digital signal processing for an instance.
//! @param instance The instance.
//! @param nsamples The number of samples.
//! @param nins The number of inputs.
//! @param inputs The input samples matrix.
//! @param nouts The number of outputs.
//! @param outputs The output samples matrix.
CPD_EXTERN void cpd_instance_dsp_perform(cpd_instance* instance, int nsamples, const int nins, const cpd_sample** inputs, const int nouts, cpd_sample** outputs);

//! @brief Releases the digital signal processing for an instance.
//! @param instance The instance.
CPD_EXTERN void cpd_instance_dsp_release(cpd_instance* instance);

//! @brief Gets the sample rate of an instance.
//! @param instance The instance.
//! @return The current sample rate of the instance.
CPD_EXTERN int cpd_instance_get_samplerate(cpd_instance* instance);

//! @}


#endif // cpd_dsp_h

/* ************************************************************************
 * Copyright 2013 Advanced Micro Devices, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ************************************************************************/


#pragma once
#if !defined( AMD_CLFFT_action_H )
#define AMD_CLFFT_action_H

#include "plan.h"


//
// FFTCopyAction
//
// Base class for every Copy action for the FFT.
// Currently do nothing special. The kernel generation and compilation occurs
// by the subclass FFTGeneratedCopyAction
// 
class FFTCopyAction : public FFTAction
{
public:
    FFTCopyAction(clfftPlanHandle plHandle, FFTPlan * plan, cl_command_queue queue, clfftStatus & err);

    clfftGenerators getGenerator() { return Copy; }
};


//
// FFTStockhamAction
//
// Base class for every Stockham action for the FFT.
// Currently do nothing special. The kernel generation and compilation occurs
// by the subclasses FFTGeneratedStockhamAction or FFTStaticStockhamAction
// 
class FFTStockhamAction : public FFTAction
{
public:
    FFTStockhamAction(clfftPlanHandle plHandle, FFTPlan * plan, cl_command_queue queue, clfftStatus & err);

    clfftGenerators getGenerator() { return Stockham; }
};


//
// FFTTransposeVLIWAction
//
// Base class for every TransposeVLIW action for the FFT.
// Currently do nothing special. The kernel generation and compilation occurs
// by the subclass FFTGeneratedTransposeVLIWAction
// 
class FFTTransposeVLIWAction : public FFTAction
{
public:
    FFTTransposeVLIWAction(clfftPlanHandle plHandle, FFTPlan * plan, cl_command_queue queue, clfftStatus & err);

    clfftGenerators getGenerator() { return Transpose_VLIW; }
};


//
// FFTTransposeGCNAction
//
// Base class for every TransposeGCN action for the FFT.
// Currently do nothing special. The kernel generation and compilation occurs
// by the subclass FFTGeneratedTransposeGCNAction
// 
class FFTTransposeGCNAction : public FFTAction
{
public:
    FFTTransposeGCNAction(clfftPlanHandle plHandle, FFTPlan * plan, cl_command_queue queue, clfftStatus & err);

    clfftGenerators getGenerator() { return Transpose_GCN; }
};

//
// FFTGeneratedCopyAction
//
// Implements a Copy action for the FFT
// Its signature is represented by FFTKernelGenKeyParams structure
// 
// This class implements:
//  - the generation of the kernel string
//  - the build of the kernel
//
// The structure FFTKernelGenKeyParams is used to characterize and generate
// the appropriate copy kernel. That structure is used for the signature of
// this action. It is common to Stockham, copy and transpose methods. For
// convenience, this structure is used for every FFTGenerated*Action class,
// but in practice the copy action only use a few information of that
// structure, so a proper structure should be used instead.
//
class FFTGeneratedCopyAction : public FFTCopyAction
{
public:
    FFTGeneratedCopyAction(clfftPlanHandle plHandle, FFTPlan * plan, cl_command_queue queue, clfftStatus & err);

    typedef FFTKernelSignature<FFTKernelGenKeyParams, FFT_DEFAULT_COPY_ACTION> Signature;

private:
    Signature signature;

    clfftStatus generateKernel  (FFTRepo& fftRepo, const cl_command_queue commQueueFFT );
    clfftStatus getWorkSizes    (std::vector<size_t> & globalws, std::vector<size_t> & localws);
    clfftStatus initParams      ();

    bool buildForwardKernel();
    bool buildBackwardKernel();

public:

    virtual const Signature * getSignatureData()
    {
        return &this->signature;
    }
};


//
// FFTGeneratedStockhamAction
//
// Represents a Stockham action for the FFT. This class implements the former
// mechanism of kernel generation and compilation for Stockham method.
// 
// This class implements:
//  - the generation of the kernel string
//  - the build of the kernel
//
// The structure FFTKernelGenKeyParams is used to characterize and generate
// the appropriate kernel. That structure is used for the signature of this
// action. For convenience, this structure is used for every
// FFTGenerated*Action class, but a "Stockham-specific" version of that
// structure should be used instead.
//
class FFTGeneratedStockhamAction : public FFTStockhamAction
{
public:
    FFTGeneratedStockhamAction(clfftPlanHandle plHandle, FFTPlan * plan, cl_command_queue queue, clfftStatus & err);
    
    typedef FFTKernelSignature<FFTKernelGenKeyParams, FFT_DEFAULT_STOCKHAM_ACTION> Signature;

private:
    Signature signature;

    clfftStatus generateKernel  (FFTRepo& fftRepo, const cl_command_queue commQueueFFT );
    clfftStatus getWorkSizes    (std::vector<size_t> & globalws, std::vector<size_t> & localws);
    clfftStatus initParams      ();

    bool buildForwardKernel();
    bool buildBackwardKernel();

public:

    virtual const Signature * getSignatureData()
    {
        return &this->signature;
    }
};

//
// FFTGeneratedTransposeVLIWAction
//
// Implements a TransposeVLIW action for the FFT
// Its signature is represented by FFTKernelGenKeyParams structure
// 
// This class implements:
//  - the generation of the kernel string
//  - the build of the kernel
// 
// The structure FFTKernelGenKeyParams is used to characterize and generate
// the appropriate transpose kernel. That structure is used for the signature of
// this action. It is common to Stockham, copy and transpose methods. For
// convenience, this structure is used for every FFTGenerated*Action class,
// but in practice the transpose action only use a few information of that
// structure, so a proper structure should be used instead.
//
class FFTGeneratedTransposeVLIWAction : public FFTTransposeVLIWAction
{
public:
    FFTGeneratedTransposeVLIWAction(clfftPlanHandle plHandle, FFTPlan * plan, cl_command_queue queue, clfftStatus & err);

    typedef FFTKernelSignature<FFTKernelGenKeyParams, FFT_DEFAULT_TRANSPOSE_ACTION> Signature;

private:
    Signature signature;

    clfftStatus generateKernel  (FFTRepo& fftRepo, const cl_command_queue commQueueFFT );
    clfftStatus getWorkSizes    (std::vector<size_t> & globalws, std::vector<size_t> & localws);
    clfftStatus initParams      ();

    bool buildForwardKernel();
    bool buildBackwardKernel();

public:

    virtual const Signature * getSignatureData()
    {
        return &this->signature;
    }
};



// FFTGeneratedTransposeGCNAction
//
// Implements a TransposeGCN action for the FFT
// Its signature is represented by FFTKernelGenKeyParams structure
// 
// This class implements:
//  - the generation of the kernel string
//  - the build of the kernel
// 
// The structure FFTKernelGenKeyParams is used to characterize and generate
// the appropriate transpose kernel. That structure is used for the signature of
// this action. It is common to Stockham, copy and transpose methods. For
// convenience, this structure is used for every FFTGenerated*Action class,
// but in practice the transpose action only use a few information of that
// structure, so a proper structure should be used instead.
//
class FFTGeneratedTransposeGCNAction : public FFTTransposeGCNAction
{
public:
    FFTGeneratedTransposeGCNAction(clfftPlanHandle plHandle, FFTPlan * plan, cl_command_queue queue, clfftStatus & err);

    typedef FFTKernelSignature<FFTKernelGenKeyParams, FFT_DEFAULT_TRANSPOSE_ACTION> Signature;

private:
    Signature signature;

    clfftStatus generateKernel  (FFTRepo& fftRepo, const cl_command_queue commQueueFFT );
    clfftStatus getWorkSizes    (std::vector<size_t> & globalws, std::vector<size_t> & localws);
    clfftStatus initParams      ();

    bool buildForwardKernel();
    bool buildBackwardKernel();

public:

    virtual const Signature * getSignatureData()
    {
        return &this->signature;
    }
};


// FFTGeneratedTransposeInplaceAction
//
// Implements a TransposeInplace action for the FFT
// Its signature is represented by FFTKernelGenKeyParams structure
// 
// This class implements:
//  - the generation of the kernel string
//  - the build of the kernel
// 
// The structure FFTKernelGenKeyParams is used to characterize and generate
// the appropriate transpose kernel. That structure is used for the signature of
// this action. It is common to Stockham, copy and transpose methods. For
// convenience, this structure is used for every FFTGenerated*Action class,
// but in practice the transpose action only use a few information of that
// structure, so a proper structure should be used instead.
//
class FFTGeneratedTransposeInplaceAction : public FFTTransposeInplaceAction
{
public:
    FFTGeneratedTransposeInplaceAction(clfftPlanHandle plHandle, FFTPlan * plan, cl_command_queue queue, clfftStatus & err);

    typedef FFTKernelSignature<FFTKernelGenKeyParams, FFT_DEFAULT_TRANSPOSE_ACTION> Signature;

private:
    Signature signature;

    clfftStatus generateKernel  (FFTRepo& fftRepo, const cl_command_queue commQueueFFT );
    clfftStatus getWorkSizes    (std::vector<size_t> & globalws, std::vector<size_t> & localws);
    clfftStatus initParams      ();

    bool buildForwardKernel();
    bool buildBackwardKernel();

public:

    virtual const Signature * getSignatureData()
    {
        return &this->signature;
    }
};
#endif // AMD_CLFFT_action_H

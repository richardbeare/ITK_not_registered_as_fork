/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    CudaImageToImageFilter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCudaImageToImageFilter_txx
#define __itkCudaImageToImageFilter_txx
#include "CudaImageToImageFilter.h"


namespace itk
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
CudaImageToImageFilter<TInputImage,TOutputImage>
::CudaImageToImageFilter()
{
  // Modify superclass default values, can be overridden by subclasses
  this->SetNumberOfRequiredInputs(1);
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
CudaImageToImageFilter<TInputImage,TOutputImage>
::~CudaImageToImageFilter()
{
}

template <class TInputImage, class TOutputImage>
void
CudaImageToImageFilter<TInputImage,TOutputImage>
::AllocateOutputs()
{
  typedef ImageBase<OutputImageDimension> ImageBaseType;
  typename ImageBaseType::Pointer outputPtr;

  // Allocate the output memory
  for (unsigned int i=0; i < this->GetNumberOfOutputs(); i++)
    {

    // Check whether the output is an image of the appropriate
    // dimension (use ProcessObject's version of the GetInput()
    // method since it returns the input as a pointer to a
    // DataObject as opposed to the subclass version which
    // static_casts the input to an TInputImage).
    outputPtr = dynamic_cast< ImageBaseType *>( this->ProcessObject::GetOutput(i) );

    if ( outputPtr )
      {
//       outputPtr->SetBufferedRegion( outputPtr->GetRequestedRegion() );
//       outputPtr->AllocateGPU();
      // may not be the best way of doing this. Casting loses the
      // AllocateGPU method
      OutputImagePointer op2 = this->GetOutput(i);
      op2->SetBufferedRegion( outputPtr->GetRequestedRegion() );
      op2->AllocateGPU();
      }
    }
}



} // end namespace itk

#endif

/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkCudaImportImageContainer.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCudaImportImageContainer_txx
#define __itkCudaImportImageContainer_txx

#include "itkCudaImportImageContainer.h"
#include "cuda.h"
#include <cstring>
#include <stdlib.h>
#include <string.h>
#include <time.h>

namespace itk
{

   template <typename TElementIdentifier, typename TElement>
      CudaImportImageContainer<TElementIdentifier , TElement>
      ::CudaImportImageContainer()
      {
	//serial = (int)(rand()/10000000);
         m_ImageLocation = CPU;
         m_DevicePointer = 0;
         m_ImportPointer = 0;
         m_ContainerManageMemory = true;
         m_ContainerManageDevice = true;
         m_Capacity = 0;
         m_Size = 0;
      }


   template <typename TElementIdentifier, typename TElement>
      CudaImportImageContainer< TElementIdentifier , TElement >
      ::~CudaImportImageContainer()
      {
         DeallocateManagedMemory();
      }


   /**
    * Tell the container to allocate enough memory to allow at least
    * as many elements as the size given to be stored.  
    */
   template <typename TElementIdentifier, typename TElement>
      void
      CudaImportImageContainer< TElementIdentifier , TElement >
      ::Reserve(ElementIdentifier size)
      {
         //std::cout << serial <<" Reserving CPU " << std::endl;

         /* Parent Class */
         if (m_ImportPointer)
         {
            if (size > m_Capacity)
            {
               TElement* temp = this->AllocateElements(size);
               // only copy the portion of the data used in the old buffer

               memcpy(temp, m_ImportPointer, m_Size*sizeof(TElement));

               DeallocateManagedMemory();

               m_ImportPointer = temp;
               m_ContainerManageMemory = true;
               m_Capacity = size;
               m_Size = size;
               this->Modified();
            }
            else
            {
               m_Size = size;
               this->Modified();
            }
         }
         else
         {
            m_ImportPointer = this->AllocateElements(size);
            m_Capacity = size;
            m_Size = size;
            m_ContainerManageMemory = true;
            this->Modified();
         }
         //std::cout << serial << " Reserved CPU " << std::endl;
      }

   /**
    * Tell the container to allocate enough memory to allow at least
    * as many elements as the size given to be stored.  
    */
   template <typename TElementIdentifier, typename TElement>
      void
      CudaImportImageContainer< TElementIdentifier , TElement >
      ::ReserveGPU(ElementIdentifier size)
      {
         //std::cout << serial <<" Reserving CPU " << std::endl;

         /* Parent Class */
         if (m_DevicePointer)
         {
            if (size > m_Capacity)
            {
               TElement* temp = this->AllocateGPUElements(size);
               // only copy the portion of the data used in the old buffer

               cudaMemcpy(temp, m_DevicePointer, m_Size*sizeof(TElement), cudaMemcpyDeviceToDevice);

               DeallocateManagedMemory();

               m_DevicePointer = temp;
               m_ContainerManageMemory = true;
               m_Capacity = size;
               m_Size = size;
               this->Modified();
            }
            else
            {
               m_Size = size;
               this->Modified();
            }
         }
         else
         {
            m_DevicePointer = this->AllocateGPUElements(size);
            m_Capacity = size;
            m_Size = size;
            m_ContainerManageMemory = true;
            this->Modified();
         }
	 m_ImageLocation = GPU;
         //std::cout << serial << " Reserved CPU " << std::endl;
      }


   /**
    * Tell the container to try to minimize its memory usage for storage of
    * the current number of elements.  
    */
   template <typename TElementIdentifier, typename TElement>
      void
      CudaImportImageContainer< TElementIdentifier , TElement >
      ::Squeeze(void)
      {
         //std::cout << serial << " Squeezing CPU " << std::endl;

         /* Parent Code */
         if (m_ImportPointer)
         {
            if (m_Size < m_Capacity)
            {
               const TElementIdentifier size = m_Size;
               TElement* temp = this->AllocateElements(size);
               memcpy(temp, m_ImportPointer, size*sizeof(TElement));

               DeallocateManagedMemory();

               m_ImportPointer = temp;
               m_ContainerManageMemory = true;
               m_Capacity = size;
               m_Size = size;

               this->Modified();
            }
         }

         //std::cout << serial << " Squeezed CPU " << std::endl;
      }


   /**
    * Tell the container to try to minimize its memory usage for storage of
    * the current number of elements.  
    */
   template <typename TElementIdentifier, typename TElement>
      void
      CudaImportImageContainer< TElementIdentifier , TElement >
      ::Initialize(void)
      {
         //std::cout << serial << " Initializing CPU " << std::endl;

         /* Parent code */
         if (m_ImportPointer)
         {
            DeallocateManagedMemory();

            m_ContainerManageMemory = true;

            this->Modified();
         }
         
         //std::cout << serial << " Initialized CPU " << std::endl;
      }


   /**
    * Set the pointer from which the image data is imported.  "num" is
    * the number of pixels in the block of memory. If
    * "LetContainerManageMemory" is false, then the application retains
    * the responsibility of freeing the memory for this image data.  If
    * "LetContainerManageMemory" is true, then this class will free the
    * memory when this object is destroyed.
    */
   template <typename TElementIdentifier, typename TElement>
      void
      CudaImportImageContainer< TElementIdentifier , TElement >
      ::SetImportPointer(TElement *ptr, TElementIdentifier num,
            bool LetContainerManageMemory)
      {
         DeallocateManagedMemory();
         m_ImportPointer = ptr;
         m_ContainerManageMemory = LetContainerManageMemory;
         m_Capacity = num;
         m_Size = num;
         //AllocateGPU();

         this->Modified();
         m_ImageLocation = CPU;
      }

   template <typename TElementIdentifier, typename TElement>
      void
      CudaImportImageContainer< TElementIdentifier, TElement >
      ::SetDevicePointer(TElement *ptr, TElementIdentifier num,
            bool LetContainerManageDevice)
      {
         DeallocateManagedMemory();
         m_DevicePointer = ptr;
         m_ContainerManageDevice = LetContainerManageDevice;
         m_Capacity = num;
         m_Size = num;
         this->Modified();
         m_ImageLocation = GPU;
      }

   template <typename TElementIdentifier, typename TElement>
      TElement* CudaImportImageContainer< TElementIdentifier , TElement >
      ::AllocateElements(ElementIdentifier size) const
      {
         // Encapsulate all image memory allocation here to throw an
         // exception when memory allocation fails even when the compiler
         // does not do this by default.

         /* Parent code */
            TElement* data;
            try
            {
            data = new TElement[size];
            }
            catch(...)
            {
            data = 0;
            }
            if(!data)
            {
         // We cannot construct an error string here because we may be out
         // of memory.  Do not use the exception macro.
         throw MemoryAllocationError(__FILE__, __LINE__,
         "Failed to allocate memory for image.",
         ITK_LOCATION);
         }
         return data;
          
      }

  template <typename TElementIdentifier, typename TElement>
      TElement* CudaImportImageContainer< TElementIdentifier , TElement >
      ::AllocateGPUElements(ElementIdentifier size) const
      {
         // Encapsulate all image memory allocation here to throw an
         // exception when memory allocation fails even when the compiler
         // does not do this by default.

         /* Parent code */
            TElement* data;
            try
            {
	    void * m_Tmp;
	    cudaMalloc( &m_Tmp, sizeof(TElement)*size);
	    data = (TElement *)m_Tmp;
            }
            catch(...)
            {
            data = 0;
            }
            if(!data)
            {
         // We cannot construct an error string here because we may be out
         // of memory.  Do not use the exception macro.
	    throw MemoryAllocationError(__FILE__, __LINE__,
					"Failed to allocate GPU memory for image.",
					ITK_LOCATION);
         }
         return data;
          
      }

   template <typename TElementIdentifier, typename TElement>
      void CudaImportImageContainer< TElementIdentifier , TElement >
      ::DeallocateManagedMemory()
      {
         // CPU Deallocate
         if (m_ImportPointer && m_ContainerManageMemory)
         {
            delete [] m_ImportPointer;
         }

         // GPU Deallocate
         if (m_DevicePointer && m_ContainerManageDevice)
         {
            cudaFree(m_DevicePointer);
         }
         
         m_DevicePointer = 0;
         m_ImportPointer = 0;
         m_Capacity = 0;
         m_Size = 0;
      }

   template <typename TElementIdentifier, typename TElement>
      void
      CudaImportImageContainer< TElementIdentifier , TElement >
      ::PrintSelf(std::ostream& os, Indent indent) const
      {
         Superclass::PrintSelf(os,indent);

         os << indent << "Pointer: " << static_cast<void *>(m_ImportPointer) << std::endl;
         os << indent << "DevPointer: " << static_cast<void *>(m_DevicePointer) << std::endl;
	 os << indent << "Image location: " << m_ImageLocation << std::endl;
         os << indent << "Container manages memory: "
            << (m_ContainerManageMemory ? "true" : "false") << std::endl;
         os << indent << "Container manages device memory: "
            << (m_ContainerManageDevice ? "true" : "false") << std::endl;
         os << indent << "Size: " << m_Size << std::endl;
         os << indent << "Capacity: " << m_Capacity << std::endl;
      }

   template <typename TElementIdentifier, typename TElement>
      void
      CudaImportImageContainer< TElementIdentifier , TElement >
   ::CopyToGPU() //const
      {

         AllocateGPU();
         cudaMemcpy(m_DevicePointer, m_ImportPointer,
               sizeof(TElement)*m_Size, cudaMemcpyHostToDevice);

         m_ImageLocation = GPU;
      }

   template <typename TElementIdentifier, typename TElement>
      void 
      CudaImportImageContainer< TElementIdentifier , TElement >
      ::CopyToCPU()
      {
	AllocateCPU();
	cudaMemcpy(m_ImportPointer, m_DevicePointer,
		   sizeof(TElement)*m_Size, cudaMemcpyDeviceToHost);

         m_ImageLocation = CPU;
      }

   template <typename TElementIdentifier, typename TElement>
      void
      CudaImportImageContainer< TElementIdentifier, TElement >
   ::AllocateGPU() //const
      {
	void * m_Tmp;
	cudaMalloc( &m_Tmp, sizeof(TElement)*m_Size);
	m_DevicePointer = (TElement *)m_Tmp;
      }

   template <typename TElementIdentifier, typename TElement>
      void
      CudaImportImageContainer< TElementIdentifier, TElement >
      ::AllocateCPU()
      {
	// should only need to allocate CPU memory if the import
	// pointer is null
	if (!m_ImportPointer)
	  m_ImportPointer = (TElement *)malloc(sizeof(TElement)*m_Size);
      }

} // end namespace itk

#endif

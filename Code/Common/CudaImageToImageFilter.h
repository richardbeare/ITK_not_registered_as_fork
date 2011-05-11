
#ifndef __itkCudaImageToImageFilter_h
#define __itkCudaImageToImageFilter_h

#include "itkImage.h"
#include "itkConceptChecking.h"
#include "itkImageToImageFilter.h"

namespace itk
{

/** \class CudaImageToImageFilter
**/
template <class TInputImage, class TOutputImage>
class ITK_EXPORT CudaImageToImageFilter : public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef CudaImageToImageFilter         Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef SmartPointer<Self>         Pointer;
  typedef SmartPointer<const Self>   ConstPointer;


  /** Run-time type information (and related methods). */
  itkTypeMacro(CudaImageToImageFilter, ImageToImageFilter);

  /** Superclass typedefs. */
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;
  typedef typename Superclass::OutputImagePixelType  OutputImagePixelType;

  typedef typename TOutputImage::Pointer OutputImagePointer;
  /** Some convenient typedefs. */
  typedef TInputImage                             InputImageType;
  typedef typename InputImageType::Pointer        InputImagePointer;
  typedef typename InputImageType::ConstPointer   InputImageConstPointer;
  typedef typename InputImageType::RegionType     InputImageRegionType;
  typedef typename InputImageType::PixelType      InputImagePixelType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

protected:
  CudaImageToImageFilter();
  ~CudaImageToImageFilter();
  virtual void AllocateOutputs();

private:
  CudaImageToImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_CudaImageToImageFilter(_, EXPORT, x, y) namespace itk { \
    _(2(class EXPORT ImageToImageFilter< ITK_TEMPLATE_2 x >))		\
      namespace Templates { typedef ImageToImageFilter< ITK_TEMPLATE_2 x > ImageToImageFilter##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/CudaImageToImageFilter+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "CudaImageToImageFilter.txx"
#endif

#endif

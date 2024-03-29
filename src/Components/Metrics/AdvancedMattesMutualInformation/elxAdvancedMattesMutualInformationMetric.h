/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __elxAdvancedMattesMutualInformationMetric_H__
#define __elxAdvancedMattesMutualInformationMetric_H__

#include "elxIncludes.h"
#include "itkParzenWindowMutualInformationImageToImageMetric.h"

#include "elxTimer.h"

namespace elastix
{
using namespace itk;

  /**
   * \class AdvancedMattesMutualInformationMetric
   * \brief A metric based on the itk::ParzenWindowMutualInformationImageToImageMetric.
   *
   * This metric is based on an adapted version of the
   * itk::MattesMutualInformationImageToImageMetric.
   *
   * The parameters used in this class are:
   * \parameter Metric: Select this metric as follows:\n
   *    <tt>(Metric "AdvancedMattesMutualInformation")</tt>
   * \parameter NumberOfHistogramBins: The size of the histogram. Must be
   *    given for each resolution, or for all resolutions at once. \n
   *    example: <tt>(NumberOfHistogramBins 32 32 64)</tt> \n
   *    The default is 32 for each resolution.
   * \parameter NumberOfFixedHistogramBins: The size of the histogram in the
   *    fixed dimension. Can be given for each resolution, or for all
   *    resolutions at once. If not given, NumberOfHistogramBins is used.\n
   *    example: <tt>(NumberOfFixedHistogramBins 32 32 64)</tt> \n
   *    The default is the value of NumberOfHistogramBins, or, if that one
   *    is also not given, 32.
   * \parameter NumberOfMovingHistogramBins: The size of the histogram in
   *    the fixed dimension. Can be given for each resolution, or for all
   *    resolutions at once. If not given, NumberOfHistogramBins is used.\n
   *    example: <tt>(NumberOfMovingHistogramBins 32 32 64)</tt> \n
   *    The default is the value of NumberOfHistogramBins, or, if that one
   *    is also not given, 32.
   * \parameter FixedKernelBSplineOrder: The B-spline order of the Parzen
   *    window, used to estimate the joint histogram. Can be given for each
   *    resolution, or for all resolutions at once. \n
   *    example: <tt>(FixedKernelBSplineOrder 0 1 1)</tt> \n
   *    The default value is 0.
   * \parameter MovingKernelBSplineOrder: The B-spline order of the Parzen
   *    window, used to estimate the joint histogram. Can be given for each
   *    resolution, or for all resolutions at once. \n
   *    example: <tt>(MovingKernelBSplineOrder 3 3 3)</tt> \n
   *    The default value is 3.
   * \parameter FixedLimitRangeRatio: The relative extension of the intensity
   *    range of the fixed image.\n
   *    If your fixed image has grey values from a to b and the
   *    FixedLimitRangeRatio is 0.001, the joint histogram will expect fixed
   *    image grey values from a-0.001(b-a) to b+0.001(b-a). This may be useful if
   *    you use high order B-spline interpolator for the fixed image.\n
   *    example: <tt>(FixedLimitRangeRatio 0.001 0.01 0.01)</tt> \n
   *    The default value is 0.01. Can be given for each resolution, or for
   *    all resolutions at once.
   * \parameter MovingLimitRangeRatio: The relative extension of the
   *    intensity range of the moving image.\n
   *    If your moving image has grey values from a to b and the
   *    MovingLimitRangeRatio is 0.001, the joint histogram will expect moving
   *    image grey values from a-0.001(b-a) to b+0.001(b-a). This may be useful if
   *    you use high order B-spline interpolator for the moving image.\n
   *    example: <tt>(MovingLimitRangeRatio 0.001 0.01 0.01)</tt> \n
   *    The default value is 0.01. Can be given for each resolution, or for
   *    all resolutions at once.
   * \parameter FiniteDifferenceDerivative: Experimental feature, do not use.
   * \parameter UseFastAndLowMemoryVersion: Switch between a version of
   *    mutual information that explicitely computes the derivatives of the
   *    joint histogram to each transformation parameter (false) and a
   *    version that computes the mutual information via another route (true).
   *    The first option allocates a large 3D matrix of size:
   *    NumberOfFixedHistogramBins * NumberOfMovingHistogramBins * number
   *    of affected B-spline parameters. This method is faster for a low
   *    number of parameters. The second method does not use this huge matrix,
   *    and is therefore much more memory efficient for large images and fine
   *    B-spline grids.
   *    example: <tt>(UseFastAndLowMemoryVersion "false")</tt> \n
   *    The default is "true".
   *
   * \sa ParzenWindowMutualInformationImageToImageMetric
   * \ingroup Metrics
   */

  template <class TElastix >
    class AdvancedMattesMutualInformationMetric :
    public
      ParzenWindowMutualInformationImageToImageMetric<
        ITK_TYPENAME MetricBase<TElastix>::FixedImageType,
        ITK_TYPENAME MetricBase<TElastix>::MovingImageType >,
    public MetricBase<TElastix>
  {
  public:

    /** Standard ITK-stuff. */
    typedef AdvancedMattesMutualInformationMetric         Self;
    typedef ParzenWindowMutualInformationImageToImageMetric<
      typename MetricBase<TElastix>::FixedImageType,
      typename MetricBase<TElastix>::MovingImageType >    Superclass1;
    typedef MetricBase<TElastix>                          Superclass2;
    typedef SmartPointer<Self>                            Pointer;
    typedef SmartPointer<const Self>                      ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro( Self );

    /** Run-time type information (and related methods). */
    itkTypeMacro( AdvancedMattesMutualInformationMetric,
      ParzenWindowMutualInformationImageToImageMetric );

    /** Name of this class.
     * Use this name in the parameter file to select this specific metric. \n
     * example: <tt>(Metric "AdvancedMattesMutualInformation")</tt>\n
     */
    elxClassNameMacro( "AdvancedMattesMutualInformation" );

    /** Typedefs from the superclass. */
    typedef typename
      Superclass1::CoordinateRepresentationType              CoordinateRepresentationType;
    typedef typename Superclass1::MovingImageType            MovingImageType;
    typedef typename Superclass1::MovingImagePixelType       MovingImagePixelType;
    typedef typename Superclass1::MovingImageConstPointer    MovingImageConstPointer;
    typedef typename Superclass1::FixedImageType             FixedImageType;
    typedef typename Superclass1::FixedImageConstPointer     FixedImageConstPointer;
    typedef typename Superclass1::FixedImageRegionType       FixedImageRegionType;
    typedef typename Superclass1::TransformType              TransformType;
    typedef typename Superclass1::TransformPointer           TransformPointer;
    typedef typename Superclass1::InputPointType             InputPointType;
    typedef typename Superclass1::OutputPointType            OutputPointType;
    typedef typename Superclass1::TransformParametersType    TransformParametersType;
    typedef typename Superclass1::TransformJacobianType      TransformJacobianType;
    typedef typename Superclass1::InterpolatorType           InterpolatorType;
    typedef typename Superclass1::InterpolatorPointer        InterpolatorPointer;
    typedef typename Superclass1::RealType                   RealType;
    typedef typename Superclass1::GradientPixelType          GradientPixelType;
    typedef typename Superclass1::GradientImageType          GradientImageType;
    typedef typename Superclass1::GradientImagePointer       GradientImagePointer;
    typedef typename Superclass1::GradientImageFilterType    GradientImageFilterType;
    typedef typename Superclass1::GradientImageFilterPointer GradientImageFilterPointer;
    typedef typename Superclass1::FixedImageMaskType         FixedImageMaskType;
    typedef typename Superclass1::FixedImageMaskPointer      FixedImageMaskPointer;
    typedef typename Superclass1::MovingImageMaskType        MovingImageMaskType;
    typedef typename Superclass1::MovingImageMaskPointer     MovingImageMaskPointer;
    typedef typename Superclass1::MeasureType                MeasureType;
    typedef typename Superclass1::DerivativeType             DerivativeType;
    typedef typename Superclass1::ParametersType             ParametersType;
    typedef typename Superclass1::FixedImagePixelType        FixedImagePixelType;
    typedef typename Superclass1::MovingImageRegionType      MovingImageRegionType;
    typedef typename Superclass1::ImageSamplerType           ImageSamplerType;
    typedef typename Superclass1::ImageSamplerPointer        ImageSamplerPointer;
    typedef typename Superclass1::ImageSampleContainerType   ImageSampleContainerType;
    typedef typename
      Superclass1::ImageSampleContainerPointer               ImageSampleContainerPointer;
    typedef typename Superclass1::FixedImageLimiterType      FixedImageLimiterType;
    typedef typename Superclass1::MovingImageLimiterType     MovingImageLimiterType;
    typedef typename
      Superclass1::FixedImageLimiterOutputType               FixedImageLimiterOutputType;
    typedef typename
      Superclass1::MovingImageLimiterOutputType              MovingImageLimiterOutputType;
    typedef typename
      Superclass1::MovingImageDerivativeScalesType           MovingImageDerivativeScalesType;

    /** The fixed image dimension. */
    itkStaticConstMacro( FixedImageDimension, unsigned int,
      FixedImageType::ImageDimension );

    /** The moving image dimension. */
    itkStaticConstMacro( MovingImageDimension, unsigned int,
      MovingImageType::ImageDimension );

    /** Typedef's inherited from Elastix. */
    typedef typename Superclass2::ElastixType               ElastixType;
    typedef typename Superclass2::ElastixPointer            ElastixPointer;
    typedef typename Superclass2::ConfigurationType         ConfigurationType;
    typedef typename Superclass2::ConfigurationPointer      ConfigurationPointer;
    typedef typename Superclass2::RegistrationType          RegistrationType;
    typedef typename Superclass2::RegistrationPointer       RegistrationPointer;
    typedef typename Superclass2::ITKBaseType               ITKBaseType;

    /** Typedef for timer. */
    typedef tmr::Timer          TimerType;
    /** Typedef for timer. */
    typedef TimerType::Pointer  TimerPointer;

    /** Execute stuff before each new pyramid resolution:
     * \li Set the number of histogram bins.
     * \li Set the CheckNumberOfSamples option.
     * \li Set the fixed/moving LimitRangeRatio
     * \li Set the fixed/moving limiter. */
    virtual void BeforeEachResolution( void );

    /** Update the CurrenIteration. This is only important
     * if a finite difference derivative estimation is used
     * (selected by the experimental parameter FiniteDifferenceDerivative)  */
    virtual void AfterEachIteration( void );

    /** Set up a timer to measure the intialisation time and
     * call the Superclass' implementation. */
    virtual void Initialize(void) throw (ExceptionObject);

    /** Set/Get c. For finite difference derivative estimation */
    itkSetMacro( Param_c, double );
    itkGetConstMacro( Param_c, double );

    /** Set/Get gamma. For finite difference derivative estimation */
    itkSetMacro( Param_gamma, double );
    itkGetConstMacro( Param_gamma, double );

    /** Set/Get the current iteration. For finite difference derivative estimation */
    itkSetMacro( CurrentIteration, unsigned int );
    itkGetConstMacro( CurrentIteration, unsigned int );

  protected:

    /** The constructor. */
    AdvancedMattesMutualInformationMetric();

    /** The destructor. */
    virtual ~AdvancedMattesMutualInformationMetric() {};

    unsigned long                 m_CurrentIteration;

    /** A function to compute the finite difference perturbation in each iteration */
    double Compute_c( unsigned long k ) const;

  private:

    /** The private constructor. */
    AdvancedMattesMutualInformationMetric( const Self& ); // purposely not implemented
    /** The private copy constructor. */
    void operator=( const Self& );                // purposely not implemented

    double                        m_Param_c;
    double                        m_Param_gamma;



  }; // end class AdvancedMattesMutualInformationMetric


} // end namespace elastix


#ifndef ITK_MANUAL_INSTANTIATION
#include "elxAdvancedMattesMutualInformationMetric.hxx"
#endif

#endif // end #ifndef __elxAdvancedMattesMutualInformationMetric_H__

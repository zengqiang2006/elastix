/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/
#ifndef __elxDisplacementMagnitudePenalty_H__
#define __elxDisplacementMagnitudePenalty_H__

#include "elxIncludes.h"
#include "itkDisplacementMagnitudePenaltyTerm.h"

#include "elxTimer.h"

namespace elastix
{
using namespace itk;

  /**
   * \class DisplacementMagnitudePenalty
   * \brief A penalty term that penalises ||T(x)-x||.
   *
   * This penalty term may look a little unusual, but it can
   * be used as a way to compute an inverse transform, by supplying
   * the transform to be inverted as an initial transform and
   * setting the HowToCombineTransforms parameter to "Compose".
   *
   * The parameters used in this class are:
   * \parameter Metric: Select this metric as follows:\n
   *    <tt>(Metric "DisplacementMagnitudePenalty")</tt>
   *
   * \ingroup Metrics
   * \sa DisplacementEnergyPenaltyTerm
   */

  template <class TElastix >
    class DisplacementMagnitudePenalty:
    public
      DisplacementMagnitudePenaltyTerm<
        ITK_TYPENAME MetricBase<TElastix>::FixedImageType,
        double >,
    public MetricBase<TElastix>
  {
  public:

    /** Standard ITK-stuff. */
    typedef DisplacementMagnitudePenalty                 Self;
    typedef DisplacementMagnitudePenaltyTerm<
      typename MetricBase<TElastix>::FixedImageType,
      double >                                            Superclass1;
    typedef MetricBase<TElastix>                          Superclass2;
    typedef SmartPointer<Self>                            Pointer;
    typedef SmartPointer<const Self>                      ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro( Self );

    /** Run-time type information (and related methods). */
    itkTypeMacro( DisplacementMagnitudePenalty, DisplacementMagnitudePenaltyTerm );

    /** Name of this class.
     * Use this name in the parameter file to select this specific metric. \n
     * example: <tt>(Metric "DisplacementMagnitudePenalty")</tt>\n
     */
    elxClassNameMacro( "DisplacementMagnitudePenalty" );

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

    /** The fixed image dimension. */
    itkStaticConstMacro( FixedImageDimension, unsigned int,
      FixedImageType::ImageDimension );

    /** The moving image dimension. */
    itkStaticConstMacro( MovingImageDimension, unsigned int,
      MovingImageType::ImageDimension );

    /** Typedef's inherited from elastix. */
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

    /** Sets up a timer to measure the initialisation time and
     * calls the Superclass' implementation.
     */
    virtual void Initialize( void ) throw (ExceptionObject);

  protected:

    /** The constructor. */
    DisplacementMagnitudePenalty(){};

    /** The destructor. */
    virtual ~DisplacementMagnitudePenalty() {}

  private:

    /** The private constructor. */
    DisplacementMagnitudePenalty( const Self& ); // purposely not implemented
    /** The private copy constructor. */
    void operator=( const Self& );              // purposely not implemented

  }; // end class DisplacementMagnitudePenalty


} // end namespace elastix


#ifndef ITK_MANUAL_INSTANTIATION
#include "elxDisplacementMagnitudePenalty.hxx"
#endif

#endif // end #ifndef __elxDisplacementMagnitudePenalty_H__


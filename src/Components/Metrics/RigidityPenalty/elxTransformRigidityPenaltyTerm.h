/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/
#ifndef __elxTransformRigidityPenaltyTerm_H__
#define __elxTransformRigidityPenaltyTerm_H__

#include "elxIncludes.h"
#include "itkTransformRigidityPenaltyTerm.h"

#include "elxTimer.h"

namespace elastix
{
using namespace itk;

  /**
   * \class TransformRigidityPenalty
   * \brief A penalty term based on non-rigidity.
   *
   * For more information check the paper:\n
   * M. Staring, S. Klein and J.P.W. Pluim,
   * "A Rigidity Penalty Term for Nonrigid Registration,"
   * Medical Physics, vol. 34, no. 11, pp. 4098 - 4108, November 2007.
   *
   * The parameters used in this class are:
   * \parameter Metric: Select this metric as follows:\n
   *    <tt>(Metric "TransformRigidityPenalty")</tt>
   *
   * \parameter LinearityConditionWeight: A parameter to weigh the linearity
   *    condition term of the rigidity term. \n
   *    example: <tt>(LinearityConditionWeight 2.0)</tt> \n
   *    Default is 1.0.
   * \parameter OrthonormalityConditionWeight: A parameter to weigh the
   *    orthonormality condition term of the rigidity term. \n
   *    example: <tt>(OrthonormalityConditionWeight 2.0)</tt> \n
   *    Default is 1.0.
   * \parameter PropernessConditionWeight: A parameter to weigh the properness
   *    condition term of the rigidity term. \n
   *    example: <tt>(PropernessConditionWeight 2.0)</tt> \n
   *    Default is 1.0.
   * \parameter UseLinearityCondition: A flag to specify the usage of the
   *    linearity condition term for optimisation. \n
   *    example: <tt>(UseLinearityCondition "false")</tt> \n
   *    Default is "true".
   * \parameter UseOrthonormalityCondition: A flag to specify the usage of
   *    the orthonormality condition term for optimisation. \n
   *    example: <tt>(UseOrthonormalityCondition "false")</tt> \n
   *    Default is "true".
   * \parameter UsePropernessCondition: A flag to specify the usage of the
   *    properness condition term for optimisation. \n
   *    example: <tt>(UsePropernessCondition "false")</tt> \n
   *    Default is "true".
   * \parameter CalculateLinearityCondition: A flag to specify if the linearity
   *    condition should still be calculated, even if it is not used for
   *    optimisation. \n
   *    example: <tt>(CalculateLinearityCondition "false")</tt> \n
   *    Default is "true".
   * \parameter CalculateOrthonormalityCondition: A flag to specify if the
   *    orthonormality condition should still be calculated, even if it is
   *    not used for optimisation. \n
   *    example: <tt>(CalculateOrthonormalityCondition "false")</tt> \n
   *    Default is "true".
   * \parameter CalculatePropernessCondition: A flag to specify if the properness
   *    condition should still be calculated, even if it is not used for
   *    optimisation. \n
   *    example: <tt>(CalculatePropernessCondition "false")</tt> \n
   *    Default is "true".
   * \parameter FixedRigidityImageName: the name of a coefficient image to
   *    specify the rigidity index of voxels in the fixed image. \n
   *    example: <tt>(FixedRigidityImageName "fixedRigidityImage.mhd")</tt> \n
   *    If not supplied the rigidity coefficient is not based on the fixed
   *    image, which is recommended.\n
   *    If neither FixedRigidityImageName nor MovingRigidityImageName are
   *    supplied, the rigidity penalty term is evaluated on the whole transform
   *    input domain.
   * \parameter MovingRigidityImageName: the name of a coefficient image to
   *    specify the rigidity index of voxels in the moving image. \n
   *    example: <tt>(MovingRigidityImageName "movingRigidityImage.mhd")</tt> \n
   *    If not supplied the rigidity coefficient is not based on the moving
   *    image, which is NOT recommended.\n
   *    If neither FixedRigidityImageName nor MovingRigidityImageName are
   *    supplied, the rigidity penalty term is evaluated on the whole transform
   *    input domain.
   * \parameter DilateRigidityImages: flag to specify the dilation of the
   *    rigidity coefficient images. With this the region of rigidity can be
   *    extended to force rigidity of the inner region. \n
   *    example: <tt>(DilateRigidityImages "false" "false" "true")</tt> \n
   *    Default is "true".
   * \parameter DilationRadiusMultiplier: the dilation radius is a multiplier
   *    times the grid spacing of the B-spline transform. \n
   *    example: <tt>(DilationRadiusMultiplier 1.0 1.0 2.0)</tt> \n
   *    Default is 1.0.
   *
   * \ingroup Metrics
   *
   */

template <class TElastix >
class TransformRigidityPenalty
  : public TransformRigidityPenaltyTerm<
  ITK_TYPENAME MetricBase<TElastix>::FixedImageType,
  double >,
  public MetricBase<TElastix>
{
public:

  /** Standard ITK-stuff. */
  typedef TransformRigidityPenalty                      Self;
  typedef TransformRigidityPenaltyTerm<
    typename MetricBase<TElastix>::FixedImageType,
    double >                                            Superclass1;
  typedef MetricBase<TElastix>                          Superclass2;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( TransformRigidityPenalty, TransformRigidityPenaltyTerm );

  /** Name of this class.
   * Use this name in the parameter file to select this specific metric. \n
   * example: <tt>(Metric "TransformRigidityPenalty")</tt>\n
   */
  elxClassNameMacro( "TransformRigidityPenalty" );

  /** Typedefs from the superclass. */
  typedef typename Superclass1::CoordinateRepresentationType CoordinateRepresentationType;
  typedef typename Superclass1::MovingImageType             MovingImageType;
  typedef typename Superclass1::MovingImagePixelType        MovingImagePixelType;
  typedef typename Superclass1::MovingImageConstPointer     MovingImageConstPointer;
  typedef typename Superclass1::FixedImageType              FixedImageType;
  typedef typename Superclass1::FixedImageConstPointer      FixedImageConstPointer;
  typedef typename Superclass1::FixedImageRegionType        FixedImageRegionType;
  typedef typename Superclass1::TransformType               TransformType;
  typedef typename Superclass1::TransformPointer            TransformPointer;
  typedef typename Superclass1::InputPointType              InputPointType;
  typedef typename Superclass1::OutputPointType             OutputPointType;
  typedef typename Superclass1::TransformParametersType     TransformParametersType;
  typedef typename Superclass1::TransformJacobianType       TransformJacobianType;
  typedef typename Superclass1::InterpolatorType            InterpolatorType;
  typedef typename Superclass1::InterpolatorPointer         InterpolatorPointer;
  typedef typename Superclass1::RealType                    RealType;
  typedef typename Superclass1::GradientPixelType           GradientPixelType;
  typedef typename Superclass1::GradientImageType           GradientImageType;
  typedef typename Superclass1::GradientImagePointer        GradientImagePointer;
  typedef typename Superclass1::GradientImageFilterType     GradientImageFilterType;
  typedef typename Superclass1::GradientImageFilterPointer  GradientImageFilterPointer;
  typedef typename Superclass1::FixedImageMaskType          FixedImageMaskType;
  typedef typename Superclass1::FixedImageMaskPointer       FixedImageMaskPointer;
  typedef typename Superclass1::MovingImageMaskType         MovingImageMaskType;
  typedef typename Superclass1::MovingImageMaskPointer      MovingImageMaskPointer;
  typedef typename Superclass1::MeasureType                 MeasureType;
  typedef typename Superclass1::DerivativeType              DerivativeType;
  typedef typename Superclass1::ParametersType              ParametersType;
  typedef typename Superclass1::FixedImagePixelType         FixedImagePixelType;
  typedef typename Superclass1::MovingImageRegionType       MovingImageRegionType;
  typedef typename Superclass1::ImageSamplerType            ImageSamplerType;
  typedef typename Superclass1::ImageSamplerPointer         ImageSamplerPointer;
  typedef typename Superclass1::ImageSampleContainerType    ImageSampleContainerType;
  typedef typename Superclass1::ImageSampleContainerPointer ImageSampleContainerPointer;
  typedef typename Superclass1::FixedImageLimiterType       FixedImageLimiterType;
  typedef typename Superclass1::MovingImageLimiterType      MovingImageLimiterType;
  typedef typename Superclass1::FixedImageLimiterOutputType FixedImageLimiterOutputType;
  typedef typename Superclass1::MovingImageLimiterOutputType MovingImageLimiterOutputType;

  typedef typename Superclass1::CoefficientImageType        CoefficientImageType;

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

  /** Sets up a timer to measure the initialization time and
   * calls the Superclass' implementation.
   */
  virtual void Initialize( void ) throw (ExceptionObject);

  /**
   * Do some things before each resolution:
   * \li Read all parameters.
   */
  virtual void BeforeEachResolution( void );

  /**
   * Do some things before registration:
   * \li Read the fixed rigidity image.
   * \li Read the moving rigidity image.
   * \li Setup some extra target cells.
   */
  virtual void BeforeRegistration( void );

  /**
   * Do some things after each iteration:
   * \li Print the OC, PC, LC parts of the rigidity term.
   */
  virtual void AfterEachIteration( void );

  /** This metric is advanced (so it has a sampling possibility), but it
   * purposely does not use samplers. The MetricBase class, however, issues
   * a warning if this is the case, so we overwrite that function.
   */
  virtual void SelectNewSamples( void ){};

protected:

  /** The constructor. */
  TransformRigidityPenalty(){};

  /** The destructor. */
  virtual ~TransformRigidityPenalty() {}

private:

  /** The private constructor. */
  TransformRigidityPenalty( const Self& ); // purposely not implemented
  /** The private copy constructor. */
  void operator=( const Self& );              // purposely not implemented

}; // end class TransformRigidityPenalty


} // end namespace elastix


#ifndef ITK_MANUAL_INSTANTIATION
#include "elxTransformRigidityPenaltyTerm.hxx"
#endif

#endif // end #ifndef __elxTransformRigidityPenaltyTerm_H__


/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __elxAffineDTITransform_H__
#define __elxAffineDTITransform_H__

#include "itkAdvancedCombinationTransform.h"
#include "itkAffineDTITransform.h"
#include "itkCenteredTransformInitializer.h"
#include "elxIncludes.h"

namespace elastix
{
using namespace itk;

  /**
   * \class AffineDTITransformElastix
   * \brief A transform based on the itk AffineDTITransform.
   *
   * This transform is an affine transformation, with a different parametrisation
   * than the usual one. It is specialised for MR-DTI applications.
   * See also the description of the itk::AffineDTI3DTransform.
   *
   * \warning: the behaviour of this transform might still change in the future. It is still experimental.
   *
   * The parameters used in this class are:
   * \parameter Transform: Select this transform as follows:\n
   *    <tt>(%Transform "AffineDTITransform")</tt>
   * \parameter Scales: the scale factor for each parameter, used by the optimizer. \n
   *    example: <tt>(Scales -1 -1 -1 100000000 -1 -1 -1 -1 -1 -1 -1 -1)</tt> \n
   *    The scales work a bit different in this transformation, compared with the other
   *    transformations like the EulerTransform. By default, if no scales are
   *    supplied, the scales are estimated automatically. With the scales parameter,
   *    the user can override the automatically determined scales. Values smaller than
   *    or equal to 0 are ignored: the automatically estimated value is used for these
   *    parameters. Positive values override the automatically estimated scales. So,
   *    in the example above, the shear_x parameter is given a very large scale, which
   *    effectively turns off the optimization of that parameters. This might be useful
   *    in some MR-DTI application.
   * \parameter CenterOfRotation: an index around which the image is rotated. \n
   *    example: <tt>(CenterOfRotation 128 128 90)</tt> \n
   *    By default the CenterOfRotation is set to the geometric center of the image.
   * \parameter AutomaticTransformInitialization: whether or not the initial translation
   *    between images should be estimated as the distance between their centers.\n
   *    example: <tt>(AutomaticTransformInitialization "true")</tt> \n
   *    By default "false" is assumed. So, no initial translation.
   *
   * The transform parameters necessary for transformix, additionally defined by this class, are:   *
   * \transformparameter CenterOfRotation: stores the center of rotation as an index. \n
   *    example: <tt>(CenterOfRotation 128 128 90)</tt>\n
   *    <b>depecrated!</b> From elastix version 3.402 this is changed to CenterOfRotationPoint!
   * \transformparameter CenterOfRotationPoint: stores the center of rotation, expressed in world coordinates. \n
   *    example: <tt>(CenterOfRotationPoint 10.555 6.666 12.345)</tt>
   * \transformparameter MatrixTranslation: the parameters as matrix and translation, so written
   *    in the same way as the parameters of the normal AffineTransform.\n
   *    example: <tt>(MatrixTranslation 1 0 0 0 1 0 0 0 0.9 0.1 0.1 0.2)</tt>\n
   *    This parameter is just for convenience, so that the user can compare the results between
   *    the AffineTransform and AffineDTITransform. It is not a mandatory parameter.
   *
   * \ingroup Transforms
   * \sa AffineDTI3DTransform
   */

  template < class TElastix >
    class AffineDTITransformElastix:
      public AdvancedCombinationTransform<
        ITK_TYPENAME elx::TransformBase< TElastix >::CoordRepType,
        elx::TransformBase< TElastix >::FixedImageDimension >,
      public elx::TransformBase< TElastix >
  {
  public:

    /** Standard ITK-stuff.*/
    typedef AffineDTITransformElastix                               Self;

    typedef AdvancedCombinationTransform<
      typename elx::TransformBase< TElastix >::CoordRepType,
      elx::TransformBase< TElastix >::FixedImageDimension >     Superclass1;

    typedef elx::TransformBase< TElastix >                      Superclass2;

    /** The ITK-class that provides most of the functionality, and
     * that is set as the "CurrentTransform" in the CombinationTransform */
    typedef AffineDTITransform<
      typename elx::TransformBase< TElastix >::CoordRepType,
      elx::TransformBase< TElastix >::FixedImageDimension >     AffineDTITransformType;

    typedef SmartPointer<Self>                                  Pointer;
    typedef SmartPointer<const Self>                            ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro( Self );

    /** Run-time type information (and related methods). */
    //itkTypeMacro( AffineDTITransformElastix, AffineDTITransform );
    itkTypeMacro( AffineDTITransformElastix, AdvancedCombinationTransform );

    /** Name of this class.
     * Use this name in the parameter file to select this specific transform. \n
     * example: <tt>(Transform "AffineDTITransform")</tt>\n
     */
    elxClassNameMacro( "AffineDTITransform" );

    /** Dimension of the fixed image. */
    itkStaticConstMacro( SpaceDimension, unsigned int, Superclass2::FixedImageDimension );

    /** Typedefs inherited from the superclass. */

    /** These are both in AffineDTI2D and AffineDTI3D. */
    typedef typename Superclass1::ScalarType                  ScalarType;
    typedef typename Superclass1::ParametersType              ParametersType;
    typedef typename Superclass1::JacobianType                JacobianType;

    typedef typename Superclass1::InputPointType              InputPointType;
    typedef typename Superclass1::OutputPointType             OutputPointType;
    typedef typename Superclass1::InputVectorType             InputVectorType;
    typedef typename Superclass1::OutputVectorType            OutputVectorType;
    typedef typename Superclass1::InputCovariantVectorType    InputCovariantVectorType;
    typedef typename Superclass1::OutputCovariantVectorType   OutputCovariantVectorType;
    typedef typename Superclass1::InputVnlVectorType          InputVnlVectorType;
    typedef typename Superclass1::OutputVnlVectorType         OutputVnlVectorType;

    typedef typename AffineDTITransformType::Pointer              AffineDTITransformPointer;
    typedef typename AffineDTITransformType::OffsetType           OffsetType;

    /** Typedef's inherited from TransformBase. */
    typedef typename Superclass2::ElastixType               ElastixType;
    typedef typename Superclass2::ElastixPointer            ElastixPointer;
    typedef typename Superclass2::ConfigurationType         ConfigurationType;
    typedef typename Superclass2::ConfigurationPointer      ConfigurationPointer;
    typedef typename Superclass2::RegistrationType          RegistrationType;
    typedef typename Superclass2::RegistrationPointer       RegistrationPointer;
    typedef typename Superclass2::CoordRepType              CoordRepType;
    typedef typename Superclass2::FixedImageType            FixedImageType;
    typedef typename Superclass2::MovingImageType           MovingImageType;
    typedef typename Superclass2::ITKBaseType               ITKBaseType;
    typedef typename Superclass2::CombinationTransformType  CombinationTransformType;

    /** Other typedef's. */
    typedef typename FixedImageType::IndexType              IndexType;
    typedef typename IndexType::IndexValueType              IndexValueType;
    typedef typename FixedImageType::SizeType               SizeType;
    typedef typename FixedImageType::PointType              PointType;
    typedef typename FixedImageType::SpacingType            SpacingType;
    typedef typename FixedImageType::RegionType             RegionType;
    typedef typename FixedImageType::DirectionType          DirectionType;

    typedef CenteredTransformInitializer<
      AffineDTITransformType, FixedImageType, MovingImageType>  TransformInitializerType;
    typedef typename TransformInitializerType::Pointer      TransformInitializerPointer;

    /** For scales setting in the optimizer */
    typedef typename Superclass2::ScalesType                ScalesType;

    /** Execute stuff before the actual registration:
     * \li Call InitializeTransform
     * \li Set the scales.
     */
    virtual void BeforeRegistration(void);

    /** Initialize Transform.
     * \li Set all parameters to zero.
     * \li Set center of rotation:
     *  automatically initialized to the geometric center of the image, or
     *   assigned a user entered voxel index, given by the parameter
     *   (CenterOfRotation <index-x> <index-y> ...);
     *   If an initial transform is present and HowToCombineTransforms is
     *   set to "Compose", the initial transform is taken into account
     *   while setting the center of rotation.
     * \li Set initial translation:
     *  the initial translation between fixed and moving image is guessed,
     *  if the user has set (AutomaticTransformInitialization "true").
     *
     * It is not yet possible to enter an initial rotation angle.
     */
    virtual void InitializeTransform(void);

    /** Set the scales
     * \li If AutomaticScalesEstimation is "true" estimate scales
     * \li If scales are provided by the user use those,
     * \li Otherwise use some default value
     * This function is called by BeforeRegistration, after
     * the InitializeTransform function is called
     */
    virtual void SetScales(void);

    /** Function to read transform-parameters from a file.
     *
     * It reads the center of rotation and calls the superclass' implementation.
     */
    virtual void ReadFromFile(void);

    /** Function to write transform-parameters to a file.
     * It writes the center of rotation to file and calls the superclass' implementation.
     */
    virtual void WriteToFile( const ParametersType & param ) const;

  protected:

    /** The constructor. */
    AffineDTITransformElastix();
    /** The destructor. */
    virtual ~AffineDTITransformElastix() {};

    /** Try to read the CenterOfRotationPoint from the transform parameter file
     * The CenterOfRotationPoint is already in world coordinates. */
    virtual bool ReadCenterOfRotationPoint( InputPointType & rotationPoint ) const;

  private:

    /** The private constructor. */
    AffineDTITransformElastix( const Self& ); // purposely not implemented
    /** The private copy constructor. */
    void operator=( const Self& );        // purposely not implemented

    AffineDTITransformPointer       m_AffineDTITransform;

  }; // end class AffineDTITransformElastix


} // end namespace elastix

#ifndef ITK_MANUAL_INSTANTIATION
#include "elxAffineDTITransform.hxx"
#endif

#endif // end #ifndef __elxAffineDTITransform_H__


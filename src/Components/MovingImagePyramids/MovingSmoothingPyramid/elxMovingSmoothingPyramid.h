/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __elxMovingSmoothingPyramid_h
#define __elxMovingSmoothingPyramid_h

#include "itkMultiResolutionGaussianSmoothingPyramidImageFilter.h"
#include "elxIncludes.h"

namespace elastix
{
using namespace itk;

  /**
   * \class MovingSmoothingPyramid
   * \brief A pyramid based on the itkMultiResolutionGaussianSmoothingPyramidImageFilter.
   *
   * The parameters used in this class are:
   * \parameter MovingImagePyramid: Select this pyramid as follows:\n
   *    <tt>(MovingImagePyramid "MovingSmoothingImagePyramid")</tt>
   *
   * \ingroup ImagePyramids
   */

  template <class TElastix>
    class MovingSmoothingPyramid :
    public
      MultiResolutionGaussianSmoothingPyramidImageFilter<
        ITK_TYPENAME MovingImagePyramidBase<TElastix>::InputImageType,
        ITK_TYPENAME MovingImagePyramidBase<TElastix>::OutputImageType >,
    public
      MovingImagePyramidBase<TElastix>
  {
  public:

    /** Standard ITK. */
    typedef MovingSmoothingPyramid                                    Self;
    typedef MultiResolutionGaussianSmoothingPyramidImageFilter<
        typename MovingImagePyramidBase<TElastix>::InputImageType,
        typename MovingImagePyramidBase<TElastix>::OutputImageType >  Superclass1;
    typedef MovingImagePyramidBase<TElastix>                          Superclass2;
    typedef SmartPointer<Self>                                        Pointer;
    typedef SmartPointer<const Self>                                  ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro( Self );

    /** Run-time type information (and related methods). */
    itkTypeMacro( MovingSmoothingPyramid, MultiResolutionGaussianSmoothingPyramidImageFilter );

    /** Name of this class.
     * Use this name in the parameter file to select this specific pyramid. \n
     * example: <tt>(MovingImagePyramid "MovingSmoothingImagePyramid")</tt>\n
     */
    elxClassNameMacro( "MovingSmoothingImagePyramid" );

    /** Get the ImageDimension. */
    itkStaticConstMacro( ImageDimension, unsigned int, Superclass1::ImageDimension );

    /** Typedefs inherited from Superclass1. */
    typedef typename Superclass1::InputImageType            InputImageType;
    typedef typename Superclass1::OutputImageType           OutputImageType;
    typedef typename Superclass1::InputImagePointer         InputImagePointer;
    typedef typename Superclass1::OutputImagePointer        OutputImagePointer;
    typedef typename Superclass1::InputImageConstPointer    InputImageConstPointer;

    /** Typedefs inherited from Elastix. */
    typedef typename Superclass2::ElastixType           ElastixType;
    typedef typename Superclass2::ElastixPointer        ElastixPointer;
    typedef typename Superclass2::ConfigurationType     ConfigurationType;
    typedef typename Superclass2::ConfigurationPointer  ConfigurationPointer;
    typedef typename Superclass2::RegistrationType      RegistrationType;
    typedef typename Superclass2::RegistrationPointer   RegistrationPointer;
    typedef typename Superclass2::ITKBaseType           ITKBaseType;

  protected:

    /** The constructor. */
    MovingSmoothingPyramid() {}
    /** The destructor. */
    virtual ~MovingSmoothingPyramid() {}

  private:

    /** The private constructor. */
    MovingSmoothingPyramid( const Self& );  // purposely not implemented
    /** The private copy constructor. */
    void operator=( const Self& );          // purposely not implemented

  }; // end class MovingSmoothingPyramid


} // end namespace elastix

#ifndef ITK_MANUAL_INSTANTIATION
#include "elxMovingSmoothingPyramid.hxx"
#endif

#endif // end #ifndef __elxMovingSmoothingPyramid_h

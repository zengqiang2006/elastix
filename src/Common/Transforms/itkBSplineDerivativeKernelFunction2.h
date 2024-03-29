/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBSplineDerivativeKernelFunction2.h,v $
  Language:  C++
  Date:      $Date: 2008-06-25 11:00:19 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBSplineDerivativeKernelFunction2_h
#define __itkBSplineDerivativeKernelFunction2_h

#include "itkKernelFunction.h"
#include "vnl/vnl_math.h"


namespace itk
{

/** \class BSplineDerivativeKernelFunction2
 * \brief Derivative of a B-spline kernel used for density estimation and
 *  nonparametric regression.
 *
 * This class encapsulates the derivative of a B-spline kernel for
 * density estimation or nonparametric regression.
 * See documentation for KernelFunction for more details.
 *
 * This class is templated over the spline order.
 * \warning Evaluate is only implemented for spline order 1 to 4
 *
 * \sa KernelFunction
 *
 * \ingroup Functions
 */
template <unsigned int VSplineOrder = 3>
class ITK_EXPORT BSplineDerivativeKernelFunction2 : public KernelFunction
{
public:
  /** Standard class typedefs. */
  typedef BSplineDerivativeKernelFunction2 Self;
  typedef KernelFunction                  Superclass;
  typedef SmartPointer<Self>              Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BSplineDerivativeKernelFunction2, KernelFunction);

  /** Enum of for spline order. */
  itkStaticConstMacro(SplineOrder, unsigned int, VSplineOrder);

  /** Evaluate the function. */
  //inline double Evaluate( const double & u ) const
  //  {
  //  return ( m_KernelFunction->Evaluate( u + 0.5 ) -
  //    m_KernelFunction->Evaluate( u - 0.5 ) );
  //  }
  /** Evaluate the function. */
  inline double Evaluate( const double & u ) const
  {
    return this->Evaluate( Dispatch<VSplineOrder>(), u );
  }

protected:
  BSplineDerivativeKernelFunction2(){};
  ~BSplineDerivativeKernelFunction2(){};

  void PrintSelf(std::ostream& os, Indent indent) const
    {
    Superclass::PrintSelf( os, indent );
    os << indent  << "Spline Order: " << SplineOrder << std::endl;
    }

private:
  BSplineDerivativeKernelFunction2(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Structures to control overloaded versions of Evaluate */
  struct DispatchBase {};
  template<unsigned int>
  struct Dispatch : DispatchBase {};

  /** Zeroth order spline. */
  /**inline double Evaluate (const Dispatch<0>&, const double & u) const
  {

    double absValue = vnl_math_abs( u );

    if ( absValue  < 0.5 )
    {
      return 1.0;
    }
    else if ( absValue == 0.5 )
    {
      return 0.5;
    }
    else
    {
      return 0.0;
    }

  }*/

  /** First order spline */
  inline double Evaluate ( const Dispatch<1>&, const double& u) const
  {

    double absValue = vnl_math_abs( u );

    if ( absValue < 1.0 )
    {
      return -vnl_math_sgn( u );
    }
    else if ( absValue == 1.0 )
    {
      return -vnl_math_sgn( u ) / 2.0;
    }
    else
    {
      return 0.0;
    }

  }

  /** Second order spline. */
  inline double Evaluate ( const Dispatch<2>&, const double& u) const
  {
    double absValue = vnl_math_abs( u );

    if ( absValue < 0.5 )
    {
      return -2.0 * u;
    }
    else if ( absValue < 1.5 )
    {
      return u - 1.5 * vnl_math_sgn( u );
    }
    else
    {
      return 0.0;
    }

  }

  /**  Third order spline. */
  inline double Evaluate ( const Dispatch<3>&, const double& u) const
  {
    const double absValue = vnl_math_abs( u );
    const double sqrValue = vnl_math_sqr( u );

    if ( absValue < 1.0 )
    {
      if ( u > 0.0)
      {
        const double dummy = vnl_math_abs( u + 0.5 );
        return ( 6.0 * sqrValue - 2.0 * u - 6.0 * dummy + 3.0 ) / 4.0;
      }
      else
      {
        const double dummy = vnl_math_abs( u - 0.5 );
        return -( 6.0 * sqrValue + 2.0 * u - 6.0 * dummy + 3.0 ) / 4.0;
      }
    }
    else if ( absValue < 2.0 )
    {
      if ( u > 0.0 )
      {
        const double dummy = vnl_math_abs( u - 0.5 );
        return ( u - sqrValue + 3.0 * dummy - 2.5 ) / 2.0;
      }
      else
      {
        const double dummy = vnl_math_abs( u + 0.5 );
        return ( u + sqrValue - 3.0 * dummy + 2.5 ) / 2.0;
      }
    }
    else
    {
      return 0.0;
    }

  }

  /** Unimplemented spline order */
  inline double Evaluate ( const DispatchBase&, const double&) const
  {
    itkExceptionMacro("Evaluate not implemented for spline\
                      order " << SplineOrder);
    return 0.0; // This is to avoid compiler warning about missing
    // return statement.  It should never be evaluated.
  }

};


} // end namespace itk

#endif

/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __itkAdaptiveStochasticGradientDescentOptimizer_h
#define __itkAdaptiveStochasticGradientDescentOptimizer_h

#include "../StandardGradientDescent/itkStandardGradientDescentOptimizer.h"

namespace itk
{

  /**
  * \class AdaptiveStochasticGradientDescentOptimizer
  * \brief This class implements a gradient descent optimizer with adaptive gain.
  *
  * If \f$C(x)\f$ is a costfunction that has to be minimised, the following iterative
  * algorithm is used to find the optimal parameters \f$x\f$:
  *
  *     \f[ x(k+1) = x(k) - a(t_k) dC/dx \f]
  *
  * The gain \f$a(t_k)\f$ at each iteration \f$k\f$ is defined by:
  *
  *     \f[ a(t_k) =  a / (A + t_k + 1)^alpha \f].
  *
  * And the time \f$t_k\f$ is updated according to:
  *
  *     \f[ t_{k+1} = [ t_k + sigmoid( -g_k^T g_{k-1} ) ]^+ \f]
  *
  * where \f$g_k\f$ equals \f$dC/dx\f$ at iteration \f$k\f$.
  * For \f$t_0\f$ the InitialTime is used, which is defined in the
  * the superclass (StandardGradientDescentOptimizer). Whereas in the
  * superclass this parameter is superfluous, in this class it makes sense.
  *
  * This method is described in the following references:
  *
  * [1] P. Cruz,
  * "Almost sure convergence and asymptotical normality of a generalization of Kesten's
  * stochastic approximation algorithm for multidimensional case."
  * Technical Report, 2005. http://hdl.handle.net/2052/74
  *
  * [2] S. Klein, J.P.W. Pluim, and M. Staring, M.A. Viergever, 
  * "Adaptive stochastic gradient descent optimisation for image registration,"
  * International Journal of Computer Vision, vol. 81, no. 3, pp. 227-239, 2009.
  * http://dx.doi.org/10.1007/s11263-008-0168-y

  * It is very suitable to be used in combination with a stochastic estimate
  * of the gradient \f$dC/dx\f$. For example, in image registration problems it is
  * often advantageous to compute the metric derivative (\f$dC/dx\f$) on a new set
  * of randomly selected image samples in each iteration. You may set the parameter
  * \c NewSamplesEveryIteration to \c "true" to achieve this effect.
  * For more information on this strategy, you may have a look at:
  *
  * \sa AdaptiveStochasticGradientDescent, StandardGradientDescentOptimizer
  * \ingroup Optimizers
  */

  class AdaptiveStochasticGradientDescentOptimizer :
    public StandardGradientDescentOptimizer
  {
  public:

    /** Standard ITK.*/
    typedef AdaptiveStochasticGradientDescentOptimizer   Self;
    typedef StandardGradientDescentOptimizer      Superclass;

    typedef SmartPointer<Self>                  Pointer;
    typedef SmartPointer<const Self>            ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro( Self );

    /** Run-time type information (and related methods). */
    itkTypeMacro( AdaptiveStochasticGradientDescentOptimizer,
      StandardGradientDescentOptimizer );

    /** Typedefs inherited from the superclass. */
    typedef Superclass::MeasureType               MeasureType;
    typedef Superclass::ParametersType            ParametersType;
    typedef Superclass::DerivativeType            DerivativeType;
    typedef Superclass::CostFunctionType          CostFunctionType;
    typedef Superclass::ScalesType                ScalesType;
    typedef Superclass::ScaledCostFunctionType    ScaledCostFunctionType;
    typedef Superclass::ScaledCostFunctionPointer ScaledCostFunctionPointer;
    typedef Superclass::StopConditionType         StopConditionType;

    /** Set/Get whether the adaptive step size mechanism is desired. Default: true */
    itkSetMacro( UseAdaptiveStepSizes, bool );
    itkGetConstMacro( UseAdaptiveStepSizes, bool );

    /** Set/Get the maximum of the sigmoid.
    * Should be >0. Default: 1.0 */
    itkSetMacro(SigmoidMax, double);
    itkGetConstMacro(SigmoidMax, double);

    /** Set/Get the maximum of the sigmoid.
    * Should be <0. Default: -0.8 */
    itkSetMacro(SigmoidMin, double);
    itkGetConstMacro(SigmoidMin, double);

    /** Set/Get the scaling of the sigmoid width. Large values
    * cause a more wide sigmoid. Default: 1e-8. Should be >0. */
    itkSetMacro(SigmoidScale, double);
    itkGetConstMacro(SigmoidScale, double);

  protected:

    AdaptiveStochasticGradientDescentOptimizer();
    virtual ~AdaptiveStochasticGradientDescentOptimizer() {};

    /** Function to update the current time
    * If UseAdaptiveStepSizes is false this function just increments
    * the CurrentTime by \f$E_0 = (sigmoid_{max} + sigmoid_{min})/2\f$.
    * Else, the CurrentTime is updated according to:\n
    * time = max[ 0, time + sigmoid( -gradient*previousgradient) ]\n
    * In that case, also the m_PreviousGradient is updated.
    */
    virtual void UpdateCurrentTime( void );

    /** The PreviousGradient, necessary for the CruzAcceleration */
    DerivativeType m_PreviousGradient;

  private:

    AdaptiveStochasticGradientDescentOptimizer( const Self& ); // purposely not implemented
    void operator=( const Self& );              // purposely not implemented

    /** Settings */
    bool                          m_UseAdaptiveStepSizes;
    double                        m_SigmoidMax;
    double                        m_SigmoidMin;
    double                        m_SigmoidScale;

  }; // end class AdaptiveStochasticGradientDescentOptimizer


} // end namespace itk


#endif // end #ifndef __itkAdaptiveStochasticGradientDescentOptimizer_h



/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __elxConjugateGradient_h
#define __elxConjugateGradient_h

#include "itkGenericConjugateGradientOptimizer.h"
#include "itkMoreThuenteLineSearchOptimizer.h"
#include "elxIncludes.h"

namespace elastix
{
using namespace itk;


  /**
   * \class ConjugateGradient
   * \brief An optimizer based on the itk::GenericConjugateGradientOptimizer.
   *
   * A ConjugateGradient optimizer, using the itk::MoreThuenteLineSearchOptimizer.
   * Different conjugate gradient methods can be selected with this optimizer.
   *
   * This optimizer support the NewSamplesEveryIteration option. It requests
   * new samples for the computation of each search direction (not during
   * the line search). Actually this makes no sense for a conjugate gradient optimizer.
   * So, think twice before using the NewSamplesEveryIteration option.
   *
   * The parameters used in this class are:
   * \parameter Optimizer: Select this optimizer as follows:\n
   *    <tt>(Optimizer "ConjugateGradient")</tt>
   * \parameter GenerateLineSearchIterations: Whether line search iteration
   *   should be counted as elastix-iterations.\n
   *   example: <tt>(GenerateLineSearchIterations "true")</tt>\n
   *   Can only be specified for all resolutions at once. \n
   *   Default value: "false".\n
   * \parameter MaximumNumberOfIterations: The maximum number of iterations in each resolution. \n
   *    example: <tt>(MaximumNumberOfIterations 100 100 50)</tt> \n
   *    Default value: 100.\n
   * \parameter MaximumNumberOfLineSearchIterations: The maximum number of iterations in each resolution. \n
   *    example: <tt>(MaximumNumberOfIterations 10 10 5)</tt> \n
   *    Default value: 10.\n
   * \parameter StepLength: Set the length of the initial step tried by the
   *    itk::MoreThuenteLineSearchOptimizer.\n
   *    example: <tt>(StepLength 2.0 1.0 0.5)</tt> \n
   *    Default value: 1.0.\n
   * \parameter LineSearchValueTolerance: Determine the Wolfe conditions that the
   *    itk::MoreThuenteLineSearchOptimizer tries to satisfy.\n
   *    example: <tt>(LineSearchValueTolerance 0.0001 0.0001 0.0001)</tt> \n
   *    Default value: 0.0001.\n
   * \parameter LineSearchGradientTolerance: Determine the Wolfe conditions that the
   *    itk::MoreThuenteLineSearchOptimizer tries to satisfy.\n
   *    example: <tt>(LineSearchGradientTolerance 0.9 0.9 0.9)</tt> \n
   *    Default value: 0.9.\n
   * \parameter ValueTolerance: Stopping criterion. See the documentation of the
   *    itk::GenericConjugateGradientOptimizer for more information.\n
   *    example: <tt>(ValueTolerance 0.001 0.0001 0.000001)</tt> \n
   *    Default value: 0.00001.\n
   * \parameter GradientMagnitudeTolerance: Stopping criterion. See the documentation of the
   *    itk::GenericConjugateGradientOptimizer for more information.\n
   *    example: <tt>(GradientMagnitudeTolerance 0.001 0.0001 0.000001)</tt> \n
   *    Default value: 0.000001.\n
   * \parameter ConjugateGradientType: a string that defines how 'beta' is computed in each resolution.
   *    The following methods are implemented: "SteepestDescent", "FletcherReeves", "PolakRibiere",
   *    "DaiYuan", "HestenesStiefel", and "DaiYuanHestenesStiefel". "SteepestDescent" simply sets beta=0.
   *    See the source code of the GenericConjugateGradientOptimizer for more information.\n
   *    example: <tt>(ConjugateGradientType "FletcherReeves" "PolakRibiere")</tt> \n
   *    Default value: "DaiYuanHestenesStiefel".\n
   * \parameter StopIfWolfeNotSatisfied: Whether to stop the optimisation if in one iteration
   *    the Wolfe conditions can not be satisfied by the itk::MoreThuenteLineSearchOptimizer.\n
   *    In general it is wise to do so.\n
   *    example: <tt>(StopIfWolfeNotSatisfied "true" "false")</tt> \n
   *    Default value: "true".\n
   *
   *
   * \ingroup Optimizers
   */

  template <class TElastix>
    class ConjugateGradient :
    public
      itk::GenericConjugateGradientOptimizer,
    public
      OptimizerBase<TElastix>
  {
  public:

    /** Standard ITK.*/
    typedef ConjugateGradient                   Self;
    typedef GenericConjugateGradientOptimizer           Superclass1;
    typedef OptimizerBase<TElastix>             Superclass2;
    typedef SmartPointer<Self>                  Pointer;
    typedef SmartPointer<const Self>            ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro( Self );

    /** Run-time type information (and related methods). */
    itkTypeMacro( ConjugateGradient, GenericConjugateGradientOptimizer );

    /** Name of this class.
     * Use this name in the parameter file to select this specific optimizer. \n
     * example: <tt>(Optimizer "ConjugateGradient")</tt>\n
     */
    elxClassNameMacro( "ConjugateGradient" );

    /** Typedef's inherited from Superclass1.*/
    typedef Superclass1::CostFunctionType                   CostFunctionType;
    typedef Superclass1::CostFunctionPointer                CostFunctionPointer;
    typedef Superclass1::StopConditionType                  StopConditionType;
    typedef Superclass1::ParametersType                     ParametersType;
    typedef Superclass1::DerivativeType                     DerivativeType;
    typedef Superclass1::ScalesType                         ScalesType;

    /** Typedef's inherited from Elastix.*/
    typedef typename Superclass2::ElastixType           ElastixType;
    typedef typename Superclass2::ElastixPointer        ElastixPointer;
    typedef typename Superclass2::ConfigurationType     ConfigurationType;
    typedef typename Superclass2::ConfigurationPointer  ConfigurationPointer;
    typedef typename Superclass2::RegistrationType      RegistrationType;
    typedef typename Superclass2::RegistrationPointer   RegistrationPointer;
    typedef typename Superclass2::ITKBaseType           ITKBaseType;

    /** Extra typedefs */
    typedef MoreThuenteLineSearchOptimizer              LineOptimizerType;
    typedef LineOptimizerType::Pointer                  LineOptimizerPointer;
    typedef ReceptorMemberCommand<Self>                 EventPassThroughType;
    typedef typename EventPassThroughType::Pointer      EventPassThroughPointer;

    /** Check if any scales are set, and set the UseScales flag on or off;
     * after that call the superclass' implementation */
    virtual void StartOptimization(void);

    /** Methods to set parameters and print output at different stages
     * in the registration process.*/
    virtual void BeforeRegistration(void);
    virtual void BeforeEachResolution(void);
    virtual void AfterEachResolution(void);
    virtual void AfterEachIteration(void);
    virtual void AfterRegistration(void);

    itkGetConstMacro(StartLineSearch, bool);


  protected:

    ConjugateGradient();
    virtual ~ConjugateGradient() {};

    LineOptimizerPointer          m_LineOptimizer;

    /** Convert the line search stop condition to a string */
    virtual std::string GetLineSearchStopCondition(void) const;

    /** Generate a string, representing the phase of optimisation
     * (line search, main) */
    virtual std::string DeterminePhase(void) const;

    /** Reimplement the superclass. Calls the superclass' implementation
     * and checks if the MoreThuente line search routine has stopped with
     * Wolfe conditions satisfied. */
    virtual bool TestConvergence( bool firstLineSearchDone );

    /** Call the superclass' implementation. If an ExceptionObject is caught,
     * because the line search optimizer tried a too big step, the exception
     * is printed, but ignored further. The optimizer stops, but elastix
     * just goes on to the next resolution. */
    virtual void LineSearch(
      const ParametersType searchDir,
      double & step,
      ParametersType & x,
      MeasureType & f,
      DerivativeType & g );

  private:

    ConjugateGradient( const Self& ); // purposely not implemented
    void operator=( const Self& );              // purposely not implemented

    void InvokeIterationEvent(const EventObject & event);

    EventPassThroughPointer     m_EventPasser;
    double                      m_SearchDirectionMagnitude;
    bool                        m_StartLineSearch;
    bool                        m_GenerateLineSearchIterations;
    bool                        m_StopIfWolfeNotSatisfied;
    bool                        m_WolfeIsStopCondition;

  }; // end class ConjugateGradient


} // end namespace elastix

#ifndef ITK_MANUAL_INSTANTIATION
#include "elxConjugateGradient.hxx"
#endif

#endif // end #ifndef __elxConjugateGradient_h


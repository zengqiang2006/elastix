/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __elxSimultaneousPerturbation_hxx
#define __elxSimultaneousPerturbation_hxx

#include "elxSimultaneousPerturbation.h"
#include <iomanip>
#include <string>
#include "vnl/vnl_math.h"

namespace elastix
{
using namespace itk;


  /**
   * ********************* Constructor ****************************
   */

  template <class TElastix>
    SimultaneousPerturbation<TElastix>
    ::SimultaneousPerturbation()
  {
    this->m_ShowMetricValues = false;
  } // end Constructor


  /**
   * ***************** BeforeRegistration ***********************
   */

  template <class TElastix>
    void SimultaneousPerturbation<TElastix>::
    BeforeRegistration(void)
  {
    std::string showMetricValues("false");
    this->GetConfiguration()->ReadParameter(
      showMetricValues, "ShowMetricValues", 0);
    if (showMetricValues == "false")
    {
      this->m_ShowMetricValues = false;
    }
    else
    {
      this->m_ShowMetricValues = true;
    }

    /** Add the target cell "stepsize" to xout["iteration"].*/
    xout["iteration"].AddTargetCell("2:Metric");
    xout["iteration"].AddTargetCell("3:Gain a_k");
    xout["iteration"].AddTargetCell("4:||Gradient||");

    /** Format the metric and stepsize as floats */
    xl::xout["iteration"]["2:Metric"]   << std::showpoint << std::fixed;
    xl::xout["iteration"]["3:Gain a_k"] << std::showpoint << std::fixed;
    xl::xout["iteration"]["4:||Gradient||"] << std::showpoint << std::fixed;

  } // end BeforeRegistration


  /**
   * ***************** BeforeEachResolution ***********************
   */

  template <class TElastix>
    void SimultaneousPerturbation<TElastix>
    ::BeforeEachResolution(void)
  {
    /** Get the current resolution level.*/
    unsigned int level = static_cast<unsigned int>(
      this->m_Registration->GetAsITKBaseType()->GetCurrentLevel() );

    /** Set the maximumNumberOfIterations.*/
    unsigned int maximumNumberOfIterations = 500;
    this->m_Configuration->ReadParameter( maximumNumberOfIterations ,
      "MaximumNumberOfIterations", this->GetComponentLabel(), level, 0 );
    this->SetMaximumNumberOfIterations( maximumNumberOfIterations );

    /** Set the number of perturbation used to construct a gradient estimate g_k. */
    unsigned int numberOfPerturbations = 1;
    this->m_Configuration->ReadParameter( numberOfPerturbations,
      "NumberOfPerturbations", this->GetComponentLabel(), level, 0 );
    this->SetNumberOfPerturbations( numberOfPerturbations );

    /** \todo call the GuessParameters function */
    double a = 400.0;
    double c = 1.0;
    double A = 50.0;
    double alpha = 0.602;
    double gamma = 0.101;

    this->GetConfiguration()->ReadParameter(a, "SP_a", this->GetComponentLabel(), level, 0 );
    this->GetConfiguration()->ReadParameter(c, "SP_c", this->GetComponentLabel(), level, 0 );
    this->GetConfiguration()->ReadParameter(A, "SP_A", this->GetComponentLabel(), level, 0 );
    this->GetConfiguration()->ReadParameter(alpha, "SP_alpha", this->GetComponentLabel(), level, 0 );
    this->GetConfiguration()->ReadParameter(gamma, "SP_gamma", this->GetComponentLabel(), level, 0 );

    this->Seta( a );
    this->Setc( c );
    this->SetA( A );
    this->SetAlpha( alpha );
    this->SetGamma( gamma );

    /** Ignore the build-in stop criterion; it's quite ad hoc. */
    this->SetTolerance(0.0);

  } // end BeforeEachResolution


  /**
   * ***************** AfterEachIteration *************************
   */

  template <class TElastix>
    void SimultaneousPerturbation<TElastix>
    ::AfterEachIteration(void)
  {
    /** Print some information */

    if (this->m_ShowMetricValues)
    {
      xl::xout["iteration"]["2:Metric"]   << this->GetValue();
    }
    else
    {
      xl::xout["iteration"]["2:Metric"]   << "---";
    }

    xl::xout["iteration"]["3:Gain a_k"] << this->GetLearningRate();
    xl::xout["iteration"]["4:||Gradient||"] << this->GetGradientMagnitude();

    /** Select new spatial samples for the computation of the metric
     * \todo You may also choose to select new samples upon every evaluation
     * of the metric value
     */
    if ( this->GetNewSamplesEveryIteration() )
    {
      this->SelectNewSamples();
    }

  } // end AfterEachIteration


  /**
   * ***************** AfterEachResolution *************************
   */

  template <class TElastix>
    void SimultaneousPerturbation<TElastix>
    ::AfterEachResolution(void)
  {

    /**
     * enum   StopConditionType {  MaximumNumberOfIterations, MetricError }
     * ignore the BelowTolerance-criterion.
     */
    std::string stopcondition;


    switch( this->GetStopCondition() )
    {

    case MaximumNumberOfIterations :
      stopcondition = "Maximum number of iterations has been reached";
      break;

    case MetricError :
      stopcondition = "Error in metric";
      break;

    default:
      stopcondition = "Unknown";
      break;

    }
    /** Print the stopping condition */


    elxout << "Stopping condition: " << stopcondition << "." << std::endl;

  } // end AfterEachResolution

  /**
   * ******************* AfterRegistration ************************
   */

  template <class TElastix>
    void SimultaneousPerturbation<TElastix>
    ::AfterRegistration(void)
  {
    /** Print the best metric value */
    double bestValue;

    bestValue = this->GetValue();
    elxout
      << std::endl
      << "Final metric value  = "
      << bestValue
      << std::endl;

  } // end AfterRegistration


  /**
   * ******************* SetInitialPosition ***********************
   */

  template <class TElastix>
    void SimultaneousPerturbation<TElastix>
    ::SetInitialPosition( const ParametersType & param )
  {
    /** Override the implementation in itkOptimizer.h, to
     * ensure that the scales array and the parameters
     * array have the same size.
     */

    /** Call the Superclass' implementation. */
    this->Superclass1::SetInitialPosition( param );

    /** Set the scales array to the same size if the size has been changed */
    ScalesType scales = this->GetScales();
    unsigned int paramsize = param.Size();

    if ( ( scales.Size() ) != paramsize )
    {
      ScalesType newscales( paramsize );
      newscales.Fill(1.0);
      this->SetScales( newscales );
    }

    /** \todo to optimizerbase? */

  } // end SetInitialPosition


} // end namespace elastix

#endif // end #ifndef __elxSimultaneousPerturbation_hxx


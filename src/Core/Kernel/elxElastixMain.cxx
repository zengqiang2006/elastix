/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __elxElastixMain_cxx
#define __elxElastixMain_cxx


/** If running on a Windows-system, include "windows.h".
 *  This is to set the priority, but which does not work on cygwin.
 */

#if defined(_WIN32) && !defined(__CYGWIN__)
  #include <windows.h>
#endif

#include "elxElastixMain.h"
#include "elxMacro.h"
#include "itkMultiThreader.h"


namespace elastix
{

using namespace xl;

/**
 * ******************* Global variables *************************
 *
 * Some global variables (not part of the ElastixMain class, used
 * by xoutSetup.
 */

/** \todo move to ElastixMain class, as static vars? */

/** xout TargetCells. */
xoutbase_type   g_xout;
xoutsimple_type g_WarningXout;
xoutsimple_type g_ErrorXout;
xoutsimple_type g_StandardXout;
xoutsimple_type g_CoutOnlyXout;
xoutsimple_type g_LogOnlyXout;
std::ofstream   g_LogFileStream;

/**
 * ********************* xoutSetup ******************************
 *
 * NB: this function is a global function, not part of the ElastixMain
 * class!!
 */

int xoutSetup( const char * logfilename )
{
  /** The namespace of xout. */
  using namespace xl;

  int returndummy = 0;
  set_xout( &g_xout );

  /** Open the logfile for writing. */
  g_LogFileStream.open( logfilename );
  if ( !g_LogFileStream.is_open() )
  {
    std::cerr << "ERROR: LogFile cannot be opened!" << std::endl;
    return 1;
  }

  /** Set std::cout and the logfile as outputs of xout. */
  returndummy |= xout.AddOutput("log", &g_LogFileStream);
  returndummy |= xout.AddOutput("cout", &std::cout);

  /** Set outputs of LogOnly and CoutOnly. */
  returndummy |= g_LogOnlyXout.AddOutput( "log", &g_LogFileStream );
  returndummy |= g_CoutOnlyXout.AddOutput( "cout", &std::cout );

  /** Copy the outputs to the warning-, error- and standard-xouts. */
  g_WarningXout.SetOutputs( xout.GetCOutputs() );
  g_ErrorXout.SetOutputs( xout.GetCOutputs() );
  g_StandardXout.SetOutputs( xout.GetCOutputs() );

  g_WarningXout.SetOutputs( xout.GetXOutputs() );
  g_ErrorXout.SetOutputs( xout.GetXOutputs() );
  g_StandardXout.SetOutputs( xout.GetXOutputs() );

  /** Link the warning-, error- and standard-xouts to xout. */
  returndummy |= xout.AddTargetCell( "warning", &g_WarningXout );
  returndummy |= xout.AddTargetCell( "error", &g_ErrorXout );
  returndummy |= xout.AddTargetCell( "standard", &g_StandardXout );
  returndummy |= xout.AddTargetCell( "logonly", &g_LogOnlyXout );
  returndummy |= xout.AddTargetCell( "coutonly", &g_CoutOnlyXout );

  /** Format the output. */
  xout["standard"] << std::fixed;
  xout["standard"] << std::showpoint;

  /** Return a value. */
  return returndummy;

} // end xoutSetup()


/**
 * ********************* Constructor ****************************
 */

ElastixMain::ElastixMain()
{
  /** Initialize the components. */
  this->m_Configuration = ConfigurationType::New();

  this->m_Elastix = 0;

  this->m_FixedImagePixelType = "";
  this->m_FixedImageDimension = 0;

  this->m_MovingImagePixelType = "";
  this->m_MovingImageDimension = 0;

  this->m_DBIndex = 0;

  this->m_FixedImageContainer = 0;
  this->m_MovingImageContainer = 0;

  this->m_FinalTransform = 0;
  this->m_InitialTransform = 0;

} // end Constructor


/**
 * ****************** Initialization of static members *********
 */

ElastixMain::ComponentDatabasePointer ElastixMain::s_CDB = 0;
ElastixMain::ComponentLoaderPointer ElastixMain::s_ComponentLoader = 0;

/**
* ********************** Destructor ****************************
*/

ElastixMain::~ElastixMain()
{
  //nothing
} // end Destructor


/**
 * *************** EnterCommandLineParameters *******************
 */

void ElastixMain
::EnterCommandLineArguments( ArgumentMapType & argmap )
{
  /** Initialize the configuration object with the
   * command line parameters entered by the user.
   */
  int dummy = this->m_Configuration->Initialize( argmap );
  if ( dummy )
  {
    xout["error"] << "ERROR: Something went wrong during initialisation of the configuration object." << std::endl;
  }

} // end EnterCommandLineParameters()


/**
 * **************************** Run *****************************
 *
 * Assuming EnterCommandLineParameters has already been invoked.
 * or that m_Configuration is initialised in another way.
 */

int ElastixMain::Run( void )
{
  /** Set process properties. */
  this->SetProcessPriority();
  this->SetMaximumNumberOfThreads();

  /** Initialize database. */
  int errorCode = this->InitDBIndex();
  if ( errorCode != 0 )
  {
    return errorCode;
  }

  /** Create the elastix component. */
  try
  {
    /** Key "Elastix", see elxComponentLoader::InstallSupportedImageTypes(). */
    this->m_Elastix = this->CreateComponent( "Elastix" );
  }
  catch( itk::ExceptionObject & excp )
  {
    /** We just print the exception and let the program quit. */
    xl::xout["error"] << excp << std::endl;
    errorCode = 1;
    return errorCode;
  }

  /** Set some information in the ElastixBase. */
  this->GetElastixBase()->SetConfiguration( this->m_Configuration );
  this->GetElastixBase()->SetComponentDatabase( this->s_CDB );
  this->GetElastixBase()->SetDBIndex( this->m_DBIndex );

  /** Populate the component containers. ImageSampler is not mandatory.
   * No defaults are specified for ImageSampler, Metric, Transform
   * and Optimizer.
   */
  this->GetElastixBase()->SetRegistrationContainer(
    this->CreateComponents( "Registration", "MultiResolutionRegistration",
    errorCode ) );

  this->GetElastixBase()->SetFixedImagePyramidContainer(
    this->CreateComponents( "FixedImagePyramid", "FixedSmoothingImagePyramid",
    errorCode ) );

  this->GetElastixBase()->SetMovingImagePyramidContainer(
    this->CreateComponents( "MovingImagePyramid", "MovingSmoothingImagePyramid",
    errorCode ) );

  this->GetElastixBase()->SetImageSamplerContainer(
    this->CreateComponents( "ImageSampler", "", errorCode, false ) );

  this->GetElastixBase()->SetInterpolatorContainer(
    this->CreateComponents( "Interpolator", "BSplineInterpolator",
    errorCode ) );

  this->GetElastixBase()->SetMetricContainer(
    this->CreateComponents( "Metric", "", errorCode ) );

  this->GetElastixBase()->SetOptimizerContainer(
    this->CreateComponents( "Optimizer", "", errorCode ) );

  this->GetElastixBase()->SetResampleInterpolatorContainer(
    this->CreateComponents( "ResampleInterpolator", "FinalBSplineInterpolator",
    errorCode ) );

  this->GetElastixBase()->SetResamplerContainer(
    this->CreateComponents( "Resampler", "DefaultResampler",
    errorCode ) );

  this->GetElastixBase()->SetTransformContainer(
    this->CreateComponents( "Transform", "", errorCode ) );

  /** Check if all component could be created. */
  if ( errorCode != 0 )
  {
    xout["error"] << "ERROR:" << std::endl;
    xout["error"] << "One or more components could not be created." << std::endl;
    return 1;
  }

  /** Set the images and masks. If not set by the user, it is not a problem.
   * ElastixTemplate will try to load them from disk.
   */
  this->GetElastixBase()->SetFixedImageContainer( this->GetFixedImageContainer() );
  this->GetElastixBase()->SetMovingImageContainer( this->GetMovingImageContainer() );
  this->GetElastixBase()->SetFixedMaskContainer( this->GetFixedMaskContainer() );
  this->GetElastixBase()->SetMovingMaskContainer( this->GetMovingMaskContainer() );

  /** Set the initial transform, if it happens to be there. */
  this->GetElastixBase()->SetInitialTransform( this->GetInitialTransform() );

  /** Set the original fixed image direction cosines (relevant in case the
   * UseDirectionCosines parameter was set to false.
   */
  this->GetElastixBase()->SetOriginalFixedImageDirectionFlat(
    this->GetOriginalFixedImageDirectionFlat() );

  /** Run elastix! */
  try
  {
    errorCode = this->GetElastixBase()->Run();
  }
  catch( itk::ExceptionObject & excp1 )
  {
    /** We just print the itk::exception and let the program quit. */
    xl::xout["error"] << excp1 << std::endl;
    errorCode = 1;
  }
  catch ( std::exception & excp2 )
  {
    /** We just print the std::exception and let the program quit. */
    xl::xout["error"] << "std: " << excp2.what() << std::endl;
    errorCode = 1;
  }
  catch ( ... )
  {
    /** We don't know what happened and just print a general message. */
    xl::xout["error"] << "ERROR: an unknown non-ITK, non-std exception was caught.\n"
      << "Please report this to elastix.support@gmail.com." << std::endl;
    errorCode = 1;
  }

  /** Return the final transform. */
  this->m_FinalTransform = this->GetElastixBase()->GetFinalTransform();

  /** Store the images in ElastixMain. */
  this->SetFixedImageContainer(  this->GetElastixBase()->GetFixedImageContainer() );
  this->SetMovingImageContainer( this->GetElastixBase()->GetMovingImageContainer() );
  this->SetFixedMaskContainer(  this->GetElastixBase()->GetFixedMaskContainer() );
  this->SetMovingMaskContainer( this->GetElastixBase()->GetMovingMaskContainer() );

  /** Store the original fixed image direction cosines (relevant in case the
   * UseDirectionCosines parameter was set to false. */
  this->SetOriginalFixedImageDirectionFlat(
    this->GetElastixBase()->GetOriginalFixedImageDirectionFlat() );

  /** Return a value. */
  return errorCode;

} // end Run()


/**
 * **************************** Run *****************************
 *
 * Calls EnterCommandLineParameters and then Run().
 */

int ElastixMain::Run( ArgumentMapType & argmap )
{

  this->EnterCommandLineArguments( argmap );
  return this->Run();

} // end Run()


/**
* ************************** InitDBIndex ***********************
*
* Checks if the configuration object has been initialized,
* determines the requested ImageTypes, and sets the m_DBIndex
* to the corresponding value (by asking the elx::ComponentDatabase).
*/

int ElastixMain::InitDBIndex( void )
{
  /** Only do something when the configuration object wasn't initialized yet. */
  if ( this->m_Configuration->IsInitialized() )
  {
    /** FixedImagePixelType. */
    if ( this->m_FixedImagePixelType.empty() )
    {
      /** Try to read it from the parameter file. */
      this->m_FixedImagePixelType = "float";
      this->m_Configuration->ReadParameter( this->m_FixedImagePixelType,
        "FixedInternalImagePixelType", 0 );
    }

    /** MovingImagePixelType. */
    if ( this->m_MovingImagePixelType.empty() )
    {
      /** Try to read it from the parameter file. */
      this->m_MovingImagePixelType = "float";
      this->m_Configuration->ReadParameter( this->m_MovingImagePixelType,
        "MovingInternalImagePixelType", 0 );
    }

    /** FixedImageDimension. */
    if ( this->m_FixedImageDimension == 0 )
    {
      /** Try to read it from the parameter file. */
      this->m_Configuration->ReadParameter( this->m_FixedImageDimension,
        "FixedImageDimension", 0 );

      if ( this->m_FixedImageDimension == 0 )
      {
        xout["error"] << "ERROR:" << std::endl;
        xout["error"] << "The FixedImageDimension is not given." << std::endl;
        return 1;
      }
    }

    /** MovingImageDimension. */
    if ( this->m_MovingImageDimension == 0 )
    {
      /** Try to read it from the parameter file. */
      this->m_Configuration->ReadParameter( this->m_MovingImageDimension,
        "MovingImageDimension", 0 );

      if ( this->m_MovingImageDimension == 0 )
      {
        xout["error"] << "ERROR:" << std::endl;
        xout["error"] << "The MovingImageDimension is not given." << std::endl;
        return 1;
      }
    }

    /** Load the components. */
    if ( this->s_CDB.IsNull() )
    {
      int loadReturnCode = this->LoadComponents();
      if ( loadReturnCode !=0 )
      {
        xout["error"] << "Loading components failed" << std::endl;
        return loadReturnCode;
      }
    }

    if ( this->s_CDB.IsNotNull() )
    {
      /** Get the DBIndex from the ComponentDatabase. */
      this->m_DBIndex = this->s_CDB->GetIndex(
        this->m_FixedImagePixelType,
        this->m_FixedImageDimension,
        this->m_MovingImagePixelType,
        this->m_MovingImageDimension );
      if ( this->m_DBIndex == 0 )
      {
        xout["error"] << "ERROR:" << std::endl;
        xout["error"] << "Something went wrong in the ComponentDatabase" << std::endl;
        return 1;
      }
    } //end if s_CDB!=0

  } // end if m_Configuration->Initialized();
  else
  {
    xout["error"] << "ERROR:" << std::endl;
    xout["error"] << "The configuration object has not been initialized." << std::endl;
    return 1;
  }

  /** Return an OK value. */
  return 0;

} // end InitDBIndex()


/**
 * ********************* SetElastixLevel ************************
 */

void ElastixMain::SetElastixLevel( unsigned int level )
{
  /** Call SetElastixLevel from MyConfiguration. */
  this->m_Configuration->SetElastixLevel( level );

} // end SetElastixLevel()


/**
 * ********************* GetElastixLevel ************************
 */

unsigned int ElastixMain::GetElastixLevel( void )
{
  /** Call GetElastixLevel from MyConfiguration. */
  return this->m_Configuration->GetElastixLevel();

} // end GetElastixLevel()


/**
 * ********************* SetTotalNumberOfElastixLevels ************************
 */

void ElastixMain::SetTotalNumberOfElastixLevels( unsigned int levels )
{
  /** Call SetTotalNumberOfElastixLevels from MyConfiguration. */
  this->m_Configuration->SetTotalNumberOfElastixLevels( levels );

} // end SetTotalNumberOfElastixLevels()


/**
 * ********************* GetTotalNumberOfElastixLevels ************************
 */

unsigned int ElastixMain::GetTotalNumberOfElastixLevels( void )
{
  /** Call GetTotalNumberOfElastixLevels from MyConfiguration. */
  return this->m_Configuration->GetTotalNumberOfElastixLevels();

} // end GetTotalNumberOfElastixLevels()


/**
 * ********************* LoadComponents **************************
 *
 * Store the install function of each component in the
 * component database.
 */

int ElastixMain::LoadComponents( void )
{
  /** Create a ComponentDatabase. */
  if ( this->s_CDB.IsNull() )
  {
    this->s_CDB = ComponentDatabaseType::New();
  }

  /** Create a ComponentLoader and set the database. */
  if ( this->s_ComponentLoader.IsNull() )
  {
    this->s_ComponentLoader = ComponentLoaderType::New();
    this->s_ComponentLoader->SetComponentDatabase( s_CDB );
  }

  /** Get the current program. */
  const char * argv0
    = this->m_Configuration->GetCommandLineArgument( "-argv0" ).c_str();

  /** Load the components. */
  return this->s_ComponentLoader->LoadComponents( argv0 );

} // end LoadComponents()


/**
 * ********************* UnloadComponents **************************
 */

void ElastixMain::UnloadComponents( void )
{
  s_CDB = 0;
  s_ComponentLoader->SetComponentDatabase( 0 );

  if ( s_ComponentLoader )
  {
    s_ComponentLoader->UnloadComponents();
  }

} // end UnloadComponents()


/**
 * ************************* GetElastixBase ***************************
 */

ElastixMain::ElastixBaseType * ElastixMain::GetElastixBase( void ) const
{
  ElastixBaseType * testpointer;

  /** Convert ElastixAsObject to a pointer to an ElastixBaseType. */
  testpointer = dynamic_cast<ElastixBaseType *>( this->m_Elastix.GetPointer() );
  if ( !testpointer )
  {
    itkExceptionMacro( << "Probably GetElastixBase() is called before having called Run()");
  }

  return testpointer;

} // end GetElastixBase()


/**
 * ************************* CreateComponent ***************************
 */

ElastixMain::ObjectPointer ElastixMain::CreateComponent(
  const ComponentDescriptionType & name )
{
  /** A pointer to the New() function. */
  PtrToCreator testcreator = 0;
  ObjectPointer testpointer = 0;
  testcreator = this->s_CDB->GetCreator( name,  this->m_DBIndex );
  testpointer = testcreator ? testcreator() : NULL;
  if ( testpointer.IsNull() )
  {
    itkExceptionMacro( << "The following component could not be created: " << name );
  }

  return testpointer;

} // end CreateComponent()


/**
 * *********************** CreateComponents *****************************
 */

ElastixMain::ObjectContainerPointer ElastixMain::CreateComponents(
  const std::string & key,
  const ComponentDescriptionType & defaultComponentName,
  int & errorcode, bool mandatoryComponent )
{
  ComponentDescriptionType componentName = defaultComponentName;
  unsigned int componentnr = 0;
  ObjectContainerPointer objectContainer = ObjectContainerType::New();
  objectContainer->Initialize();

  /** Read the component name.
   * If the user hasn't specified any component names, use
   * the default, and give a warning.
   */
  bool found = this->m_Configuration->ReadParameter(
    componentName, key, componentnr, true );

  /** If the default equals "" (so no default), the mandatoryComponent
   * flag is true, and not component was given by the user,
   * then elastix quits.
   */
  if ( !found && ( defaultComponentName == "" ) )
  {
    if ( mandatoryComponent )
    {
      xout["error"]
        << "ERROR: the following component has not been specified: "
        << key << std::endl;
      errorcode = 1;
      return objectContainer;
    }
    else
    {
      /* Just return an empty container without nagging. */
      errorcode = 0;
      return objectContainer;
    }
  }

  /** Try creating the specified component. */
  try
  {
    objectContainer->CreateElementAt( componentnr )
      = this->CreateComponent( componentName );
  }
  catch ( itk::ExceptionObject & excp )
  {
    xout["error"]
      << "ERROR: error occurred while creating "
      << key << " "
      << componentnr << "." << std::endl;
    xout["error"] << excp << std::endl;
    errorcode = 1;
    return objectContainer;
  }

  /** Check if more than one component name is given. */
  while ( found )
  {
    ++componentnr;
    found = this->m_Configuration->ReadParameter(
      componentName, key, componentnr, false );
    if ( found )
    {
      try
      {
        objectContainer->CreateElementAt( componentnr )
          = this->CreateComponent( componentName );
      }
      catch ( itk::ExceptionObject & excp )
      {
        xout["error"]
          << "ERROR: error occurred while creating "
          << key << " "
          << componentnr  << "." << std::endl;
        xout["error"] << excp << std::endl;
        errorcode = 1;
        return objectContainer;
      }
    }  // end if
  } // end while

  return objectContainer;

} // end CreateComponents()


/**
 * *********************** SetProcessPriority *************************
 */

void ElastixMain::SetProcessPriority( void ) const
{
  /** If wanted, set the priority of this process high or below normal. */
  std::string processPriority = "";
  processPriority = this->m_Configuration->GetCommandLineArgument( "-priority" );
  if ( processPriority == "high" )
  {
    #if defined(_WIN32) && !defined(__CYGWIN__)
    SetPriorityClass( GetCurrentProcess(), HIGH_PRIORITY_CLASS );
    #endif
  }
  else if ( processPriority == "belownormal" )
  {
    #if defined(_WIN32) && !defined(__CYGWIN__)
    SetPriorityClass( GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS );
    #endif
  }

} // end SetProcessPriority()


/**
 * *********************** SetMaximumNumberOfThreads *************************
 */

void ElastixMain::SetMaximumNumberOfThreads( void ) const
{
  /** Get the number of threads from the command line. */
  std::string maximumNumberOfThreadsString = "";
  maximumNumberOfThreadsString
    = this->m_Configuration->GetCommandLineArgument( "-threads" );

  /** If supplied, set the maximum number of threads. */
  if ( maximumNumberOfThreadsString != "" )
  {
    const int maximumNumberOfThreads =
      atoi( maximumNumberOfThreadsString.c_str() );
    itk::MultiThreader::SetGlobalMaximumNumberOfThreads(
      maximumNumberOfThreads );
  }

} // end SetMaximumNumberOfThreads()


/**
 * ******************** SetOriginalFixedImageDirectionFlat ********************
 */

void ElastixMain::SetOriginalFixedImageDirectionFlat(
  const FlatDirectionCosinesType & arg )
{
  this->m_OriginalFixedImageDirection = arg;
}

/**
 * ******************** GetOriginalFixedImageDirectionFlat ********************
 */

const ElastixMain::FlatDirectionCosinesType &
ElastixMain::GetOriginalFixedImageDirectionFlat( void ) const
{
  return this->m_OriginalFixedImageDirection;
}


} // end namespace elastix

#endif // end #ifndef __elxElastixMain_cxx


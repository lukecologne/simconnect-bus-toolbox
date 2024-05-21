#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME SimConnectBusInterface

#include "simstruc.h"

#include <string>

/*====================*
 * S-function methods *
 *====================*/

/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 3);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        /* Return if number of expected != number of actual parameters */
        return;
    }

    // Check Parameter types
    bool res = true;
    res &= mxIsChar(ssGetSFcnParam(S, 0));
    res &= mxIsLogical(ssGetSFcnParam(S, 1));
    res &= mxIsClass(ssGetSFcnParam(S, 2), "Simulink.Bus");

    if (!res) {
        return;
    }

    // Check if the block should act as source or sink. Return if an invalid type is encountered.

    bool isSink = mxGetLogicals(ssGetSFcnParam(S, 1));
    int numInputs;
    int numOutputs;
    if (isSink) {
        numInputs = 1;
        numOutputs = 0;
    } else {
        numInputs = 0;
        numOutputs = 1;
    }

    if (!ssSetNumInputPorts(S, numInputs)) return;
    if (numInputs > 0) {
        ssSetInputPortWidth(S, 0, 1);
        ssSetBusInputAsStruct(S, 0, 1);
        ssSetInputPortBusMode(S, 0, SL_BUS_MODE);
        ssSetInputPortRequiredContiguous(S, 0, true); /*direct input signal access*/
    }

    if (!ssSetNumOutputPorts(S, numOutputs)) return;
    if (numOutputs > 0) {
        ssSetOutputPortWidth(S, 0, 1);
        ssSetBusOutputAsStruct(S, 0, 1);
        ssSetOutputPortBusMode(S, 0, SL_BUS_MODE);
    }

    /* Compile-time handling */
    if (ssGetSimMode(S) != SS_SIMMODE_SIZES_CALL_ONLY) {
        int id = 0;

        /* Register bus object data type (passed in as the
         * third parameter in the block dialog
         */
        ssRegisterTypeFromParameter(S, 2, &id);

        /* Set the bus data type identifier for the input
         * and output port data type of the block.
         */
        if (numInputs > 0) ssSetInputPortDataType(S, 0, id);
        if (numOutputs > 0) ssSetOutputPortDataType(S, 0, id);
    }

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 1); // Set Pointer Work vector size to 1, the SimConnect Handler
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    /* Specify the operating point save/restore compliance to be same as a 
     * built-in block */
    ssSetOperatingPointCompliance(S, USE_DEFAULT_OPERATING_POINT);

    ssSetRuntimeThreadSafetyCompliance(S, RUNTIME_THREAD_SAFETY_COMPLIANCE_TRUE);
    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
}



/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);

}



#define MDL_INITIALIZE_CONDITIONS   /* Change to #undef to remove function */
#if defined(MDL_INITIALIZE_CONDITIONS)
  /* Function: mdlInitializeConditions ========================================
   * Abstract:
   *    In this function, you should initialize the continuous and discrete
   *    states for your S-function block.  The initial states are placed
   *    in the state vector, ssGetContStates(S) or ssGetRealDiscStates(S).
   *    You can also perform any other initialization activities that your
   *    S-function may require. Note, this routine will be called at the
   *    start of simulation and if it is present in an enabled subsystem
   *    configured to reset states, it will be call when the enabled subsystem
   *    restarts execution to reset the states.
   */
  static void mdlInitializeConditions(SimStruct *S)
  {
  }
#endif /* MDL_INITIALIZE_CONDITIONS */



#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 
  /* Function: mdlStart =======================================================
   * Abstract:
   *    This function is called once at start of model execution. If you
   *    have states that should be initialized once, this is the place
   *    to do it.
   */
  static void mdlStart(SimStruct *S)
  {
  }
#endif /*  MDL_START */



/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block.
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    //const real_T *u = (const real_T*) ssGetInputPortSignal(S,0);
    //real_T       *y = (real_T*) ssGetOutputPortSignal(S,0);
}

#define MDL_UPDATE  /* Change to #undef to remove function */
#if defined(MDL_UPDATE)
  /* Function: mdlUpdate ======================================================
   * Abstract:
   *    This function is called once for every major integration time step.
   *    Discrete states are typically updated here, but this function is useful
   *    for performing any tasks that should only take place once per
   *    integration step.
   */
  static void mdlUpdate(SimStruct *S, int_T tid)
  {
  }
#endif /* MDL_UPDATE */



#define MDL_DERIVATIVES  /* Change to #undef to remove function */
#if defined(MDL_DERIVATIVES)
  /* Function: mdlDerivatives =================================================
   * Abstract:
   *    In this function, you compute the S-function block's derivatives.
   *    The derivatives are placed in the derivative vector, ssGetdX(S).
   */
  static void mdlDerivatives(SimStruct *S)
  {
  }
#endif /* MDL_DERIVATIVES */



/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S)
{
}

// Required S-function trailer
#ifdef MATLAB_MEX_FILE /* Is this file being compiled as a MEX-file? */
#include <simulink.c> /* MEX-file interface mechanism */
#else
#include "cg_sfun.h" /* Code generation registration function */
#endif

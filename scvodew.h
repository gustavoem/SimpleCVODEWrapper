#ifndef _SCVODEW_H_
#define _SCVODEW_H_

#include <cvode/cvode.h>
#define STIFF_INTEGRATOR CV_BDF
#define NONSTIFF_INTEGRATOR CV_ADAMS

/* This library is a wrapper of CVODE that hides integration details 
 * that might not be of interest of someone that is new to CVODE. For 
 * more general use of CVODE, we recommend using directly the library
 * itself. 
 * */

typedef struct cvode_solver {
    void *cvode_mem;
    SUNLinearSolver LS;
    SUNMatrix J;
    N_Vector y0;
} SimpleCVODESolver;

/* 
This function creates a CVODE solver of type SimpleCVODESolver and 
returns a pointer to it.
 
Arguments
    lmm: specifies the linear multistep method and it must be either
        CV_ADAMS (for non-stiff problems) or CV_BDF (for stiff 
        problems).

Returns a pointer to the newly created object. Returns NULL if failed
to create such object.
*/
SimpleCVODESolver *new_cvode_solver(int lmm);


/* This function allows the user to set the initial values for variables
of the system of ODEs.

Arguments
    cvode_mem: a pointer to the cvode object;
    f: a function that defines the system of ODEs. This function must 
        have the signature f(realtype t, N_Vector y, N_Vector ydot, 
        void *f_data);
    t0: the starting time of integration;
    y0: a pointer to a list of initial values;
    n: the size of the list y0.

Returns 0 if success.
*/
int init_solver(SimpleCVODESolver *solver, void *f, float t0, float *y0, 
        int n);


/* 
This function sets the tolerances for the integrator. 

Arguments
    cvode_mem: a pointer to the cvode object;
    abstol: absolute error tolerance;
    reltol: relative error tolerance.

Returns 0 if success.
*/
int set_tolerance(SimpleCVODESolver *solver, float abstol, 
        float reltol);


/*
Prepares the solver for integration.
Arguments
    cvode_mem: a pointer to the cvode object.

Returns 0 if success.
*/
int prepare_solver(SimpleCVODESolver *solver);


/*
This function allows the user to define data that will be passed to f, 
during integration.

Arguments
    data: a pointer to the data.

Returns 1 if success.
*/
int set_system_data(SimpleCVODESolver *solver, void *data);


/* 
Integrates the system.

Arguments
    
*/
float ** integrate(SimpleCVODESolver *solver, float *t);


/*
Frees the memory used by a SimpleCVODESolver object.
*/
void delete_solver(SimpleCVODESolver *solver);

#endif

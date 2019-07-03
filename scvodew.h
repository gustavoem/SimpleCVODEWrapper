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

/* 
This function creates a CVODE object and returns a pointer to the 
newly created object.
 
Arguments
    lmm: specifies the linear multistep method and it must be either
        CV_ADAMS (for non-stiff problems) or CV_BDF (for stiff 
        problems).

Returns a pointer to the newly created object.
*/
void *create_cvode_obj(int lmm);


/* This function allows the user to set the initial values for variables
of the system of ODEs.

Arguments
    cvode_mem: a pointer to the cvode object;
    f: a function that defines the system of ODEs. This function must 
        have the signature f(realtype t, N_Vector y, N_Vector ydot, 
        void *f_data);
    t0: the starting time of integration;
    y0: a pointer to a list of initial values.

Returns 1 if success.
*/
int init_solver(void *cvode_mem, void *f, realtype t0, N_Vector y0);


/* 
This function sets the tolerances for the integrator. 

Arguments
    cvode_mem: a pointer to the cvode object;
    abstol: absolute error tolerance;
    reltol: relative error tolerance.

Returns 1 if success.
*/
int set_tolerance(void *cvode_mem, float abstol, float reltol);


/*
This function allows the user to define data that will be passed to f, 
during integration.

Arguments
    data: a pointer to the data.

Returns 1 if success.
*/
int set_system_data(void *cvode_mem, void *data);


/* 
Integrates the system.

Arguments
    
*/
float ** integrate(void *cvode_mem, float *t);

#endif

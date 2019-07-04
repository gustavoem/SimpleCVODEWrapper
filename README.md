# SimpleCVODEWrapper
A C library that wraps CVODE in a simpler interface for solving system of differential equations.

The following steps are necessary to succesfully accomplish the integration of a system of ordinary differential equations.

#### Define your system function
You should declare a function `f` that has the signature `int f(realtype t, N_Vector y, N_Vector ydot, void *f_data)`. 

You can get a definition of the type `N_Vector` by including the library nvector, `#include<nvector/nvector_serial.h>`.

#### Create you solver
Call the function `new_cvode_solver(lmm)` to start a `SimpleCVODESolver` object. The argument `lmm` stands for **l**inear 
**m**ultistep **m**ethod and can be either set as `STIFF_INTEGRATOR` (which uses BDF method), or `NONSTIFF_INTEGRATOR` 
(which uses Adams method).

#### Initialize the solver
Use the function `init_solver(SimpleCVODESolver *solver, void *f, float t0, float *y0, int n)`, initialize the solver `solver`.
The param `f` should point to your system function; `t0` should be the starting time for integration, `y0` is an array that 
determines the initial conditions; and `n` is the size of y0.

#### Set integration error tolerances parameters
Use the function `set_tolerance(SimpleCVODESolver *solver, float abstol, float reltol)` to define the integration error 
tolerance.

#### Prepare the solver for integration
The function ```prepare_solver``` creates data structures necessary for the integrator to work. These structures are related 
to the estimation of the Jacobian matrix, which is set to be approximated through difference quotients.

#### Define system optional arguments
It is possible to make the integrator pass arguments to the system function. To define these argments, the function 
`set_system_data(SimpleCVODESolver *solver, void *data)` should be called.

#### Integrate
Finally, one can integrate the system by calling the function `integrate(SimpleCVODESolver *solver, float *t, int m)`.
The times for which the integration values should be stored are defined in the array `t`, which should have size `m`. This 
function returns a matrix of size `m x n` where `n` is the cardinality of the system. On this matrix, each line has the 
integrated values of the system on a specific time step.

### After integration
Once you have integrated your system, you can restart it by calling the function `reset_solver`. This function is useful for
repeating the integration process by changing optional system parameters or even initial conditions. When you are done with 
integrations, use the function `delete_solver` to free all allocated memory related to the solver.

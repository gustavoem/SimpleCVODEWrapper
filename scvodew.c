#include "scvodew.h" 
#include <nvector/nvector_serial.h>


/*
Check function return value...
    opt == 0 means SUNDIALS function allocates memory so check if
        returned NULL pointer
    opt == 1 means SUNDIALS function returns an integer value so check 
        if retval < 0
    This function is a modified copy from one of the examples 
    distributed together with CVODE.
*/
static int check_retval(void *returnvalue, const char *funcname, 
        int opt);


SimpleCVODESolver *new_cvode_solver(int lmm) 
{
    void *cvode_mem = CVodeCreate(lmm);
    if (check_retval (cvode_mem, "CVodeCreate", 0))
        return NULL;

    SimpleCVODESolver * solver = malloc (sizeof (SimpleCVODESolver))
    solver->cvode_mem = cvode_mem;
    solver->LS = NULL;
    solver->J = NULL;
    return solver;
}


int init_solver(SimpleCVODESolver *solver, void *f, float t0, float *y0, 
        int n)
{
    int i, flag;
    void *cvode_mem = solver->cvode_mem;
    N_Vector vec_y0 = N_VNew_Serial(n);
    realtype real_t0 = t0;
    
    for (i = 0; i < n; i++) 
        NV_Ith_S(vec_y0, i) = y0[i];

    flag = CVodeInit(cvode_mem, f, real_t0, vec_y0);
    if (check_retval(cvode_mem, "CVodeInit", 1))
        return -1;
    return 0;
}


int set_tolerance(SimpleCVODESolver *solver, float abstol, float reltol) 
{
    void *cvode_mem = solver->cvode_mem;
    flag = CVodeSStolerances(cvode_mem, reltol, abstol);
    if (check_retval(flag, "CVodeSStolerances", 1))
        return -1;
    return 0;
}


int prepare_solver(SimpleCVODESolver *solver)
{
    SUNMatrix J;
    SUNLinearSolver LS;
    void *cvode_mem = solver->cvode_mem;

    // Create matrix object
    J = SUNDenseMatrix(N, N);
    if (check_retval (J, "SUNDenseMatrix", 0))
        return -1;
        
    // Create linear solver object
    LS = SUNLinSol_Dense(y, J);
    if (check_retval (LS, "SUNLinSol_Dense", 0))
        return -1;

    // Attach linear solver module
    flag = CVodeSetLinearSolver(cvode_mem, LS, J);
    if (check_retval (flag, "CVodeSetLinearSolver", 1))
        return -1;
    solver->J = J;
    solver->LS = LS;
    return 0;
}


int set_system_data(SimpleCVODESolver *solver, void *data) 
{
    void *cvode_mem = solver->cvode_mem;
    flag = CVodeSetUserData(cvode_mem, data);
    if (check_retval(flag, "CVodeSetUserData", 1))
        return -1;
    return 0;
}

static int check_retval(void *returnvalue, const char *funcname, 
        int opt)
{
  int *retval;

  /* Check if SUNDIALS memory allocation worked, i.e. returned value is 
   * not NULL. */
  if (opt == 0 && returnvalue == NULL) 
  {
      fprintf(stderr, "\nSUNDIALS_ERROR: %s() failed - returned NULL \
              pointer\n\n", funcname);
      return 1; 
  }

  /* Check if SUNDIALS return value is greater than zero. */
  else if (opt == 1) 
  {
      retval = (int *) returnvalue;
      if (*retval < 0) 
      {
          fprintf(stderr, "\nSUNDIALS_ERROR: %s() failed with retval = \
                  %d\n\n", funcname, *retval);
            return 1; 
      }
  }
  return 0;
}


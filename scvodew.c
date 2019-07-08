#include "scvodew.h" 
#include <nvector/nvector_serial.h>
#include <sundials/sundials_dense.h>
#include <sundials/sundials_types.h>
#include <sunmatrix/sunmatrix_dense.h>
#include <sunlinsol/sunlinsol_dense.h>
#include <stdlib.h>


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
    SimpleCVODESolver * solver;
    if (check_retval (cvode_mem, "CVodeCreate", 0))
        return NULL;

    solver = malloc (sizeof (SimpleCVODESolver));
    solver->cvode_mem = cvode_mem;
    solver->LS = NULL;
    solver->J = NULL;
    return solver;
}


int init_solver(SimpleCVODESolver *solver, int (*f)(realtype, 
            N_Vector, N_Vector, void *), float t0, float *y0, int n)
{
    int i, flag;
    void *cvode_mem = solver->cvode_mem;
    N_Vector vec_y0 = N_VNew_Serial(n);
    realtype real_t0 = t0;
    for (i = 0; i < n; i++) 
        NV_Ith_S(vec_y0, i) = y0[i];

    flag = CVodeInit(cvode_mem, f, real_t0, vec_y0);
    if (check_retval(&flag, "CVodeInit", 1))
        return -1;
    solver->y0 = vec_y0;
    return 0;
}


int set_tolerance(SimpleCVODESolver *solver, float abstol, float reltol) 
{
    void *cvode_mem = solver->cvode_mem;
    int flag = CVodeSStolerances(cvode_mem, reltol, abstol);
    if (check_retval(&flag, "CVodeSStolerances", 1))
        return -1;
    return 0;
}


int set_max_step(SimpleCVODESolver *solver, int mxsteps)
{
    void *cvode_mem = solver->cvode_mem;
    int flag = CVodeSetMaxNumSteps(cvode_mem, mxsteps);
    if (check_retval(&flag, "CVodeSetMaxNumSteps", 1))
        return -1;
    return 0;
}


int prepare_solver(SimpleCVODESolver *solver)
{
    SUNMatrix J;
    SUNLinearSolver LS;
    void *cvode_mem = solver->cvode_mem;
    int flag, n;
    N_Vector y0 = solver->y0;
    n = NV_LENGTH_S(y0);
    
    /* Create matrix object */
    J = SUNDenseMatrix(n, n);
    if (check_retval(J, "SUNDenseMatrix", 0))
        return -1;
        
    /* Create linear solver object */
    LS = SUNLinSol_Dense(y0, J);
    if (check_retval(LS, "SUNLinSol_Dense", 0))
        return -1;

    /* Attach linear solver module */
    flag = CVodeSetLinearSolver(cvode_mem, LS, J);
    if (check_retval(&flag, "CVodeSetLinearSolver", 1))
        return -1;
    solver->J = J;
    solver->LS = LS;    
    return 0;
}


int set_system_data(SimpleCVODESolver *solver, void *data) 
{
    void *cvode_mem = solver->cvode_mem;
    int flag = CVodeSetUserData(cvode_mem, data);
    if (check_retval(&flag, "CVodeSetUserData", 1))
        return -1;
    return 0;
}


float **integrate(SimpleCVODESolver *solver, float *t, int m)
{
    realtype tout, treach;
    int itask = CV_NORMAL;
    int i, j, n;
    int flag;
    N_Vector yout, y0;
    void *cvode_mem;
    float **result;

    cvode_mem = solver->cvode_mem;
    y0 = solver->y0;
    n = NV_LENGTH_S(y0);
    yout = N_VNew_Serial(n);
    result = malloc(m * sizeof (float *));
    for (i = 0; i < m; i++) 
        result[i] = malloc(n * sizeof (float));

    for (i = 0; i < m; i++)
    {
        tout = t[i];
        flag = CVode(cvode_mem, tout, yout, &treach, itask);
        if (check_retval(&flag, "CVode", 1)) 
        {
            for (j = 0; j < m; j++)
                free(result[j]);
            free(result);
            N_VDestroy_Serial(yout);
            return NULL;
        }

        for (j = 0; j < n; j++)
            result[i][j] = NV_Ith_S(yout, j);
    }

    N_VDestroy_Serial(yout);
    return result;
}


int reset_solver(SimpleCVODESolver *solver, float t0, float *y0)
{
    N_Vector solver_y0 = solver->y0;
    realtype real_t0 = t0;
    int flag, i;
    int n = NV_LENGTH_S(solver_y0);
    for (i = 0; i < n; i++) 
        NV_Ith_S(solver_y0, i) = y0[i];

    flag = CVodeReInit(solver->cvode_mem, real_t0, solver_y0);
    if (check_retval(&flag, "CVodeReInit", 1))
        return -1;
    return 0;
}


void delete_solver(SimpleCVODESolver *solver) 
{
    CVodeFree(&(solver->cvode_mem));
    SUNLinSolFree(solver->LS);
    SUNMatDestroy(solver->J);
    N_VDestroy_Serial(solver->y0);
    free(solver);
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


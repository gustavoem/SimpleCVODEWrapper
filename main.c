#include "scvodew.h"
#include <nvector/nvector_serial.h>
#include <stdlib.h>
#include <math.h>


static int f(realtype t, N_Vector y, N_Vector ydot, void *f_data) 
{
    NV_Ith_S(ydot,0) = NV_Ith_S(y,0);
    NV_Ith_S(ydot,1) = NV_Ith_S(y,1);
    return 0;
}




int main()
{
    SimpleCVODESolver *solver = new_cvode_solver(STIFF_INTEGRATOR);
    float t0 = 0;
    float *y0;
    float *t = malloc(10 * sizeof (float));
    float **result;
    int i;
    
    for (i = 0; i < 10; i++)
        t[i] = (i + 1) / 10.0;
    y0 = malloc(2 * sizeof (float));
    y0[0] = 1;
    y0[1] = 2;

    init_solver(solver, f, t0, y0, 2);
    set_tolerance(solver, 1e-8, 1e-8);
    prepare_solver(solver);
    result = integrate(solver, t, 10);

    for (i = 0; i < 10; i++) 
    {
        printf("y1(%.4f) = %.3f\n", t[i], result[i][0]);
        printf("y2(%.4f) = %.3f\n", t[i], result[i][1]);
        printf("exp(%.3f) = %.3f\n", t[i], exp (t[i]));
        /*printf("exp(%.3f) = %.3f\n\n", t[i], exp (t[i]));*/
    }

    delete_solver(solver);
    for (i = 0; i < 10; i++)
        free(result[i]);
    free(result);
    free(t);
    free(y0);
    return 0;
}

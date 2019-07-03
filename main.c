#include "scvodew.h"
#include <nvector/nvector_serial.h>
#include <stdlib.h>


static int f(realtype t, N_Vector y, N_Vector ydot, void *f_data) 
{
    float *param_data = f_data;
    NV_Ith_S(ydot,0) = NV_Ith_S(y,0) * param_data[1];
    NV_Ith_S(ydot,1) = NV_Ith_S(y,1) * param_data[2];
    return 0;
}




int main()
{
    SimpleCVODESolver *solver = new_cvode_solver(STIFF_INTEGRATOR);
    float t0 = 0;
    float *y0;

    y0 = malloc (2 * sizeof (float));
    y0[0] = 1;
    y0[1] = 2;
    init_solver(solver, f, t0, y0, 2);
    return 0;
}

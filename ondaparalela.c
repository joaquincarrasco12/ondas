#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NX 100
#define NT 500
#define L 1.0
#define C 1.0

int main() {

    double dx = L / (NX - 1);
    double dt = 0.9 * dx / C;

    double r = C * dt / dx;

    double u_prev[NX];
    double u_curr[NX];
    double u_next[NX];

    FILE *file = fopen("onda.txt", "w");

    if (file == NULL) {
        printf("Error al abrir archivo\n");
        return 1;
    }


    // Pulso triangular

    for (int i = 0; i < NX; i++) {

        double x = i * dx;

        if (x < 0.5)
            u_curr[i] = 2.0 * x;
        else
            u_curr[i] = 2.0 * (1.0 - x);

        u_prev[i] = u_curr[i];
    }

    // bordes fijos
    u_curr[0] = 0.0;
    u_curr[NX-1] = 0.0;

    u_prev[0] = 0.0;
    u_prev[NX-1] = 0.0;

    double t = 0.0;

    // guardar estado inicial
    for (int i = 0; i < NX; i++) {

        double x = i * dx;

        fprintf(file, "%f %f %f\n",
                x, t, u_curr[i]);
    }

    fprintf(file, "\n");


    // EVOLUCIÓN TEMPORAL
  

    for (int n = 0; n < NT; n++) {

        t += dt;

    

        for (int i = 1; i < NX - 1; i++) {

            u_next[i] =
                2*u_curr[i]
                - u_prev[i]
                + r*r * (
                    u_curr[i+1]
                    - 2*u_curr[i]
                    + u_curr[i-1]
                );
        }

        // condiciones de borde
        u_next[0] = 0.0;
        u_next[NX-1] = 0.0;

        // guardar resultados
        for (int i = 0; i < NX; i++) {

            double x = i * dx;

            fprintf(file, "%f %f %f\n",
                    x, t, u_next[i]);
        }

        fprintf(file, "\n");

 

        for (int i = 0; i < NX; i++) {

            u_prev[i] = u_curr[i];
            u_curr[i] = u_next[i];
        }
    }

    fclose(file);

    printf("Datos guardados en onda.txt\n");

    return 0;
}
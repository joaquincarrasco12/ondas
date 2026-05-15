#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NX 100        // puntos espaciales
#define NT 500        // pasos de tiempo
#define L 1.0         // largo de la cuerda
#define C 1.0         // velocidad de la onda

int main() {
    double dx = L / (NX - 1);
    double dt = 0.9 * dx / C;  // condición CFL
    double r = C * dt / dx;

    double u_prev[NX], u_curr[NX], u_next[NX];

    FILE *file = fopen("onda.txt", "w");
    if (file == NULL) {
        printf("Error al abrir archivo\n");
        return 1;
    }

    // condición inicial: pulso gaussiano
    for (int i = 0; i < NX; i++) {
        double x = i * dx;
        u_curr[i] = exp(-100 * (x - 0.5)*(x - 0.5));
        u_prev[i] = u_curr[i];  // velocidad inicial = 0
    }

    // condiciones de borde
    u_curr[0] = u_curr[NX-1] = 0.0;
    u_prev[0] = u_prev[NX-1] = 0.0;

    // guardar estado inicial (t = 0)
    double t = 0.0;
    for (int i = 0; i < NX; i++) {
        double x = i * dx;
        fprintf(file, "%f %f %f\n", x, t, u_curr[i]);
    }
    fprintf(file, "\n"); // separador opcional

    // evolución temporal
    for (int n = 0; n < NT; n++) {
        t += dt;

        // actualizar puntos internos
        for (int i = 1; i < NX - 1; i++) {
            u_next[i] = 2*u_curr[i] - u_prev[i]
                      + r*r * (u_curr[i+1] - 2*u_curr[i] + u_curr[i-1]);
        }

        // condiciones de borde
        u_next[0] = 0.0;
        u_next[NX-1] = 0.0;

        // guardar en archivo
        for (int i = 0; i < NX; i++) {
            double x = i * dx;
            fprintf(file, "%f %f %f\n", x, t, u_next[i]);
        }
        fprintf(file, "\n"); // separador entre tiempos

        // avanzar en el tiempo
        for (int i = 0; i < NX; i++) {
            u_prev[i] = u_curr[i];
            u_curr[i] = u_next[i];
        }
    }

    fclose(file);
    printf("Datos guardados en onda.txt\n");

    return 0;
}

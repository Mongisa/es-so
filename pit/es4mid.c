#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
// Calcola le radici di un'equazione di secondo grado
void Calcola_radici(float x[]) {
	float a = x[0];
	float b = x[1];
	float c = x[2];
	float delta = b * b - 4 * a * c;
	if (delta == 0) {
		/* radici coincidenti */
		printf("radici coincidenti: x0 = x1 = %f\n", -b / (2 * a));
	} else {
		if (delta > 0) {
		/* radici reali distinte */
		printf("radici reali distinte: x0 = %f, x1 = %f\n",(-b + sqrt(delta)) / (2 * a), (-b - sqrt(delta)) / (2 * a));
		} else {
		/* radici complesse */
			printf("radici complesse coniugate: \n"
			"parte reale = %f, parte immaginaria = +/- %f\n",
			-b / (2 * a), sqrt(-delta) / 2 * a);
		}
	}
}

int main () {
	int fd[2];
	float n;

	if(pipe(fd)<0) {
		printf("pipe error!\n");
		return EXIT_FAILURE;
	}

	pid_t pid = fork();

	if(pid<0) {
		printf("fork error\n");
		return EXIT_FAILURE;
	}

	if(pid>0) {
		//Father process
		close(fd[0]);

		for(int i=0;i<3;i++) {
			printf("Sono il processo padre. Inserisci %do coefficiente: ",i+1);
			scanf("%f",&n);

			int sent = write(fd[1],&n, sizeof(float));
			if(sent<sizeof(float)) {
				printf("Sending error\n");
				return EXIT_FAILURE;
			}
		}

		wait(NULL);

		printf("Sono il processo padre. Il figlio ha terminato\n");

		close(fd[1]);
	} else {
		//Child process
		close(fd[1]);

		float coeff[3];

		for(int i=0;i<3;i++) {
			int received = read(fd[0], &n, sizeof(float));
			if(received<sizeof(float)) {
				printf("Receiving error\n");
				return EXIT_FAILURE;
			}
			coeff[i]=n;
		}

		Calcola_radici(coeff);

		close(fd[0]);
	}
	return EXIT_SUCCESS;
}

#include "mypopen.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

FILE* mypopen(const char* command, const char* type) {
	
	int pipefd[2];
	int new_pipefd[2];
	pid_t cpid;
	char buf;

	if (pipe(pipefd) == -1) {
		return NULL; /*errno wird schon innerhalb von pipe zugewiesen*/
	}

	if (cpid >= 0) { /*�berpr�ft ob nicht schon ein Child Prozess l�uft*/
		errno = EAGAIN;
		return NULL;
	}

	cpid = fork();
	
	if (cpid == -1) {
		return NULL; /*errno wird schon innerhalb von pipe zugewiesen*/
	}

	if (cpid == 0) {    /* Child*/
		dup2(pipefd, new_pipefd); /* Creates copy pipefd to new_pipefd */

		if (strcmp(*type, 'r') == 0) {
			close(pipefd[1]);	/* Close unused write end */
		}

		if (strcmp(*type, 'w') == 0) {
			close(pipefd[0]);	/* Close unused read end */
		}

		else {
			errno = EINVAL; /*invalid value*/
			return NULL;
		}
		

		while (read(pipefd[0], &buf, 1) > 0)
			write(STDOUT_FILENO, &buf, 1);

		write(STDOUT_FILENO, "\n", 1);
		close(pipefd[0]);
		exit(EXIT_SUCCESS);

	}
	else {            /* Parent */
		close(pipefd[0]);          /* Close unused read end */
		write(pipefd[1], argv[1], strlen(argv[1]));
		close(pipefd[1]);          /* Reader will see EOF */
		wait(NULL);                /* Wait for child */
		exit(EXIT_SUCCESS);
	}
}

int mypclose(FILE* stream) {
	pid_t wait_pid;
}
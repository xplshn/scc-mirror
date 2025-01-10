#define _POSIX_C_SOURCE

#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "make.h"

int
is_dir(char *fname)
{
	struct stat st;

	if (stat(fname, &st) < 0) {
		perror("make: cheking if target is a dir");
		return 0;
	}
	return (st.st_mode & S_IFMT) == S_IFDIR;
}

void
exportvar(char *var, char *value)
{
	int n;
	char *buf;

	n = snprintf(NULL, 0, "%s=%s", var, value);
	buf = emalloc(n+1);
	snprintf(buf, n+1, "%s=%s", var, value);
	putenv(buf);
}

time_t
stamp(char *name)
{
	struct stat st;

	if (stat(name, &st) < 0)
		return -1;

	return st.st_mtime;
}

int
launch(char *cmd, int ignore)
{
	int st;
	pid_t pid;
	char *name, *shell;
	char *args[] = {NULL, "-ec" , cmd, NULL};
	static int initsignals;
	extern char **environ;
	extern void sighandler(int);


	if (!initsignals) {
		struct sigaction act = {
			.sa_handler = sighandler
		};

		/* avoid BSD weirdness signal restart handling */
		sigaction(SIGINT, &act, NULL);
		sigaction(SIGHUP, &act, NULL);
		sigaction(SIGTERM, &act, NULL);
		sigaction(SIGQUIT, &act, NULL);
		initsignals = 1;
	}

	switch (pid = fork()) {
	case -1:
		return -1;
	case 0:
		shell = getmacro("SHELL");

		if (ignore)
			args[1] = "-c";
		if ((name = strrchr(shell, '/')) != NULL)
			++name;
		else
			name = shell;
		args[0] = name;
		execve(shell, args, environ);
		_exit(127);
	default:
		if (waitpid(pid, &st, 0) < 0)
			kill(pid, SIGTERM);
		return st;
	}
}

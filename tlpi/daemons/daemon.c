#include <sys/stat.h>
#include <fcntl.h>
#include "daemon.h"

int becomeDaemon(int flags)    /* returns 0 on success, -1 on error */
{
	int maxfd, fd;
	
	switch(fork()){                                         /* Become background process */
		case -1: return -1;
		case 0: break;					/* Child falls through, parent terminates */
		default: _exit(EXIT_SUCCESS);
	}

	if(setsid() == -1)					/* Become leader of new session */
		return -1;

	switch (fork()){					/* Ensure we are not session leader */
		case -1: return -1;
			 case 0; break;
		default: _exit(EXIT_SUCCESS);
	}
	
	if(!(flags & BD_NO_UMASKo))
		umask(0);

	if(!(flags & BD_NO_CHDIR))
		chdir("/");

	if(!(flags & BD_NO_CLOSE_FILES)) {
		maxfd = sysconf(_SC_OPEN_MAX);
	if(maxfd == -1)
		maxfd = BD_MAX_CLOSE;

	for(fd = 0; fd < maxfd; fd++)
		close(fd);

	}

	if(!(flags & BD_NO_REOPEN_STD_FDS)){
		close(STDIN_FILENO);

		fd = open("/dev/null",O_RDWR);
		if(fd != STDIN_FILENO)
			return -1;
		if(dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
			return -1;
		if(dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
			return -1;
	}

	return 0;
}

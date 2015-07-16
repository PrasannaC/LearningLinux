#include "daemon.c"

int main()
{
	becomeDaemon(BD_NO_CLOSE_FILES);
	while(1)
	{
	}

	return 0;
}


/* After executing this program, you can verify that the process is now a daemon by running the command:
 * ` ps -C test_daemon -o "pid ppid pgid sid tty command" `
 *
 * If you see '?' in the TTY column, then the process has successfully become a daemon
 */


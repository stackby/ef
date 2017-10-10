#if defined __linux
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

#define FAIL_VALUE      -1
#define FILE_BUFFER_SIZ 0xFF

__attribute__((noinline)) static ssize_t getpassword (char **, size_t *, FILE *);

int main (int argc, char **argv, char **envp) {

	if (argc == 2) {
		FILE *stream = fopen (argv[1], "r");
			if (!stream) {
				fprintf (stderr, "%s", "\n file not exists or not found \n");
				exit (EXIT_FAILURE);
			} else {
				char file_buffer [FILE_BUFFER_SIZ];
				if (fgets (file_buffer, FILE_BUFFER_SIZ, stream) != NULL) {
					char *buffer_ptr = file_buffer;
					char *password_crypt_buffer;
					char *password_buffer_salt = NULL;
					size_t nreads = 0;
					printf ("\n\x1b[33minsert key: \x1b[0m");
					  if (getpassword (&password_buffer_salt, &nreads, stdin)) {
						if ((password_crypt_buffer = crypt (buffer_ptr, password_buffer_salt)) != NULL) {
							fclose (stream);
							stream = fopen (argv[1], "w");
								if (!stream) {
									fprintf (stderr, "%s", "\n file nor existis or not found \n");
									exit (EXIT_FAILURE);
								} else {
									if (fputs (password_crypt_buffer, stream) != EOF) {
										fclose (stream);
									} else {
										fprintf (stderr, "\n fputs () error - line %d", __LINE__);
										exit(EXIT_FAILURE);
									}
								}
						} else {
							fprintf (stderr, "\n crypt () error - line %d \n", __LINE__);
							exit (EXIT_FAILURE);
						}
				} else {
					fprintf (stderr, "\n fgets () error - line %d \n", __LINE__);
					exit (EXIT_FAILURE);
				}
			  }	
			}	
	} else {
		fprintf (stderr, "%s", "\n usage: termios <file> \n");
		exit (EXIT_FAILURE);
	}
	putc ('\n', stdout);
	return 0;
}

ssize_t getpassword (char **lineptr, size_t *n, FILE *stream) {
	struct termios oldtc;
	struct termios newtc;
	int nread;
		if (tcgetattr (fileno (stream), &oldtc) != 0)
			return FAIL_VALUE;
	newtc = oldtc;
	newtc.c_lflag = newtc.c_lflag & ~ECHO;
		if (tcsetattr (fileno (stream), TCSAFLUSH, &newtc) != 0)
			return FAIL_VALUE;
	nread = getline (lineptr, n, stream);
	(void) tcsetattr (fileno (stream), TCSAFLUSH, &oldtc);
	return nread;			
}

#else
	#error "this program run on linux only"
#endif
#endif
#endif

#include "config.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void
killed(int sig)
{
    fprintf(stderr, "This is killing me.\n");
    exit(0);
}


int
main(int argc, char * const argv[])
{
    signal(SIGTERM, killed);

    setbuf(stdout, 0);
    printf("Started\n");
    sleep(10);
    printf("Too late :-(\n");

    exit(0);
}

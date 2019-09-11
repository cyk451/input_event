#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include "uinput.h"
#include "common.h"

struct context{
	size_t num_event_fds;
	struct pollfd* in_fds;

	const char *out_fname;
	int out_fd;
};

int init(struct context * c)
{
	char buf[256];
	unsigned i;
	DIR *dp;
	struct dirent *ep; 

	if (c->out_fname != NULL) {
		c->out_fd = open(c->out_fname, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
		if(c->out_fd < 0) {
			printf("Couldn't open output file %d\n", c->out_fd);
			return 1;
		}
	} else 
		c->out_fd = STDOUT_FILENO;

	c->num_event_fds = get_event_fds(&c->in_fds);
	if (!c->num_event_fds)
		return c->num_event_fds;

	return 0;
}

void usage() {
	printf("record [filename]\n");
	printf("\tIf filename is omitted, captured raw events are printed to stdout.\n");
}

int record(struct context * c)
{
	int numread;
	size_t i;
	struct recorded_event re;

	memset(&re, 0, sizeof(re));

	while(1) {
		int e = poll(c->in_fds, c->num_event_fds, -1);
		// maybe fork here
		if(e < 0) {
			fprintf(stderr, "poll error %d\n", e);
			return 1;
		}
		
		for(i = 0; (i < c->num_event_fds) && e; ++i) {
			if((c->in_fds[i].revents & POLLIN) == 0)
				continue;
			re.fd = i;

			/* Data available */
			while (1) {
				int rc;
				numread = read(c->in_fds[i].fd, &re.event, 
						sizeof(re.event));
				if(numread != sizeof(re.event)) {
					// likely no more to read
					break;
				}

				// TODO write in the end
				rc = write(c->out_fd, &re, sizeof(re));
				if(rc != sizeof(re)) {
					fprintf(stderr, "Write error %d\n", rc);
					return 1;
				}
			}
			--e;
		}
	}

	return 0;
}

int main(int argc, char**args)
{
	struct context c;
	int rc;

	memset(&c, 0, sizeof(c));

	if (argc >= 2) {
		c.out_fname = (const char*)(args[1]);
	}

	rc = init(&c);
	if (rc != 0) {
		perror("Init failed");
		return rc;
	}

	return record(&c);
}


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/time.h>
#include <string.h>

#include "uinput.h"
#include "common.h"


struct context {
	int in_fd;
	size_t num_event_fds;
	struct pollfd *out_fds;
	const char* in_fname;
};

int init(struct context* c)
{

	if (c->in_fname != NULL) {
		c->in_fd = open(c->in_fname, O_RDONLY);
		if(c->in_fd < 0) {
			printf("Couldn't open input file %d\n", c->in_fd);
			return 1;
		}
	} else
		c->in_fd = STDIN_FILENO;

	c->num_event_fds = get_event_fds(&c->out_fds);
	if (c->num_event_fds == 0) {
		return c->num_event_fds;
	}

	return 0;
}

// here we assume input device always the same
int replay(struct context* c)
{
	struct timeval toffset;
	// struct input_event event;
	struct recorded_event re;

	timerclear(&toffset);

	while(1) {
		struct timeval now, tevent, tsleep;

		if(read(c->in_fd, &re, sizeof(re)) != sizeof(re)) {
			break;
		}

		gettimeofday(&now, NULL);
		if (!timerisset(&toffset)) {
			timersub(&now, &re.event.time, &toffset);
		}

		timeradd(&re.event.time, &toffset, &tevent);
		timersub(&tevent, &now, &tsleep);
		if (tsleep.tv_sec > 0 || tsleep.tv_usec > 100)
			select(0, NULL, NULL, NULL, &tsleep);

		re.event.time = tevent;

		if(write(c->out_fds[re.fd].fd, &re.event, 
					sizeof(re.event)) != sizeof(re.event)) {
			fprintf(stderr, "Output write error\n");
			return 2;
		}

	}

	return 0;
}

void usage() {
	printf("replay [filename]\n");
	printf("\tIf filename is not given stdin is used.\n");
}

int main(int argc, char** args)
{
	struct context c;
	int rc;

	memset(&c, 0, sizeof(c));

	if (argc >= 2) {
		c.in_fname = (const char*)(args[1]);
	}

	rc = init(&c);
	if(rc != 0) {
		fprintf(stderr, "init failed %d\n", rc);
		return rc;
	}

	return replay(&c);
}


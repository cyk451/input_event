#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <linux/input.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include "uinput.h"

const char *EV_PREFIX = "/dev/input";

struct context{
	size_t num_event_fds;
	struct pollfd* in_fds;

	const char *out_fname;
	int out_fd;
};

size_t count_events();

int init(struct context * c)
{
	char buf[256];
	int fd;
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

	c->num_event_fds = count_events();

	c->in_fds = (struct pollfd*)malloc(sizeof(struct pollfd) * c->num_event_fds);
	if (c->in_fds < 0)
		return 1;

	dp = opendir(EV_PREFIX);
	if (!dp)  
		return 0;

	while ((ep = readdir(dp))) {
		int nfd;

		// printf("%s is on \n", ep->d_name);
		if (ep->d_name[0] == '.')
			continue;

		snprintf(buf, 256, "%s/%s", EV_PREFIX, ep->d_name);
		nfd = open(buf, O_RDONLY | O_NDELAY);
		if(nfd < 0) {
			printf("Couldn't open input device %s\n", ep->d_name);
			return 1;
		}

		c->in_fds[i].fd = nfd;
		c->in_fds[i].events = POLLIN;
		// printf("device %s inited\n", buf);
		i++;
	}

	closedir(dp);
	return 0;
}

int record(struct context * c)
{
	int numread;
	unsigned i;
	struct input_event event;

	while(1) {
		int e = poll(c->in_fds, c->num_event_fds, -1);
		if(e < 0) {
			printf("Poll error\n");
			return 1;
		}

		for(i = 0; (i < c->num_event_fds) && e; ++i, --e) {
			if((c->in_fds[i].revents & POLLIN) == 0)
				continue;

			/* Data available */
			while ((numread = read(c->in_fds[i].fd, &event, sizeof(event)))) {
				if(numread != sizeof(event)) {
					perror("incomplete event read..\n");
					break;
				}

				// TODO write in the end
				if(write(c->out_fd, &i, sizeof(i)) != sizeof(i)) {
					perror("Write error\n");
					return 1;
				}
				if(write(c->out_fd, &event, sizeof(event)) != sizeof(event)) {
					perror("Write error\n");
					return 1;
				}
			}
		}
	}
	return 0;
}

int main(int argc, char**args)
{
	struct context c;
	int rc;

	memset(&c, 0, sizeof(c));

	if (argc < 2) {
		c.out_fname = (const char*)(args[1]);
	}

	printf("fn %s\n", c.out_fname);

	rc = init(&c);
	if (rc != 0) {
		printf("Init failed");
		return rc;
	}

	return record(&c);
}



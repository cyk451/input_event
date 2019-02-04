#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <linux/input.h>
#include <dirent.h>
#include <stdlib.h>

#include "uinput.h"
#include "common.h"

static size_t count_events() {
	struct dirent *ep; 
	size_t num = 0;
	DIR *dp = opendir(EV_PREFIX);
	char buf[256];

	if (!dp)  
		return 0;

	while ((ep = readdir(dp))) {
		int nfd;

		if (ep->d_name[0] == '.')
			continue;

		snprintf(buf, 256, "%s/%s", EV_PREFIX, ep->d_name);
		nfd = open(buf, O_RDONLY | O_NDELAY);
		if (nfd < 0) {
			fprintf(stderr, "Couldn't open %s, err %d\n", ep->d_name, nfd);
			continue;
		}

		++num;
		close(nfd);
	}

	closedir(dp);
	return num;
}

int get_event_fds(struct pollfd** pfds) {
	int n, i = 0;
	DIR *dp;
	struct pollfd* fds = NULL;
	struct dirent *ep; 
	char buf[256];

	n = count_events();

	if (n <= 0)
		return n;

	dpf("%u devices to open\n", n);

	dp = opendir(EV_PREFIX);
	if (!dp)  
		return 0;

	fds = (struct pollfd*)malloc(sizeof(struct pollfd) * n);
	if (fds < 0)
		return -1;

	dp = opendir(EV_PREFIX);
	if (!dp)  
		return 0;

	while ((ep = readdir(dp))) {
		int nfd;

		if (ep->d_name[0] == '.')
			continue;

		snprintf(buf, 256, "%s/%s", EV_PREFIX, ep->d_name);
		// TODO: make open flag a func arg.
		nfd = open(buf, O_RDWR | O_NDELAY);
		if(nfd < 0) {
			fprintf(stderr, "Couldn't open input device %s\n", buf);
			return -1;
		}

		dpf("%s is as device %d\n", buf, i);

		fds[i].fd = nfd;
		fds[i].events = POLLIN;
		i++;
	}

	closedir(dp);
	*pfds = fds;
	return i;
}

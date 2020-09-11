#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <linux/input.h>
#include <dirent.h>
#include <stdlib.h>

#include "uinput.h"
#include "common.h"

/*
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
*/

size_t get_event_fds(struct pollfd** pfds) {
	int n, i = 0;
	// DIR *dp;
	struct pollfd* fds = NULL;
	// struct dirent *ep; 
	struct dirent **namelist;
	char buf[256];

	// n = count_events();

	// if (n <= 0)
		// return 0;


	/*
	dp = opendir(EV_PREFIX);
	if (!dp)  
		return 0;
		*/

	/*
	dp = opendir(EV_PREFIX);
	if (!dp)  
		return 0;
		*/

	n = scandir(EV_PREFIX /* dir */
			, &namelist /* out list */
			, NULL /* filter func ptr */
			, alphasort /* sorting comp func ptr */
			);

	if (n < 0) {
		return 0;
	}
	dpf("%u devices to open\n", n);

	fds = (struct pollfd*)malloc(sizeof(struct pollfd) * n);
	if (fds < 0)
		return 0;


	while (n--) {
		int nfd;

		if (namelist[n]->d_name[0] == '.')
			continue;

		snprintf(buf, 256, "%s/%s", EV_PREFIX, namelist[n]->d_name);
		// TODO: make open flag a func arg.
		nfd = open(buf, O_RDWR | O_NDELAY);
		if(nfd < 0) {
			fprintf(stderr, "Couldn't open input device %s\n", buf);
			return 0;
		}

		dpf("%s is as device %d\n", buf, i);

		fds[i].fd = nfd;
		fds[i].events = POLLIN;
		i++;
		free(namelist[n]);

	}

	free(namelist);

	/*
	while ((ep = readdir(dp))) {
		int nfd;

		if (ep->d_name[0] == '.')
			continue;

		snprintf(buf, 256, "%s/%s", EV_PREFIX, ep->d_name);
		// TODO: make open flag a func arg.
		nfd = open(buf, O_RDWR | O_NDELAY);
		if(nfd < 0) {
			fprintf(stderr, "Couldn't open input device %s\n", buf);
			return 0;
		}

		dpf("%s is as device %d\n", buf, i);

		fds[i].fd = nfd;
		fds[i].events = POLLIN;
		i++;
	}

	closedir(dp);
	*/
	*pfds = fds;
	return i;
}

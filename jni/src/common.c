#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <linux/input.h>
#include <dirent.h>
#include <stdlib.h>

#include "uinput.h"

extern const char *EV_PREFIX;

size_t count_events() {
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
			printf("Couldn't open %s, err %d\n", ep->d_name, nfd);
			continue;
		}

		++num;
		close(nfd);
	}

	closedir(dp);
	return num;
}

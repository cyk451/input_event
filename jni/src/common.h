#ifndef __INPUT_RECORDER_H__
#define __INPUT_RECORDER_H__

#define EV_PREFIX "/dev/input"
#include <poll.h>

struct recorded_event {
	size_t fd;
	struct input_event event;
};

// List all input event fs as pollfds.
// Returns num fds opened, and negative value upon errors.
int get_event_fds(struct pollfd**fds);

#ifdef DEBUG
#define dpf(...) printf("debug: " __VA_ARGS__)
#else
#define dpf(...)
#endif

#endif // end of guard

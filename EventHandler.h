#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

class EventHandler {

public:
	virtual int handle_signal(int signum) = 0;
	virtual ~EventHandler () {};
};

#endif /* EVENTHANDLER_H_ */

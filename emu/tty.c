#include "pdp6.h"
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <poll.h>

static void
recalc_tty_req(Tty *tty)
{
	setreq(tty->bus, TTY, tty->tto_flag || tty->tti_flag ? tty->pia : 0);
}

/* TODO: implement some kind of sleep if no tty is attached */
static void*
ttythread(void *dev)
{
	int n;
	char c;
	Tty *tty;

	tty = dev;
	for(;;){
		while(tty->fd >= 0 && (n = read(tty->fd, &c, 1)) > 0){
			tty->tti_busy = 1;
			tty->tti = c|0200;
			tty->tti_busy = 0;
			tty->tti_flag = 1;
			recalc_tty_req(tty);
		}
	}
}

static void
wake_tty(void *dev)
{
	Tty *tty;
	IOBus *bus;

	tty = dev;
	bus = tty->bus;
	if(IOB_RESET){
		tty->pia = 0;
		tty->tto_busy = 0;
		tty->tto_flag = 0;
		tty->tto = 0;
		tty->tti_busy = 0;
		tty->tti_flag = 0;
		tty->tti = 0;
	}
	if(bus->devcode == TTY){
		if(IOB_STATUS){
			if(tty->tti_busy) bus->c12 |= F29;
			if(tty->tti_flag) bus->c12 |= F30;
			if(tty->tto_busy) bus->c12 |= F31;
			if(tty->tto_flag) bus->c12 |= F32;
			bus->c12 |= tty->pia & 7;
		}
		if(IOB_DATAI){
			bus->c12 |= tty->tti;
			tty->tti_flag = 0;
		}
		if(IOB_CONO_CLEAR)
			tty->pia = 0;
		if(IOB_CONO_SET){
			if(bus->c12 & F25) tty->tti_busy = 0;
			if(bus->c12 & F26) tty->tti_flag = 0;
			if(bus->c12 & F27) tty->tto_busy = 0;
			if(bus->c12 & F28) tty->tto_flag = 0;
			if(bus->c12 & F29) tty->tti_busy = 1;
			if(bus->c12 & F30) tty->tti_flag = 1;
			if(bus->c12 & F31) tty->tto_busy = 1;
			if(bus->c12 & F32) tty->tto_flag = 1;
			tty->pia |= bus->c12 & 7;
		}
		if(IOB_DATAO_CLEAR){
			tty->tto = 0;
			tty->tto_busy = 1;
			tty->tto_flag = 0;
		}
		if(IOB_DATAO_SET){
			tty->tto = bus->c12 & 0377;
			if(tty->fd >= 0){
				tty->tto &= ~0200;
				write(tty->fd, &tty->tto, 1);
			}
			// TTO DONE
			tty->tto_busy = 0;
			tty->tto_flag = 1;
		}
	}
	recalc_tty_req(tty);
}

void
attachdevtty(Tty *tty, const char *path)
{
	int fd;
	struct termios tio;

	fd = tty->fd;
	if(fd >= 0){
		tty->fd = -1;
		close(fd);
	}
	fd = open(path, O_RDWR);
	if(fd < 0)
		goto fail;
	if(tcgetattr(fd, &tio) < 0)
		goto fail;
	cfmakeraw(&tio);
//	cfsetspeed(&tio, B300);
	if(tcsetattr(fd, TCSAFLUSH, &tio) < 0)
		goto fail;
	tty->fd = fd;
	return;

fail:
	if(fd >= 0) close(fd);
	fprintf(stderr, "couldn't open tty %s\n", path);
}

void
inittty(Tty *tty, IOBus *bus)
{
	pthread_t thread_id;
	tty->fd = -1;
	tty->bus = bus;
	bus->dev[TTY] = (Busdev){ tty, wake_tty, 0 };
	pthread_create(&thread_id, nil, ttythread, tty);
}

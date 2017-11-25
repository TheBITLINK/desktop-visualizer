#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>

#define _NET_WM_STATE_REMOVE        0    /* remove/unset property */
#define _NET_WM_STATE_ADD           1    /* add/set property */
#define _NET_WM_STATE_TOGGLE        2    /* toggle property  */

#define ATOM(a) XInternAtom(display, #a, False)


struct MwmHints {
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
    long input_mode;
    unsigned long status;
};
enum {
    MWM_HINTS_FUNCTIONS = (1L << 0),
    MWM_HINTS_DECORATIONS =  (1L << 1),

    MWM_FUNC_ALL = (1L << 0),
    MWM_FUNC_RESIZE = (1L << 1),
    MWM_FUNC_MOVE = (1L << 2),
    MWM_FUNC_MINIMIZE = (1L << 3),
    MWM_FUNC_MAXIMIZE = (1L << 4),
    MWM_FUNC_CLOSE = (1L << 5)
};

void x11_window_set_desktop (Display* display, Window wnd) {
  Atom xa = ATOM(_NET_WM_WINDOW_TYPE);
  if (xa != None) {
    Atom prop = ATOM(_NET_WM_WINDOW_TYPE_DESKTOP);
    XChangeProperty(display, wnd, xa, XA_ATOM, 32, PropModeReplace, (unsigned char *) &prop, 1);
  }
}

void x11_window_set_borderless (Display* display, Window wnd) {
  Atom xa = ATOM(_MOTIF_WM_HINTS);
	if (xa != None) {
		long prop[5] = { 2, 0, 0, 0, 0 };
		XChangeProperty(display, wnd, xa, xa, 32,
  	PropModeReplace, (unsigned char *) prop, 5);
  }
}

void x11_window_set_below (Display* display, Window wnd) {
  Atom xa = ATOM(_WIN_LAYER);
	if (xa != None) {
		long prop = 0;
    XChangeProperty(display, wnd, xa, XA_CARDINAL, 32, PropModeAppend, (unsigned char *) &prop, 1);
	}
  xa = ATOM(_NET_WM_STATE);
	if (xa != None) {
		Atom xa_prop = ATOM(_NET_WM_STATE_BELOW);
    XChangeProperty(display, wnd, xa, XA_ATOM, 32, PropModeAppend, (unsigned char *) &xa_prop, 1);
  }
}

void x11_window_set_sticky (Display* display, Window wnd) {
  Atom xa = ATOM(_NET_WM_DESKTOP);
	if (xa != None) {
		long xa_prop = 0xFFFFFFFF;
    XChangeProperty(display, wnd, xa, XA_CARDINAL, 32, PropModeAppend, (unsigned char *) &xa_prop, 1);
	}
  xa = ATOM(_NET_WM_STATE);
	if (xa != None) {
		Atom xa_prop = ATOM(_NET_WM_STATE_STICKY);
		XChangeProperty(display, wnd, xa, XA_ATOM, 32, PropModeAppend, (unsigned char *) &xa_prop, 1);
  }
}

void x11_window_set_skip_taskbar (Display* display, Window wnd) {
  Atom xa = ATOM(_NET_WM_STATE);
	if (xa != None) {
		Atom xa_prop = ATOM(_NET_WM_STATE_SKIP_TASKBAR);
		XChangeProperty(display, wnd, xa, XA_ATOM, 32, PropModeAppend, (unsigned char *) &xa_prop, 1);
  }
}

void x11_window_set_skip_pager (Display* display, Window wnd) {
  Atom xa = ATOM(_NET_WM_STATE);
	if (xa != None) {
		Atom xa_prop = ATOM(_NET_WM_STATE_SKIP_PAGER);
    XChangeProperty(display, wnd, xa, XA_ATOM, 32, PropModeAppend, (unsigned char *) &xa_prop, 1);
  }
}

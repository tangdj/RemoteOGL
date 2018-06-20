#include <X11/Xlib.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

void sleep_ms(int milliseconds) // cross-platform sleep function
{
#ifdef WIN32
Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
struct timespec ts;
ts.tv_sec = milliseconds / 1000;
ts.tv_nsec = (milliseconds % 1000) * 1000000;
nanosleep(&ts, NULL);
#else
usleep(milliseconds * 1000);
#endif
}


void main(void) {
    Display *dpy;
    Window root_window, focused, target, default_root_window;
    Pixmap pm;
    GC gc;
    XGCValues gcv, gcv_ret;
    int revert_to;
    XTextProperty text_prop;

    int screen, depth, whiteColor, blackColor;
    XEvent e;
    char text[1];
    KeySym key;

    dpy = XOpenDisplay(":0.0");
    screen = DefaultScreen(dpy);
    depth = DefaultDepth(dpy, screen);

    whiteColor = WhitePixel(dpy, screen);
    blackColor = BlackPixel(dpy, screen);

    root_window = RootWindow (dpy, screen);
    XGetInputFocus(dpy, &focused, &revert_to);
    XGetWMName(dpy, focused, &text_prop);

    printf("Active Window name: %s\n", text_prop.value);

    target = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 400, 400, 1, whiteColor, blackColor);

    XSelectInput(dpy, target, ButtonPressMask | StructureNotifyMask | ExposureMask | KeyPressMask | PropertyChangeMask | VisibilityChangeMask);
    XMapRaised(dpy, target);

    for(;;) {
        XNextEvent(dpy, &e);
        if (e.type == MapNotify)
            break;
    }

    pm = XCreatePixmap(dpy, focused, 200, 200, depth);
    gc = XCreateGC(dpy, focused, 0, NULL);
    XSetSubwindowMode(dpy, gc, IncludeInferiors);

    gcv.function = GXcopyInverted;

    XChangeGC(dpy, gc, GCFunction, &gcv);

    XFlushGC(dpy, gc);
    while(1) {

        XNextEvent(dpy, &e);
        if(e.type == Expose) {
            XSetInputFocus(dpy, target, RevertToNone, CurrentTime);
        } else if (e.type == MapNotify) {
            printf("Map notify\n");
        } else if (e.type == KeyPress) {
            XLookupString(&e.xkey, text, 255, &key,0);
            if (text[0] == 'c') {
                printf("Copying plane\n");
                XCopyArea(dpy, focused, target, gc, 0, 0, 200, 200, 0, 0);
                XFlush(dpy);
            }
            if (text[0] == 'q') {
                printf("Quitting ...\n");
                break;
            }
        }
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#include <Imlib2.h>


int main(int argc, char **argv) {
  char *pcDisplay;
  Display *dpy;
  Imlib_Image img;
  int ret;
  int width, height;

  if ( (pcDisplay = getenv("DISPLAY")) == NULL) {
    fprintf(stderr, "Error: DISPLAY environment variable not set\n");
    exit(1);
  }

  if ( (dpy = XOpenDisplay(pcDisplay)) == NULL) {
    fprintf(stderr, "Error: Can't open display: %s\n", pcDisplay);
    exit(1);
  }

   img = imlib_load_image("icon.bmp");
    if (!img) {
        fprintf(stderr, "%s:Unable to load image\n","icon.bmp");
        return 0;
    }
   imlib_context_set_image(img);
   width = imlib_image_get_width();
   height = imlib_image_get_height();
   Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 
                                 0, 0, width, height, 0, 0L,
                                 WhitePixel(dpy, 0));
  XSelectInput(dpy, w, ExposureMask);

  Pixmap canvas = XCreatePixmap(dpy, w, width, height, DefaultDepth(dpy, 0));
  GC gc = XCreateGC(dpy, canvas, 0, NULL);
  imlib_context_set_drawable(canvas);

  XSetForeground(dpy, gc, BlackPixel(dpy, 0));
  XFillRectangle(dpy, canvas, gc, 0, 0, width, height);
//  XSetForeground(dpy, gc, WhitePixel(dpy, 0));
//  XFillRectangle(dpy, canvas, gc, 50, 50, 50, 50);
  XCopyArea(dpy, canvas, w, gc, 0, 0, width, height, 0, 0);
  XMapWindow(dpy, w);

  for (;;) {
    XEvent e;
    XNextEvent(dpy, &e);
    switch (e.type) {
      case Expose:
        XCopyArea(dpy, canvas, w, gc, e.xexpose.x, e.xexpose.y,
                  e.xexpose.width, e.xexpose.height,
                  e.xexpose.x, e.xexpose.y);
        break;
      default:
        printf("Got event %d\n", e.type);
    }
  }
  getchar();
  return 0;
} /* int main(...) */

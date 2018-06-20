#include <stdio.h>
#include <X11/Xlib.h>
#include <Imlib2.h>

int main(int argc, char **argv)
{
    Imlib_Image img;
    Display *dpy;
    Pixmap pix;
    Window root;
    Screen *scn;
    int width, height;
    //const char *filename = NULL;

   /* if (argc < 2)
        goto usage;*/
   // filename = argv[1];
  //  for(int i=0;i<100;i++)
  // {
    enum Constants { max_filename = 256 };
    char filename[max_filename];
    //snprintf(filename, max_filename, "%s%d.bmp", "tmp", i);
   /* img = imlib_load_image("tmp0.bmp");
    if (!img) {
        fprintf(stderr, "%s:Unable to load image\n", filename);
        return 0;
    }
    imlib_context_set_image(img);*/
    /*width = imlib_image_get_width();
    height = imlib_image_get_height();*/

   /* dpy = XOpenDisplay(NULL);
    if (!dpy)
        return 0;*/
  //  scn = DefaultScreenOfDisplay(dpy);
  /*  root = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 
                                 0, 0, width, height, 0, 0L,
                                 WhitePixel(dpy, 0));*/
  //  GC gc = XCreateGC(dpy, pix, 0, NULL);

   // pix = XCreatePixmap(dpy, root, width, height, DefaultDepth(dpy,0));
    for(int i=0;i<100;i++)
   {
    snprintf(filename, max_filename, "%s%d.bmp", "tmp", i);
     printf("%s\n",filename);
    img = imlib_load_image(filename);
    if (!img) {
        fprintf(stderr, "%s:Unable to load image\n", filename);
        return 0;
    }
    imlib_context_set_image(img);
    width = imlib_image_get_width();
    height = imlib_image_get_height();

    dpy = XOpenDisplay(NULL);
    if (!dpy)
        return 0;
    scn = DefaultScreenOfDisplay(dpy);
    root = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 
                                 0, 0, width, height, 0, 0L,
                                 WhitePixel(dpy, 0));
    pix = XCreatePixmap(dpy, root, width, height, DefaultDepth(dpy,0));
    imlib_context_set_display(dpy);
    imlib_context_set_visual(DefaultVisualOfScreen(scn));
    imlib_context_set_colormap(DefaultColormapOfScreen(scn));
    imlib_context_set_drawable(pix);
    

    imlib_render_image_on_drawable(0, 0);
    XSetWindowBackgroundPixmap(dpy, root, pix);
    XMapWindow(dpy, root);
    
    XEvent e;
    XPutBackEvent(dpy, &e);
    printf("%d\n",XPending(dpy));
    while(XPending(dpy)){
     XNextEvent(dpy, &e);
    } 
    XFlush(dpy);
   }
   XDestroyWindow(dpy,root);
   getchar();
     return 0;
}

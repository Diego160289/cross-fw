#include "UIImpl.h"

#include <math.h>

void draw()
{
    // 1. set up the viewport
    glViewport(0, 0, 500, 500); // set viewport
    // to be the whole width and height
    // of the CLIENT AREA (drawable region) of the window,
    // (the CLIENT AREA excludes the titlebar and the
    // maximize/minimize buttons).

    // 2. projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,(float)500/(float)500, 1, 1000);

    // 3. viewing transformation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(  0, 0, 10,
                0, 0, 0,
                0, 1, 0);

    // 4. modelling transformation and drawing
    glClearColor( 0.5, 0, 0, 0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    static float i = 0.01f;
    // Notice that 'i' is a STATIC variable.
    // That's very important. (imagine me saying
    // that like Conchords in "Business Time")
    // http://youtube.com/watch?v=WGOohBytKTU

    // A 'static' variable is created ONCE
    // when the function in which it sits first runs.

    // The static variable will "LIVE ON"
    // between seperate calls to the function
    // in which it lives UNTIL THE PROGRAM ENDS.

    i+= 0.001f;     // increase i by 0.001 from its
    // it had on the LAST FUNCTION CALL to the draw() function

    float c = cos( i );
    float s = sin( i );

    glBegin (GL_TRIANGLES);
        glColor3f(  c, 0, 0 );      // red
        glVertex3f( 1+c, 0+s, 0 );

        glColor3f(  c, s, 0 );      // yellow
        glVertex3f( 0+c, 1+s, 0 );

        glColor3f(  s, 0.1f, s );   // magenta
        glVertex3f(-1+c, 0+s, 0 );
    glEnd();


}

namespace OGLUI
{

  void ClientContext::ClientContextImpl::Draw()
  {
    draw();
    SwapBuffers(DC);
  }

  ClientContext::ClientContextImpl::ClientContextImpl(System::WindowHandle wnd, const Rect &startRect)
    : Wnd(wnd)
    , DC(Wnd)
    , WndRect(startRect)
  {
    if (!Wnd)
      throw ClientContextException("Empty window handle");
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL|
                  PFD_DOUBLEBUFFER  |
                  PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 32;
    int PixelFormat = ::ChoosePixelFormat(DC, &pfd);
    if(!PixelFormat)
      throw ClientContextException("Can't choose pixel format");
    if (!::SetPixelFormat(DC, PixelFormat, &pfd))
      throw ClientContextException("Can't set pixel format");
    RenderingContext.Reset(new GLDC(DC));
  }

  ClientContext::ClientContextImpl::~ClientContextImpl()
  {
  }

}

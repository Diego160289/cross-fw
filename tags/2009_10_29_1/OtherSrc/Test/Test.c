#include "flash_dll.h"
#include "system.h"
#include "flash.h"

#include <stdio.h>


flash_dll *fl_dll;

static int on_event_proc(window *w, enum WindowEvent ev, void *data)
{
  if (ev == WE_CLOSE)
  {
    printf("To destroy....\n");
    system_loop_exit();
  }
  return 0;
}

static int on_f_ev(window *w, NPEvent *e)
{
  printf("OnEvent!\n");
  return 0;
}

int main()
{
  const char *Error;
  int i;
  fl_dll = 0;  
  Error = 0;
  fl_dll = dll_init("/usr/lib/browser-plugins/libflashplayer.so", &Error);
  if (fl_dll)
  {
    printf("Success init dll\n");
    if (!system_init())
    {
      printf("Success system init!\n");
      
      window *Win = system_window_create((const char*)"Hello Flash!!!", 400, 400, WF_FULLSCREEN, (on_event)&on_event_proc);
      if (Win)
      {
        printf("Created window!!!\n");
	system_window_show(Win, 0);
	flash *Flash = flashp_new(Win);
	if (Flash)
    	{
	  flashp_set_call(Flash, &on_f_ev);
	  flashp_set_attribute(Flash, "SRC", "file:///home/Dmitry/MyCpp/1/Test/test.swf");
	  printf("Create new falsh instance!!!\n");
	  system_window_set_flash(Win, Flash);
	  if (flashp_start(Flash))
	  {
	    printf("Flash started!!!!!!!!!!!!!!!!!!!!\n");
	  }
	  //updateFlashMetrics(Win, 350, 350);
	  //applyFlashMetrics(Win);
	  system_loop();
	  flashp_destroy(Flash);
	}
      }
      system_cleanup();
      dll_close(fl_dll);
      fl_dll = 0;
    }
  }
  else
    printf("Can't init dll. %s\n", Error);
  return 0;
}

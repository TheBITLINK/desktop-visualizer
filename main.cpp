#include <iostream>
#include <fftw3.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include "util.cpp"
#include "input/pulse.h"
#include "input/pulse.cpp"

float fps = 60;
int MAX_HEIGHT = 256;
int WINDOW_WIDTH = 1024;
float bars[42];

Window TransparentWindow () {
  Display* display = XOpenDisplay(NULL);
  XVisualInfo visualinfo;
  XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &visualinfo);

  // Create window
  Window wnd;
  GC gc;
  XSetWindowAttributes attr;
  attr.colormap = XCreateColormap(display, DefaultRootWindow(display), visualinfo.visual, AllocNone);
  attr.event_mask = ExposureMask | KeyPressMask;
  attr.background_pixmap = None;
  attr.border_pixel = 0;
  attr.override_redirect = true;
  wnd = XCreateWindow(
    display, DefaultRootWindow(display),
    (sf::VideoMode::getDesktopMode().width / 2) - (WINDOW_WIDTH / 2),
    sf::VideoMode::getDesktopMode().height - (MAX_HEIGHT * 2),
    WINDOW_WIDTH,
    MAX_HEIGHT * 2,
    0,
    visualinfo.depth,
    InputOutput,
    visualinfo.visual,
    CWColormap|CWEventMask|CWBackPixmap|CWBorderPixel,
    &attr
  );
  gc = XCreateGC(display, wnd, 0, 0);
  XStoreName(display, wnd, "Visualizer");

  XSizeHints sizehints;
  sizehints.flags = PPosition | PSize;
  sizehints.x = (sf::VideoMode::getDesktopMode().width / 2) - (WINDOW_WIDTH / 2);
  sizehints.y = sf::VideoMode::getDesktopMode().height - (MAX_HEIGHT * 2);
  sizehints.width = WINDOW_WIDTH;
  sizehints.height = MAX_HEIGHT * 2;
  XSetWMNormalHints(display, wnd, &sizehints);

  XSetWMProtocols(display, wnd, NULL, 0);

  x11_window_set_desktop(display, wnd);
  x11_window_set_borderless(display, wnd);
  x11_window_set_below(display, wnd);
  x11_window_set_sticky(display, wnd);
  x11_window_set_skip_taskbar(display, wnd);
  x11_window_set_skip_pager(display, wnd);

  return wnd;
}
#undef None

void draw(sf::RenderWindow* window) {
  int i;
  
  sf::Vector2u s = window->getSize();
  window->clear(sf::Color::Transparent);
  
  for (i = 0; i < 42; i++) {
    float bar = bars[i];
    float width = (float)s.x / (float)42;
    float height = bar * MAX_HEIGHT;
    float posY = (s.y / 2) - (height / 2);

    sf::RectangleShape rect(sf::Vector2f(width, height));
    rect.setPosition(sf::Vector2f(width * i, posY));
    rect.setFillColor(sf::Color(0, 255, 255, 255 * bar));

    window->draw(rect);
  }

  window->display();
  // std::cout << "FPS: " + std::to_string(fps) << std::endl;
}

int main () {
  Window win = TransparentWindow();
  sf::RenderWindow window(win);
  window.setFramerateLimit(60);
  
  sf::Clock clock;
  pthread_t p_thread;
  int i, thr_id, silence, sleep;
  struct timespec req = { .tv_sec = 0, .tv_nsec = 0 };
  struct audio_data audio;
  double in[2050];
  fftw_complex out[1025][2];
  fftw_plan p = fftw_plan_dft_r2c_1d(2048, in, *out, FFTW_MEASURE);
  int *freq;
  
  // Initialization
  for (i = 0; i < 42; i++) {
		bars[i] = 0;
	}

  audio.source = (char*)"auto";
  audio.format = -1;
  audio.terminate = 0;
  for (i = 0; i < 2048; i++) {
		audio.audio_out[i] = 0;
	}
  getPulseDefaultSink((void*)&audio);
	thr_id = pthread_create(&p_thread, NULL, input_pulse, (void*)&audio); 
	audio.rate = 44100;

  // Main Loop
  while (window.isOpen()) {
    // Handle Events
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
      window.close();
    }

    // Copy Audio Data
    silence = 1;
		for (i = 0; i < 2050; i++) {
			if (i < 2048) {
				in[i] = audio.audio_out[i];
				if (in[i]) silence = 0;
			} else {
				in[i] = 0;
			}
    }

    // Check if there was silence
    if (silence == 1) sleep++;
    else sleep = 0;

    if (sleep > fps * 5) {
      // i sleep
      req.tv_sec = 1;
      req.tv_nsec = 0;
      nanosleep(&req, NULL);
      continue;
    }
    
    // real shit??
    fftw_execute(p);

    for (i = 0; i < 42; i++) {
      int ab = pow(pow(*out[i * 2][0], 2) + pow(*out[i * 2][1], 2), 0.5);
      ab = ab + (ab * i / 30);
      float bar = (float)ab / (float)3500000;
      if (bar > 1) bar = 1;
      if (bar > bars[i]) {
        bars[i] = bar;
      } else {
        bars[i] -= 1 / fps;
      }
      if (bars[i] < 0) bars[i] = 0;
    }

    // Render
    draw(&window);
    fps = 1 / clock.restart().asSeconds();
  }

  // Free resources
  audio.terminate = 1;
	pthread_join(p_thread, NULL);
  free(audio.source);

  return 0;
}
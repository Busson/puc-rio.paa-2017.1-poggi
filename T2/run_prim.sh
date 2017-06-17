g++ cpu_timer/CPUTimer.cpp input_reader/input_reader.c prim.c `pkg-config --cflags --libs glib-2.0 gobject-2.0 gio-2.0` -o prim 

./prim
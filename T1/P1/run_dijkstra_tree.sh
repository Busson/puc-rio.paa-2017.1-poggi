g++ cpu_timer/CPUTimer.cpp stp/stp_reader.c dijkstra_tree.c `pkg-config --cflags --libs glib-2.0 gobject-2.0 gio-2.0` -w -o dijkstra_tree

./dijkstra_tree
g++ cpu_timer/CPUTimer.cpp bucket/bucket.c stp/stp_reader.c dijkstra_bucket.c `pkg-config --cflags --libs glib-2.0 gobject-2.0 gio-2.0` -w -o dijkstra_bucket

./dijkstra_bucket
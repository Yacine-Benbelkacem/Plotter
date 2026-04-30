CC = gcc
frame_test_src_dir = ..
frame_test_src = $(frame_test_src_dir)/Frame.c
frame_test_inc = -I$(frame_test_src_dir)

frame_test: $(frame_test_src)
	$(CC) -o $@ frame_test.c $^ $(frame_test_inc) -lm

object_test_src_dir = ..
object_test_src = $(object_test_src_dir)/Object.c 
object_test_inc = -I$(object_test_src_dir)

object_test: $(frame_test_src) $(object_test_src)
	$(CC) -o $@ object_test.c  $^ $(object_test_inc) -lm

figure_test_src_dir = ..
figure_test_src = $(figure_test_src_dir)/Figure.c
figure_test_inc = -I$(figure_test_src_dir)

figure_test: $(figure_test_src) $(frame_test_src) $(object_test_src)
	$(CC) -o $@ figure_test.c $^ $(figure_test_inc) -lm -lSDL2

subplot_test_src_dir = ..
subplot_test_src = $(subplot_test_src_dir)/Subplot.c
subplot_test_inc = -I$(subplot_test_src_dir)

subplot_test: $(subplot_test_src) $(frame_test_src) $(object_test_src) $(figure_test_src)
	$(CC) -o $@ subplot_test.c $^ $(subplot_test_inc) -lm -lSDL2

clean:
	rm -f frame_test object_test figure_test subplot_test *.o
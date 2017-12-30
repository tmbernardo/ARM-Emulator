PROGS = driver
OBJS = 

CFLAGS = -g

all : ${PROGS}

driver : driver.c armemu.c header.h find_max_a.s sum_array_a.s find_str_a.s fib_iter_a.s fib_rec_a.s
	gcc ${CFLAGS} -o driver driver.c armemu.c sum_array_c.c sum_array_a.s find_max_c.c find_max_a.s \
		fib_iter_c.c fib_iter_a.s fib_rec_c.c fib_rec_a.s find_str_c.c find_str_a.s

clean:
	rm -rf ${PROGS} ${OBJS}

CC ?= gcc
DIR ?= .

CFLAGS ?= -fstack-protector-strong -Wall -Wextra -Wformat -Werror=format-security \
	-D_FORTIFY_SOURCE=2 -fPIC
LDFLAGS ?= -Wl,-z,relro -Wl,-z,now -Wl,--as-needed -pie -L$(DIR) -lc
OPTIMIZATION ?= -O3

test_objects = main.o base64.o Timer.o testbase64.o

nibble-and-a-half: libnaah64.so libnaah64.a

test: $(test_objects)
	$(CC) $(CFLAGS) $(test_objects) -o $(DIR)/test $(LDFLAGS)
	chmod +x $(DIR)/test

%.o: %.c
	$(CC) -c $(OPTIMIZATION) $(CFLAGS) -o $@ $<

libnaah64.so: base64.h base64.o
	$(CC) -shared -o $@ base64.o

libnaah64.a: base64.o
	ar cr $@ $+

.PHONY: clean install nibble-and-a-half

install: nibble-and-a-half
	cp $(DIR)/libnaah64.a ~/.local/lib/
	cp $(DIR)/libnaah64.so ~/.local/lib/

clean:
	rm -rf $(DIR)/libnaah64.a $(DIR)/libnaah64.so $(DIR)/test $(test_objects)

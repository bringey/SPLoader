
CFLAGS   := -std=c11 -Wall -Wextra -pedantic
CPPFLAGS := -I../../include
LIBS     :=
LDFLAGS  :=


mkimage: mkimage.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LIBS) $(LDFLAGS)

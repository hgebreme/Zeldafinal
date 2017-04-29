CC= g++
CFLAGS = -lSDL2 -lSDL2_image

all:	$(TARGETS)

ltexture.o: ltexture.cpp ltexture.h
	$(CC) $(CFLAGS) -c -o $@ $<

character.o: character.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

link.o: link.cpp link.h character.o
	$(CC) $(CFLAGS) -c -o $@ $<

enemy.o: enemy.cpp enemy.h character.o
	$(CC) $(CFLAGS) -c -o $@ $<

boss.o: boss.cpp boss.h character.o
	$(CC) $(CFLAGS) -c -o $@ $<

classes.o: classes.cpp
	$(CC) $(CFLAGS) -c -o $@ $<
#add link.o enemy.o and boss.o
classes: classes.o ltexture.o link.o character.o enemy.o boss.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS) *.o

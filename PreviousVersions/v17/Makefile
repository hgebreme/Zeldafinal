CC= g++
CFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer

takedamage: main.o ltexture.o link.o character.o enemy.o boss.o
	$(CC) $(CFLAGS) -o $@ $^

ltexture.o: ltexture.cpp ltexture.h
	$(CC) $(CFLAGS) -c -o $@ $<

character.o: character.cpp character.h
	$(CC) $(CFLAGS) -c -o $@ $<

link.o: link.cpp link.h character.o
	$(CC) $(CFLAGS) -c -o $@ $<

enemy.o: enemy.cpp enemy.h character.o
	$(CC) $(CFLAGS) -c -o $@ $<

boss.o: boss.cpp boss.h character.o
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.cpp ltexture.o character.o link.o enemy.o boss.o
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGETS) *.o

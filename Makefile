CC = gcc
CFLAGS = -Wall -g -lm
EXEC = ChaineMain HachageMain ArbreQuatMain GrapheMain ReconstitueReseau ex6

all: $(EXEC)

ChaineMain: ChaineMain.o Chaine.o SVGwriter.o
	$(CC) $^ -o $@ $(CFLAGS)

HachageMain: HachageMain.o Hachage.o Chaine.o Reseau.o SVGwriter.o
	$(CC) $^ -o $@ $(CFLAGS)

ArbreQuatMain: ArbreQuatMain.o ArbreQuat.o Chaine.o Reseau.o SVGwriter.o
	$(CC) $^ -o $@ $(CFLAGS)

GrapheMain: GrapheMain.o Graphe.o Chaine.o Reseau.o SVGwriter.o Struct_File.o Struct_Liste.o
	$(CC) $^ -o $@ $(CFLAGS)
	
ReconstitueReseau: ReconstitueReseau.o Hachage.o ArbreQuat.o Reseau.o Chaine.o SVGwriter.o
	$(CC) $^ -o $@ $(CFLAGS)

ex6: ex6.o Hachage.o ArbreQuat.o Reseau.o Chaine.o SVGwriter.o
	$(CC) $^ -o $@ $(CFLAGS)

%.o: %.c %.h
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean run 

clean:
	rm -f *.o *.html 
	rm -f $(EXEC)

run:  
	valgrind ./ChaineMain 00014_burma.cha && echo
	valgrind ./HachageMain 00014_burma.cha && echo 
	valgrind ./ArbreQuatMain 00014_burma.cha && echo 
	valgrind ./ReconstitueReseau 00014_burma.cha 1 && echo 
	valgrind ./ReconstitueReseau 00014_burma.cha 2 && echo 
	valgrind ./ReconstitueReseau 00014_burma.cha 3 && echo 
	valgrind ./GrapheMain 00014_burma.cha && echo    

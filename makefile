flags = -ansi -lMLV

main: picgene.o imageIO.o utils.o amoeba.o pool.o view.o
	gcc -o main $^ $(flags) -O1

picgene.o: picgene.c
	gcc -c $^ $(flags)

imageIO.o: modules/imageIO.c
	gcc -c $^ $(flags)

utils.o: modules/utils.c
	gcc -c $^ $(flags)

amoeba.o: modules/amoeba.c
	gcc -c $^ $(flags)

pool.o: modules/pool.c
	gcc -c $^ $(flags)

view.o: modules/view.c
	gcc -c $^ $(flags)

clean:
	rm *.o
	rm main
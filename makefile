flags = -ansi -lMLV

main: picgene.o imageIO.o utils.o amoeba.o
	gcc -o main $^ $(flags)

picgene.o: picgene.c
	gcc -c $^ $(flags)

imageIO.o: imageIO.c
	gcc -c $^ $(flags)

utils.o: utils.c
	gcc -c $^ $(flags)

amoeba.o: amoeba.c
	gcc -c $^ $(flags)

clean:
	rm *.o
	rm main
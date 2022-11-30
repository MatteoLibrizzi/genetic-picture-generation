flags = -ansi -lMLV

main: picgene.o imageIO.o
	gcc -o main $^ $(flags)

picgene.o: picgene.c
	gcc -c $^ $(flags)

imageIO.o: imageIO.c
	gcc -c $^ $(flags)

clean:
	rm *.o
	rm main
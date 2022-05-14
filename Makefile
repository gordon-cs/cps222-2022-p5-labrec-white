#RT This has multiple problems.  See correct rules below.
#project5:	graph.cc project5_main.cc
#	g++ -std=c++11 -o project5 project5_main.cc

#RT Rebuild (relink) the executable whenever one of the source .cc files
# is recompiled, to use the latest .o file.
project5:	graph.o project5_main.o
	g++ -std=c++11 -o project5 graph.o project5_main.o

#RT Tell compiler to use these flags, including asking for warnings.
CPPFLAGS=-std=c++11 -Wall

# .o files depend on .h file that the .cc files include.  If the .h
# changes, the .cc needs to be recompiled.  Make knows how to do the compile,
# it just needs to know when.
graph.o:	graph.h

project5_main.o:	graph.h

test: project5
	mkdir test-output
	./project5 < t01-nowhere.in > ./test-output/t01-nowhere.out
	./project5 < t02-one-road.in > ./test-output/t02-one-road.out
	./project5 < t03-simple.in > ./test-output/t03-simple.out
	./project5 < t05-local.in > ./test-output/t05-local.out
	./project5 < t08-local.in > ./test-output/t08-local.out
	./project5 < t09-middle-earth.in > ./test-output/t09-middle-earth.out
	./project5 < t10-straight-line.in > ./test-output/t10-straight-line.out
	./project5 < t0502-combo.in > ./test-output/t0502-combo.out
	-diff t01-nowhere.out ./test-output/t01-nowhere.out
	-diff t02-one-road.out ./test-output/t02-one-road.out
	-diff t03-simple.out ./test-output/t03-simple.out
	-diff t05-local.out ./test-output/t05-local.out
	-diff t08-local.out ./test-output/t08-local.out
	-diff t09-middle-earth.out ./test-output/t09-middle-earth.out
	-diff t10-straight-line.out ./test-output/t10-straight-line.out
	-diff t0502-combo.out ./test-output/t0502-combo.out
	rm -r test-output

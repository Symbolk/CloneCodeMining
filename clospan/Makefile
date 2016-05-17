all   : bin/clospan

bin/clospan : src/*.cpp src/SeqTree/*.cpp src/*.h src/SeqTree/*.h
	g++ -fpermissive -O3 -o bin/clospan src/*.cpp src/SeqTree/*.cpp

debug : bin/clospan_debug 

bin/clospan_debug: src/*.cpp src/SeqTree/*.cpp src/*.h src/SeqTree/*.h
	g++ -fpermissive -fprofile-arcs -ftest-coverage -fbounds-check -g -o bin/clospan_debug src/*.cpp src/SeqTree/*.cpp

test  : all
	test/test_all.sh

doc :
	doxygen

clean :
	rm -Rf bin/* www/webtest.* www/scheduler.* www/frequent.dat www/error.tmp


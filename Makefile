all:
	 g++ orbits.cpp -o orbits -lglut -lGL

clean:
	rm -rf orbits

my_functions.so: my_functions.cpp
	g++ -fPIC -shared -o my_functions.so my_functions.cpp lodepng.cpp
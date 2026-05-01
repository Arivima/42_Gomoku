
empty:
	g++ empty_sfml_window.cpp -o app \
		-std=c++17 \
		-I/opt/homebrew/opt/sfml/include \
		-L/opt/homebrew/opt/sfml/lib \
		-Wl,-rpath,/opt/homebrew/opt/sfml/lib \
		-lsfml-graphics -lsfml-window -lsfml-system
	./app
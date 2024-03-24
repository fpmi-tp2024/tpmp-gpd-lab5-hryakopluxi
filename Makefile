run: bin/program
	./bin/program

bin/program: obj/car.o obj/sql.o obj/config.o obj/controller.o obj/dispatcher.o obj/driver.o obj/order.o obj/user.o obj/validator.o obj/view.o obj/main.o
	g++ -o bin/program \
	obj/sql.o \
	obj/config.o \
 	obj/car.o \
 	obj/user.o \
 	obj/dispatcher.o \
 	obj/driver.o \
 	obj/order.o \
 	obj/validator.o \
 	obj/controller.o \
 	obj/view.o \
 	obj/main.o \
 	-lsqlite3 -lbcrypt

obj/main.o: src/main.cpp
	g++ -o obj/main.o -c src/main.cpp

obj/car.o: src/car.cpp
	g++ -o obj/car.o -c src/car.cpp

obj/config.o: src/config.cpp
	g++ -o obj/config.o -c src/config.cpp

obj/controller.o: src/controller.cpp
	g++ -o obj/controller.o -c src/controller.cpp

obj/dispatcher.o: src/dispatcher.cpp
	g++ -o obj/dispatcher.o -c src/dispatcher.cpp

obj/driver.o: src/driver.cpp
	g++ -o obj/driver.o -c src/driver.cpp

obj/order.o: src/order.cpp
	g++ -o obj/order.o -c src/order.cpp

obj/user.o: src/user.cpp
	g++ -o obj/user.o -c src/user.cpp

obj/validator.o: src/validator.cpp
	g++ -o obj/validator.o -c src/validator.cpp

obj/view.o: src/view.cpp
	g++ -o obj/view.o -c src/view.cpp

obj/sql.o: src/sql.cpp
	g++ -o obj/sql.o -c src/sql.cpp

clean:
	rm -rf obj/*.o bin/program

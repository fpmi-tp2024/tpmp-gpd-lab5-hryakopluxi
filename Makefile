LIB = -lsqlite3 -lssl -lcrypto
LIBTEST = -lgtest -lgtest_main
FLAGS = -std=c++20
CPPFLAGS = $(FLAGS)
TESTFLAGS = $(FLAGS)
CXX = g++
obj = obj/car.o obj/menu.o obj/sql.o obj/config.o obj/controller.o obj/dispatcher.o obj/driver.o obj/order.o obj/user.o obj/validator.o obj/view.o obj/main.o
testobj = obj/test.o obj/tcar.o obj/tmenu.o obj/tsql.o obj/tconfig.o obj/tcontroller.o obj/tdispatcher.o obj/tdriver.o obj/torder.o obj/tuser.o obj/tvalidator.o obj/tview.o
EXEC = bin/main
TESTEXEC = bin/test_main


run: dirs $(EXEC)
	./$(EXEC)

build: dirs $(EXEC)

test: dirs $(TESTEXEC)
	./$(TESTEXEC); git restore db

$(EXEC): $(obj)
	$(CXX) -o $(EXEC) $(obj) $(LIB)

$(TESTEXEC): $(testobj)
	$(CXX) -o $(TESTEXEC) $(testobj) $(LIBTEST) $(LIB)

obj/%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

obj/t%.o: src/%.cpp
	$(CXX) $(TESTFLAGS) -c $< -o $@

obj/%.o: test/%.cpp
	$(CXX) $(TESTFLAGS) -c $< -o $@

dirs:
	-mkdir -p obj bin

clean:
	rm -rf obj/* bin/*
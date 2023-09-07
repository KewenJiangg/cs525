CXX = g++
TARGET = test_assign1
OBJ = test_assign1_1.o dberror.o storage_mgr.o

$(TARGET): $(OBJ)
	$(CXX) -o $(TARGET) $(OBJ)

test_assign1_1.o: test_assign1_1.c
	$(CXX) -c test_assign1_1.c

dberror.o: dberror.c
	$(CXX) -c dberror.c

storage_mgr.o: storage_mgr.c
	$(CXX) -c storage_mgr.c

.PHONY:clean
clean:
	rm -f test *.o 
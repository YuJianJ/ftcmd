# brcm
#64bit
#32bit

# qca
#64bit
#32bit

# ubuntu 
CC=gcc

SRC	=	$(wildcard ./src/*.c)
OBJ	=	$(patsubst %.c , %.o , $(SRC))
TAG_FILE =	./bin/ftcmd

.PHONY:all
all : $(TAG_FILE)

$(TAG_FILE):$(OBJ)
	@$(CC) -o $(TAG_FILE) $(OBJ) -I ./include  
	@echo "==> bin is $(TAG_FILE) "

.PHONY:clean
clean:
	@/bin/rm -rf  ./*.o ./*/*.o $(TAG_FILE) 
	@echo "clean all *.o and $(TAG_FILE)"


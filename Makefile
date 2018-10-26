OBJ:=test

DEPEND1:=main
DEPEND2:=mcount

${OBJ}: ${DEPEND1}.o ${DEPEND2}.o
	${CC} mcount.o main.o -o $@

${DEPEND1}.o: ${DEPEND1}.c
	${CC} -pg -c $^ -o $@

${DEPEND2}.o:mcount.c
	${CC} -c $^ -o $@

sym:
	objdump -axd ${OBJ} > ${OBJ}.s

arm:
	${CC} -c  arm_mcount.c -o mcount.o
	ar -r libmcount.a mcount.o

clean:
	rm -rf ${DEPEND1}.o ${DEPEND2}.o ${OBJ} 

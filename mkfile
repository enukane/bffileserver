</$objtype/mkfile

TARG=bf

OFILES=\
       main.$O\
       bf.$O\

HFILES=\
       bf.h\

BIN=/$objtype/bin

UPDATE=\
	mkfile\
 	$HFILES\
	${OFILES:%.$O=%.c}\

</sys/src/cmd/mkone


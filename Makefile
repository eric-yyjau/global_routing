#
# This is for your executable
#
#CFLAGS = -O3 -DNDEBUG
CFLAGS = -O3
#CFLAGS = -Wall -Dr_debug
#CFLAGS = -Wall -Ds_debug
#CFLAGS = -Wall -Dt_debug
#CFLAGS = -Wall -Du_debug
#CFLAGS = -Wall -Dv_debug
#CFLAGS = -Wall -Dheap_debug
#CFLAGS = -Wall

#
# This is the setting for TA
#
# CFLAGS = -g -Wall -DTA_KB_SETTING
# CFLAGS = -O3 -DTA_KB_SETTING
# CFLAGS = -O3 -m32 -DTA_KB_SETTING

###############
# PHONY rules #
###############
.PHONY : routing


routing : MAKEFLAGS =  $(CFLAGS)


routing :  clean global_routing


##############
# Executable #
##############
global_routing: 	main.o parser.o routing.o
	g++ -o $@ $(MAKEFLAGS)  main.o parser.o routing.o


#global_routing: 	main.o 
#	g++ -o $@ $(MAKEFLAGS)  main.o libparser.a



################
# Object files #
################
main.o: main.cpp parser.h 
	g++ -c $(MAKEFLAGS) main.cpp 

#myString.o: myString.cpp myString.h
#	g++ -c $(MAKEFLAGS) myString.cpp

parser.o: parser.cpp parser.h
	g++ -c $(MAKEFLAGS) parser.cpp

routing.o: routing.cpp routing.h myMinHeap.h
	g++ -c $(MAKEFLAGS) routing.cpp


#mps.o: mps.h
#	g++ -c $(MAKEFLAGS) mps.cpp



#######################
# No dependency rules #
#######################
clean:
	rm -f *.o main parser

#mergesort:
	
	

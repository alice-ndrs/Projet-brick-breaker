# Definitions de macros

CXX     = g++
CXXFLAGS = -g -Wall -std=c++11
CXXFILES = project.cc ball.cc brick.cc paddle.cc game.cc tool.cc
OBJ = $(CXXFILES:.cc=.o)


# Definition de la premiere regle

project: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o project
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@
#pas sur de la ligne au dessus
# Definitions de cibles particulieres

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ project

# DO NOT DELETE THIS LINE
project.o: project.cc
ball.o: ball.cc ball.h
brick.o: brick.cc brick.h
paddle.o: paddle.cc paddle.h
game.o: game.cc game.h
tool.o: tool.cc

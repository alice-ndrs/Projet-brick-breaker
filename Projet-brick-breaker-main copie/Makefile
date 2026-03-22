# Definitions de macros

CXX     = g++
CXXFLAGS = -g -Wall -std=c++11
CXXFILES = Project.cc Ball.cc Brick.cc Paddle.cc Game.cc Message.cc Rainbow_Brick.cc
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
Project.o: Project.cc Game.h Brick.h Paddle.h Ball.h
Ball.o: Ball.cc Ball.h Constants.h tools.h Message.h Brick.h Paddle.h
Brick.o: Brick.cc Brick.h Paddle.h Message.h Constants.h tools.h
Paddle.o: Paddle.cc Paddle.h Message.h Constants.h tools.h
Game.o: Game.cc Game.h Brick.h Paddle.h Ball.h Message.h Constants.h \
  tools.h
Message.o: Message.cc Message.h
Rainbow_Brick.o: Rainbow_Brick.cc Rainbow_Brick.h Brick.h Paddle.h

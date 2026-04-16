# Definitions de macros

CXX     = g++
CXXFLAGS = -g -Wall -std=c++11
CXXFILES = Project.cc Ball.cc Brick.cc Paddle.cc Game.cc Message.cc tools.cc
OBJ = $(CXXFILES:.cc=.o)


# Definition de la premiere regle

project: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o project
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

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
Project.o: Project.cc Game.h Ball.h tools.h Paddle.h
Ball.o: Ball.cc Ball.h tools.h Paddle.h Brick.h Constants.h Message.h
Brick.o: Brick.cc Brick.h tools.h Constants.h Message.h Paddle.h
Paddle.o: Paddle.cc Paddle.h tools.h Message.h Constants.h
Game.o: Game.cc Game.h Ball.h tools.h Paddle.h Message.h Brick.h \
  Constants.h
Message.o: Message.cc Message.h
tools.o: tools.cc tools.h

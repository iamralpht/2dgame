TARGETS=	Gen/float/hw

PROJECT=	../..

#CXXFLAGS=	-g -O2 -I/usr/lib/include -I$(PROJECT)/Contrib -I$(PROJECT)/Include -DGLUI_FREEGLUT

BOX2D = /home/ralpht/tmp/b2d

CXXFLAGS = -g -O3 -I$(BOX2D)/include

LDFLAGS=	-L/opt/local/lib $(BOX2D)/lib/libBox2D.a -lpng -lz

SOURCES=	HelloWorld.cpp MapView.cpp MapModel.cpp LoadPng.cpp CameraController.cpp CarView.cpp KeyboardController.cpp BoxMapView.cpp CarModel.cpp DebugDraw.cpp

ifneq ($(INCLUDE_DEPENDENCIES),yes)

all:	
	$(MAKE) --no-print-directory INCLUDE_DEPENDENCIES=yes $(TARGETS)

.PHONY:	clean
clean:
	rm -rf Gen

else

-include $(addprefix Gen/float/,$(SOURCES:.cpp=.d))
-include $(addprefix Gen/fixed/,$(SOURCES:.cpp=.d))

endif



FLOAT_OBJECTS= $(addprefix Gen/float/,$(SOURCES:.cpp=.o))

Gen/float/%.o:		%.cpp
	mkdir -p $(dir $@)
	c++ $(CXXFLAGS) -c -o $@ $<

Gen/float/hw:	$(FLOAT_OBJECTS)
	c++ -o $@ $^ $(LDFLAGS) -lglui -lglut -lGLU -lGL

Gen/float/%.d:		%.cpp
	@mkdir -p $(dir $@)
	c++ -M -MT $(@:.d=.o) $(CXXFLAGS) -o $@ $<



TARGETS=	out/driving

PROJECT=	../..

#CXXFLAGS=	-g -O2 -I/usr/lib/include -I$(PROJECT)/Contrib -I$(PROJECT)/Include -DGLUI_FREEGLUT

BOX2D = /home/ralpht/tmp/b2d

CXXFLAGS = -g -O3 -I$(BOX2D)/include

LDFLAGS=	$(BOX2D)/lib/libBox2D.a -lpng -lz

SOURCES=	HelloWorld.cpp MapView.cpp MapModel.cpp LoadPng.cpp CameraController.cpp CarView.cpp KeyboardController.cpp BoxMapView.cpp CarModel.cpp DebugDraw.cpp

ifneq ($(INCLUDE_DEPENDENCIES),yes)

all:	
	@$(MAKE) --no-print-directory INCLUDE_DEPENDENCIES=yes $(TARGETS)

.PHONY:	clean
clean:
	@rm -rf out

else
-include $(addprefix out/,$(SOURCES:.cpp=.d))
endif



FLOAT_OBJECTS= $(addprefix out/,$(SOURCES:.cpp=.o))

out/%.o:		%.cpp
	@mkdir -p $(dir $@)
	@echo c++ $<
	@c++ $(CXXFLAGS) -c -o $@ $<

out/driving:	$(FLOAT_OBJECTS)
	@echo link $@
	@c++ -o $@ $^ $(LDFLAGS) -lglut -lGLU -lGL

out/%.d:		%.cpp
	@mkdir -p $(dir $@)
	@echo deps $@
	@c++ -M -MT $(@:.d=.o) $(CXXFLAGS) -o $@ $<



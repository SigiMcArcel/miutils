

DESTDIR=
LIBDIR=/usr/lib
INCLUDEDIR=/usr/include

INCLUDES = -I. -I/.
CXX_FLAGS = -fpic -Wall
LD_FLAGS = -shared -lpthread

SOURCES=miString.cpp Time.cpp Timer.cpp
OBJECTS=$(SOURCES:.cpp=.o)

LIBRARYOUT=libmiutils.so


.cpp.o:
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -c $<
$(LIBRARYOUT):$(OBJECTS)
	$(CXX) $(LD_FLAGS) $(OBJECTS) -o $(LIBRARYOUT)
clean:
	$(RM) $(LIBRARYOUT) *.o
	

install: $(LIBRARYOUT)
	mkdir -p $(DESTDIR)/$(LIBDIR)
	mkdir -p $(DESTDIR)/$(INCLUDEDIR)
	cp miString.h $(DESTDIR)/$(INCLUDEDIR)
	cp Time.h $(DESTDIR)/$(INCLUDEDIR)
	cp Timer.h $(DESTDIR)/$(INCLUDEDIR)
	cp CriticalSection.h $(DESTDIR)/$(INCLUDEDIR)
	cp Event.h $(DESTDIR)/$(INCLUDEDIR)
	cp $(LIBRARYOUT) $(DESTDIR)/$(LIBDIR)
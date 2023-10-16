DESTDIR=
prefix=/usr
includedir=$(prefix)/include
libdir=$(prefix)/lib



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
	install -d $(DESTDIR)/${libdir}
	install -D -m 0644 libmidriver.so $(DESTDIR)/${libdir}/$(LIBRARYOUT)
	install -d $(DESTDIR)/${includedir}
	install -d $(DESTDIR)/${includedir}/mi
	install -d $(DESTDIR)/${includedir}/mi/miutils
	install -D -m 0644 miString.h $(DESTDIR)/${includedir}/mi/midriver/miString.h
	install -D -m 0644 Time.h $(DESTDIR)/${includedir}/mi/midriver/Time.h
	install -D -m 0644 Timer.h $(DESTDIR)/${includedir}/mi/midriver/Timer.h
	install -D -m 0644 CriticalSection.h $(DESTDIR)/${includedir}/mi/midriver/CriticalSection.h
	install -D -m 0644 Event.h $(DESTDIR)/${includedir}/mi/midriver/Event.h

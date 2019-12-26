# Makefile pour un unique exécutable

# liste des variables à renseigner
#   CIBLE : nom du programme ( $(CIBLE).c doit contenir main() )
#   SRCS : ensemble des fichiers sources 
#   LIBS : liste des bibliothèques utiles à l'édition des liens 
#          (format : -lnom1 -lnom2 ...) 
#   PREFIX : chemin de la hiérarchie 
#
# NE PAS OUBLIER D'AJOUTER LA LISTE DES DEPENDANCES A LA FIN DU FICHIER

CIBLE = tp
SRCS =  src/Camera.cpp tp.cpp src/Trackball.cpp ann/src/ANN.cpp ann/src/brute.cpp ann/src/kd_tree.cpp ann/src/kd_util.cpp ann/src/kd_split.cpp \
	ann/src/kd_dump.cpp ann/src/kd_search.cpp ann/src/kd_pr_search.cpp ann/src/kd_fix_rad_search.cpp \
	ann/src/bd_tree.cpp ann/src/bd_search.cpp ann/src/bd_pr_search.cpp ann/src/bd_fix_rad_search.cpp \
	ann/src/perf.cpp
LIBS =  -L/usr/lib -lglut -lGLU -lGL -lm -lpthread -lgsl -lgslcblas

#########################################################"

INCDIR = .
LIBDIR = .
BINDIR = .

# nom du compilateur
CC = g++
CPP = g++

# options du compilateur          
CFLAGS = -Wall -O3 
CXXFLAGS = -Wall -O3 

# option du preprocesseur
CPPFLAGS =  -I$(INCDIR) -Iann/include

# options du linker et liste des bibliothèques à charger
LDFLAGS = -L/usr/X11R6/lib              
LDLIBS = -L$(LIBDIR) $(LIBS)  

# construire la liste des fichiers objets une nouvelle chaine à partir
# de SRCS en substituant les occurences de ".c" par ".o" 
OBJS = $(SRCS:.cpp=.o)   

# cible par défaut
$(CIBLE): $(OBJS)

install:  $(CIBLE)
	cp $(CIBLE) $(BINDIR)/

installdirs:
	test -d $(INCDIR) || mkdir $(INCDIR)
	test -d $(LIBDIR) || mkdir $(LIBDIR)
	test -d $(BINDIR) || mkdir $(BINDIR)

clean:
	rm -f  *~  $(CIBLE) $(OBJS)

veryclean: clean
	rm -f $(BINDIR)/$(CIBLE)

dep:
	gcc $(CPPFLAGS) -MM $(SRCS)

# liste des dépendances générée par 'make dep'
Camera.o: src/Camera.cpp src/Camera.h src/Vec3.h src/Trackball.h
tp.o: tp.cpp src/Vec3.h src/Camera.h src/Trackball.h
Trackball.o: src/Trackball.cpp src/Trackball.h



#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=DebugStatic
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/ClientSocket.o \
	${OBJECTDIR}/JPEGImage.o \
	${OBJECTDIR}/ServerSocket.o \
	${OBJECTDIR}/Socket.o \
	${OBJECTDIR}/myconverters.o \
	${OBJECTDIR}/mycurl.o \
	${OBJECTDIR}/mystdlib.o \
	${OBJECTDIR}/myxml.o


# C Compiler Flags
CFLAGS=-O0

# CC Compiler Flags
CCFLAGS=-g -O0
CXXFLAGS=-g -O0

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbase.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbase.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbase.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbase.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbase.a

${OBJECTDIR}/ClientSocket.o: ClientSocket.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -I. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ClientSocket.o ClientSocket.cpp

${OBJECTDIR}/JPEGImage.o: JPEGImage.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -I. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/JPEGImage.o JPEGImage.cpp

${OBJECTDIR}/ServerSocket.o: ServerSocket.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -I. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ServerSocket.o ServerSocket.cpp

${OBJECTDIR}/Socket.o: Socket.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -I. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Socket.o Socket.cpp

${OBJECTDIR}/myconverters.o: myconverters.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -I. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/myconverters.o myconverters.cpp

${OBJECTDIR}/mycurl.o: mycurl.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -I. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mycurl.o mycurl.cpp

${OBJECTDIR}/mystdlib.o: mystdlib.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -I. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mystdlib.o mystdlib.cpp

${OBJECTDIR}/myxml.o: myxml.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -I. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/myxml.o myxml.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbase.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc

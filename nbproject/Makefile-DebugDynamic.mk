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
CND_PLATFORM=GNU-MacOSX
CND_DLIB_EXT=dylib
CND_CONF=DebugDynamic
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/ClientSocket.o \
	${OBJECTDIR}/FFJSON.o \
	${OBJECTDIR}/JPEGImage.o \
	${OBJECTDIR}/ServerSocket.o \
	${OBJECTDIR}/Socket.o \
	${OBJECTDIR}/logger.o \
	${OBJECTDIR}/myconverters.o \
	${OBJECTDIR}/mycurl.o \
	${OBJECTDIR}/mystdlib.o \
	${OBJECTDIR}/myxml.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f2

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
LDLIBSOPTIONS=-lxml2 -lpthread -lssl -lcrypto -lz -lrt

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${TESTDIR}/TestFiles/f1

${TESTDIR}/TestFiles/f1: ${OBJECTFILES}
	${MKDIR} -p ${TESTDIR}/TestFiles
	g++ -o ${TESTDIR}/TestFiles/f1 ${OBJECTFILES} ${LDLIBSOPTIONS} -Wl,-soname,lib${APPNAME}.so.${MAJOR_VERSION} -dynamiclib -install_name f1 -fPIC

${OBJECTDIR}/ClientSocket.o: ClientSocket.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ClientSocket.o ClientSocket.cpp

${OBJECTDIR}/FFJSON.o: FFJSON.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FFJSON.o FFJSON.cpp

${OBJECTDIR}/JPEGImage.o: JPEGImage.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/JPEGImage.o JPEGImage.cpp

${OBJECTDIR}/ServerSocket.o: ServerSocket.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ServerSocket.o ServerSocket.cpp

${OBJECTDIR}/Socket.o: Socket.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Socket.o Socket.cpp

${OBJECTDIR}/logger.o: logger.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/logger.o logger.cpp

${OBJECTDIR}/myconverters.o: myconverters.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/myconverters.o myconverters.cpp

${OBJECTDIR}/mycurl.o: mycurl.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mycurl.o mycurl.cpp

${OBJECTDIR}/mystdlib.o: mystdlib.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mystdlib.o mystdlib.cpp

${OBJECTDIR}/myxml.o: myxml.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/myxml.o myxml.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/ffjsonTest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} 


${TESTDIR}/tests/ffjsonTest.o: tests/ffjsonTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/ffjsonTest.o tests/ffjsonTest.cpp


${OBJECTDIR}/ClientSocket_nomain.o: ${OBJECTDIR}/ClientSocket.o ClientSocket.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/ClientSocket.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ClientSocket_nomain.o ClientSocket.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/ClientSocket.o ${OBJECTDIR}/ClientSocket_nomain.o;\
	fi

${OBJECTDIR}/FFJSON_nomain.o: ${OBJECTDIR}/FFJSON.o FFJSON.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/FFJSON.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FFJSON_nomain.o FFJSON.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/FFJSON.o ${OBJECTDIR}/FFJSON_nomain.o;\
	fi

${OBJECTDIR}/JPEGImage_nomain.o: ${OBJECTDIR}/JPEGImage.o JPEGImage.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/JPEGImage.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/JPEGImage_nomain.o JPEGImage.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/JPEGImage.o ${OBJECTDIR}/JPEGImage_nomain.o;\
	fi

${OBJECTDIR}/ServerSocket_nomain.o: ${OBJECTDIR}/ServerSocket.o ServerSocket.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/ServerSocket.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ServerSocket_nomain.o ServerSocket.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/ServerSocket.o ${OBJECTDIR}/ServerSocket_nomain.o;\
	fi

${OBJECTDIR}/Socket_nomain.o: ${OBJECTDIR}/Socket.o Socket.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Socket.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Socket_nomain.o Socket.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Socket.o ${OBJECTDIR}/Socket_nomain.o;\
	fi

${OBJECTDIR}/logger_nomain.o: ${OBJECTDIR}/logger.o logger.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/logger.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/logger_nomain.o logger.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/logger.o ${OBJECTDIR}/logger_nomain.o;\
	fi

${OBJECTDIR}/myconverters_nomain.o: ${OBJECTDIR}/myconverters.o myconverters.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/myconverters.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/myconverters_nomain.o myconverters.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/myconverters.o ${OBJECTDIR}/myconverters_nomain.o;\
	fi

${OBJECTDIR}/mycurl_nomain.o: ${OBJECTDIR}/mycurl.o mycurl.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/mycurl.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mycurl_nomain.o mycurl.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/mycurl.o ${OBJECTDIR}/mycurl_nomain.o;\
	fi

${OBJECTDIR}/mystdlib_nomain.o: ${OBJECTDIR}/mystdlib.o mystdlib.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/mystdlib.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mystdlib_nomain.o mystdlib.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/mystdlib.o ${OBJECTDIR}/mystdlib_nomain.o;\
	fi

${OBJECTDIR}/myxml_nomain.o: ${OBJECTDIR}/myxml.o myxml.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/myxml.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -D_DEBUG -I/usr/include/libxml2 -I. -I. -I. -std=c++11 -fPIC  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/myxml_nomain.o myxml.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/myxml.o ${OBJECTDIR}/myxml_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f2 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${TESTDIR}/TestFiles/f1

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc

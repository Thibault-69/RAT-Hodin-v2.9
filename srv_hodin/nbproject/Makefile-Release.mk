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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/keylogger/debian_keylogger_utils.o \
	${OBJECTDIR}/src/keylogger/keylogger.o \
	${OBJECTDIR}/src/keylogger/keylogger_utils.o \
	${OBJECTDIR}/src/keylogger/log_keys.o \
	${OBJECTDIR}/src/srv_main.o \
	${OBJECTDIR}/src/utils.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs gtk+-2.0` `pkg-config --cflags --libs gstreamer-1.0` -lpthread  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/srv_hodin

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/srv_hodin: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/srv_hodin ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/keylogger/debian_keylogger_utils.o: src/keylogger/debian_keylogger_utils.c
	${MKDIR} -p ${OBJECTDIR}/src/keylogger
	${RM} "$@.d"
	$(COMPILE.c) -O2 `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/keylogger/debian_keylogger_utils.o src/keylogger/debian_keylogger_utils.c

${OBJECTDIR}/src/keylogger/keylogger.o: src/keylogger/keylogger.c
	${MKDIR} -p ${OBJECTDIR}/src/keylogger
	${RM} "$@.d"
	$(COMPILE.c) -O2 `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/keylogger/keylogger.o src/keylogger/keylogger.c

${OBJECTDIR}/src/keylogger/keylogger_utils.o: src/keylogger/keylogger_utils.c
	${MKDIR} -p ${OBJECTDIR}/src/keylogger
	${RM} "$@.d"
	$(COMPILE.c) -O2 `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/keylogger/keylogger_utils.o src/keylogger/keylogger_utils.c

${OBJECTDIR}/src/keylogger/log_keys.o: src/keylogger/log_keys.c
	${MKDIR} -p ${OBJECTDIR}/src/keylogger
	${RM} "$@.d"
	$(COMPILE.c) -O2 `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/keylogger/log_keys.o src/keylogger/log_keys.c

${OBJECTDIR}/src/srv_main.o: src/srv_main.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/srv_main.o src/srv_main.c

${OBJECTDIR}/src/utils.o: src/utils.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/utils.o src/utils.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc

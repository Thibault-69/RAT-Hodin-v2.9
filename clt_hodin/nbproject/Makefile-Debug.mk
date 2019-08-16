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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/callbacks_ddos.o \
	${OBJECTDIR}/src/callbacks_execute_commands.o \
	${OBJECTDIR}/src/callbacks_keyloggers.o \
	${OBJECTDIR}/src/callbacks_main.o \
	${OBJECTDIR}/src/callbacks_remote_shell.o \
	${OBJECTDIR}/src/callbacks_rudy_arme_ddos.o \
	${OBJECTDIR}/src/callbacks_tchat.o \
	${OBJECTDIR}/src/clt_main.o \
	${OBJECTDIR}/src/downloader.o \
	${OBJECTDIR}/src/keylogger.o \
	${OBJECTDIR}/src/remote_shell.o \
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
LDLIBSOPTIONS=`pkg-config --libs gtk+-2.0` `pkg-config --cflags --libs gstreamer-1.0` -lpthread -lgtkextra-x11-3.0  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/clt_hodin

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/clt_hodin: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/clt_hodin ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/callbacks_ddos.o: src/callbacks_ddos.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I/usr/include `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/callbacks_ddos.o src/callbacks_ddos.c

${OBJECTDIR}/src/callbacks_execute_commands.o: src/callbacks_execute_commands.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I/usr/include `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/callbacks_execute_commands.o src/callbacks_execute_commands.c

${OBJECTDIR}/src/callbacks_keyloggers.o: src/callbacks_keyloggers.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I/usr/include `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/callbacks_keyloggers.o src/callbacks_keyloggers.c

${OBJECTDIR}/src/callbacks_main.o: src/callbacks_main.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I/usr/include `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/callbacks_main.o src/callbacks_main.c

${OBJECTDIR}/src/callbacks_remote_shell.o: src/callbacks_remote_shell.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I/usr/include `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/callbacks_remote_shell.o src/callbacks_remote_shell.c

${OBJECTDIR}/src/callbacks_rudy_arme_ddos.o: src/callbacks_rudy_arme_ddos.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I/usr/include `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/callbacks_rudy_arme_ddos.o src/callbacks_rudy_arme_ddos.c

${OBJECTDIR}/src/callbacks_tchat.o: src/callbacks_tchat.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I/usr/include `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/callbacks_tchat.o src/callbacks_tchat.c

${OBJECTDIR}/src/clt_main.o: src/clt_main.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I/usr/include `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/clt_main.o src/clt_main.c

${OBJECTDIR}/src/downloader.o: src/downloader.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I/usr/include `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/downloader.o src/downloader.c

${OBJECTDIR}/src/keylogger.o: src/keylogger.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I/usr/include `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/keylogger.o src/keylogger.c

${OBJECTDIR}/src/remote_shell.o: src/remote_shell.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I/usr/include `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/remote_shell.o src/remote_shell.c

${OBJECTDIR}/src/utils.o: src/utils.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I/usr/include `pkg-config --cflags gtk+-2.0` `pkg-config --cflags --cflags gstreamer-1.0`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/utils.o src/utils.c

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

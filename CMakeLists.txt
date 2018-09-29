cmake_minimum_required(VERSION 3.12)
project(persona)

set(CMAKE_CXX_STANDARD 14)

add_executable(people main.cpp Fifos/Fifo.cpp Fifos/FifoEscritura.cpp Fifos/FifoLectura.cpp entities/People.h entities/People.cpp consts.h entities/TicketOffice.cpp entities/TicketOffice.h Lock/LockFile.cpp Lock/LockFile.h)
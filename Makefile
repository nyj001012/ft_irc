NAME = ircserv
SRCS = main.cpp \
	   debug/debug.cpp \
		 debug/Serializable.cpp \
		 debug/json.cpp \
		 debug/Reflector.cpp \
	   server/Server.cpp \
	   utils/utils.cpp \
		 utils/socket_utils.cpp \
		 utils/string_utils.cpp \
		 irc/IrcCommand.cpp \
		 irc/IrcError.cpp \
		 irc/IrcReply.cpp \
		 irc/IrcUtil.cpp \
		 irc/IrcLabels.cpp \
		 user/User.cpp \
		 channel/Channel.cpp \
		 data/UserData.cpp \
		 data/ChannelData.cpp \
		 parse/parse.cpp \
		 task/Task.cpp \
		 task/UserTask.cpp \
		 task/ChannelTask.cpp \
		 task/MessageTask.cpp \
		 task/PingTask.cpp \
		 message/Message.cpp \
		 handler/RequestHandler.cpp 

OBJS = ${SRCS:.cpp=.o}
INCLUDE = -I include -I include/server

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g
RM = rm -f

all:	${NAME}

${NAME}:	${OBJS}
		@${CXX} ${CXXFLAGS} -o ${NAME} ${OBJS}
		@echo "\033[40;35mExecutable ${NAME} created"

%.o: %.cpp
		@${CXX} ${CXXFLAGS} ${INCLUDE} -c $< -o ${<:.cpp=.o}

clean:
		${RM} ${OBJS}

fclean:	clean
		${RM} ${NAME}

re:		fclean all

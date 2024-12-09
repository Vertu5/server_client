CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++17 -O2 -pthread
INC = -I src/serveur -I src/chat

SRC_SERVER = src/serveur/main.cpp src/serveur/serveur.cpp src/serveur/client_handler.cpp
SRC_CHAT = src/chat/main.cpp src/chat/normal_mode.cpp src/chat/bot_mode.cpp src/chat/manual_mode.cpp src/chat/signals.cpp

all: serveur-chat chat

serveur-chat: $(SRC_SERVER)
	$(CXX) $(CXXFLAGS) $(INC) $(SRC_SERVER) -o serveur-chat

chat: $(SRC_CHAT)
	$(CXX) $(CXXFLAGS) $(INC) $(SRC_CHAT) -o chat

clean:
	rm -f serveur-chat chat


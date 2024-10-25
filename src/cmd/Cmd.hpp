#pragma once

#include "../../Irc.hpp"

class Cmd
{
    private:
		string buffer;	// Vai guardando o que foi lido para depois setar o ss
		Client* currentClient;
		std::istringstream ss; //Vai conter duo o que foi colocado
		
		typedef void (Cmd::*Command)(void); //Recebe os mesmos parâmetros das funções de cada comando, é usado no map
		
        void passCmd(void);
		// void nickCmd(void);
		// void userCmd(void);
		// void quitCmd(void);
		// void noticeCmd(void);
		// void joinCmd(void);
		// void whoCmd(void);
		// void partCmd(void);
		// void modeCmd(void);
		// void topicCmd(void);
		// void inviteCmd(void);
		// void kickCmd(void);
		// void privmsgCmd(void);

    public:
		void appendBufer(string msg);
		void setSs(void);
		
		Cmd(Client* ptr);
		~Cmd(void);
        map<string, Command> cmd; //Map para facilitar a chamada de cada comando. Key == comando Value == função de cada comando
};

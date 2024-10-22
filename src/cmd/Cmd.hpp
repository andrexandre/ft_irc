#pragma once

#include "../../Irc.hpp"

class Cmd{

    private:
		typedef void (Cmd::*Command)(std::istringstream &ss, Client* actualClient); //Recebe os mesmos parâmetros das funções de cada comando, é usado no map
		
        void passCmd(std::istringstream &ss, Client* actualClient);
		/*void nickCmd(std::istringstream &ss, Client* actualClient);
		void userCmd(std::istringstream &ss, Client* actualClient);
		void quitCmd(std::istringstream &ss, Client* actualClient);
		void noticeCmd(std::istringstream &ss, Client* actualClient);
		void joinCmd(std::istringstream &ss, Client* actualClient);
		void whoCmd(std::istringstream &ss, Client* actualClient);
		void partCmd(std::istringstream &ss, Client* actualClient);
		void modeCmd(std::istringstream &ss, Client* actualClient);
		void topicCmd(std::istringstream &ss, Client* actualClient);
		void inviteCmd(std::istringstream &ss, Client* actualClient);
		void kickCmd(std::istringstream &ss, Client* actualClient);
		void privmsgCmd(std::istringstream &ss, Client* actualClient);*/

    public:
		Cmd(void);
		~Cmd(void);
        map<string, Command> cmd; //Map para facilitar a chamada de cada comando. Key == comando Value == função de cada comando
};

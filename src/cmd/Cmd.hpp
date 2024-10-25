#pragma once

#include "../../Irc.hpp"

class Cmd{

    private:
        string msg; //Para ter o buffer da msg enviada pelo client
        vector<string> args;
        Client* actualClient;
		typedef void (Cmd::*Command)(); //Recebe os mesmos parâmetros das funções de cada comando, é usado no map
		
        void passCmd();
		/*void nickCmd();
		void userCmd();
		void quitCmd();
		void noticeCmd();
		void joinCmd();
		void whoCmd();
		void partCmd();
		void modeCmd();
		void topicCmd();
		void inviteCmd();
		void kickCmd();
		void privmsgCmd();*/

    public:
		Cmd(void);
		~Cmd(void);
        map<string, Command> commands; //Map para facilitar a chamada de cada comando. Key == comando Value == função de cada comando
    
        void setMsg(char buffer[30000]);
        string getMsg(void);
        void addArg(string arg);
        void setActualClient(Client* client);
};

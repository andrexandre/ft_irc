# Ft_irc

This project involves creating an IRC server from scratch using C++. It implements basic IRC functionality allowing multiple clients to connect and communicate through channels.

## Features

- TCP/IP communication
- Private messaging between users
- Channel management by operators

## Usage

1. Clone the repository
```sh
git clone https://github.com/andrexandre/ft_irc.git ; cd ft_irc
```

2. Compile the program
```sh
make
```

3. Run the server
```sh
./ircserv 8080 1234
```
> It is possible to change the `port` and `password` to your preference

4. Open hexchat and configure hexchat
- Click `Add` a network
- Click `Edit...` the network
- Change the name of the first server from `newserver/6697` to `localhost/8080`
- Deactivate the checkbox `Use SSL for all the servers on this network`
- Change the password to `1234`
- `Close` the settings
- And finally `Connect`
> If hexchat is not installed, you can install it by running:\
> `sudo apt install hexchat -y`

## IRC Commands

Once connected to the server, you can use the following commands:

### User Commands
- `/nick <nickname>` - Change your nickname
- `/join #<channel>` - Join a channel
- `/part #<channel>` - Leave a channel
- `/quit` - Disconnect from server
- `/msg <target> <message>` - Send private message to user or channel
- `/topic #<channel> <topic>` - View or set channel topic

### Channel Operator Commands

When you join a channel and your the only one there, you are automatically the channel operator. Some commands can be used outside a channel but should be used inside a channel, so it is easier to write

- `/kick <user> <reason>` - Remove user from channel
- `/mode [mode] [args]` - Set channel modes:
  - `+i` - Set channel invite-only
  - `+t` - Only operators can change topic
  - `+k <password>` - Set channel password
  - `+o <nickname>` - Give operator status
  - `-o <nickname>` - Remove operator status
  - `+l <limit>` - Set user limit for channel

> [!NOTE]
> This project is part of the 42 School curriculum

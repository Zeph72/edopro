#ifndef DISCORD_WRAPPER_H
#define DISCORD_WRAPPER_H

#include <string>
#include "text_types.h"
struct DiscordUser;
class DiscordWrapper {
public:
	struct DiscordSecret {
		unsigned int game_id;
		unsigned int server_address;
		unsigned short server_port;
		std::string pass;
	};
	enum PresenceType {
		MENU,
		IN_LOBBY,
		DUEL,
		DUEL_STARTED,
		REPLAY,
		PUZZLE,
		DECK,
		DECK_SIDING,
		CLEAR,
		INITIALIZE,
		DISCONNECT,
		TERMINATE
	};
	DiscordWrapper();
	bool Initialize(path_string workingDir);
	void UpdatePresence(PresenceType type);
	bool connected;
	void Check();
private:
	static void Connect();
	static void Disconnect();
#ifdef DISCORD_APP_ID
	std::string& CreateSecret(bool update) const;
	static void OnReady(const DiscordUser* connectedUser, void* payload);
	static void OnDisconnected(int errcode, const char* message, void* payload);
	static void OnError(int errcode, const char* message, void* payload);
	static void OnJoin(const char* secre, void* payloadt);
	static void OnSpectate(const char* secret, void* payload);
	static void OnJoinRequest(const DiscordUser* request, void* payload);
#endif
};

#endif //DISCORD_WRAPPER_H

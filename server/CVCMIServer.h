/*
 * CVCMIServer.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */
#pragma once

#include <boost/program_options.hpp>

#include <boost/asio.hpp>

class CMapInfo;

class CConnection;
struct CPackForSelectionScreen;
class CGameHandler;
struct SharedMemory;

typedef boost::asio::basic_socket_acceptor<boost::asio::ip::tcp, boost::asio::socket_acceptor_service<boost::asio::ip::tcp> > TAcceptor;
typedef boost::asio::basic_stream_socket < boost::asio::ip::tcp , boost::asio::stream_socket_service<boost::asio::ip::tcp>  > TSocket;

class CVCMIServer
{
	ui16 port;
	boost::asio::io_service *io;
	TAcceptor * acceptor;
	SharedMemory * shared;

	CConnection *firstConnection;
public:
	CVCMIServer();
	~CVCMIServer();

	void start();
	std::shared_ptr<CGameHandler> initGhFromHostingConnection(CConnection &c);

	void newGame();
	void loadGame();
	void newPregame();

#ifdef VCMI_ANDROID
    static void create();
#endif
};

struct StartInfo;
class CPregameServer
{
public:
	CConnection *host;
	int listeningThreads;
	std::set<CConnection *> connections;
	std::list<CPackForSelectionScreen*> toAnnounce;
	boost::recursive_mutex mx;

	TAcceptor *acceptor;
	TSocket *upcomingConnection;

	const CMapInfo *curmap;
	StartInfo *curStartInfo;

	CPregameServer(CConnection *Host, TAcceptor *Acceptor = nullptr);
	~CPregameServer();

	void run();

	void processPack(CPackForSelectionScreen * pack);
	void handleConnection(CConnection *cpc);
	void connectionAccepted(const boost::system::error_code& ec);
	void initConnection(CConnection *c);

	void start_async_accept();

	enum { INVALID, RUNNING, ENDING_WITHOUT_START, ENDING_AND_STARTING_GAME
	} state;

	void announceTxt(const std::string &txt, const std::string &playerName = "system");
	void announcePack(const CPackForSelectionScreen &pack);

	void sendPack(CConnection * pc, const CPackForSelectionScreen & pack);
	void startListeningThread(CConnection * pc);
};

extern boost::program_options::variables_map cmdLineOptions;

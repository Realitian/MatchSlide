/**
 * User: Bruce Washington
 * Date: 31/3/20.
 *
 */

#include <string>
#include <iostream>
#include "JSONAPI.h"
#include "netLink.h"
#include <nlohmann/json.hpp>
#include <vector>
#include <iomanip>

// GLOBAL state
netLink::SocketManager socketManager;
// Allocate a new socket and insert it into the SocketManager
std::shared_ptr<netLink::Socket> Socket = socketManager.newMsgPackSocket();
//define current Image and Font vector
std::vector<struct SuperImageData> currentSuperImgData;
std::vector<struct SuperFontData> currentSuperFontData;

template <typename T>
class Storetempdata {
public:
    T x[100];
};

static bool cbOnConnReq(netLink::SocketManager* manager, std::shared_ptr<netLink::Socket> serverSocket, std::shared_ptr<netLink::Socket> clientSocket)
{

    std::cout<<"Accepted connection from {0}:{1}"<< clientSocket->hostRemote<<"::"<< clientSocket->portRemote<<std::endl;
    // Accept all new connections
    return true;
}
static void cbOnStatusChg(netLink::SocketManager* manager, std::shared_ptr<netLink::Socket> socket, netLink::Socket::Status prev)
{

//		netLink::MsgPackSocket& msgPackSocket = *static_cast<netLink::MsgPackSocket*>(socket.get());

    switch (socket->getStatus()) {
        case netLink::Socket::Status::READY:

            std::cout<<"Connection got accepted at {0}:{1}"<<":" <<socket->hostRemote<<"  ::"<< socket->portRemote<<std::endl;

#if 0
        // Prepare a MsgPack encoded message
		        msgPackSocket << MsgPack__Factory(MapHeader(2));
		        msgPackSocket << MsgPack::Factory("type");
		        msgPackSocket << MsgPack::Factory("post");
		        msgPackSocket << MsgPack::Factory("message");
		        msgPackSocket << MsgPack::Factory("Hello World!");
#endif
            break;
        case netLink::Socket::Status::NOT_CONNECTED:
            if (prev == netLink::Socket::Status::CONNECTING)
                std::cout<<"Connecting to {0}:{1} failed"<< socket->hostRemote<<"::"<<socket->portRemote<<std::endl;
            else
            std::cout<<"Lost connection of {0}:{1}" <<socket->hostRemote<<"::" <<socket->portRemote<<std::endl;
            break;
        default:
            std::cout<<"Status of {0}:{1} changed from {2} to {3}"<<socket->hostRemote<<"::" <<socket->portRemote<<"  " <<prev<<" "<< socket->getStatus()<<std::endl;
            break;
    }
}

static void cbOnRecMsgPack(netLink::SocketManager* manager, std::shared_ptr<netLink::Socket> socket, std::unique_ptr<MsgPack::Element> element)
{
    // hostRemote and portRemote are now set to the origin of the last received message
    std::cout<<"Received data from {0}:{1}"<< socket->hostRemote<<"::"<<socket->portRemote<<std::endl;


    // Parse the *element
    auto elementMap = dynamic_cast<MsgPack::Map*>(element.get());
    if (elementMap) { // The received element is a map
        // Iterate them sequentially
        auto container = elementMap->getElementsMap();
            for (auto &pair : container) {
                // Coercion from native Javascript Object to valid JSON
                std::ostringstream SuperValuesFont;
                SuperValuesFont << "{ \"" << pair.first << "\": " << *pair.second << " }";
                std::string superDetails = SuperValuesFont.str();
                nlohmann::json superdatas;
                superdatas = nlohmann::json::parse(superDetails);
                //  std::cout << std::setw(4) << superdata << "\n\n";
                try {
                    if (superdatas["Image_Details"]["data"] == true) {
                        //clear the data of Image
                        currentSuperImgData.clear();
                        Storetempdata<std::string> tempImagestring;  //define all string from Json file for Image
                        Storetempdata<GLfloat> tempImageint; //define all float from Json file for Image
                        Storetempdata<bool> tempImagebool;  //define all boolean from Json file for Image
                        {
                            tempImagestring.x[0] = superdatas["Image_Details"]["Image"]["ImagePath"];
                            tempImagestring.x[1] = superdatas["Image_Details"]["Image"]["Filetype"];
                            tempImagestring.x[2] = superdatas["Image_Details"]["Image"]["ImageRotation"]["details"];
                            tempImagestring.x[3] = superdatas["Image_Details"]["Image"]["Animation"]["details"];
                        }
                        { //Image postion & Co-ordinates
                            tempImageint.x[0] = superdatas["Image_Details"]["Image"]["ImageResolution"]["Obj_Height"];
                            tempImageint.x[1] = superdatas["Image_Details"]["Image"]["ImageResoluation"]["Obj_Width"];
                            tempImageint.x[2] = superdatas["Image_Details"]["Image"]["ImagePosition"]["Pos-X-coordinates_start(Width)"];
                            tempImageint.x[3] = superdatas["Image_Details"]["Image"]["ImagePosition"]["Pos-Y-coordinates_start(Width)"];
                            tempImageint.x[4] = superdatas["Image_Details"]["Image"]["ImagePosition"]["Pos-z-coordinates_start"];
                            tempImageint.x[5] = superdatas["Image_Details"]["Image"]["ImagePosition_End"]["Pos-X-coordinates_end(Width)"];
                            tempImageint.x[6] = superdatas["Image_Details"]["Image"]["ImagePosition_End"]["Pos-Y-coordinates_end(Height)"];
                            tempImageint.x[7] = superdatas["Image_Details"]["Image"]["ImagePosition_End"]["Pos-Z-coordinates_end"];
                            tempImageint.x[8] = superdatas["Image_Details"]["Image"]["Rotation"]["Rot-x"];
                            tempImageint.x[9] = superdatas["Image_Details"]["Image"]["Rotation"]["Rot-Y"];
                            tempImageint.x[10] = superdatas["Image_Details"]["Image"]["Rotation"]["Rot-Z"];
                            tempImageint.x[11]=superdatas["Image_Details"]["Image"]["Animation"]["Animation Speed"];
                            tempImageint.x[12]=superdatas["Image_Details"]["Image"]["Animation"]["Delay"];
                            tempImageint.x[13]=superdatas["Image_Details"]["Image"]["Scale"];
                            tempImageint.x[14]=superdatas["Image_Details"]["Image"]["Opacity"];
                        }
                        {
                            tempImagebool.x[0] = superdatas["Image_Details"]["Image"]["ImageRotation"];
                            tempImagebool.x[1] = superdatas["Image_Details"]["Animation"]["boolean"];
                        }
                        currentSuperImgData.push_back(
                                {tempImagestring.x[0], tempImagestring.x[1],  tempImageint.x[0], tempImageint.x[1],
                                 tempImageint.x[2], tempImageint.x[3], tempImageint.x[4],
                                 tempImageint.x[5], tempImageint.x[6], tempImageint.x[7],
                                 tempImagebool.x[0],  tempImageint.x[8], tempImageint.x[9], tempImageint.x[10],  tempImagestring.x[2],
                                 tempImagebool.x[1],  tempImageint.x[11], tempImageint.x[12],tempImagestring.x[3],
                                 tempImageint.x[13],  tempImageint.x[14]
                                });
                    } else if (superdatas["Font_Details"]["data"] == true) {
                        //clear the data of Font
                        currentSuperFontData.clear();
                        Storetempdata<std::string> tempFontstring;  //define all string from Json file for Font
                        Storetempdata<GLfloat> tempFontint; //define all float from Json file for Font
                        Storetempdata<bool> tempFontbool;  //define all boolean from Json file for Font
                        {
                            tempFontstring.x[0] = superdatas["Font_Details"]["Texture"]["Texture_string"];
                            tempFontstring.x[1] = superdatas["Font_Details"]["Texture"]["TextureFont"]["Fontpath"];
                            tempFontstring.x[2] = superdatas["Font_Details"]["Texture"]["FontRotation"]["details"];
                            tempFontstring.x[3] = superdatas["Font_Details"]["Texture"]["Animation"]["details"];
                        }
                        {
                            tempFontint.x[0] = superdatas["Font_Details"]["Texture"]["TextureFont"]["FontSize"];
                            tempFontint.x[1] = superdatas["Font_Details"]["Texture"]["FontPosition"]["Pos-X-coordinates_start(Width)"];
                            tempFontint.x[2] = superdatas["Font_Details"]["Texture"]["FontPosition"]["Pos-Y-coordinates_start(Height)"];
                            tempFontint.x[3] = superdatas["Font_Details"]["Texture"]["FontPosition"]["Pos-z-coordinates_start"];
                            tempFontint.x[4] = superdatas["Font_Details"]["Texture"]["FontColor"]["R"];
                            tempFontint.x[5] = superdatas["Font_Details"]["Texture"]["FontColor"]["G"];
                            tempFontint.x[6] = superdatas["Font_Details"]["Texture"]["FontColor"]["B"];
                            tempFontint.x[7] = superdatas["Font_Details"]["Texture"]["FontRotation"]["angle"];
                            tempFontint.x[8] = superdatas["Font_Details"]["Texture"]["Animation"]["Delay"];
                            tempFontint.x[9]= superdatas["Font_Details"]["Texture"]["Scale"];
                        }
                        {
                            tempFontbool.x[0] = superdatas["Font_Details"]["Texture"]["Animation"]["boolean"];
                        }
                        currentSuperFontData.push_back(
                                {
                                    tempFontstring.x[0],tempFontstring.x[1],tempFontint.x[0],
                                    tempFontint.x[1],tempFontint.x[2],tempFontint.x[3],
                                    tempFontint.x[4],tempFontint.x[5],tempFontint.x[6],
                                    tempFontint.x[7], tempFontstring.x[2],
                                    tempFontbool.x[0],  tempFontint.x[8], tempFontstring.x[3],
                                    tempFontint.x[9]
                                });
                    }else if(superdatas["Super"]["Image"] == true){
                        Storetempdata<bool> tempbool;
                        tempbool.x[0] = superdatas["Super"]["Image"];
                        tempbool.x[0] = superdatas["Super"]["Texture"];
                    }
                    else
                        {
                        Storetempdata<std::string> tempstring;
                        tempstring.x[0] = superdatas["string"];
                    }
                }
                catch (nlohmann::json::exception &e) {
                    // output exception information
                    std::cout << "message: " << e.what() << '\n'
                             << "exception id: " << e.id << std::endl;
                }
            }
    }
}

JSONAPI::JSONAPI()
{
    // Define a callback, fired when a new client tries to connect
    socketManager.onConnectRequest = &cbOnConnReq;
    // Define a callback, fired when a sockets state changes
    socketManager.onStatusChange = &cbOnStatusChg;
    // Define a callback, fired when a socket receives data
    socketManager.onReceiveMsgPack = &cbOnRecMsgPack;

//socketManager.onStatusChange = [](netLink::SocketManager* manager, std::shared_ptr<netLink::Socket> socket, netLink::Socket::Status prev) {
}

JSONAPI::~JSONAPI()
{
    stop_thread = true;
}

void JSONAPI::ThreadMain()
{
    try {
        // Init socket as TCP server or client on port 3823
        Socket->hostRemote = "*";
        Socket->initAsUdpPeer("*", 3824);
    } catch(netLink::Exception exc) {
        std::cout<<"Address is already in use, please try again..."<<std::endl;
    }
    try {
        // Define the destination for the next sent message (depending on the choosen IP version)
        Socket->hostRemote = (Socket->getIPVersion() == netLink::Socket::IPv4) ? "224.0.0.100" : "FF02:0001::";
        Socket->portRemote =Socket->portLocal;

        // Join the multicast group to receive messages from the given address
        Socket->setMulticastGroup(Socket->hostRemote, true);
    }catch(netLink::Exception exc) {
        std::cout<<exc.code<<std::endl;
    }

    while (!stop_thread) {
        // Let the SocketManager poll from all sockets, events will be triggered here
        while(Socket->getStatus() != netLink::Socket::Status::NOT_CONNECTED) {
            try {
                socketManager.listen();
            } catch(netLink::Exception exc) {
              //  std::cout<<"netLink::Exception throw type = {0}"<< exc.code<<std::endl;
            }
        }
    }
}

std::vector<struct SuperImageData> &JSONAPI::getSuperImageDetails() {
    return currentSuperImgData;
}

std::vector<struct SuperFontData> &JSONAPI::getSuperFontDetails() {
    return currentSuperFontData;
}

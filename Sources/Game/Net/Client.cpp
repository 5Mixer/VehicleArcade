#include "Client.h"

Game::Net::Client::Client() {
    if (enet_initialize() != 0) {
        std::cerr << "Could not initialise enet";
        exit(1);
    }

    client = enet_host_create(nullptr, 1, 2, 0, 0);

    if (client == NULL) {
        std::cerr << "An error occurred while trying to create an ENet client host." << std::endl;
        exit(1);
    }

    ENetAddress address;
    ENetEvent event;
    ENetPeer *peer;
    enet_address_set_host(&address, "localhost");
    address.port = 9320;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    peer = enet_host_connect(client, &address, 2, 0);
    if (peer == nullptr) {
        std::cerr << "No available peers for initiating an ENet connection." << std::endl;
        exit(1);
    }

    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connected to server" << std::endl;
    } else {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset(peer);

        std::cerr << "Failed to connect to server" << std::endl;
        exit(1);
    }
}

void Game::Net::Client::service() {
    ENetEvent event;
    /* Wait up to 1000 milliseconds for an event. */
    while (enet_host_service(client, &event, 1000) > 0) {
    }
}

Game::Net::Client::~Client() {
    enet_deinitialize();
}
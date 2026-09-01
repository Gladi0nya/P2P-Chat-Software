# P2P Chat Software

Peer to peer chatting System.

```src/
├── main.c                          # parse les arguments, appelle bootstrap_start()
│
├── core/
│   ├── bootstrap.c / .h            # init unique au démarrage (socket, STUN, registres, dispatcher)
│   └── node.c / .h                 # boucle poll() qui tourne pendant toute la vie du programme
│
├── net/                            # UNIQUEMENT ce qui touche socket_fd / sockaddr_in
│   ├── udp_socket.c / .h           # socket UDP unique, sendto/recvfrom
│   ├── stun_client.c / .h          # découverte de son IP:port public
│   └── hole_punch.c / .h           # envoi répété des paquets PUNCH + retransmissions/timeout
│
├── protocol/                       # format des messages, aucune dépendance réseau
│   ├── message.h                   # struct Message, enum MessageType
│   └── codec.c / .h                # encode_message() / decode_message()
│
├── dispatch/                       # routage, ne connaît pas les sockets
│   ├── dispatcher.c / .h           # dispatch_message() via table de function pointers
│   └── handlers/
│       ├── handler_punch.c / .h    # paquets PUNCH / PUNCH_ACK
│       ├── handler_handshake.c / .h # HELLO applicatif une fois le trou confirmé
│       └── handler_data.c / .h
│
├── state/                          # machine à états, ne connaît pas le réseau
│   ├── peer_state.h                # enum PeerState, struct PeerContext
│   └── peer_registry.c / .h        # liste des peers ajoutés manuellement par l'utilisateur
│
└── ui/
    └── input.c / .h                # saisie manuelle de l'IP:port du peer distant```
    
** Rappel des règles de tri (pour quand tu ajoutes un fichier plus tard)
- Question	Si oui → dossier

- Ça manipule un socket_fd ou sockaddr_in directement ?	net/

- Ça définit/encode/décode le format des messages ?	protocol/

- Ça route un Message déjà décodé vers un handler ?	dispatch/
- Ça définit ou fait transitionner un état (peer ou global) ?	state/
- Ça s'exécute une seule fois au lancement, dans l'ordre ?	core/bootstrap.c
- Ça tourne en continu pendant la vie du programme ?	core/node.c



**Copyright (c) 2026 Tom Schmitt and Augustin Barniet. All rights reserved.**

This source code is proprietary and confidential.  
Unauthorized copying, modification, or distribution is strictly prohibited.
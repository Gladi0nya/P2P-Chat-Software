# P2P Chat Software

Peer to peer chatting System.

```src/
├── main.c                          # parse arguments, call bootstrap_start()
│
├── core/
│   ├── bootstrap.c / .h            # init at boot time (socket, STUN, registers, dispatcher)
│   └── node.c / .h                 # poll() loop for program lifetime
│
├── net/                            # Only things related to socket_fd / sockaddr_in
│   ├── udp_socket.c / .h           # socket UDP, sendto/recvfrom
│   ├── stun_client.c / .h          # discovery of public IP:port
│   └── hole_punch.c / .h           # sending repeatedly PUNCH packets + resend / timeout
│
├── protocol/                       # Message format 
│   └── message.h                   # struct Message, enum MessageType
│
├── dispatch/                       # routing
│   ├── dispatcher.c / .h           # dispatch_message()
│   └── handlers/
│       ├── handler_punch.c / .h    # packets PUNCH / PUNCH_ACK
│       ├── handler_close.c / .h    # Close connection
│       └── handler_data.c / .h     # Chat messages
│
├── state/                          # state machine
│   ├── peer_state.h                # enum PeerState, struct PeerContext
│   └── peer_registry.c / .h        # Peers added by user
│
└── ui/
    └── chat_input.c / .h           # user message input
```
    
## Rules

- Related to socket_fd / sockaddr_in directly ?	net/
- Encode/decode messages format ?	protocol/

- Route encoded Message to an handler ?	dispatch/
- Definie / change state (peer ou global) ?	state/
- Boot time execution ?	core/bootstrap.c
- Run during program lfietime ?	core/node.c

**Copyright (c) 2026 Tom Schmitt and Augustin Barniet. All rights reserved.**

This source code is proprietary and confidential.  
Unauthorized copying, modification, or distribution is strictly prohibited.
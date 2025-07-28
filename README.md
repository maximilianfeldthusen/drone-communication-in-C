
## drone-communication-in-C

##  Header Inclusions

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "c_library_v2/common/mavlink.h"
```

- Standard C libraries for I/O, memory, network, and socket handling.
- `mavlink.h` provides MAVLink protocol utilities: message packing, parsing, and enums.

---

##  Configuration Constants

```c
#define BUFFER_LENGTH 2041
#define TARGET_PORT   14550
#define SYSTEM_ID     1    // Unique per drone
#define COMPONENT_ID  190
```

- `BUFFER_LENGTH`: Max size for MAVLink packets.
- `TARGET_PORT`: UDP port for communication.
- `SYSTEM_ID`: Identifies each drone in the swarm.
- `COMPONENT_ID`: Describes the drone's subsystem (usually autopilot or communicator).

---

##  `send_heartbeat()` Function

```c
void send_heartbeat(int sockfd, struct sockaddr_in *target_addr)
```

Sends a MAVLink **heartbeat** to indicate the drone is alive and active.

- Uses `mavlink_msg_heartbeat_pack()` to create a heartbeat message.
- Specifies:
  - `MAV_TYPE_QUADROTOR`
  - `MAV_AUTOPILOT_GENERIC`
  - `MAV_MODE_GUIDED_ARMED`
  - `MAV_STATE_ACTIVE`
- Converts it into a byte buffer and transmits via UDP `sendto()`.

 Useful for drone presence and health monitoring.

---

##  `send_command_takeoff()` Function

```c
void send_command_takeoff(int sockfd, struct sockaddr_in *target_addr, float altitude)
```

Sends a `MAV_CMD_NAV_TAKEOFF` command.

- Uses `mavlink_msg_command_long_pack()` to structure a command message.
- Fields set altitude parameter to guide takeoff target height.
- Sends over UDP similarly to heartbeat.


---

##  `receive_messages()` Function

```c
void receive_messages(int sockfd)
```

Receives and parses incoming MAVLink messages.

- Reads raw UDP data using `recvfrom()`.
- Iterates over each byte with `mavlink_parse_char()` to build valid messages.
- Handles specific message types:
  - `COMMAND_ACK`: parses and prints command acknowledgment status.


---

##  `main()` Function

```c
int main(int argc, char *argv[])
```

Sets up networking and starts communication loop.

### Logic:
1. Validates input argument (target IP).
2. Creates UDP socket.
3. Initializes target address.
4. Enters infinite loop:
   - Sends heartbeat every second.
   - (Optional) Sends takeoff command.
   - Checks and handles incoming messages.
5. Closes socket on exit.

 This loop provides continuous communication and monitoring.

---

##  How to Setup & Use on Linux

###  Install dependencies

```bash
sudo apt update
sudo apt install gcc make netcat
```

###  Clone MAVLink C headers

```bash
git clone https://github.com/mavlink/c_library_v2.git
```

Ensure code includes headers from this path.

---

##  Sample `Makefile`

```makefile
CC = gcc
CFLAGS = -Wall -I./c_library_v2/common
SRC = drone_com.c
BIN = drone_com

all:
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

clean:
	rm -f $(BIN)
```

Build with:

```bash
make
```

---

##  Running Your Drone

### Ground Station (UDP listener):

```bash
nc -u -l 14550
```

### Drone Terminal:

```bash
./drone_comm 127.0.0.1
```

Change `SYSTEM_ID` and target IP for each drone.


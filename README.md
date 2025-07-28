
## drone-communication-in-C

## Linux Setup Instructions

### 1. Install build tools

```bash
sudo apt update
sudo apt install gcc make netcat
```

### 2. Clone MAVLink headers

```bash
git clone https://github.com/mavlink/c_library_v2.git
```

Ensure `drone_comm.c` includes:

```c
#include "c_library_v2/common/mavlink.h"
```

Or adjust paths via `-I` in the Makefile.

### 3. Build the module

```bash
make
```

### 4. Launch drone(s)

Each drone should use a **different `SYSTEM_ID`**. Modify and recompile for each instance.

```bash
./drone_comm <target-IP>
```

To simulate a ground station:

```bash
nc -u -l 14550
```
